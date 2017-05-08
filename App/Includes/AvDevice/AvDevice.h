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

#include "Apis/AvWareCplusplus.h"
#include "CAvObject.h"
#include "Apis/LibSystem.h"
#include "Apis/LibEncode.h"
#include "AvConfigs/AvConfigManagerNetWork.h"
#include "Signals.h"
#include "config.h"
#include "AvThread/AvTask.h"


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

#define EInitLuaFileName					"init.lua"
#define EKey_ConfigsPath					"avWare_configs_path"
#define EKey_WebRoot						"avWare_webroot"
#define EKey_WebIndex						"avWare_webindex"

#define EKey_DefaultIpaddr					"avWare_default_ipaddr"
#define EKey_DefaultNetMask					"avWare_default_netmask"
#define EKey_DefaultGateWay					"avWare_default_gateway"
#define EKey_DefaultDns1					"avWare_default_dns1"
#define EKey_DefaultDns2					"avWare_default_dns2"
#define EKey_DefaultHostName				"avWare_default_hostname"

#define EKey_DefaultWinRecordHomeDir		"avWare_default_WinRecordHomeDir"
#define EKey_DefaultLinRecordHomeDir		"avWare_default_LinRecordHomeDir"
#define EKey_DefaultRecordHomeDir			"avWare_default_RecordHomeDir"

#define EKey_Chip							"avWare_Chip"
#define EKey_Sensor							"avWare_Sensor"

#define EKey_Call2avWarePlatformConfigs		"avWarePlatformConfigs"

#define EKey_BvalidFactoryInfo				"avWare_bValid_FactoryInfo"



#define _D_AVDEVICE_REBOOT_DELAY			3
class CAvDevice:public CAvObject
{
public:
	SINGLETON_DECLARE(CAvDevice);
	CAvDevice();
	virtual ~CAvDevice();

public:
	av_bool Initialize();//这个函数一定要在加载配置表之前运行；
	av_bool InitializeLua();
	av_bool InitializeConfigs();//这个函数一定要放在加载完配置表之后 
public:
	typedef enum{
		Upgradeing,
		RebootSoon,
		ModifyNetDevice,
		ModifySysTime,
	}EDeviceStatus;
	typedef TSignal1 <EDeviceStatus>::SigProc SIG_PROC_DEVICESTATUS;

	av_bool AttachDeviceStatus(CAvObject *obj, SIG_PROC_DEVICESTATUS OnModifyStatus);
	av_bool DetachDeviceStatus(CAvObject *obj, SIG_PROC_DEVICESTATUS OnModifyStatus);
	av_bool DeviceSignal(EDeviceStatus _status);
private:
	TSignal1<EDeviceStatus> m_DeviceStatusSignal;


public:
	static av_bool		IsEmbeddedSystem();
	static std::string	GetSoftVersionString();
	static av_u32		GetSoftVersionU32();

public:
	static av_bool GetEnv(std::string key, std::string &value);
	static av_bool SetEnv(std::string key, std::string &value);

private:
	static CMutex m_SEnvMutex;
	static std::map<std::string, std::string> m_SEnv;

public:
	static av_bool GetDspCaps		(C_DspCaps &DspCaps);
	static av_bool GetEncodeCaps	(av_ushort Channel, C_EncodeCaps &EncodeCaps);
	static av_bool GetDecodeCaps	(av_ushort Channel);
	static av_bool GetSerialCaps	(C_SerialCaps &SerialCaps);
	static av_bool GetNetCommCaps	(C_NetCommCaps &NetCommCaps);
	static av_bool SetNetCommAttribute(NetCommT comt, C_NetCommAttribute &NetCommAttribute);
	static av_bool GetNetCommAttribute(NetCommT comt, C_NetCommAttribute &NetCommAttribute);
	static av_bool GetImageCaps(av_ushort Channel, C_ImageCaps &ImageCaps);
	static av_bool GetCaputreCaps	(av_ushort Channel, C_CaptureCaps &CaptureCaps);
public:
	static av_bool GetACaptureCaps	(E_AUDIO_CHL Chl,C_AudioCaps &AudioCaps);

private:
	av_void		OnTimer(CAvTimer &Timer);
	av_u32		m_TimerDeviceCount;
	av_u32		m_TimerDeviceRebootTime;

private:
	CAvTask		m_PlatformTimerTask;
	av_void		PlatformTimerTask();

private:
	av_void OnConfigsNetComm(CAvConfigNetComm *NetComm, int &result);
	static CAvConfigNetComm m_ConfigNetComm;

public:
	static av_bool Reboot();

	static av_bool GetSysTime(av_timeval &tv);
	static av_bool SetSysTime(av_timeval &tv);
	
	static av_bool TimeUtc2Local(av_u32 &utcSec, av_u32 &localSec);
	static av_bool TimeLocal2Utc(av_u32 &utcSec, av_u32 &localSec);

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
	static av_bool GetNetStatusInfo(C_NetStatusInfo &NetStatusInfo);
	static av_bool GetCpuMemStatusInfo(C_CpuMemStatusInfo &CpuMemStatusInfo);
	static av_bool GetStartUpGuid(std::string &guid);

private:
	av_bool Start();
	av_bool Stop();


private:
	static C_DeviceFactoryInfo	m_FactoryInfo;
	static std::string			m_SStartGUID;
};

#define g_AvDevice (*CAvDevice::instance())


#endif