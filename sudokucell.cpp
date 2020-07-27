#include "sudokucell.h"

SudokuCell::SudokuCell()
{
	setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    setTextColor(Default);
	setDirty(false);
}

SudokuCell *SudokuCell::clone() const
{
    return new SudokuCell(*this);
}

QVariant SudokuCell::data(int role) const
{
	QVariant value = QTableWidgetItem::data(role);
    if(role == Qt::DisplayRole || role == Qt::EditRole)
	{
		bool ok = false;
        const int num = value.toInt(&ok);
        if(ok && num == 0)
        {
			return QVariant("");
        }
		else
        {
			return value;
        }
	}
	else
    {
		return value;
    }
}

void SudokuCell::setData(int role, const QVariant &value)
{
    if(role == Qt::EditRole)
	{
		setDirty(true);
		bool ok = false;
        const int num = value.toInt(&ok);
        if(!ok)
        {
			return QTableWidgetItem::setData(role, 0);
        }
        else if(num > 9 || num < 0)
        {
			return QTableWidgetItem::setData(role, 0);
        }
		else
        {
			return QTableWidgetItem::setData(role, value);
        }
	}
	else
    {
		return QTableWidgetItem::setData(role, value);
    }
}

bool SudokuCell::editEnabled() const
{
	Qt::ItemFlags f = flags();
	return f & Qt::ItemIsEditable;
}

void SudokuCell::setEditEnabled(bool enable)
{
    if(editEnabled() == enable)
    {
        return;
    }

    if(enable)
    {
		setFlags(Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled);
    }
	else
    {
		setFlags(Qt::ItemFlag(~Qt::ItemIsEditable));
    }
}

bool SudokuCell::dirty() const
{
    return m_dirty;
}

void SudokuCell::setDirty(bool dirty)
{
    m_dirty = dirty;
}

void SudokuCell::setTextColor(const TextColor color)
{
    bool enable = editEnabled();
	setEditEnabled(true);

	QFont font;
	font.setPixelSize(22);
    if(color == Default)
	{
		font.setBold(true);
		setForeground(Qt::black);
	}
    else if(color == Right)
	{
		font.setBold(false);
		setForeground(QColor(48, 175, 207));
	}
    else if(color == Wrong)
	{
		font.setBold(true);
		setForeground(QColor(234, 43, 21));
	}

	setFont(font);
    setEditEnabled(enable);
}

void SudokuCell::setBackgroundColor(const BackgroundColor color)
{
    bool enable = editEnabled();
	setEditEnabled(true);

    if(color == White)
    {
		setBackground(QColor(255, 255, 255));
    }
    else if(color == Grey)
    {
		setBackground(QColor(235, 235, 235));
    }
	else
    {
		setBackground(QColor(217, 179, 255));
    }

    setEditEnabled(enable);
}
