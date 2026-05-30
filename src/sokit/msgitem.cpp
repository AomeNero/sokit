#include "msgitem.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QTextCodec>

QString MsgItem::toLine() const
{
    QString chk = checked ? "1" : "0";
    QString cnt = content;
    cnt.replace("|", "\\|");
    QString dsc = description;
    dsc.replace("|", "\\|");

    return QString("%1|%2|%3|%4|%5")
        .arg(chk)
        .arg(cnt)
        .arg(dsc)
        .arg(order)
        .arg(delayMs);
}

MsgItem MsgItem::fromLine(const QString& line)
{
    MsgItem result;

    QString trimmed = line.trimmed();
    if (trimmed.isEmpty() || trimmed.startsWith("#"))
        return result;

    QStringList parts = trimmed.split("|");
    if (parts.size() >= 5)
    {
        result.checked = (parts[0] == "1");
        result.content = parts[1];
        result.content.replace("\\|", "|");
        result.description = parts[2];
        result.description.replace("\\|", "|");
        result.order = parts[3].toInt();
        result.delayMs = parts[4].toInt();
    }

    return result;
}

QList<MsgItem> MsgItem::loadFromMd(const QString& filepath)
{
    QList<MsgItem> items;

    QFile file(filepath);
    if (!file.exists())
        return items;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file for reading:" << filepath;
        return items;
    }

    QTextStream in(&file);
    in.setCodec(QTextCodec::codecForName("UTF-8"));

    while (!in.atEnd())
    {
        QString line = in.readLine();
        MsgItem it = MsgItem::fromLine(line);
        // Only add non-empty items (skip empty lines and comments)
        if (!line.trimmed().isEmpty() && !line.trimmed().startsWith("#"))
        {
            items.append(it);
        }
    }

    file.close();
    return items;
}

bool MsgItem::saveToMd(const QString& filepath, const QList<MsgItem>& items)
{
    QFile file(filepath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open file for writing:" << filepath;
        return false;
    }

    QTextStream out(&file);
    out.setCodec(QTextCodec::codecForName("UTF-8"));

    out << "# Sokit Message List\n";
    out << "# Format: checked|content|description|order|delay_ms\n";
    out << "# checked: 1=checked, 0=unchecked\n\n";

    for (int i = 0; i < items.size(); ++i)
    {
        out << items[i].toLine() << "\n";
    }

    file.close();
    return true;
}
