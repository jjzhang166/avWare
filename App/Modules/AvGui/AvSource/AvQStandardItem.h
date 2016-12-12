#ifndef _AVQSTANDARDITEM_H_
#define _AVQSTANDARDITEM_H_


#include <QStandardItem>

class CAvQStandardItem :public QStandardItemModel
{
public:
	CAvQStandardItem();
	~CAvQStandardItem();
	CAvQStandardItem(const QString &text);
	CAvQStandardItem(const CAvQStandardItem &other);
	CAvQStandardItem &operator=(const CAvQStandardItem &other);
	virtual bool operator<(const QStandardItem &other) const;


};












#endif