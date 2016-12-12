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
#include "AvWatchDog/AvWatchDog.h"
#include "Apis/AvWareCplusplus.h"
#include "Apis/LibSystem.h"

SINGLETON_IMPLEMENT(CWatchDog);

CWatchDog::CWatchDog() :CThread(__FUNCTION__)
{

}
CWatchDog::~CWatchDog()
{
}
av_bool CWatchDog::Initialize()
{
	av_msg("%s Started\n", __FUNCTION__);
	Open();
	CThread::ThreadStart();
	return av_true;
}

void CWatchDog::ThreadProc()
{
	while (m_Loop == av_true){
		av_msleep(1000 * 3);
		Feed();
	}
}


av_bool CWatchDog::Open()
{
	return AvWatchDogInit(5);
}
av_bool CWatchDog::Close()
{
	AvWatchDogDeInit();
	return CThread::ThreadStop();
}
av_bool CWatchDog::Feed()
{
	return AvWatchDogFeed();
}
