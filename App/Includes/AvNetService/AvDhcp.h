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
#ifndef _AV_DHCP_H_
#define _AV_DHCP_H_

#include "Apis/AvWareCplusplus.h"


class CAvDhcp
{
public:
	CAvDhcp();
	~CAvDhcp();

	av_bool Start();
	av_bool Stop();
	
	av_bool GetResult(std::string &IpAddr, std::string &SubMask, std::string &GateWay, std::string &Dns1, std::string &Dns2);

private:

};



#endif