#ifndef _QAVEVENT_H_
#define _QAVEVENT_H_

#include <QEvent>


class QAvEvent:public QEvent
{
public:
	typedef enum{
		QAvEvent_Start = QEvent::User,
		QAvEvent_SearchDevice,
	}QAvEventType;

public:
	QAvEvent(QAvEventType EType);
	QAvEvent(QAvEvent &AvEvent);
	~QAvEvent();

	QAvEvent &operator = (const QAvEvent &AvEvent);
	bool			FillInUsrData(const char *usrdata, const int datalen, int subType);
	char *			UsrData();
	int				UsrDataLen();
	bool			UsrData(char *usrdata, int &datalen, int &subType);
	int				SubType();
	QAvEventType	AvEventType();
	
private:
	char *			m_usrdata;
	int				m_usrdatalen;
	int				m_usrdatasize;
	int				m_subType;
	QAvEventType	m_AvEventType;
};










#endif



