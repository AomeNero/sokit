#ifndef __MSGITEM_H__
#define __MSGITEM_H__

#include <QString>
#include <QList>

struct MsgItem
{
    bool checked;
    QString content;
    QString description;
    int order;
    int delayMs;

    MsgItem() : checked(false), order(0), delayMs(0) {}
    MsgItem(bool chk, const QString& cnt, const QString& desc, int ord, int delay)
        : checked(chk), content(cnt), description(desc), order(ord), delayMs(delay) {}

    QString toLine() const;
    static MsgItem fromLine(const QString& line);

    static QList<MsgItem> loadFromMd(const QString& filepath);
    static bool saveToMd(const QString& filepath, const QList<MsgItem>& items);
};

#endif // __MSGITEM_H__
