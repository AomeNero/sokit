#include "msglistpanel.h"
#include "setting.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QScrollBar>
#include <QCheckBox>
#include <QApplication>
#include <QItemSelectionModel>
#include <QString>
#include <QThread>
#include <QFile>
#include <QItemDelegate>
#include <QSpinBox>
#include <QLineEdit>

// Custom SpinBox with 500ms step alignment
class AlignedSpinBox : public QSpinBox
{
public:
    AlignedSpinBox(QWidget* parent = nullptr) : QSpinBox(parent)
    {
        setMinimum(0);
        setMaximum(999999);
        setSingleStep(500);
    }

protected:
    void stepBy(int steps) override
    {
        int currentValue = value();
        int stepSize = 500;

        if (steps > 0)
        {
            // Increase: round up to nearest 500 multiple
            // Example: 1234 -> 1500, 1000 -> 1500, 1500 -> 2000
            int aligned = ((currentValue + stepSize - 1) / stepSize) * stepSize;
            setValue(aligned + (steps - 1) * stepSize);
        }
        else if (steps < 0)
        {
            // Decrease: round down to nearest 500 multiple
            // Example: 1234 -> 1000, 1000 -> 500, 500 -> 0
            int aligned = (currentValue / stepSize) * stepSize;
            int newValue = aligned + steps * stepSize;
            setValue(qMax(minimum(), newValue));
        }
    }
};

// Custom delegate for delay column with 500ms step alignment
class DelayDelegate : public QItemDelegate
{
public:
    DelayDelegate(QObject* parent = nullptr) : QItemDelegate(parent) {}

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const override
    {
        AlignedSpinBox* editor = new AlignedSpinBox(parent);
        return editor;
    }

    void setEditorData(QWidget* editor, const QModelIndex& index) const override
    {
        int value = index.model()->data(index, Qt::EditRole).toInt();
        AlignedSpinBox* spinBox = static_cast<AlignedSpinBox*>(editor);
        spinBox->setValue(value);
    }

    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index) const override
    {
        AlignedSpinBox* spinBox = static_cast<AlignedSpinBox*>(editor);
        spinBox->interpretText();
        int value = spinBox->value();
        model->setData(index, value, Qt::EditRole);
    }

    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index) const override
    {
        editor->setGeometry(option.rect);
    }
};

MsgListPanel::MsgListPanel(QWidget* parent)
    : QWidget(parent)
    , m_model(new MsgListModel(this))
    , m_expanded(true)
    , m_autoSendTimer(new QTimer(this))
    , m_queueIndex(0)
    , m_currentDelay(0)
    , m_autoSending(false)
    , m_loopSending(false)
    , m_lastDelayExecuted(false)
    , m_loopCount(0)
    , m_currentMatchIndex(-1)
{
    setupUI();

    connect(m_autoSendTimer, SIGNAL(timeout()), this, SLOT(onAutoSendTimerTick()));
}

MsgListPanel::~MsgListPanel()
{
    saveMessages();
}

void MsgListPanel::setStoragePrefix(const QString& prefix)
{
    // Use application directory (where exe is) instead of Setting::path()
    QString appDir = QCoreApplication::applicationDirPath();
    m_msgFilePath = appDir + "/" + prefix + "_messages.md";
}

void MsgListPanel::setupUI()
{
    // Remove fixed width, use flexible width
    // setFixedWidth(PANEL_WIDTH_EXPANDED);

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(2, 2, 2, 2);
    mainLayout->setSpacing(2);

    QHBoxLayout* toolBar = new QHBoxLayout();
    toolBar->setSpacing(2);

    m_btnToggle = new QPushButton(">>", this);
    m_btnToggle->setFixedSize(22, 22);
    connect(m_btnToggle, SIGNAL(clicked()), this, SLOT(onToggleClicked()));

    m_btnAdd = new QPushButton("+", this);
    m_btnAdd->setFixedSize(22, 22);
    connect(m_btnAdd, SIGNAL(clicked()), this, SLOT(onAddClicked()));

    m_btnRemove = new QPushButton("-", this);
    m_btnRemove->setFixedSize(22, 22);
    connect(m_btnRemove, SIGNAL(clicked()), this, SLOT(onRemoveClicked()));

    m_btnUp = new QPushButton("^", this);
    m_btnUp->setFixedSize(22, 22);
    connect(m_btnUp, SIGNAL(clicked()), this, SLOT(onMoveUpClicked()));

    m_btnDown = new QPushButton("v", this);
    m_btnDown->setFixedSize(22, 22);
    connect(m_btnDown, SIGNAL(clicked()), this, SLOT(onMoveDownClicked()));

    toolBar->addWidget(m_btnToggle);
    toolBar->addSpacing(4);
    toolBar->addWidget(m_btnAdd);
    toolBar->addWidget(m_btnRemove);
    toolBar->addWidget(m_btnUp);
    toolBar->addWidget(m_btnDown);
    toolBar->addStretch();

    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setPlaceholderText(tr("Search..."));
    m_searchEdit->setFixedWidth(350);
    m_searchEdit->setClearButtonEnabled(true);
    m_searchEdit->setToolTip(tr("Search in Data and Desc columns (Enter=Next, Shift+Enter=Prev)"));

    m_btnSearchNext = new QPushButton(QString::fromUtf8("\xe2\x96\xbc"), this);
    m_btnSearchNext->setFixedSize(22, 22);
    m_btnSearchNext->setToolTip(tr("Next match (Enter)"));

    m_btnSearchPrev = new QPushButton(QString::fromUtf8("\xe2\x96\xb2"), this);
    m_btnSearchPrev->setFixedSize(22, 22);
    m_btnSearchPrev->setToolTip(tr("Previous match (Shift+Enter)"));

    m_searchResultLabel = new QLabel(this);
    m_searchResultLabel->setFixedWidth(40);
    m_searchResultLabel->setAlignment(Qt::AlignCenter);
    m_searchResultLabel->hide();

    toolBar->addWidget(m_searchEdit);
    toolBar->addWidget(m_btnSearchNext);
    toolBar->addWidget(m_btnSearchPrev);
    toolBar->addWidget(m_searchResultLabel);

    mainLayout->addLayout(toolBar);

    m_table = new QTableView(this);
    m_table->setModel(m_model);
    m_table->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->setAlternatingRowColors(true);
    // Force selected rows to always show blue, even when table doesn't have focus
    m_table->setStyleSheet(
        "QTableView::item:selected {"
        "    background-color: #0078D7;"
        "    color: white;"
        "}"
    );
    m_table->setWordWrap(true);
    m_table->verticalHeader()->setVisible(false);
    m_table->horizontalHeader()->setStretchLastSection(true);

    m_table->setColumnWidth(MsgListModel::ColChecked, 25);
    m_table->setColumnWidth(MsgListModel::ColContent, 200); // Default width 200px
    m_table->setColumnWidth(MsgListModel::ColDescription, 120); // Default width 120px
    m_table->setColumnWidth(MsgListModel::ColOrder, 40);
    m_table->setColumnWidth(MsgListModel::ColDelayMs, 65);
    // Set Data column to be interactive (user resizable), others to be fixed
    m_table->horizontalHeader()->setStretchLastSection(false);
    m_table->horizontalHeader()->setSectionResizeMode(MsgListModel::ColContent, QHeaderView::Stretch);
    m_table->horizontalHeader()->setSectionResizeMode(MsgListModel::ColDescription, QHeaderView::Stretch);
    m_table->horizontalHeader()->setSectionResizeMode(MsgListModel::ColChecked, QHeaderView::Fixed);
    m_table->horizontalHeader()->setSectionResizeMode(MsgListModel::ColOrder, QHeaderView::Fixed);
    m_table->horizontalHeader()->setSectionResizeMode(MsgListModel::ColDelayMs, QHeaderView::Fixed);
    
    // Set custom delegate for delay column with 500ms step
    m_table->setItemDelegateForColumn(MsgListModel::ColDelayMs, new DelayDelegate(this));
    
    // Set header style: no bold font, gray background (same as buttons)
    m_table->horizontalHeader()->setStyleSheet(
        "QHeaderView {"
        "    font-weight: normal;"
        "    font-size: 9pt;"
        "    color: #000000;"
        "}"
        "QHeaderView::section, QHeaderView::section:checked, QHeaderView::section:pressed, QHeaderView::section:hover, QHeaderView::section:selected {"
        "    font-weight: normal;"
        "    font-size: 9pt;"
        "    background-color: #F0F0F0;"
        "    border: 1px solid #D0D0D0;"
        "    color: #000000;"
        "    padding: 4px;"
        "}"
    );

    mainLayout->addWidget(m_table, 1);

    QHBoxLayout* actionBar = new QHBoxLayout();
    actionBar->setSpacing(10); // Increase horizontal spacing

    m_btnSend = new QPushButton(tr("Send"), this);
    m_btnSend->setContentsMargins(10, 5, 10, 5); // Left, Top, Right, Bottom
    m_btnSend->setToolTip(tr("Send selected message"));
    m_btnSend->setFixedHeight(40); // Increase button height
    connect(m_btnSend, SIGNAL(clicked()), this, SLOT(onSendSelectedClicked()));

    m_btnLoop = new QPushButton(tr("Loop"), this);
    m_btnLoop->setContentsMargins(10, 5, 10, 5); // Left, Top, Right, Bottom
    m_btnLoop->setToolTip(tr("Loop send selected messages"));
    m_btnLoop->setFixedHeight(40); // Increase button height
    m_btnLoop->setCheckable(true);
    connect(m_btnLoop, SIGNAL(toggled(bool)), this, SLOT(onLoopSendToggled(bool)));

    actionBar->addWidget(m_btnSend);
    actionBar->addWidget(m_btnLoop);
    actionBar->addStretch();

    // Add top and bottom spacers, make buttons 10px away from top and bottom
    mainLayout->addSpacing(10);
    mainLayout->addLayout(actionBar);
    mainLayout->addSpacing(10);

    connect(m_table->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(updateButtonStates()));
    connect(m_model, SIGNAL(itemsChanged()), this, SLOT(updateStatus()));

    connect(m_searchEdit, &QLineEdit::textChanged, this, &MsgListPanel::onSearchTextChanged);
    connect(m_searchEdit, &QLineEdit::returnPressed, this, &MsgListPanel::onSearchNext);
    connect(m_btnSearchNext, &QPushButton::clicked, this, &MsgListPanel::onSearchNext);
    connect(m_btnSearchPrev, &QPushButton::clicked, this, &MsgListPanel::onSearchPrev);
}

void MsgListPanel::setSendCallback(SendCallback callback)
{
    m_sendCallback = callback;
}

void MsgListPanel::loadMessages()
{
    // Check if messages.md file exists
    QFile file(m_msgFilePath);
    bool fileExists = file.exists();
    
    m_model->loadFromMd(m_msgFilePath);
    
    // Add 40 default messages only if the file doesn't exist
    if (!fileExists && m_model->rowCount() == 0)
    {
        for (int i = 0; i < 40; ++i)
        {
            MsgItem it;
            it.checked = false; // Default not checked
            it.content = ""; // Default empty content
            it.order = i + 1; // Order starts from 1
            it.delayMs = 0; // Default delay 0ms
            it.description = ""; // Default empty description
            m_model->addItem(it);
        }
    }
    
    // Always renumber orders to ensure they are consecutive starting from 1
    m_model->renumberOrders();
}

void MsgListPanel::saveMessages()
{
    m_model->saveToMd(m_msgFilePath);
}

void MsgListPanel::togglePanel()
{
    m_expanded = !m_expanded;

    if (m_expanded)
    {
        // Set minimum width and allow stretching to fill available space
        setMinimumWidth(PANEL_WIDTH_EXPANDED);
        setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        m_btnToggle->setText(">>");
        m_table->show();
        m_btnAdd->show();
        m_btnRemove->show();
        m_btnUp->show();
        m_btnDown->show();
        m_btnSend->show();
        m_btnLoop->show();
        m_searchEdit->show();
        m_btnSearchNext->show();
        m_btnSearchPrev->show();
        m_searchResultLabel->show();
    }
    else
    {
        // Set back to original size
        setMinimumWidth(PANEL_WIDTH_EXPANDED);
        setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
        m_btnToggle->setText("<<");
        m_table->hide();
        m_btnAdd->hide();
        m_btnRemove->hide();
        m_btnUp->hide();
        m_btnDown->hide();
        m_btnSend->hide();
        m_btnLoop->hide();
        m_searchEdit->hide();
        m_btnSearchNext->hide();
        m_btnSearchPrev->hide();
        m_searchResultLabel->hide();
    }
}

void MsgListPanel::onToggleClicked()
{
    togglePanel();
}

void MsgListPanel::onAddClicked()
{
    MsgItem it;
    it.checked = false; // 默认不勾选
    it.delayMs = 0; // 默认延迟0ms
    it.description = ""; // 说明为空

    QModelIndexList selected = m_table->selectionModel()->selectedRows();
    if (selected.isEmpty())
    {
        // No row selected, add to the end
        m_model->addItem(it);
        
        int lastRow = m_model->rowCount() - 1;
        m_table->selectRow(lastRow);
        m_table->edit(m_model->index(lastRow, MsgListModel::ColContent));
    }
    else
    {
        // Row selected, add above the selected row
        int row = selected.first().row();
        m_model->insertItem(row, it);
        
        m_table->selectRow(row);
        m_table->edit(m_model->index(row, MsgListModel::ColContent));
    }
    
    // Renumber orders to ensure they are consecutive starting from 1
    m_model->renumberOrders();
}

void MsgListPanel::onRemoveClicked()
{
    QModelIndexList selected = m_table->selectionModel()->selectedRows();
    if (selected.isEmpty())
        return;

    m_model->removeItem(selected.first().row());
}

void MsgListPanel::onMoveUpClicked()
{
    QModelIndexList selected = m_table->selectionModel()->selectedRows();
    if (selected.isEmpty())
        return;

    int row = selected.first().row();
    m_model->moveUp(row);

    if (row > 0)
        m_table->selectRow(row - 1);
}

void MsgListPanel::onMoveDownClicked()
{
    QModelIndexList selected = m_table->selectionModel()->selectedRows();
    if (selected.isEmpty())
        return;

    int row = selected.first().row();
    m_model->moveDown(row);

    if (row < m_model->rowCount() - 1)
        m_table->selectRow(row + 1);
}

void MsgListPanel::onSendSelectedClicked()
{
    if (!m_sendCallback)
        return;

    QList<MsgItem> checkedItems = m_model->getCheckedItemsSorted();
    if (checkedItems.isEmpty())
    {
        emit output(tr("No messages checked"));
        return;
    }

    // Use the same auto send mechanism for consistent delay behavior
    m_autoSendQueue = checkedItems;
    m_queueIndex = 0;
    m_currentDelay = 0; // Start immediately with first message
    m_autoSending = true;
    m_loopSending = false; // Don't loop

    m_autoSendTimer->start(100);

    emit output(tr("Send started, %1 messages in queue").arg(m_autoSendQueue.count()));
    updateStatus();
}

void MsgListPanel::onLoopSendToggled(bool checked)
{
    if (checked)
    {
        m_loopSending = true;
        startAutoSend();
    }
    else
    {
        m_loopSending = false;
        stopAutoSend();
    }
}

void MsgListPanel::startAutoSend()
{
    m_autoSendQueue = m_model->getCheckedItemsSorted();

    if (m_autoSendQueue.isEmpty())
    {
        emit output(tr("No messages selected for auto send"));
        return;
    }

    if (!m_sendCallback)
    {
        emit output(tr("Send callback not set"));
        return;
    }

    m_queueIndex = 0;
    m_currentDelay = 0; // Start immediately with first message
    m_autoSending = true;
    m_loopCount = 0;

    m_autoSendTimer->start(100);

    emit output(tr("Send started, %1 messages in queue").arg(m_autoSendQueue.count()));
    updateStatus();
}

void MsgListPanel::stopAutoSend()
{
    m_autoSending = false;
    m_loopSending = false;
    m_autoSendTimer->stop();
    m_autoSendQueue.clear();
    m_loopCount = 0;

    m_btnLoop->setChecked(false);

    updateStatus();
}

void MsgListPanel::onAutoSendTimerTick()
{
    if (!m_autoSending || m_autoSendQueue.isEmpty())
    {
        stopAutoSend();
        return;
    }

    if (m_currentDelay > 0)
    {
        m_currentDelay -= 100;
        updateStatus();
        return;
    }

    sendNextMessage();
}

void MsgListPanel::sendNextMessage()
{
    if (m_queueIndex >= m_autoSendQueue.count())
    {
        if (m_loopSending)
        {
            // Check if the last message's delay needs to be executed
            if (!m_lastDelayExecuted && m_currentDelay > 0)
            {
                // Execute the last message's delay
                m_lastDelayExecuted = true;
                return;
            }
            // Reset queue index for loop sending
            m_queueIndex = 0;
            m_lastDelayExecuted = false;
            m_loopCount++;
            emit output(tr("Loop send restarted (Round %1)").arg(m_loopCount));
        }
        else
        {
            stopAutoSend();
            m_btnLoop->setChecked(false);
            emit output(tr("Send completed"));
        }
        return;
    }

    const MsgItem& it = m_autoSendQueue[m_queueIndex];

    if (m_sendCallback && !it.content.isEmpty())
    {
        m_sendCallback(it.content);
    }

    // Get the current message's delay
    int delay = it.delayMs;

    m_queueIndex++;

    // Set delay for next message
    m_currentDelay = delay;
    m_lastDelayExecuted = false;

    updateStatus();
}

void MsgListPanel::updateButtonStates()
{
    QModelIndexList selected = m_table->selectionModel()->selectedRows();
    bool hasSelection = !selected.isEmpty();
    int row = hasSelection ? selected.first().row() : -1;

    m_btnRemove->setEnabled(hasSelection);
    m_btnUp->setEnabled(hasSelection && row > 0);
    m_btnDown->setEnabled(hasSelection && row < m_model->rowCount() - 1);
    m_btnSend->setEnabled(hasSelection);
}

void MsgListPanel::updateStatus()
{
    // Remove status display
}

void MsgListPanel::performSearch(const QString& text)
{
    m_searchMatches.clear();

    if (text.isEmpty())
    {
        m_currentMatchIndex = -1;
        m_searchResultLabel->hide();
        return;
    }

    int rowCount = m_model->rowCount();
    for (int i = 0; i < rowCount; ++i)
    {
        MsgItem item = m_model->getItem(i);
        if (item.content.contains(text, Qt::CaseInsensitive) ||
            item.description.contains(text, Qt::CaseInsensitive))
        {
            m_searchMatches.append(i);
        }
    }

    if (m_searchMatches.isEmpty())
    {
        m_currentMatchIndex = -1;
        m_searchResultLabel->setText("0/0");
        m_searchResultLabel->show();
    }
}

void MsgListPanel::selectMatch(int index)
{
    if (index < 0 || index >= m_searchMatches.count())
        return;

    int row = m_searchMatches[index];
    m_table->selectRow(row);
    m_table->scrollTo(m_model->index(row, 0));
    m_searchResultLabel->setText(QString("%1/%2").arg(index + 1).arg(m_searchMatches.count()));
    m_searchResultLabel->show();
}

void MsgListPanel::onSearchTextChanged(const QString& text)
{
    performSearch(text);

    if (text.isEmpty())
        return;

    if (!m_searchMatches.isEmpty())
    {
        m_currentMatchIndex = 0;
        selectMatch(0);
    }
}

void MsgListPanel::onSearchNext()
{
    if (QApplication::keyboardModifiers() & Qt::ShiftModifier)
    {
        onSearchPrev();
        return;
    }

    if (m_searchMatches.isEmpty())
    {
        performSearch(m_searchEdit->text());
        if (m_searchMatches.isEmpty())
            return;
        m_currentMatchIndex = 0;
    }
    else
    {
        m_currentMatchIndex = (m_currentMatchIndex + 1) % m_searchMatches.count();
    }

    selectMatch(m_currentMatchIndex);
}

void MsgListPanel::onSearchPrev()
{
    if (m_searchMatches.isEmpty())
    {
        performSearch(m_searchEdit->text());
        if (m_searchMatches.isEmpty())
            return;
        m_currentMatchIndex = m_searchMatches.count() - 1;
    }
    else
    {
        m_currentMatchIndex = (m_currentMatchIndex - 1 + m_searchMatches.count()) % m_searchMatches.count();
    }

    selectMatch(m_currentMatchIndex);
}
