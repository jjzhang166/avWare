/*******************************************************************
|  Copyright(c) 2015-2016 Graceport Technology Development Co.,Ltd
|  All rights reserved.
|
|  版本: 1.0
|  作者: Knight [knight@graceport.cn]
|  日期: 2017年3月31日
|  说明:
|
|  版本: 1.1
|  作者:
|  日期:
|  说明:
******************************************************************/


#ifndef _AVCONFIG_MANAGER_NETWORK_H_
#define _AVCONFIG_MANAGER_NETWORK_H_

#include "AvConfigs/AvConfigIndex.h"
#include "AvConfigs/AvConfigManagerBase.h"
#include "AvConfigs/AvConfigNetService.h"







// typedef C_LanAttribute			ConfigLanAttribute;
// typedef C_BlueToothAttribute	ConfigBlueTooth;
// typedef C_SimAttribute			ConfigSimAttribute;
// typedef C_WirelessAttribute		ConfigWirelessAttribute;
// 
// typedef struct {
// 	av_bool			Enable;		//打开或者关闭
// 	av_bool			Support;	//硬件上是否支持
// 	NetCommT		type;
// 	NetCommGetMode	GetMode;
// 	av_char			Ifrname[24];
// 	union
// 	{
// 		ConfigLanAttribute		LanAttr;
// 		ConfigBlueTooth			BlueToothAttr;
// 		ConfigWirelessAttribute	WirelessAttr;
// 		ConfigSimAttribute		SimAttr;
// 	};
// }ConfigNetComm;

typedef C_NetCommAttribute ConfigNetComm;
typedef struct {
	char HostName[128];
}ConfigDeviceInfo;


typedef TAvConfig<ConfigNetComm, 0, NetCommT_LAST> CAvConfigNetComm;
typedef TAvConfig<ConfigDeviceInfo, 0, 1>CAvConfigDeviceInfo;



class CAvConfigManagerNetWork :public CAvConfigManagerBase
{
public:
	SINGLETON_DECLARE(CAvConfigManagerNetWork);
	CAvConfigManagerNetWork();
	~CAvConfigManagerNetWork();
	virtual void Initialize();

	int RestoreDefaultAllGeneralConfig();

private:
	CAvConfigNetComm				m_ConfigNetComm;
	CAvConfigDeviceInfo				m_ConfigDeviceInfo;
};


#endif //_AVCONFIG_MANAGER_NETCAPTURE_H_
