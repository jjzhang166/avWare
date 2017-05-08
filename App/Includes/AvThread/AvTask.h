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
#ifndef _AV_TASK_H_
#define _AV_TASK_H_
#include "Apis/AvWareCplusplus.h"
#include "AvThread/AvThread.h"

class CAvTask:public CThread
{
//protected:
public:
	CAvTask();
	virtual ~CAvTask();
public:
	typedef av_void(CAvObject::*CAvTaskThreadProc)(void);
public:
	av_bool StartTask(av_bool bAtutoDestruct = av_true);
	av_bool SetTaskName(std::string TaskName); 
	av_bool StartTask(CAvObject *Obj, CAvTaskThreadProc proc, av_bool bAtutoDestruct = av_true);
protected:
	virtual av_void TaskJob(){};

private:
	void ThreadProc();

private:
	CAvObject *m_Obj;
	CAvTaskThreadProc m_Proc;
};



#endif
