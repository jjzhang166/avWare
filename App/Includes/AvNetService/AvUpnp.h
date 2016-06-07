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
#ifndef _AV_UPNP_H_
#define _AV_UPNP_H_

#include "Apis/AvWareType.h"

class CAvUpnp
{
public:
	typedef enum{
		UPNP_TCP,
		UPNP_UDP,
		UPNP_ALL,
		UPNP_NR,
	}UPNP_TRAN_MODE;
	inline static std::string GetUpnpTranModeString(UPNP_TRAN_MODE e)
	{ 
		static std::string sUpnpTranModeStrings[UPNP_NR + 1] = 
		{"TCP", "UDP", "TCPUDP", "NR"};
		return sUpnpTranModeStrings[e];
	};
	CAvUpnp();
	~CAvUpnp();

	av_bool Start();
	av_bool Start(std::string LocalIp, std::string MapPort, UPNP_TRAN_MODE utm);
	av_bool Stop();

	av_bool SetConf(std::string LocalIp, std::string MapPort, UPNP_TRAN_MODE utm);
	av_bool GetExternalPort(int &ExternalPort);

private:
	std::string		m_LocalAddr;
	std::string		m_MapPort;
	int				m_ExternalPort;
	UPNP_TRAN_MODE	m_TranMode;
};




#endif