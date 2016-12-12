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
#ifndef _AVNETSERVICE_H_
#define _AVNETSERVICE_H_
#include "Apis/AvWareCplusplus.h"
#include "AvConfigs/AvConfigNetService.h"
#include "AvThread/AvThread.h"
#include "CAvObject.h"
#include "AvNetService/AvRtmp.h"


class CAvNetService :public CThread
{
public:
	SINGLETON_DECLARE(CAvNetService);

	av_bool Initialize();
	av_bool Start();
	av_bool Stop();
	av_bool Restart();

private:
	CAvNetService();
	~CAvNetService();

private:
	av_void OnConfigRtmpModify(CAvConfigRtmp *configRtmp, int &result);

private:
	void ThreadProc();

private:
	CAvConfigRtmp				m_ConfigRtmp;
	std::map<av_u32, AvRtmp*>	m_AvRtmpMap;
};
#define g_AvNetService (*CAvNetService::instance())

#endif
