/*******************************************************************
|  Copyright(c) 2015-2016 Graceport Technology Development Co.,Ltd
|  All rights reserved.
|
|  版本: 1.0
|  作者: ironman [ironman@graceport.cn]
|  日期: 2016年6月6日
|  说明:
|
|  版本: 1.1
|  作者:
|  日期:
|  说明:
******************************************************************/
#include "AvConfigs/AvConfigNetService.h"
#include "AvDevice/AvDevice.h"

template<> void ProcessValue<ConfigNetSmtp>(CAvConfigBase &av_conf, AvConfigValue &val, ConfigNetSmtp &config, int index, int diff)
{
	av_conf.Process("Enable",	val, config.bEnable, av_false);
	av_conf.Process("Port",		val, config.ServicePort,	25, 0, 65535);
	av_conf.Process("EncodeMode", val, (int &)config.EncodeType, (int)EmailEncodeType_NULL, (int)EmailEncodeType_None, (int)EmailEncodeType_Last);
	
	av_conf.Process("ServerAddress",	val, config.ServerAddress,	"smtp.email.com");
	av_conf.Process("UserName", val, config.UserName, "UserName");
	av_conf.Process("Password", val, config.PassWord,	"Password");
	av_conf.Process("Sender",	val, config.Sender,		"Sender@email.com");
	av_conf.Process("Title", val, config.Title, "Title");
	av_conf.Process("Receiver1", val, config.Receiver1, "Receiver1@email.com");
	av_conf.Process("Receiver2", val, config.Receiver2, "Receiver2@email.com");
}

template<> void ProcessValue<ConfigNetFtp>(CAvConfigBase &av_conf, AvConfigValue &val, ConfigNetFtp &config, int index, int diff)
{
	av_conf.Process("Enable", val, config.bEnable, av_false);
	av_conf.Process("Port", val, config.ServicePort, 21, 0, 65535);
	av_conf.Process("FtpServer", val, config.ServerAddress, "ftp://ftp.server.com");
	av_conf.Process("UsrName", val, config.UserName, "UsrName");
	av_conf.Process("Passwd", val, config.PassWord, "Passwd");
	av_conf.Process("RemotePath", val, config.RemotePath, "RemotePath");
}

template<> void ProcessValue<ConfigNetDdns>(CAvConfigBase &av_conf, AvConfigValue &val, ConfigNetDdns &config, int index, int diff)
{
	av_conf.Process("Enable", val, config.bEnable, av_false);
	av_conf.Process("Type", val, (int &)config.Type, DdnsType_Ddns3322, DdnsType_None, DdnsType_Last);
	av_conf.Process("Server", val, config.ServerAddress, "ddns.com");
	av_conf.Process("UsrName", val, config.UserName, "user");
	av_conf.Process("Passwd", val, config.PassWord, "passwd");
}

template<> void ProcessValue<ConfigNetUpnp>(CAvConfigBase &av_conf, AvConfigValue &val, ConfigNetUpnp &config, int index, int diff)
{

	av_conf.Process("bEnableHttpPort", val, config.bEnableHttpPort, av_false);
	av_conf.Process("bEnableMoonProtoPort", val, config.bEnableMoonProtoPort, av_false);
	av_conf.Process("bEnableRtspPort", val, config.bEnableRtspPort, av_false);
}

template<> void ProcessValue<ConfigNetNtp>(CAvConfigBase &av_conf, AvConfigValue &val, ConfigNetNtp &config, int index, int diff)
{
	av_conf.Process("Enable", val, config.bEnable, av_false);
	av_conf.Process("Server", val, config.ServerAddress, "ntp.server.com");
	av_conf.Process("Timezone", val, (int &)config.Timezone, TimeZone_UTC_E8, TimeZone_UTC_None+1, TimeZone_UTC_Last-1);
	av_conf.Process("UpdateInterval", val, config.UpdateInterval, 3*3600, 0, 0xffffffff);
}

template<> void ProcessValue<ConfigNetComm>(CAvConfigBase &av_conf, AvConfigValue &val, ConfigNetComm &config, int index, int diff)
{
	std::string DefaultString;
	C_NetCommCaps NetCommCaps;
	CAvDevice::GetNetCommCaps(NetCommCaps);

	NetCommT MaxGetMode = NetCommT_LAST;
	NetCommGetMode MinGetMode = NetCommGetMode_MANUAL;

	av_findMaxMask(MaxGetMode, NetCommCaps.NetCommGetModeMask[index], NetCommT);
	av_findMinMask(MinGetMode, NetCommCaps.NetCommGetModeMask[index], NetCommGetMode);

	if (AvMask(index) & NetCommCaps.NetCommMask){
		av_conf.Process("Support", val, (int &)config.Support, (int)av_true, (int)av_true, (int)av_true);
		av_conf.Process("GetMode", val, (int &)config.GetMode, (int)MinGetMode, (int)MinGetMode, (int)MaxGetMode);
	}
	else{
		av_conf.Process("Support", val, (int &)config.Support, (int)av_false, (int)av_false, (int)av_false);
		av_conf.Process("GetMode", val, (int &)config.GetMode, (int)NetCommGetMode_LAST, (int)NetCommGetMode_LAST, (int)NetCommGetMode_LAST);
	}
	AvConfigValue &Attribute = val["Attribute"];
	switch (index)
	{
	case NetCommT_BlueTooth:
		if (~(AvMask(NetCommT_BlueTooth))&NetCommCaps.NetCommMask){
			av_conf.Process("Enable", val, (int &)config.Enable, (int)av_true, (int)av_false, (int)av_true);
		}
		else{
			av_conf.Process("Enable", val, (int &)config.Enable, (int)av_true, (int)av_true, (int)av_true);
		}
		av_conf.Process("NetCommType", val, (int &)config.type, (int)NetCommT_BlueTooth, (int)NetCommT_BlueTooth, (int)NetCommT_BlueTooth);
		av_conf.Process("Ifrname", val, config.Ifrname, "BlueTooth");
		av_conf.Process("MarkCode", Attribute, config.BlueToothAttr.MarkCode, "0123456");
		break;
	case NetCommT_LAN0:
		if (~(AvMask(NetCommT_LAN0))&NetCommCaps.NetCommMask){
			av_conf.Process("Enable", val, (int &)config.Enable, (int)av_true, (int)av_false, (int)av_true);
		}
		else{
			av_conf.Process("Enable", val, (int &)config.Enable, (int)av_true, (int)av_true, (int)av_true);
		}
		av_conf.Process("NetCommType", val, (int &)config.type, (int)NetCommT_LAN0, (int)NetCommT_LAN0, (int)NetCommT_LAN0);
		av_conf.Process("Ifrname", val, config.Ifrname, "eth0");
		
		CAvDevice::GetEnv(EKey_DefaultDns1,		DefaultString);
		av_conf.Process("Dns1",		Attribute, config.LanAttr.Dns1,		DefaultString.c_str());
		CAvDevice::GetEnv(EKey_DefaultDns2,		DefaultString);
		av_conf.Process("Dns2",		Attribute, config.LanAttr.Dns2,		DefaultString.c_str());
		CAvDevice::GetEnv(EKey_DefaultGateWay,	DefaultString);
		av_conf.Process("Gateway",	Attribute, config.LanAttr.Gateway,	DefaultString.c_str());
		CAvDevice::GetEnv(EKey_DefaultHostName, DefaultString);
		av_conf.Process("Host",		Attribute, config.LanAttr.Host,		DefaultString.c_str());
		CAvDevice::GetEnv(EKey_DefaultIpaddr,	DefaultString);
		av_conf.Process("IpAddr",	Attribute, config.LanAttr.IpAddr,	DefaultString.c_str());
		CAvDevice::GetEnv(EKey_DefaultNetMask,	DefaultString);
		av_conf.Process("Submask",	Attribute, config.LanAttr.Submask,	DefaultString.c_str());
		break;
	case NetCommT_LAN1:
		if (~(AvMask(NetCommT_LAN1))&NetCommCaps.NetCommMask){
			av_conf.Process("Enable", val, (int &)config.Enable, (int)av_true, (int)av_false, (int)av_true);
		}
		else{
			av_conf.Process("Enable", val, (int &)config.Enable, (int)av_true, (int)av_true, (int)av_true);
		}
		av_conf.Process("NetCommType", val, (int &)config.type, (int)NetCommT_LAN1, (int)NetCommT_LAN1, (int)NetCommT_LAN1);
		av_conf.Process("Ifrname", val, config.Ifrname, "eth1");
		CAvDevice::GetEnv(EKey_DefaultDns1, DefaultString);
		av_conf.Process("Dns1",		Attribute, config.LanAttr.Dns1,		DefaultString.c_str());
		CAvDevice::GetEnv(EKey_DefaultDns2, DefaultString);
		av_conf.Process("Dns2",		Attribute, config.LanAttr.Dns2,		DefaultString.c_str());
		CAvDevice::GetEnv(EKey_DefaultGateWay, DefaultString);
		av_conf.Process("Gateway",	Attribute, config.LanAttr.Gateway,	DefaultString.c_str());
		CAvDevice::GetEnv(EKey_DefaultHostName, DefaultString);
		av_conf.Process("Host",		Attribute, config.LanAttr.Host,		DefaultString.c_str());
		CAvDevice::GetEnv(EKey_DefaultIpaddr, DefaultString);
		av_conf.Process("IpAddr",	Attribute, config.LanAttr.IpAddr,	DefaultString.c_str());
		CAvDevice::GetEnv(EKey_DefaultNetMask, DefaultString);
		av_conf.Process("Submask",	Attribute, config.LanAttr.Submask,	DefaultString.c_str());
		break;
	case NetCommT_Wireless:
		if (~(AvMask(NetCommT_Wireless))&NetCommCaps.NetCommMask){
			av_conf.Process("Enable", val, (int &)config.Enable, (int)av_true, (int)av_false, (int)av_true);
		}
		else{
			av_conf.Process("Enable", val, (int &)config.Enable, (int)av_true, (int)av_true, (int)av_true);
		}
		av_conf.Process("NetCommType",	val, (int &)config.type, (int)NetCommT_Wireless, (int)NetCommT_Wireless, (int)NetCommT_Wireless);
		av_conf.Process("Ifrname",		val, config.Ifrname, "wlan0");
		for (int i = 0; i < MAX_CONF_ROUTER_LINK; i++){
			av_conf.Process("SSID", Attribute[i], config.WirelessAttr.RouterLinkInfo[i].SSID, "shareWifi");
			av_conf.Process("Passwd", Attribute[i], config.WirelessAttr.RouterLinkInfo[i].Passwd, "********");
		}
		break;
	case NetCommT_SIM:
		if (~(AvMask(NetCommT_SIM))&NetCommCaps.NetCommMask){
			av_conf.Process("Enable", val, (int &)config.Enable, (int)av_true, (int)av_false, (int)av_true);
		}
		else{
			av_conf.Process("Enable", val, (int &)config.Enable, (int)av_true, (int)av_true, (int)av_true);
		}
		av_conf.Process("NetCommType",	val, (int &)config.type, (int)NetCommT_SIM, (int)NetCommT_SIM, (int)NetCommT_SIM);
		av_conf.Process("Ifrname",		val, config.Ifrname, "sim0");
		av_conf.Process("PhoneCode",	Attribute, config.SimAttr.PhoneCode, "********");
		break;

	default:
		assert(0);
		break;
	}

	
}

template<> void ProcessValue<ConfigRtmp>(CAvConfigBase &ConfBase, AvConfigValue &ConfValue, ConfigRtmp &config, int index, int diff)
{
	ConfBase.Process("RtmpAddress", ConfValue, config.PushServer, "rtmp://push.rtmpserver.com:1935");
	ConfBase.Process("PushChannel", ConfValue, config.PushChannel, index, index, index);
	ConfBase.Process("PushStream", ConfValue, config.PushStream, CHL_MAIN_T, CHL_MAIN_T, CHL_JPEG_T);
	ConfBase.Process("RtmpString", ConfValue, config.PushStrings, "sample");
	ConfBase.Process("bEnable", ConfValue, (av_int&)config.bEnable, av_false, av_false, av_true);
	ConfBase.Process("bAudio", ConfValue, (av_int&)config.bEnableAudio, av_false, av_false, av_true);
}


template<> void ProcessValue<ConfigNetP2p>(CAvConfigBase &ConfBase, AvConfigValue &ConfValue, ConfigNetP2p &config, int index, int diff)
{
	ConfBase.Process("bEnable", ConfValue, config.bEnable, av_false);
	ConfBase.Process("bUseLinkId", ConfValue, config.bUseLinkId, av_true);
	ConfBase.Process("PassWord", ConfValue, config.PassWord, "Passwd");
	ConfBase.Process("ServerAddress", ConfValue, config.ServerAddress, "p2pserver");
	ConfBase.Process("ServicePort", ConfValue, config.ServicePort, 0);
	ConfBase.Process("UserName", ConfValue, config.UserName, "UserName");
	ConfBase.Process("LinkId", ConfValue, config.LinkId, "sample");
	
}

template<> void ProcessValue<ConfigNetRtsp>(CAvConfigBase &ConfBase, AvConfigValue &ConfValue, ConfigNetRtsp &config, int index, int diff)
{
	ConfBase.Process("bEnable", ConfValue, config.bEnable, av_true);
	ConfBase.Process("ServicePort", ConfValue, config.ServicePort, 554, 1);

}
