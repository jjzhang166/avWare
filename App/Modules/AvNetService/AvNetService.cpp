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
#include "AvNetService/AvNetService.h"
#include "AvNetService/AvRtmp.h"
#include "AvNetService/AvOnvif.h"
#include "AvNetService/AvRtsp.h"
#include "AvNetService/AvWeb.h"
#include "AvNetService/AvDdnsCli.h"
#include "AvNetService/AvNtpCli.h"



PATTERN_SINGLETON_IMPLEMENT(CAvNetService);
CAvNetService::CAvNetService()
{
}
CAvNetService::~CAvNetService()
{

}
av_bool CAvNetService::Initialize()
{
	av_msg("%s Started\n", __FUNCTION__);
	av_bool ret = Start();
	return ret;
}

av_bool CAvNetService::Start()
{
	av_bool ret = av_false;
	ret = g_AvRtmp.Start();
	ret = g_AvOnvifSer.Start();
	ret = g_AvRtspServer.Start();
	ret = g_AvWebSer.Start();
	ret = g_AvDdnsCli.Start();
	ret = g_AvNtpCli.Start();
	return ret;
}
av_bool CAvNetService::Stop()
{
	av_bool ret = av_false;
	ret = g_AvRtmp.Stop();
	ret = g_AvOnvifSer.Stop();
	ret = g_AvRtspServer.Stop();
	ret = g_AvWebSer.Stop();
	return ret;
}
av_bool CAvNetService::Restart()
{
	av_bool ret = av_false;
	Stop();
	Start();
	return ret;
}

