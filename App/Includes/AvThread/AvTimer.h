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
#ifndef _AVTIMER_H_
#define _AVTIMER_H_
#include "Apis/AvWareCplusplus.h"
#include "CAvObject.h"
#include "AvThread/AvThread.h"
#include "AvThread/AvThreadPool.h"
#include "Apis/AvWareLibDef.h"
/*
default CAvTimer
m_InterlValMSec = 0;
m_StartRunMSec = 0;
m_bContinual = av_true;
m_bBigTask = av_false;
*/

class AVWARE_API CAvTimer:public CThread
{
public:
	typedef av_void(CAvObject::*ONTIMER_PROC)(CAvTimer &Timer);
public:
	CAvTimer();
	~CAvTimer();
	SINGLETON_DECLARE(CAvTimer);

	av_void SetOnTimerProc(CAvObject *Obj, ONTIMER_PROC proc);
	av_void SetInterlvalMsec(av_u32 InterlValMSec);
	av_void SetStartRunMSec(av_u32 StartRunMSec);
	av_void SetContinual(av_bool bContinual = av_false);
	av_void SetBigTask(av_bool bBigTask = av_true);
	av_void StopTimer();

public:
	static av_bool StartTimer(CAvTimer &Timer);
	static av_bool StartTimer(av_u32 InterlValMSec, CAvObject *Obj, ONTIMER_PROC proc, av_bool bBigTask = av_false, av_u32 StartRunMSec = 0, av_bool bContinual = av_true);
	static av_bool StopTimer(CAvTimer &Timer);

	av_bool Initialize();
	av_void Dump();

private:
	av_void ThreadProc();
	av_bool m_bIsTimerManager;

private:
	ONTIMER_PROC		m_OnTimerProc;
	CAvObject			*m_Object;
	av_u32				 m_InterlValMSec;
	av_u32				 m_StartRunMSec;
	av_bool				 m_bContinual;
	av_bool				 m_bStop;
	av_u32				 m_TimerID;
	av_u32				 m_TimerLast;
	av_bool				 m_bBigTask;

private:
	static std::list<CAvTimer>		   m_TimerList;
	static CMutex					   m_MutexList;

};

#define g_AvTimer (*CAvTimer::instance())



#endif