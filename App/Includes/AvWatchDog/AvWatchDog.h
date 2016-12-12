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
#ifndef _AVWATCHDOG_H_
#define _AVWATCHDOG_H_

#include "Apis/AvWareCplusplus.h"
#include "CAvObject.h"
#include "AvThread/AvThread.h"
class CWatchDog :public CThread
{
public:
	SINGLETON_DECLARE(CWatchDog);
	
	av_bool Initialize();
	void ThreadProc();
	av_bool Open();
	av_bool Close();
	av_bool Feed();

private:
	CWatchDog();
	~CWatchDog();
};

#define g_AvWatchDog (*CWatchDog::instance())

#endif

