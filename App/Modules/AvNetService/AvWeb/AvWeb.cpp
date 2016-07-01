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
#include "AvNetService/AvWeb.h"

SINGLETON_IMPLEMENT(CAvWebSer)

CAvWebSer::CAvWebSer()
{
}

CAvWebSer::~CAvWebSer()
{
}

av_bool CAvWebSer::Start()
{
	WebStart();
	return av_true;
}
av_bool CAvWebSer::Stop()
{
	WebStop();
	return av_true;
}



int CAvWebSer::WebCallBackProc(std::string InBuf, std::string &OutBuf)
{
	av_msg("CAvWebSer::WebCallBackProc\n");
	puts(InBuf.c_str());
	return 0;
}
