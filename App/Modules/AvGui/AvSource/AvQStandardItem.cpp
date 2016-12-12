#include "AvQStandardItem.h"



CAvQStandardItem::CAvQStandardItem()
{
}

CAvQStandardItem::~CAvQStandardItem()
{
}

CAvQStandardItem::CAvQStandardItem(const QString &text) :QStandardItemModel(text)
{
}
CAvQStandardItem::CAvQStandardItem(const CAvQStandardItem &other) : QStandardItemModel(other)
{
}

CAvQStandardItem &CAvQStandardItem::operator=(const CAvQStandardItem &other)
{
	QStandardItemModel::operator=(other);
	return *this;
}

bool CAvQStandardItem::operator<(const QStandardItem &other) const
{
	const QVariant l = data(Qt::DisplayRole), r = other.data(Qt::DisplayRole);
	if (column() == other.column() && other.column() >= 1 && other.column() <= 15)
	{
		return l.toDouble() < r.toDouble();
	}

	return QStandardItemModel::operator<(other);
}


