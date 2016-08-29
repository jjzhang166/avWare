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
#include "CAvObject.h"
#include "Apis/LibSystem.h"
#include "Apis/LibEncode.h"
#include "AvConfigs/AvConfigNetService.h"
#include "config.h"


#ifndef _AV_WARE_VERSION_MAJOR
#define _AV_WARE_VERSION_MAJOR	0
#endif

#ifndef _AV_WARE_VERSION_MINOR
#define _AV_WARE_VERSION_MINOR	0
#endif

#ifndef _AV_WARE_VERSION_PATCH
#define _AV_WARE_VERSION_PATCH	0
#endif

#ifndef _AV_WARE_VERSION_OEM
#define _AV_WARE_VERSION_OEM	""
#endif


//base、alpha、beta 、RC 、 release(R)
#ifndef _AV_WARE_VERSION_RUNTIME
#define _AV_WARE_VERSION_RUNTIME "beta"
#endif


#define EKey_ConfigsPath	"EKey_ConfigsPath"
#define EKey_WebRoot		"EKey_WebRoot"
#define EKey_WebIndex		"EKey_WebIndex"

class CAvDevice:public CAvObject
{
public:
	SINGLETON_DECLARE(CAvDevice);
	av_bool Initialize();//这个函数一定要在加载配置表之前运行；
	av_bool InitializeLua();
	av_bool InitializeConfigs();//这个函数一定要放在加载完配置表之后 

public:
	static std::string GetSoftVersionString();
	static av_u32      GetSoftVersionU32();

public:
	static av_bool GetEnv(std::string key, std::string &value);
	static av_bool SetEnv(std::string key, std::string &value);

private:
	static CMutex m_SEnvMutex;
	static std::map<std::string, std::string> m_SEnv;

public:
	static av_bool GetDspCaps		(C_DspCaps &DspCaps);
	static av_bool GetCaptureCaps	(av_ushort Channel, C_EncodeCaps &EncodeCaps);
	static av_bool GetDecodeCaps	(av_ushort Channel);
	static av_bool GetSerialCaps	(C_SerialCaps &SerialCaps);
	static av_bool GetNetCommCaps	(C_NetCommCaps &NetCommCaps);
	static av_bool GetImageCaps		(av_ushort Channel, C_ImageQualityCaps &ImageCaps);
	static av_bool GetCaputreInCaps	(av_ushort Channel, C_CaptureInCaps &CaptureInCaps);
public:
	static av_bool GetACaptureCaps	(E_AUDIO_CHL Chl,C_AudioCaps &AudioCaps);

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
public:
	static av_bool SystemUpgrade(std::string UpgradeFilePath);
	static av_bool SystemUpgrade(av_uchar *ptr, av_uint length);
	static av_u32  SystemUpgradeProgress();
private:
	static C_UpgradeProgress m_SystemUpgradeProgress;

public:
	static av_bool GetMemLoadInfo(C_MemoryLoadInfo &MemLoadInfo);
	static av_bool GetNetLoadInfo(C_NetLoadInfo &NetLoadInfo);
	static av_bool GetCpuLoadInfo(C_CpuLoadInfo &CpuLoadInfo);
	static av_bool GetStartUpGuid(std::string &guid);
private:
	CAvDevice();
	~CAvDevice();
	av_bool Start();
	av_bool Stop();


private:
	static C_DeviceFactoryInfo	m_FactoryInfo;
	static std::string			m_SStartGUID;
};

#define g_AvDevice (*CAvDevice::instance())


#endif