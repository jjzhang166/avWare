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
#ifndef _AVALARM_H_
#define _AVALARM_H_

#include "Signals.h"
#include "Apis/AvWareCplusplus.h"
#include "AvThread/AvThread.h"
#include "Apis/LibSystem.h"
#include "AvThread/AvQmsg.h"
#include "Apis/LibLocalAlm.h"
#include "AvThread/AvTask.h"
#include "AvConfigs/AvConfigNetService.h"
#include "AvConfigs/AvConfigAlarm.h"
#include "Apis/AvWareStruct.h"

#define ALARM_QUEUE_MSG_NAME "Alm_Msg_Queue"

class CAvAlarm:public CThread
{
public:
	SINGLETON_DECLARE(CAvAlarm);
private:
	CAvAlarm();
	~CAvAlarm();
public:


public:
	typedef TSignal1<C_AlmMsg &>::SigProc OnAvAlarmSigNalFunc;
	av_bool Initialize();

	av_bool Start(CAvObject *obj, OnAvAlarmSigNalFunc proc);
	av_bool Stop(CAvObject *obj, OnAvAlarmSigNalFunc proc);

private:
	av_void ThreadProc();
	CAvQmsg m_AlarmMsg;

	TSignal1<C_AlmMsg &> m_AlarmSignal;


};



#define g_AvAlarm (*CAvAlarm::instance())



class CAvAlmTask:public CAvTask
{
public:
	CAvAlmTask();
	~CAvAlmTask();
	

	av_void TaskJob();
	av_bool SetAlmMsg(C_AlmMsg m_AlmMsg);
	av_bool SetalmSignal(TSignal1<C_AlmMsg &> *signal);
private:

	av_void OnSendEmail();
	av_void OnFtpSend();
	av_bool CheckEmailStartUp();
	av_void OnTestFtpUpload();
	//av_bool CheckTimeSection(C_WeekSpan *pWeekSpan);
	
	C_AlmMsg m_AlmMsg;
	TSignal1<C_AlmMsg &> *m_AlarmSignal;
};



#endif

