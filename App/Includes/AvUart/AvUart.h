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
#ifndef _AVUART_H_
#define _AVUART_H_
#include "Apis/AvWareCplusplus.h"
#include "Apis/LibSystem.h"
#include "AvConfigs/AvConfigDevice.h"
#include "CAvObject.h"
#include "Apis/AvWareStruct.h"
#include "Apis/AvSystemStruct.h"

class CAvUart:public CAvObject
{
public:
	SINGLETON_DECLARE(CAvUart)
public:
	av_bool Initialize();
	av_bool Write(av_uchar index, av_buf &buf);
	av_bool Read(av_uchar index, av_buf &buf);
public:
	av_bool PtzGetCaps(C_PtzCaps &PtzCaps);
	av_bool PtzGetProfile(C_PtzProfile &PtzProfile);
	av_bool PtzSetProfile(C_PtzProfile &PtzProfile);
	av_bool PtzSetCommand(C_PtzCmd &PtzCmd);
public:
	av_bool PtzGetAdvancedCaps(C_AdvancedSystemCaps &PtzAdvancedCaps);
	av_bool PtzGetAdvancedProfile(C_AdvancedSystemProfile &AdvancedSystemProfile);
	av_bool PtzSetAdvancedProfile(C_AdvancedSystemProfile &AdvancedSystemProfile);

private:
	av_void OnConfigPtzCameralensModify(CAvConfigPtzCameraLensProfile *Config, int &result);
	CAvConfigPtzCameraLensProfile m_ConfigPtzCameraLensProfile;
	



private:
	av_bool UartOpen(av_uchar index);
	av_bool UartClose(av_uchar index);
	av_bool UartCaps();
	av_bool SetAttr(av_uchar index, C_SerialAttr &Attr);
	av_void OnConfigsModify(CAvConfigUart *Config, int &result);

private:
	C_SerialCaps m_SerialCaps;

private:
	CAvConfigUart m_Config;

private:
	CAvUart();
	~CAvUart();
};

#define g_AvUart (*CAvUart::instance())






#endif