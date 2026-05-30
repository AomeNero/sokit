#ifndef __MSGLISTPANEL_H__
#define __MSGLISTPANEL_H__

#include <QWidget>
#include <QTableView>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QTimer>
#include <functional>
#include "msglistmodel.h"

class MsgListPanel : public QWidget
{
    Q_OBJECT

public:
    typedef std::function<void(const QString& data)> SendCallback;

    explicit MsgListPanel(QWidget* parent = nullptr);
    ~MsgListPanel();

    void setSendCallback(SendCallback callback);
    void setStoragePrefix(const QString& prefix);
    void loadMessages();
    void saveMessages();
    void togglePanel();
    bool isExpanded() const { return m_expanded; }

signals:
    void output(const QString& info);

private slots:
    void onAddClicked();
    void onRemoveClicked();
    void onMoveUpClicked();
    void onMoveDownClicked();
    void onSendSelectedClicked();
    void onLoopSendToggled(bool checked);
    void onAutoSendTimerTick();
    void onToggleClicked();
    void onSearchTextChanged(const QString& text);
    void onSearchNext();
    void onSearchPrev();

private:
    void setupUI();
    void updateButtonStates();
    void updateStatus();
    void startAutoSend();
    void stopAutoSend();
    void sendNextMessage();
    void selectMatch(int index);
    void performSearch(const QString& text);

private:
    QTableView* m_table;
    MsgListModel* m_model;
    QPushButton* m_btnToggle;
    QPushButton* m_btnAdd;
    QPushButton* m_btnRemove;
    QPushButton* m_btnUp;
    QPushButton* m_btnDown;
    QPushButton* m_btnSend;
    QPushButton* m_btnLoop;
    QLineEdit* m_searchEdit;
    QPushButton* m_btnSearchNext;
    QPushButton* m_btnSearchPrev;
    QLabel* m_searchResultLabel;
    QList<int> m_searchMatches;
    int m_currentMatchIndex;

    QString m_msgFilePath;
    SendCallback m_sendCallback;
    bool m_expanded;

    QTimer* m_autoSendTimer;
    QList<MsgItem> m_autoSendQueue;
    int m_queueIndex;
    int m_currentDelay;
    bool m_autoSending;
    bool m_loopSending;
    bool m_lastDelayExecuted;
    int m_loopCount;

    static const int PANEL_WIDTH_EXPANDED = 280;
};

#endif // __MSGLISTPANEL_H__
