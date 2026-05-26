#ifndef __MSGLISTMODEL_H__
#define __MSGLISTMODEL_H__

#include <QAbstractTableModel>
#include <QList>
#include "msgitem.h"

class MsgListModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum Columns {
        ColChecked = 0,
        ColContent = 1,
        ColDescription = 2,
        ColOrder = 3,
        ColDelayMs = 4,
        ColCount = 5
    };

    explicit MsgListModel(QObject* parent = nullptr);
    ~MsgListModel();

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void addItem(const MsgItem& item);
    void insertItem(int row, const MsgItem& item);
    void removeItem(int row);
    void moveUp(int row);
    void moveDown(int row);

    MsgItem getItem(int row) const;

    QList<MsgItem> getCheckedItemsSorted() const;

    bool loadFromMd(const QString& filepath);
    bool saveToMd(const QString& filepath);
    void renumberOrders();

signals:
    void itemsChanged();

private:
    QList<MsgItem> m_items;
};

#endif // __MSGLISTMODEL_H__
