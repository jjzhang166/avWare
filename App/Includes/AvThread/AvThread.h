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
#ifndef _AVTHREAD_H_
#define _AVTHREAD_H_
#include <string>
#include "Apis/AvWareType.h"
#include "CAvObject.h"
#include "Apis/LibSystem.h"

#if defined(WIN32)
#else
#include <sys/prctl.h>
#endif
class CMutex
{
public:
	inline CMutex()
	{
		m_hMutex = NULL;
		m_hMutex = AvMutexCreate();
		assert(m_hMutex);
	};
	inline virtual ~CMutex()
	{
		AvMutexDestroy(m_hMutex);
	};
	inline av_bool Enter()
	{
		assert(m_hMutex);
		return AvMutexLock(m_hMutex);
	};
	inline av_bool Leave()
	{
		assert(m_hMutex);
		return AvMutexUnLock(m_hMutex);
	};
protected:
private:
	av_mutex *m_hMutex;
};


class CGuard
{
public:
	inline CGuard(CMutex& Mutex)
		:m_Mutex(Mutex)
	{
		m_Mutex.Enter();
	};

	inline ~CGuard()
	{
		m_Mutex.Leave();
	};
protected:
private:
	CMutex &m_Mutex;
};

// class CCond
// {
// public:
// 	inline CCond():m_hCond(0){
// 		
// 	}
// 	inline ~CCond(){
// 
// 	}
// 
// 	inline av_bool CondWait(){
// 
// 	}
// 	inline av_bool CondSignal(){
// 
// 	}
// 	inline av_bool Cond
// 
// 
// private:
// 	av_cond *m_hCond;
// };

class CSemaphore
{
public:
	inline CSemaphore(unsigned int dwInitialCount = 0)
		:m_hSemphore(0)
	{
		m_hSemphore = AvSemCreate();
		assert(m_hSemphore);
	};
	inline virtual ~CSemaphore()
	{
		assert(m_hSemphore);
		AvSemDestroy(m_hSemphore);
	};
	inline av_bool Pend()
	{
		assert(m_hSemphore);
		return AvSemWait(m_hSemphore);
	};
	inline int Post()
	{
		assert(m_hSemphore);
		return AvSemPost(m_hSemphore);
	};
protected:
private:
	av_sem *m_hSemphore;
};


#if defined(WIN32)
#define THREAD_SET_THREADNAME(TaskName) 
#else
#define THREAD_SET_THREADNAME(TaskName) \
	do \
		{\
		if (0 != prctl(PR_SET_NAME, TaskName)){\
			av_error("prctl set task name [%s] error \n", TaskName);\
		}\
	} while (0)
#endif

class CThread:public CAvObject
{
public:
	enum CThreadStatus{
		Uninitialized,
		Runing,
		Stop,
	};

	enum CThreadType{
		Thread,
		Task,
		Timer,
	};
	
public:
	CThread(const std::string threadName = std::string("NullName"));
	virtual ~CThread();
	virtual void ThreadProc() = 0;
	av_bool run();
	av_bool stop(av_bool wait = av_false);

	av_bool SetThreadName(const std::string threadName);
	av_bool GetThreadName(std::string &threadName);
	av_bool GetThreadId(av_thread_t &threadId);
	av_bool SetThreadId();
	av_bool isThreadRun();
	CThreadStatus GetThreadStatus();

	av_void SetAutoDestruct();
	av_bool GetAutoDestruct();
protected:
	volatile av_bool m_Loop;

private:
	CThreadStatus	m_ThreadStatus;
	av_thread_t		m_threadId;
	std::string		m_threadName;
	av_bool			m_bAutoDestruct;
};





#endif
