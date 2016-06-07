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
#ifndef _AV_NTP_H_
#define _AV_NTP_H_
#include "Apis/AvWareType.h"
#include "AvThread/AvTimer.h"
#include "AvThread/AvThread.h"

typedef enum _av_timezone_e
{
	av_GMT_W12, //GMT-12:00
	av_GMT_W11,
	av_GMT_W10,
	av_GMT_W9,
	av_GMT_W8,
	av_GMT_W7,
	av_GMT_W6,
	av_GMT_W5,
	av_GMT_W4,
	av_GMT_W3,
	av_GMT_W2,
	av_GMT_W1,
	av_GMT_0,	//GMT-0:00 
	av_GMT_E1,
	av_GMT_E2,
	av_GMT_E3,
	av_GMT_E4,
	av_GMT_E5,
	av_GMT_E6,
	av_GMT_E7,
	av_GMT_E8,
	av_GMT_E9,
	av_GMT_E10,
	av_GMT_E11,
	av_GMT_E12
}av_timezone_t;



class CAvNtp 
{
public:
	CAvNtp();
	~CAvNtp();
	av_bool Start();
	av_bool Stop();

	av_bool SetTimeZone(av_timezone_t timezone);
	av_bool SetNtpServer(std::string Server);
//	av_timezone_t GetCurTimeZone();	

private:
	std::string		m_NtpServer;
	av_timezone_t	m_TimeZone;
private:
//	C_ConfigNetNtp m_CfgNtp;
	int ConstructNtpPacket(char *packet);
	int GetNtpTime(int sk, struct addrinfo *addr, struct ntp_packet *ret_time);
};

#endif