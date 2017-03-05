#ifndef _QAVEVENT_H_
#define _QAVEVENT_H_

#include <QEvent>

class QAvEvent:public QEvent
{
public:
	typedef enum{
		QAvEvent_Start = QEvent::User,
		QAvEvent_SearchDevice,
		QAvEvent_MaxWindows,
		QAvEvent_MaxVideoAreaWindows,

		QAvEvent_ShowMainMenum = QEvent::User + 100,
		QAvEvent_MainMenuDevices_Click,
		QAvEvent_MainMenuSystemSet_Click,
		QAvEvent_MainMenuAlarm_Click,
		QAvEvent_MainMenuSysTools_Click,
		QAvEvent_MainMenuRecord_Click,
		QAvEvent_MainMenuIntelligenceAlgorthm_Click,
		QAvEvent_MainMenuSystemInfo_Click,
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



