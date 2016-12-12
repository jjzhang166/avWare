/*******************************************************************
|  Copyright(c) 2015-2016 Graceport Technology Development Co.,Ltd
|  All rights reserved.
|
|  版本: 1.0
|  作者: FootMan [FootMan@graceport.cn]
|  日期: 2016年6月6日
|  说明:
|
|  版本: 1.1
|  作者:
|  日期:
|  说明:
******************************************************************/
#ifndef _AVONVIF_H_
#define _AVONVIF_H_
#include "CAvObject.h"
#include "Apis/AvWareCplusplus.h"
#include "Onvif/onvifAPI.h"

class CAvOnvifSer
{
public:
	SINGLETON_DECLARE(CAvOnvifSer);
	CAvOnvifSer();
	~CAvOnvifSer();
public:
	av_bool Start();
	av_bool ReStart();
	av_bool Stop();

private:
	av_bool InitCbfun();
private:
	OnvifSerHandle_S m_OnvifSerHandle;
};

#define g_AvOnvifSer (*CAvOnvifSer::instance())



class CAvOnivfCli
{
public:
	CAvOnivfCli();
	~CAvOnivfCli();

	av_bool Start(std::string url);
	av_bool Stop();

private:
	
};



#endif

