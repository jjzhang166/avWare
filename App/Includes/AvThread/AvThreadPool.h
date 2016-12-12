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
#ifndef _AVTHREADPOOL_H_
#define _AVTHREADPOOL_H_
#include "Apis/AvWareCplusplus.h"
#include "AvThread/AvQueue.h"
#include "AvThread/AvThread.h"
#include "CAvObject.h"
#include "AvThread/AvTimer.h"
#include "Apis/AvWareLibDef.h"

/*
	线程池目前没法使用timer，因为timer依赖于线程池
*/
class AVWARE_API CThreadPool:public CAvObject
{
public:
	SINGLETON_DECLARE(CThreadPool);
private:
	CThreadPool();
	~CThreadPool();
public:
	friend void *ThreadRunBody(void *pdat);

	av_bool Run(CThread *obj);
	av_bool Stop(CThread *obj);
	av_void Dump();
	av_bool Initialize(av_u32 ThreadMaxNum = 8, av_bool Badd = av_true);
	//av_void	OnTimer(CAvTimer &Timer);
private:
	av_bool RcvTask(CThread *&Task);
	av_bool SndTask(CThread *Task);

private:
	static std::list <CThread *>	s_RuningList;
	static std::list <CThread *>	s_ThreadList;
	static CSemaphore				s_Sem;
	static CMutex					s_Mutex;
	static CQueue					s_Queue;
	static av_u32					s_MaxThreads;
	static av_bool					s_DynamicAdd;
	//static CAvTimer					s_Timer;
};

#define g_AvThreadPool (*CThreadPool::instance())


#endif