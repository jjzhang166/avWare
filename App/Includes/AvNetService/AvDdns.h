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
#ifndef _AV_DDNS_H_
#define _AV_DDNS_H_
#include "Apis/AvWareType.h"

typedef enum
{
	DDNS_3322_TYPE,
}DDNS_TYPE;

class CAvDdns
{
public:	
	CAvDdns();
	~CAvDdns();

	av_bool Start();
	av_bool Stop();
	
	av_bool SetConf(std::string server, std::string usrname, std::string passwd);

private:
	std::string m_Server;
	std::string m_Usrname;
	std::string m_Passwd;
	DDNS_TYPE   m_DdnsType;
};








#endif