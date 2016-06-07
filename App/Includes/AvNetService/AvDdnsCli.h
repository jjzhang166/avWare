/*******************************************************************
|  Copyright(c) 2015-2016 Graceport Technology Development Co.,Ltd
|  All rights reserved.
|
|  版本: 1.0
|  作者: ironman [ironman@graceport.cn]
|  日期: 2016年6月6日
|  说明:
|
|  版本: 1.1
|  作者:
|  日期:
|  说明:
******************************************************************/
#ifndef __AV_DDNSCLI_H__
#define __AV_DDNSCLI_H__

#include "AvThread/AvTimer.h"
#include "AvConfigs/AvConfigNetService.h"

class CAvDdnsCli : public CTimer
{
public:
	PATTERN_SINGLETON_DECLARE(CAvDdnsCli);
private:
	CAvDdnsCli();
	virtual ~CAvDdnsCli();

public:
	av_bool Start();
	av_bool Stop();
private:
	av_void OnTime();
	av_bool DdnsUpdateConfig();
	av_void AvConfigDdns(CAvConfigNetDdns* obj, int &num);

	int m_AvConfigChanged;
	CMutex m_Mutex;
	CAvConfigNetDdns m_AvConfigsDdns;

};

#define g_AvDdnsCli (*CAvDdnsCli::instance())

#endif /*__AV_DDNSCLI_H__*/