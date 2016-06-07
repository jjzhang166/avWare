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
#ifndef _AVDEVICE_H_
#define _AVDEVICE_H_

#include "Apis/AvWareType.h"
#include "CObject.h"
#include "Apis/LibSystem.h"
#include "Apis/LibEncode.h"
#include "AvConfigs/AvConfigNetService.h"
class CAvDevice:public CObject
{
public:
	PATTERN_SINGLETON_DECLARE(CAvDevice);
	av_bool Initialize();
	

public:
	static av_bool GetDspCaps(C_DspCaps &DspCaps);
	static av_bool GetCaptureCaps(av_ushort Channel, C_EncodeCaps &EncodeCaps);
	static av_bool GetDecodeCaps(av_ushort Channel);
	static av_bool GetSerialCaps(C_SerialCaps &SerialCaps);
	static av_bool GetNetCommCaps(C_NetCommCaps &NetCommCaps);
	static av_bool GetImageCaps(av_ushort Channel, C_ImageQualityCaps &ImageCaps);
	static av_bool GetCaputreInCaps(av_ushort Channel, C_CaptureInCaps &CaptureInCaps);

private:
	av_void OnConfigsNetComm(CAvConfigNetComm *NetComm, int &result);
	CAvConfigNetComm m_ConfigNetComm;

public:
	static av_bool Reboot();
	static av_bool GetSysTime(av_timeval &tv);
	static av_bool SetSysTime(av_timeval &tv);
	static av_bool GetDeviceInfo(C_DeviceFactoryInfo &FactoryInfo);
	static av_bool SetDeviceInfo(C_DeviceFactoryInfo &FactoryInfo);
	
	static av_uint GetDeviceStartUp();
	static av_bool SystemBeep();

	static av_bool SystemUpgrade(std::string UpgradeFilePath, av_uint &Progress);
	static av_bool SystemUpgrade(av_uchar *ptr, av_uint length, av_uint &Progress);
	

	static av_bool GetMemLoadInfo(C_MemoryLoadInfo &MemLoadInfo);
	static av_bool GetNetLoadInfo(C_NetLoadInfo &NetLoadInfo);
	static av_bool GetCpuLoadInfo(C_CpuLoadInfo &CpuLoadInfo);

private:
	CAvDevice();
	~CAvDevice();
	av_bool Start();
	av_bool Stop();


private:
	static C_DeviceFactoryInfo m_FactoryInfo;

};

#define g_AvDevice (*CAvDevice::instance())


#endif