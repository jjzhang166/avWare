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
#include <string.h>
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
	av_void OnConfigRtspModify(CAvConfigNetRtsp *Config, int &Result);
	av_void OnConfigP2pModify(CAvConfigNetP2p *Config, int &Result);
	av_void OnConfigUpnpModify(CAvConfigNetUpnp *Config, int &Result);
	av_void OnConfigNtpModify(CAvConfigNetNtp *Config, int &Result);
	av_void OnConfigDdnsModify(CAvConfigNetDdns *Config, int &Result);
	av_void OnConfigEmailModify(CAvConfigNetSmtp *Config, int &Result);
	av_void OnConfigFtpModify(CAvConfigNetFtp *Config, int &Result);
private:
	CAvConfigRtmp				m_ConfigRtmp;
	CAvConfigNetRtsp			m_ConfigRtsp;
	CAvConfigNetP2p				m_ConfigP2p;
	CAvConfigNetUpnp			m_ConfigUpnp;
	CAvConfigNetNtp				m_ConfigNtp;
	CAvConfigNetDdns			m_ConfigDdns;
	CAvConfigNetSmtp			m_ConfigSmtp;
	CAvConfigNetFtp				m_ConfigFtp;

private:
	void ThreadProc();

private:
	
	std::map<av_u32, AvRtmp*>	m_AvRtmpMap;
};
#define g_AvNetService (*CAvNetService::instance())


#endif
