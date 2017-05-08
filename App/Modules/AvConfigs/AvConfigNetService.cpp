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
