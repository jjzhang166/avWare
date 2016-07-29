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
#include "AvDevice/AvDevice.h"
#include "AvConfigs/AvConfigNetService.h"


SINGLETON_IMPLEMENT(CAvDevice);


C_DeviceFactoryInfo CAvDevice::m_FactoryInfo;
std::string CAvDevice::m_SStartGUID;
av_bool CAvDevice::Initialize()
{
	av_msg("%s Started\n", __FUNCTION__);
	AvSystemInit();
	return av_true;
}
av_bool CAvDevice::InitializeConfigs()
{
	Start();
	return av_true;
}
CAvDevice::~CAvDevice()
{

}
CAvDevice::CAvDevice()
{

}


CMutex CAvDevice::m_SEnvMutex;
std::map<std::string, std::string> CAvDevice::m_SEnv;
av_bool CAvDevice::GetEnv(std::string &key, std::string &value)
{
	CGuard m(m_SEnvMutex);
	if (key.size() == 0){
		return av_false;
	}
	if (m_SEnv[key].size() == 0){
		value.clear();
		return av_false;
	}
	value = m_SEnv[key];
	return av_true;
}
av_bool CAvDevice::SetEnv(std::string &key, std::string &value)
{
	CGuard m(m_SEnvMutex);
	if (key.size() == 0){
		return av_false;
	}
	m_SEnv[key] = value;
	if (value == std::string("")){
		std::map<std::string, std::string>::iterator i;
		i = m_SEnv.find(key);
		if (i != m_SEnv.end()){
			m_SEnv.erase(i);
		}
	}
	return av_true;
}

#include "AvConfigs/AvConfigCapture.h"
av_bool CAvDevice::Start()
{
	//## bsp sys
	C_DeviceFactoryInfo FactoryInfo;
	GetDeviceInfo(FactoryInfo);


	AvGpioInit();
	{
		//## Set NetWork
		m_ConfigNetComm.Update();
		m_ConfigNetComm.Attach(this, (AvConfigCallBack)&CAvDevice::OnConfigsNetComm);

		C_NetCommCaps NetCommCaps;
		GetNetCommCaps(NetCommCaps);
		for (int i = 0; i < NetCommT_Nr; i++)
		{
			if (AvMask(i) & NetCommCaps.NetCommMask){
		
				ConfigNetComm &Formats = m_ConfigNetComm.GetConfig(i);
				C_NetCommAttribute NetCommAttr;
				NetCommAttr.Enable = Formats.Enable;
				switch ((E_NetComm_Type)i)
				{
				case NetCommT_Lan0:
				case NetCommT_Lan1:
					NetCommAttr.LanAttr = Formats.LanAttr;
					break;
				case NetCommT_Wireless:
					NetCommAttr.WirelessAttr = Formats.WirelessAttr.RouterLinkInfo[0];
					break;
				case NetCommT_SIM:
					NetCommAttr.SimAttr = Formats.SimAttr;
					break;

				default:
					break;
				}
				av_bool ret = avNetCommSet(Formats.type, &NetCommAttr);
				if (ret != av_true){
					av_error("Set %d NetCommDev Error\n", i);
				}
			}
		}

		

	}

	{
		//## Rtc Time 2 SysTime
		AvRtcInit();
		AvTimeRtc2System();
	}

	{
		av_timeval timeval;
		AvGetTimeOfDay(&timeval);
		char guid[128];
		srand((unsigned int)timeval.tv_usec);

		sprintf(guid, "%s-%04X-%04X-%08X", FactoryInfo.ProductMacAddr, timeval.tv_sec, timeval.tv_usec, rand());

		m_SStartGUID.clear();
		m_SStartGUID.assign(guid);
	}

	return av_true;
}
av_bool CAvDevice::Stop()
{
	AvSystemDeInit();

	AvTimeSystem2Rtc();



	return av_true;
}

av_bool CAvDevice::GetDspCaps(C_DspCaps &DspCaps)
{
	memset(&DspCaps, 0x00, sizeof(C_DspCaps));
	return AvGetDspCaps(&DspCaps);
}
av_bool CAvDevice::GetCaptureCaps(av_ushort Channel, C_EncodeCaps &EncodeCaps)
{
	memset(&EncodeCaps, 0x00, sizeof(C_EncodeCaps));
	av_bool ret = AvCaptureGetCaps((av_uchar)Channel, &EncodeCaps);
	return ret;
}
av_bool CAvDevice::GetDecodeCaps(av_ushort Channel)
{
	return av_true;
}
av_bool CAvDevice::GetSerialCaps(C_SerialCaps &SerialCaps)
{
	memset(&SerialCaps, 0x00, sizeof(C_SerialCaps));
	return AvSerialCaps(&SerialCaps);
}

av_bool CAvDevice::GetNetCommCaps(C_NetCommCaps &NetCommCaps)
{
	memset(&NetCommCaps, 0x00, sizeof(C_NetCommCaps));
	return avNetCommCaps(&NetCommCaps);
}
av_bool CAvDevice::GetImageCaps(av_ushort Channel, C_ImageQualityCaps &ImageCaps)
{
	memset(&ImageCaps, 0x00, sizeof(C_ImageQualityCaps));
	return AvImageCaps((av_char)Channel, &ImageCaps);
}
av_bool CAvDevice::GetCaputreInCaps(av_ushort Channel, C_CaptureInCaps &CaptureInCaps)
{
	memset(&CaptureInCaps, 0x00, sizeof(C_CaptureInCaps));
	return AvCaptureInCaps((av_char)Channel, &CaptureInCaps);
}
av_bool CAvDevice::GetACaptureCaps(E_AUDIO_CHL Chl, C_AudioCaps &AudioCaps)
{
	memset(&AudioCaps, 0x00, sizeof(C_AudioCaps));
	return AvACaptureCaps(Chl, &AudioCaps);
}
av_void CAvDevice::OnConfigsNetComm(CAvConfigNetComm *NetComm, int &result)
{
	int cmpRet = 0;
	C_NetCommAttribute NetCommAttr;
	for (int i = 0; i < NetCommT_Nr; i++)
	{
		ConfigNetComm &newConf = NetComm->GetConfig(i);
		ConfigNetComm &oldConf = m_ConfigNetComm.GetConfig(i);
		switch (i)
		{
		case NetCommT_Lan0:
			cmpRet = memcmp(&newConf.LanAttr, &oldConf.LanAttr, sizeof(ConfigLanAttribute));
			if (newConf.Enable != oldConf.Enable || cmpRet != 0){
				NetCommAttr.Enable = newConf.Enable;
				NetCommAttr.LanAttr = newConf.LanAttr;
				avNetCommSet(NetCommT_Lan0, &NetCommAttr);
				oldConf.Enable = newConf.Enable;
				oldConf.LanAttr = newConf.LanAttr;
			}
			break;
		case NetCommT_Lan1:
			cmpRet = memcmp(&newConf.LanAttr, &oldConf.LanAttr, sizeof(ConfigLanAttribute));
			if (newConf.Enable != oldConf.Enable || cmpRet != 0){
				NetCommAttr.Enable = newConf.Enable;
				NetCommAttr.LanAttr = newConf.LanAttr;
				avNetCommSet(NetCommT_Lan1, &NetCommAttr);
				oldConf.Enable = newConf.Enable;
				oldConf.LanAttr = newConf.LanAttr;
			}
			break;
		case NetCommT_Wireless:
			cmpRet = memcmp(&newConf.WirelessAttr, &oldConf.WirelessAttr, sizeof(ConfigWirelessAttribute));
			if (newConf.Enable != oldConf.Enable || cmpRet != 0){
				NetCommAttr.Enable = newConf.Enable;
				NetCommAttr.WirelessAttr = newConf.WirelessAttr.RouterLinkInfo[0];
				avNetCommSet(NetCommT_Wireless, &NetCommAttr);
				oldConf.Enable = newConf.Enable;
				oldConf.WirelessAttr = newConf.WirelessAttr;
			}
			break;
		case NetCommT_SIM:
			cmpRet = memcmp(&newConf.SimAttr, &oldConf.SimAttr, sizeof(ConfigSimAttribute));
			if (newConf.Enable != oldConf.Enable || cmpRet != 0){
				NetCommAttr.Enable = newConf.Enable;
				NetCommAttr.SimAttr = newConf.SimAttr;
				avNetCommSet(NetCommT_SIM, &NetCommAttr);
				oldConf.Enable = newConf.Enable;
				oldConf.SimAttr = newConf.SimAttr;
			}
			break;
		case NetCommT_BlueTooth:
			cmpRet = memcmp(&newConf.BlueToothAttr, &oldConf.BlueToothAttr, sizeof(ConfigBlueTooth));
			if (newConf.Enable != oldConf.Enable || cmpRet != 0){
				NetCommAttr.Enable = newConf.Enable;
				NetCommAttr.BluteToothAttr = newConf.BlueToothAttr;
				avNetCommSet(NetCommT_BlueTooth, &NetCommAttr);
				oldConf.Enable = newConf.Enable;
				oldConf.BlueToothAttr = newConf.BlueToothAttr;
			}
			break;
		default:
			break;
		}
	}
}
av_bool CAvDevice::Reboot()
{
	g_AvDevice.Stop();

	AvReboot();
	return av_true;
}

av_bool CAvDevice::GetSysTime(av_timeval &tv)
{
	return AvGetTimeOfDay(&tv);
}

av_bool CAvDevice::SetSysTime(av_timeval &tv)
{
	AvSetTimeofDay(&tv);
	return AvTimeSystem2Rtc();
}

av_bool CAvDevice::GetDeviceInfo(C_DeviceFactoryInfo &FactoryInfo)
{
	if (0 == strlen(m_FactoryInfo.SerialNumber)){
		if (av_true != AvGetDeviceInfo(&m_FactoryInfo)){
			memset(&m_FactoryInfo, 0x00, sizeof(C_DeviceFactoryInfo));
			sprintf(m_FactoryInfo.FactoryName, "%s", "aVware Factory");
			sprintf(m_FactoryInfo.SerialNumber, "%s", "AV-0001-0002-0003-0004");
			sprintf(m_FactoryInfo.ProductMacAddr, "%s", "00:1A:2B:3C:4D:5E");
			sprintf(m_FactoryInfo.HardWareVersion, "%s", "AV-HardWare-00-00-01");
			sprintf(m_FactoryInfo.ProductModel, "%s", "AV-Product-Model-A");
		}
	}
	FactoryInfo = m_FactoryInfo;
	return av_true;
}
av_bool CAvDevice::SetDeviceInfo(C_DeviceFactoryInfo &FactoryInfo)
{
	m_FactoryInfo = FactoryInfo;
	return AvSetDeviceInfo(&m_FactoryInfo);
}

av_uint CAvDevice::GetDeviceStartUp()
{
	return AvGetDeviceStartUp();
}
av_bool CAvDevice::SystemBeep()
{
	return AvSystemBeep(0xff, 100);
}

av_bool CAvDevice::SystemUpgrade(std::string UpgradeFilePath, av_uint &Progress)
{
	av_bool ret = AvSystemUpgradeFile(UpgradeFilePath.c_str(), &Progress);
	return ret;
}
av_bool CAvDevice::SystemUpgrade(av_uchar *ptr, av_uint length, av_uint &Progress)
{
	return av_true;
}


av_bool CAvDevice::GetMemLoadInfo(C_MemoryLoadInfo &MemLoadInfo)
{
	return AvGetMemLoadInfo(&MemLoadInfo);
}
av_bool CAvDevice::GetNetLoadInfo(C_NetLoadInfo &NetLoadInfo)
{
	return AvGetNetLoadInfo(&NetLoadInfo);
}
av_bool CAvDevice::GetCpuLoadInfo(C_CpuLoadInfo &CpuLoadInfo)
{
	return AvGetCpuLoadInfo(&CpuLoadInfo);
}

av_bool CAvDevice::GetStartUpGuid(std::string &guid)
{
	guid = m_SStartGUID;
	return av_true;
}