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
#include "AvNetService/AvDhcp.h"

CAvDhcp::CAvDhcp()
{
}

CAvDhcp::~CAvDhcp()
{
}



av_bool CAvDhcp::Start()
{
	return av_true;
}
av_bool CAvDhcp::Stop()
{
	return av_true;
}

av_bool CAvDhcp::GetResult(std::string &IpAddr, std::string &SubMask, std::string &GateWay, std::string &Dns1, std::string &Dns2)
{
	return av_true;
}