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
#include "AvNetService/AvDdns.h"
#include "Ddns/NetDdns.h"

CAvDdns::CAvDdns()
{
}

CAvDdns::~CAvDdns()
{
}

av_bool CAvDdns::Start()
{
	ENU_DDNS iDdnsType;
	int nRet;
	
	if (m_DdnsType == DDNS_3322_TYPE)
	{
		iDdnsType = DDNS_3322;
	}

	nRet = DDNS_Enable(iDdnsType, (char *)(m_Server.c_str()), (char *)(m_Usrname.c_str()), (char *)(m_Passwd.c_str()));
	av_msg("DDNS_Enable nRet = %d\n",nRet);
	if (0 == nRet)
	{
		av_msg("ddns ok...\n");
		return av_true;
	}
	else if (1 == nRet)
	{
		av_msg("no be ddns_active...\n");
		return av_true;
	}
	else
	{
		av_msg("ddns fail...\n");
		return av_false;
	}
	
	return av_true;
}
av_bool CAvDdns::Stop()
{
	return av_true;
}

av_bool CAvDdns::SetConf(std::string server, std::string usrname, std::string passwd)
{
	if (0 == server.size() || 0 == usrname.size())
	{
		av_msg("input param is error...\n");
		return av_false;
	}

	m_Server = server;
	m_Usrname = usrname;
	m_Passwd = passwd;
	m_DdnsType = DDNS_3322_TYPE;
	
	return av_true;
}
