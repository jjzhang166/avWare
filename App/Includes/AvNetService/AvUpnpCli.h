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
#ifndef __AV_UPNPCLI_H__
#define __AV_UPNPCLI_H__

#include "AvThread/AvTimer.h"
#include "AvConfigs/AvConfigManagerNetWork.h"


class CAvUpnpCli : public CAvObject
{
public:
	SINGLETON_DECLARE(CAvUpnpCli);
private:
	CAvUpnpCli();
	virtual ~CAvUpnpCli();

public:
	av_bool Start();
	av_bool Stop();
private:
	av_void OnTime();
	av_bool UpnpUpdateConfig();
	av_void AvConfigUpnp(CAvConfigNetUpnp* obj, int &num);

	int m_AvConfigChanged;
	CMutex m_Mutex;
	CAvConfigNetUpnp m_AvConfigNetUpnp;

	char m_LocalIP[16];
	int m_InternalPort[10];
};

#define g_AvDdnsCli (*CAvUpnpCli::instance())


#endif /*__AV_UPNPCLI_H__*/
