/*******************************************************************
|  Copyright(c) 2015-2016 Graceport Technology Development Co.,Ltd
|  All rights reserved.
|
|  版本: 1.0
|  作者: Knight [knight@graceport.cn]
|  日期: 2016年6月6日
|  说明:
|
|  版本: 1.1
|  作者:
|  日期:
|  说明:
******************************************************************/
#ifndef _PROTO_ALARM_H_
#define _PROTO_ALARM_H_

#include "ProtoStruct.h"

#ifdef __MOONPROTO_ //FOR VFWARE
#include "system/Object.h"
#include "system/SignalS.h"
#else
#include "Object.h"
#include "SignalS.h"
#endif


#ifdef PROTOSHARE_EXPORTS

#if defined (WIN32)

#if defined (USE_STATIC_LIB)
#define PROTOSHARE_API
#else
#define PROTOSHARE_API __declspec(dllexport)
#endif

#else //else define win32
#define PROTOSHARE_API __attribute__((visibility("default")))
#endif //end define win32

#else //else PROTOSHARE_EXPORTS

#if defined(WIN32)
#if defined (USE_STATIC_LIB)
#define PROTOSHARE_API
#else
#define PROTOSHARE_API __declspec(dllimport)
#endif

#else
#define PROTOSHARE_API
#endif

#endif //end PROTOSHARE_EXPORTS



class PROTOSHARE_API CProtoAlarmManage :public CAvObject
{
public:
	typedef TSignal1<C_Alarm &>::SigProc OnAlarmSigNalFunc;
	CProtoAlarmManage();
	~CProtoAlarmManage();
	SINGLETON_DECLARE(CProtoAlarmManage);

public:
	int AlarmSignal(C_Alarm &Alarm);

public:
	int Start(int Channel, CAvObject *obj, OnAlarmSigNalFunc proc);
	int End(int Channel, CAvObject *obj, OnAlarmSigNalFunc proc);
private:
	TSignal1<C_Alarm &> m_OnAlarmSignal[PROTO_MAX_CHANNEL];
};



#define g_ProtoAlarmManager (*CProtoAlarmManage::instance())



#endif


