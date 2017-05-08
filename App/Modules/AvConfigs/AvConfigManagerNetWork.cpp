#include "AvConfigs/AvConfigManagerNetWork.h"
#include "AvDevice/AvDevice.h"


SINGLETON_IMPLEMENT(CAvConfigManagerNetWork);

CAvConfigManagerNetWork::CAvConfigManagerNetWork():
CAvConfigManagerBase(AvConfigFileNameNetWork, AvConfigFileNameNetWorkBak)
{

}
CAvConfigManagerNetWork::~CAvConfigManagerNetWork()
{

}
void CAvConfigManagerNetWork::Initialize()
{
	std::string ConfigPath;
	bool bLoadConfigFlag = false;
	av_bool abRet = CAvDevice::GetEnv(std::string(EKey_ConfigsPath), ConfigPath);
	if (abRet == av_true){
		SetAvConfigPath(ConfigPath, AvConfigFileNameNetWork, AvConfigFileNameNetWorkBak);
	}

	if (av_false == LoadConfigFromFile()) {
		av_warning("Load config from config file failed\n");
	}
	else{
		bLoadConfigFlag = true;
	}

	SetUpConfig("NetComm", m_ConfigNetComm, CONF_NET_COMM);
	SetUpConfig("DeviceInfo", m_ConfigDeviceInfo, CONF_DEVICE_INFO);

	if (bLoadConfigFlag == false){
		av_msg("bLoadConfigFlag == false\n");
		WriteFile();
	}
}

int CAvConfigManagerNetWork::RestoreDefaultAllGeneralConfig()
{
	return RestoreDefaultAll();
}

template<> void ProcessValue<ConfigDeviceInfo>(CAvConfigBase &ConfigBase, AvConfigValue &val, ConfigDeviceInfo &config, int index, int diff)
{
	ConfigBase.Process("HostName", val, config.HostName, "avWare");
}
template<> void ProcessValue<ConfigNetComm>(CAvConfigBase &ConfigBase, AvConfigValue &val, ConfigNetComm &config, int index, int diff)
{
	std::string DefaultString;
	C_NetCommCaps NetCommCaps;
	CAvDevice::GetNetCommCaps(NetCommCaps);

	NetCommGetMode MaxGetMode = NetCommGetMode_LAST;
	NetCommGetMode MinGetMode = NetCommGetMode_MANUAL;

	av_findMaxMask(MaxGetMode, NetCommCaps.NetCommGetModeMask[index], NetCommGetMode);
	av_findMinMask(MinGetMode, NetCommCaps.NetCommGetModeMask[index], NetCommGetMode);

	if (AvMask(index) & NetCommCaps.NetCommMask){
		ConfigBase.Process("Support", val, (int &)config.bSupport, (int)av_true, (int)av_true, (int)av_true);
		ConfigBase.Process("GetMode", val, (int &)config.mGetMode, (int)MinGetMode, (int)MinGetMode, (int)MaxGetMode);
	}
	else{
		ConfigBase.Process("Support", val, (int &)config.bSupport, (int)av_false, (int)av_false, (int)av_false);
		ConfigBase.Process("GetMode", val, (int &)config.mGetMode, (int)NetCommGetMode_LAST, (int)NetCommGetMode_LAST, (int)NetCommGetMode_LAST);
	}
	AvConfigValue &Attribute = val["Attribute"];
	switch (index)
	{
	case NetCommT_BlueTooth:
		if (~(AvMask(NetCommT_BlueTooth))&NetCommCaps.NetCommMask){
			ConfigBase.Process("Enable", val, (int &)config.bEnable, (int)av_true, (int)av_false, (int)av_true);
		}
		else{
			ConfigBase.Process("Enable", val, (int &)config.bEnable, (int)av_true, (int)av_true, (int)av_true);
		}
		ConfigBase.Process("iFrNameType", val, (int &)config.iFrNameType, (int)NetCommT_BlueTooth, (int)NetCommT_BlueTooth, (int)NetCommT_BlueTooth);
		ConfigBase.Process("MarkCode", Attribute, config.BluteToothAttr.MarkCode, "");
		ConfigBase.Process("SearchName", Attribute, config.BluteToothAttr.SearchName, "");
		break;
	case NetCommT_LAN0:
	case NetCommT_LAN1:
		if (~(AvMask(index))&NetCommCaps.NetCommMask){
			ConfigBase.Process("Enable", val, (int &)config.bEnable, (int)av_true, (int)av_false, (int)av_true);
		}
		else{
			ConfigBase.Process("Enable", val, (int &)config.bEnable, (int)av_true, (int)av_true, (int)av_true);
		}
		ConfigBase.Process("iFrNameType", val, (int &)config.iFrNameType, (int)index, (int)index, (int)index);
		
		CAvDevice::GetEnv(EKey_DefaultDns1, DefaultString);
		ConfigBase.Process("Dns1", Attribute, config.LanAttr.Dns1, DefaultString.c_str());
		CAvDevice::GetEnv(EKey_DefaultDns2, DefaultString);
		ConfigBase.Process("Dns2", Attribute, config.LanAttr.Dns2, DefaultString.c_str());
		CAvDevice::GetEnv(EKey_DefaultGateWay, DefaultString);
		ConfigBase.Process("Gateway", Attribute, config.LanAttr.Gateway, DefaultString.c_str());
		CAvDevice::GetEnv(EKey_DefaultIpaddr, DefaultString);
		ConfigBase.Process("IpAddr", Attribute, config.LanAttr.IpAddr, DefaultString.c_str());
		CAvDevice::GetEnv(EKey_DefaultNetMask, DefaultString);
		ConfigBase.Process("Submask", Attribute, config.LanAttr.Submask, DefaultString.c_str());
		break;

	case NetCommT_Wireless1:
	case NetCommT_Wireless:{
		if (~(AvMask(NetCommT_Wireless))&NetCommCaps.NetCommMask){
			ConfigBase.Process("Enable", val, (int &)config.bEnable, (int)av_true, (int)av_false, (int)av_true);
		}
		else{
			ConfigBase.Process("Enable", val, (int &)config.bEnable, (int)av_true, (int)av_true, (int)av_true);
		}
		ConfigBase.Process("iFrNameType", val, (int &)config.iFrNameType, (int)index, (int)index, (int)index);
		ConfigBase.Process("Mode", val, (int &)config.WirelessAttr.Mode, WirelessMode_AP, WirelessMode_AP, WirelessMode_STATION);
		ConfigBase.Process("WirelessPreferred", val, config.WirelessAttr.WirelessPreferred, 0, 0, MAX_CONF_ROUTER_LINK - 1);

		ConfigBase.Process("Dns1", Attribute, config.WirelessAttr.WirelessConf.Dns1, "");
		ConfigBase.Process("Dns2", Attribute, config.WirelessAttr.WirelessConf.Dns2, "");
		ConfigBase.Process("Gateway", Attribute, config.WirelessAttr.WirelessConf.Gateway, "");
		ConfigBase.Process("IpAddr", Attribute, config.WirelessAttr.WirelessConf.IpAddr, "");
		ConfigBase.Process("Submask", Attribute, config.WirelessAttr.WirelessConf.Submask, "");
		AvConfigValue &WirelessList = val["WirelessList"];
		for (int i = 0; i < MAX_CONF_ROUTER_LINK; i++){
			ConfigBase.Process("SSID", WirelessList[i], config.WirelessAttr.WirelessInfo[i].SSID, "");
			ConfigBase.Process("Passwd", WirelessList[i], config.WirelessAttr.WirelessInfo[i].Passwd, "");
		}
		
		AvConfigValue &WirelessApConf = val["WirelessApConf"];
		ConfigBase.Process("SSID", WirelessApConf, config.WirelessAttr.WirelessApConf.SSID, "avWareWirelessAp");
		ConfigBase.Process("Passwd", WirelessApConf, config.WirelessAttr.WirelessApConf.Passwd, "qwer1234");
	}
		break;
	case NetCommT_SIM:
	case NetCommT_SIM1:
		if (~(AvMask(index))&NetCommCaps.NetCommMask){
			ConfigBase.Process("Enable", val, (int &)config.bEnable, (int)av_true, (int)av_false, (int)av_true);
		}
		else{
			ConfigBase.Process("Enable", val, (int &)config.bEnable, (int)av_true, (int)av_true, (int)av_true);
		}
		ConfigBase.Process("iFrNameType", val, (int &)config.iFrNameType, (int)index, (int)index, (int)index);
		ConfigBase.Process("PhoneCode", Attribute, config.SimAttr.PhoneCode, "");
		break;

	default:
		assert(0);
		break;
	}
}