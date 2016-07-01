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
#include "Apis/AvWareType.h"
#include "CAvObject.h"
#include "AvThread/AvThread.h"
#include "AvThread/AvThreadPool.h"


class CTimer:public CThread
{
public:
	typedef enum{
		OnTimeing,/*执行定时器函数中*/
		UnOnTime,/*未到定时时间*/
		Deleteing,/*准备销毁*/
		UnStart,/*未开始启动定时器*/
	}Status;

public:
	CTimer(std::string TimerName = std::string("TimerNoName"));
	virtual ~CTimer();

public:
	friend class CTimerManage;
	av_bool SetTimerName(std::string name);
	std::string GetTimerName();
	av_bool StartTimer(av_u32 InterlvalMSec, av_u32 StartRunMSec = 0, av_bool IsContinual = av_false, av_bool bAutoDestruct = av_true);
	av_bool StopTimer();

	Status  GetStatus();

	virtual av_void OnTime() = 0;

private:
	av_bool StartTask();
	void ThreadProc();
private:
	av_long		m_StartRunMsec;
	av_long		m_RegisterMSec;
	av_long		m_TmLast;
	av_long		m_TmInterval;
	std::string m_TimerName;
	av_bool		m_Continual;
	av_bool		m_bAutoDestruct;
	Status		m_Status;

};


class CTimerManage :public CThread
{
public:
	SINGLETON_DECLARE(CTimerManage);
private:
	~CTimerManage();
	CTimerManage();
public:
	friend class CTimer;
	av_bool TimerAdd(CTimer *pTimer);
	av_bool TimerRemove(CTimer *pTimer);
	av_void Dump();


	av_bool Initialize();
private:
	av_void ThreadProc();
	std::list <CTimer *> m_TimerList;
	CMutex m_MutexList;
};

#define g_AvTimerManager (*CTimerManage::instance())


#endif