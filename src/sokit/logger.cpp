#include <QDate>
#include <QDir>
#include <QTextStream>
#include <QKeySequence>
#include <QApplication>
#include <QClipboard>
#include <QTreeWidget>
#include <QPlainTextEdit>
#include "toolkit.h"
#include "setting.h"
#include "logger.h"

#define SET_MAX_LOGITM  500
#define SET_BATCH_DELETE  500

Logger::Logger(QObject *parent)
: QObject(parent),m_chkWrite(0),m_treeOut(0),m_textOut(0)
{
}

Logger::~Logger()
{
	m_file.close();
}

void Logger::init(QTreeWidget* o, QCheckBox* w, QPlainTextEdit* d)
{
	m_cmlog.clear();
	m_cmtxt.clear();

	if (m_treeOut)
		m_treeOut->disconnect(this);

	if (m_textOut)
		m_textOut->disconnect(this);

	if (m_chkWrite)
		m_chkWrite->disconnect(this);

    m_treeOut = o;
	m_textOut = d;
    m_chkWrite = w;

	if (m_treeOut && m_textOut && m_chkWrite)
	{
		QList<QKeySequence> ks;
		ks << QKeySequence(Qt::CTRL + Qt::Key_D);

		QAction* copy = new QAction(tr("Copy"), this);
		copy->setShortcuts(QKeySequence::Copy);
		connect(copy, SIGNAL(triggered()), this, SLOT(copy()));

		QAction* clear = new QAction(tr("Clear"), this);
		clear->setShortcuts(ks);
		connect(clear, SIGNAL(triggered()), this, SIGNAL(clearLog()));

		QAction* all = new QAction(tr("Select All"), this);
		all->setShortcuts(QKeySequence::SelectAll);
        connect(all, SIGNAL(triggered()), m_textOut, SLOT(selectAll()));

		m_cmlog.addAction(copy);
		m_cmlog.addSeparator();
		m_cmlog.addAction(clear);

		m_cmtxt.addAction(copy);
		m_cmtxt.addSeparator();
		m_cmtxt.addAction(all);

		QPalette pal = m_textOut->palette();
		pal.setBrush(QPalette::Base, m_treeOut->palette().brush(QPalette::Window));
		m_textOut->setPalette(pal);

		m_treeOut->setContextMenuPolicy(Qt::CustomContextMenu);
		connect(m_treeOut, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ctxmenu(const QPoint&)));
		connect(m_treeOut, SIGNAL(itemSelectionChanged()), this, SLOT(syncOutput()));

		m_textOut->setContextMenuPolicy(Qt::CustomContextMenu);
		connect(m_textOut, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ctxmenu(const QPoint&)));
	}
}

void Logger::syncOutput()
{
	QList<QTreeWidgetItem*> list = m_treeOut->selectedItems();
	if (!list.isEmpty())
		m_textOut->setPlainText(list.first()->text(0));
	else
		m_textOut->clear();
}

void Logger::ctxmenu(const QPoint& pos)
{
	if (sender() == (QObject*)m_treeOut)
		m_cmlog.exec(m_treeOut->mapToGlobal(pos));
	else
		m_cmtxt.exec(m_textOut->mapToGlobal(pos));
}

void Logger::copy()
{
	// Check if tree has selection - copy from tree
	QList<QTreeWidgetItem*> list = m_treeOut->selectedItems();
	if (!list.isEmpty())
	{
		QTreeWidgetItem* item = list.first();
		QString text = item->text(0);

		// Check if this is a child item (HEX data format: [XX XX XX ...])
		if (item->parent() && text.startsWith('['))
		{
			// This is HEX data, extract pure hex values without brackets
			// Format: "[XX XX XX ...]"
			QString hex = text.mid(1, text.length() - 2); // Remove [ and ]
			QStringList hexList = hex.split(' ', QString::SkipEmptyParts);

			// Convert hex to binary, then to display format with [XX] for non-printable
			QString result;
			foreach (const QString& h, hexList)
			{
				bool ok;
				int val = h.toInt(&ok, 16);
				if (ok)
				{
					if (val >= 0x20 && val < 0x7F)
					{
						// Printable ASCII
						result += QChar(val);
					}
					else
					{
						// Non-printable: convert to [XX]
						result += '[';
						result += h.toUpper();
						result += ']';
					}
				}
			}

			QApplication::clipboard()->setText(result);
			return;
		}

		// Parent item: extract data part only (skip timestamp and title)
		// Format: "HH:mm:ss TITLE data..."
		// Client: "HH:mm:ss DAT <--- <len> data" or "HH:mm:ss DAT ---> <len> data"
		// Server: "HH:mm:ss DAT <--- [TCP] ip:port <len> data" or "HH:mm:ss DAT ---> [UDP] ip:port <len> data"
		// MSG:    "HH:mm:ss MSG message"

		// Find "<digits> " pattern from the end (e.g., "<10> ")
		// Search backwards to find the last occurrence which is the length marker
		int dataStart = -1;
		int pos = text.length() - 1;

		while (pos >= 0)
		{
			// Find '>' from current position going backwards
			int endBracket = text.lastIndexOf('>', pos);
			if (endBracket < 0) break;

			// Find matching '<' before this '>'
			int startBracket = text.lastIndexOf('<', endBracket - 1);
			if (startBracket < 0)
			{
				pos = endBracket - 1;
				continue;
			}

			// Check if content between < and > is all digits
			QString numStr = text.mid(startBracket + 1, endBracket - startBracket - 1);
			bool isNum = !numStr.isEmpty();
			for (int i = 0; i < numStr.length() && isNum; ++i)
			{
				if (!numStr[i].isDigit())
					isNum = false;
			}

			if (isNum && endBracket + 1 < text.length() && text[endBracket + 1] == ' ')
			{
				// Found "<digits> ", data starts after it
				dataStart = endBracket + 2;
				break;
			}

			pos = startBracket - 1;
		}

		if (dataStart >= 0)
		{
			text = text.mid(dataStart);
		}
		else
		{
			// No "<len> ", try to skip timestamp and title for MSG format
			int firstSpace = text.indexOf(' ');
			if (firstSpace >= 0)
			{
				int secondSpace = text.indexOf(' ', firstSpace + 1);
				if (secondSpace >= 0)
					text = text.mid(secondSpace + 1);
			}
		}

		// Convert non-printable characters to [XX] format
		QString result;
		result.reserve(text.size() * 3);
		for (int i = 0; i < text.size(); ++i)
		{
			ushort c = text[i].unicode();
			if (c >= 0x20 && c < 0x7F)
			{
				// Printable ASCII
				result += text[i];
			}
			else
			{
				// Non-printable: convert to [XX]
				result += '[';
				result += QString::number((c >> 4) & 0xF, 16).toUpper();
				result += QString::number(c & 0xF, 16).toUpper();
				result += ']';
			}
		}

		QApplication::clipboard()->setText(result);
	}
	else
	{
		// No tree selection - copy from text edit
		m_textOut->copy();
	}
}

const QString Logger::getLogFileName()
{
	int i = 0;
	while (2 > i++)
	{
		if (!m_dir.isEmpty())
		{
			QDir d;
			if (d.exists(m_dir) || d.mkpath(m_dir)) {
				i = 0;
				break;
			}
		}

        m_dir = Setting::path() + "/" + property(SET_SEC_DIR).toString();
	}

	return (i==2) ? QString() : m_dir + QDir::separator() + 
		QDate::currentDate().toString("yyyyMMdd.log");
}

void Logger::writeLogFile(const QString& info)
{
	if (!m_chkWrite->isChecked())
		return;

	m_file.close();
	m_file.setFileName(getLogFileName());

	if (m_file.open(QIODevice::Append|
					QIODevice::WriteOnly|
					QIODevice::Text))
	{
		QByteArray a(info.toUtf8());

		const char* d = a.data();
		for (int n=a.size(); n>0;)
		{
			int w = m_file.write(d, n);

			d += w;
			n -= w;
		}

		m_file.close();
	}
}

void Logger::clear()
{
	m_treeOut->clear();
	m_textOut->clear();
}

void Logger::output(const QString& info)
{
	output("MSG", info);
}

void Logger::output(const char* buf, uint len)
{
	output("DAT", buf, len);
}

void Logger::pack()
{
	if (m_treeOut->topLevelItemCount() > SET_MAX_LOGITM)
	{
		// Batch delete: remove SET_BATCH_DELETE items at once
		m_treeOut->model()->removeRows(0, SET_BATCH_DELETE);
	}

	m_treeOut->scrollToBottom();
}

QTreeWidgetItem* Logger::appendLogEntry(QTreeWidgetItem* p, const QString& t)
{
	QTreeWidgetItem* res = new QTreeWidgetItem(p);
	if (res)
	{
		res->setText(0, t);

		if (p)
		{
			p->addChild(res);
		}
		else
		{
			m_treeOut->addTopLevelItem(res);
			m_textOut->setPlainText(t);
		}
	}
	return res;
}

void Logger::output(const QString& title, const QString& info)
{
	QTreeWidgetItem* it = new QTreeWidgetItem(0);
	if (!it) return;

	QString lab(QTime::currentTime().toString("HH:mm:ss "));
	
	lab += title;
	lab += ' ';
	lab += info;

	appendLogEntry(0, lab);

	pack();

	lab += '\n';
	lab += '\n';

	writeLogFile(lab);
}

void Logger::output(const QString& title, const char* buf, quint32 len)
{
	QString lab(QTime::currentTime().toString("HH:mm:ss "));
	
	QTextStream out(&lab);

	out << title
		<< " <" << len << "> "
		<< TK::bin2ascii(buf, len);

	QString hex = TK::bin2hex(buf, len);

	QTreeWidgetItem* it = appendLogEntry(0, lab);
	if (it)
	{
		appendLogEntry(it, hex);

		pack();
	}

	out << '\n' << hex << '\n' << '\n';

	writeLogFile(lab);
}
