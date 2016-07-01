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

template<> void ProcessValue<ConfigNetSerSmtp>(CAvConfigBase &av_conf, AvConfigValue &val, ConfigNetSerSmtp &config, int index, int diff)
{
	av_conf.Process("Enable",	val, config.enable, av_false);
	av_conf.Process("Port",		val, config.port,	25, 0, 65535);

	av_conf.Process("EncodeMode", val, config.save_mode, CAvSmtp::E_SMTP_EN_MODE::MAIL_ENNULL,
						CAvSmtp::E_SMTP_EN_MODE::MAIL_ENTLS, CAvSmtp::E_SMTP_EN_MODE::MAIL_ENNr);
	
	av_conf.Process("Account",	val, config.account,	"smtp.xxx.com");
	av_conf.Process("Password", val, config.password,	"\0");
	av_conf.Process("Sender",	val, config.sender,		"account@xxx.com");
	
	AvConfigValue &receiver_array = val["Receiver"];
	for (int i = 0; i < MAX_EMAIL_RECEIVER; i++) {
		av_conf.Process(i, receiver_array, config.receiver[i], "account@xxx.com");
	}
	av_conf.Process("Title", val, config.title, "Event");
}

template<> void ProcessValue<ConfigNetFtp>(CAvConfigBase &av_conf, AvConfigValue &val, ConfigNetFtp &config, int index, int diff)
{
	av_conf.Process("Enable", val, config.Enable, av_false);
	av_conf.Process("Port", val, config.Port, 21, 0, 65535);
	av_conf.Process("FtpServer", val, config.FtpServer, "\0");
	av_conf.Process("UsrName", val, config.UsrName, "\0");
	av_conf.Process("Passwd", val, config.Passwd, "\0");
	av_conf.Process("PathName", val, config.RemotePathName, "\0");
	av_conf.Process("Anonymity", val, config.Anonymity, av_false);
}

template<> void ProcessValue<ConfigNetDdns>(CAvConfigBase &av_conf, AvConfigValue &val, ConfigNetDdns &config, int index, int diff)
{
	av_conf.Process("Enable", val, config.Enable, av_false);
	av_conf.Process("Server", val, config.Server, "\0");
	av_conf.Process("UsrName", val, config.UsrName, "\0");
	av_conf.Process("Passwd", val, config.Passwd, "\0");
}

template<> void ProcessValue<ConfigNetUpnp>(CAvConfigBase &av_conf, AvConfigValue &val, ConfigNetUpnp &config, int index, int diff)
{
	av_conf.Process("Enable", val, config.Enable, av_false);
	av_conf.Process("Name", val, config.Name, "Upnp");
	av_conf.Process("TranMode", val, config.TranMode, CAvUpnp::UPNP_TRAN_MODE::UPNP_TCP, CAvUpnp::UPNP_TRAN_MODE::UPNP_TCP, CAvUpnp::UPNP_TRAN_MODE::UPNP_NR);
	AvConfigValue &localport = val["LocalPort"];
	for (int i = 0; i < PORT_MAP_NUM; i++)
		av_conf.Process(i, localport,  config.LocalPort[i], 25, 0, 65535);

	AvConfigValue &routerport = val["RouterPort"];
	for (int i = 0; i < PORT_MAP_NUM; i++)
		av_conf.Process(i, routerport,  config.RouterPort[i], 25, 0, 65535);
}

template<> void ProcessValue<ConfigNetNtp>(CAvConfigBase &av_conf, AvConfigValue &val, ConfigNetNtp &config, int index, int diff)
{
	av_conf.Process("Enable", val, config.Enable, av_false);
	av_conf.Process("Server", val, config.Server, "\0");
	av_conf.Process("Zone", val, config.Zone, 0, 0, 65535);
}

template<> void ProcessValue<ConfigNetComm>(CAvConfigBase &av_conf, AvConfigValue &val, ConfigNetComm &config, int index, int diff)
{

	C_NetCommCaps NetCommCaps;
	CAvDevice::GetNetCommCaps(NetCommCaps);

	E_NetComm_Mode MaxGetMode = NetCommGetMode_NR;
	E_NetComm_Mode MinGetMode = NetCommGetMode_MANUAL;

	av_findMaxMask(MaxGetMode, NetCommCaps.NetCommGetModeMask[index], E_NetComm_Mode);
	av_findMinMask(MinGetMode, NetCommCaps.NetCommGetModeMask[index], E_NetComm_Mode);

	if (AvMask(index) & NetCommCaps.NetCommMask){
		av_conf.Process("Support", val, (int &)config.Support, (int)av_true, (int)av_true, (int)av_true);
		av_conf.Process("GetMode", val, (int &)config.GetMode, (int)MinGetMode, (int)MinGetMode, (int)MaxGetMode);
	}
	else{
		av_conf.Process("Support", val, (int &)config.Support, (int)av_false, (int)av_false, (int)av_false);
		av_conf.Process("GetMode", val, (int &)config.GetMode, (int)NetCommGetMode_NR, (int)NetCommGetMode_NR, (int)NetCommGetMode_NR);
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
	case NetCommT_Lan0:
		if (~(AvMask(NetCommT_Lan0))&NetCommCaps.NetCommMask){
			av_conf.Process("Enable", val, (int &)config.Enable, (int)av_true, (int)av_false, (int)av_true);
		}
		else{
			av_conf.Process("Enable", val, (int &)config.Enable, (int)av_true, (int)av_true, (int)av_true);
		}
		av_conf.Process("NetCommType", val, (int &)config.type, (int)NetCommT_Lan0, (int)NetCommT_Lan0, (int)NetCommT_Lan0);
		av_conf.Process("Ifrname", val, config.Ifrname, "eth0");
		av_conf.Process("Dns1",		Attribute, config.LanAttr.Dns1, "8.8.8.8");
		av_conf.Process("Dns2",		Attribute, config.LanAttr.Dns2, "8.8.8.8");
		av_conf.Process("Gateway",	Attribute, config.LanAttr.Gateway, "192.168.1.1");
		av_conf.Process("Host",		Attribute, config.LanAttr.Host, "HostName");
		av_conf.Process("IpAddr",	Attribute, config.LanAttr.IpAddr, "192.168.1.90");
		av_conf.Process("Submask",	Attribute, config.LanAttr.Submask, "255.255.255.0");
		break;
	case NetCommT_Lan1:
		if (~(AvMask(NetCommT_Lan1))&NetCommCaps.NetCommMask){
			av_conf.Process("Enable", val, (int &)config.Enable, (int)av_true, (int)av_false, (int)av_true);
		}
		else{
			av_conf.Process("Enable", val, (int &)config.Enable, (int)av_true, (int)av_true, (int)av_true);
		}
		av_conf.Process("NetCommType", val, (int &)config.type, (int)NetCommT_Lan1, (int)NetCommT_Lan1, (int)NetCommT_Lan1);
		av_conf.Process("Ifrname", val, config.Ifrname, "eth1");
		av_conf.Process("Dns1", Attribute, config.LanAttr.Dns1, "8.8.8.8");
		av_conf.Process("Dns2", Attribute, config.LanAttr.Dns2, "8.8.8.8");
		av_conf.Process("Gateway", Attribute, config.LanAttr.Gateway, "192.168.1.1");
		av_conf.Process("Host", Attribute, config.LanAttr.Host, "HostName");
		av_conf.Process("IpAddr", Attribute, config.LanAttr.IpAddr, "192.168.1.91");
		av_conf.Process("Submask", Attribute, config.LanAttr.Submask, "255.255.255.0");
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
