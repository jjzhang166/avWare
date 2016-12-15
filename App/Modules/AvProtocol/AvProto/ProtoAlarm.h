#ifndef _PROTO_ALARM_H_
#define _PROTO_ALARM_H_

#include "CAvObject.h"
#include "Signals.h"
#include "AvAlarm/AvAlarm.h"
class CProtoAlarmManage :public CAvObject
{
public:
	typedef TSignal1<CAvAlarm::AlmMsg &>::SigProc OnAlarmSigNalFunc;
	CProtoAlarmManage();
	~CProtoAlarmManage();
	SINGLETON_DECLARE(CProtoAlarmManage);

public:
	int AlarmSignal(CAvAlarm::AlmMsg &Alarm);

public:
	int Start(int Channel, CAvObject *obj, OnAlarmSigNalFunc proc);
	int End(int Channel, CAvObject *obj, OnAlarmSigNalFunc proc);
private:
	TSignal1<CAvAlarm::AlmMsg &> m_OnAlarmSignal[32];
};



#define g_ProtoAlarmManager (*CProtoAlarmManage::instance())



#endif


