#include "QAvEvent.h"

#define QAVEVENT_MIN_SUR_BUFFER_LEN 1024
QAvEvent::QAvEvent(QAvEventType EType) : QEvent(Type(EType))
{
	QEvent::registerEventType(EType);
	m_usrdata = NULL;
	m_usrdatalen = 0;
	m_subType = 0;
	m_AvEventType = EType;
	m_usrdatasize = 0;
}
QAvEvent::QAvEvent(QAvEvent &AvEvent) : QEvent(Type(AvEvent.AvEventType()))
{

	m_usrdata = NULL;
	m_usrdatalen = 0;
	m_subType = 0;
	m_AvEventType = QAvEvent_SubMsgNULL;
	m_usrdatasize = 0;


	m_usrdatalen = AvEvent.UsrDataLen();
	m_subType = AvEvent.SubType();
	m_AvEventType = AvEvent.AvEventType();

	if (m_usrdatasize < AvEvent.UsrDataLen()){
		m_usrdatasize = AvEvent.UsrDataLen();
		if (m_usrdatasize < QAVEVENT_MIN_SUR_BUFFER_LEN){
			m_usrdatasize = QAVEVENT_MIN_SUR_BUFFER_LEN;
		}
		m_usrdata = new char[m_usrdatasize];
	}

	memcpy(m_usrdata, AvEvent.UsrData(), m_usrdatalen);
}

QAvEvent::~QAvEvent()
{
	if (NULL == m_usrdata){
		delete[] m_usrdata;
		m_usrdatalen = 0;
		m_subType = 0;
		m_usrdatasize = 0;
	}
}

QAvEvent &QAvEvent::operator = (const QAvEvent &AvEvent)
{
	if (this == &AvEvent){
		return *this;
	}
	this->m_usrdatalen = AvEvent.m_usrdatalen;
	this->m_subType = AvEvent.m_subType;
	this->m_AvEventType = AvEvent.m_AvEventType;
	if (NULL != AvEvent.m_usrdata){
		if (this->m_usrdatasize < AvEvent.m_usrdatalen){
			if (NULL != this->m_usrdata){
				delete[] this->m_usrdata;
			}
			this->m_usrdatasize = AvEvent.m_usrdatalen;
			if (this->m_usrdatasize < QAVEVENT_MIN_SUR_BUFFER_LEN){
				this->m_usrdatasize = QAVEVENT_MIN_SUR_BUFFER_LEN;
			}
			this->m_usrdata = new char[this->m_usrdatasize];
		}
		memcpy(this->m_usrdata, AvEvent.m_usrdata, AvEvent.m_usrdatalen);
	}

	return *this;
}

bool QAvEvent::FillInUsrData(const char *usrdata, const int datalen, int subType)
{

	if (m_usrdatasize < datalen){
		m_usrdatasize = datalen;
		if (m_usrdatasize < QAVEVENT_MIN_SUR_BUFFER_LEN){
			m_usrdatasize = QAVEVENT_MIN_SUR_BUFFER_LEN;
		}

		if (NULL != m_usrdata){
			delete[] m_usrdata;
		}
		m_usrdata = new char[m_usrdatasize];
	}

	memcpy(m_usrdata, usrdata, datalen);
	m_subType = subType;
	m_usrdatalen = datalen;
	return true;
}
char *QAvEvent::UsrData()
{
	return m_usrdata;
}
int QAvEvent::UsrDataLen()
{
	return m_usrdatalen;
}
bool QAvEvent::UsrData(char *usrdata, int &datalen, int &subType)
{
	if (NULL == m_usrdata){
		return false;
	}
	else{
		memcpy(usrdata, m_usrdata, m_usrdatalen);
		datalen = m_usrdatalen;
		subType = m_subType;
		return true;
	}
}

int   QAvEvent::SubType()
{
	return m_subType;
}

QAvEvent::QAvEventType	QAvEvent::AvEventType()
{
	return m_AvEventType;
}



