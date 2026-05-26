#include "msglistmodel.h"
#include <QApplication>
#include <QStyle>
#include <algorithm>

MsgListModel::MsgListModel(QObject* parent)
    : QAbstractTableModel(parent)
{
}

MsgListModel::~MsgListModel()
{
}

int MsgListModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return m_items.count();
}

int MsgListModel::columnCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return ColCount;
}

QVariant MsgListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= m_items.count())
        return QVariant();

    const MsgItem& it = m_items[index.row()];

    if (role == Qt::DisplayRole || role == Qt::EditRole)
    {
        switch (index.column())
        {
        case ColContent:
            return it.content;
        case ColDescription:
            return it.description;
        case ColOrder:
            return it.order;
        case ColDelayMs:
            return it.delayMs;
        }
    }

    if (role == Qt::CheckStateRole && index.column() == ColChecked)
    {
        return it.checked ? Qt::Checked : Qt::Unchecked;
    }

    if (role == Qt::TextAlignmentRole)
    {
        if (index.column() == ColOrder || index.column() == ColDelayMs)
            return QVariant(Qt::AlignCenter);
        return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
    }

    return QVariant();
}

bool MsgListModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || index.row() >= m_items.count())
        return false;

    MsgItem& it = m_items[index.row()];

    if (role == Qt::EditRole || role == Qt::CheckStateRole)
    {
        switch (index.column())
        {
        case ColChecked:
            it.checked = (value.toInt() == Qt::Checked);
            break;
        case ColContent:
            it.content = value.toString();
            break;
        case ColDescription:
            it.description = value.toString();
            break;
        case ColOrder:
            it.order = value.toInt();
            break;
        case ColDelayMs:
            it.delayMs = value.toInt();
            break;
        default:
            return false;
        }

        emit itemsChanged();
        return true;
    }

    return false;
}

Qt::ItemFlags MsgListModel::flags(const QModelIndex& index) const
{
    Qt::ItemFlags f = QAbstractTableModel::flags(index);

    if (index.column() == ColChecked)
    {
        f |= Qt::ItemIsUserCheckable;
    }
    else
    {
        f |= Qt::ItemIsEditable;
    }

    return f;
}

QVariant MsgListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation != Qt::Horizontal || role != Qt::DisplayRole)
        return QVariant();

    switch (section)
    {
    case ColChecked:
        return "";
    case ColContent:
        return tr("Data");
    case ColDescription:
        return tr("Desc");
    case ColOrder:
        return tr("Order");
    case ColDelayMs:
        return tr("Delay(ms)");
    }

    return QVariant();
}

void MsgListModel::addItem(const MsgItem& item)
{
    beginInsertRows(QModelIndex(), m_items.count(), m_items.count());
    m_items.append(item);
    endInsertRows();
}

void MsgListModel::insertItem(int row, const MsgItem& item)
{
    if (row < 0 || row > m_items.count())
        row = m_items.count();

    beginInsertRows(QModelIndex(), row, row);
    m_items.insert(row, item);
    endInsertRows();
}

void MsgListModel::removeItem(int row)
{
    if (row < 0 || row >= m_items.count())
        return;

    beginRemoveRows(QModelIndex(), row, row);
    m_items.removeAt(row);
    endRemoveRows();
}

void MsgListModel::moveUp(int row)
{
    if (row <= 0 || row >= m_items.count())
        return;

    beginMoveRows(QModelIndex(), row, row, QModelIndex(), row - 1);
    m_items.move(row, row - 1);
    endMoveRows();
}

void MsgListModel::moveDown(int row)
{
    if (row < 0 || row >= m_items.count() - 1)
        return;

    beginMoveRows(QModelIndex(), row, row, QModelIndex(), row + 2);
    m_items.move(row, row + 1);
    endMoveRows();
}

MsgItem MsgListModel::getItem(int row) const
{
    if (row >= 0 && row < m_items.count())
        return m_items[row];
    return MsgItem();
}

QList<MsgItem> MsgListModel::getCheckedItemsSorted() const
{
    QList<MsgItem> checked;

    for (int i = 0; i < m_items.count(); ++i)
    {
        if (m_items[i].checked)
            checked.append(m_items[i]);
    }

    std::sort(checked.begin(), checked.end(), [](const MsgItem& a, const MsgItem& b) {
        return a.order < b.order;
    });

    return checked;
}

bool MsgListModel::loadFromMd(const QString& filepath)
{
    QList<MsgItem> items = MsgItem::loadFromMd(filepath);

    beginResetModel();
    m_items = items;
    endResetModel();

    return true;
}

bool MsgListModel::saveToMd(const QString& filepath)
{
    return MsgItem::saveToMd(filepath, m_items);
}

void MsgListModel::renumberOrders()
{
    for (int i = 0; i < m_items.count(); ++i)
    {
        m_items[i].order = i + 1;
    }
    emit itemsChanged();
}
