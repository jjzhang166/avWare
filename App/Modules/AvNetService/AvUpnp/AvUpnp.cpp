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
#include "AvNetService/AvUpnp.h"
#include "Upnp/upnplib.h"

CAvUpnp::CAvUpnp()
{

}
CAvUpnp::~CAvUpnp()
{

}

av_bool CAvUpnp::Start()
{
	int ret = 0;
	char mapping_name[20] = "avWare upnp";
	char router_ip[20];
	int  mapped_port;
	char mapping_protocol[5] = "TCP";

	switch (m_TranMode)
	{
	case UPNP_TCP:
		memcpy(mapping_protocol, "TCP", sizeof(mapping_protocol));
		break;
	case UPNP_UDP:
		memcpy(mapping_protocol, "UDP", sizeof(mapping_protocol));
		break;

	default:
		break;
	}


	ret = Ipcam_upnpc((char *)m_LocalAddr.c_str(), atoi(m_MapPort.c_str()), mapping_name, mapping_protocol, router_ip, &mapped_port);
	if (ret < 0)
	{
		av_msg("upnp start error...\n");
		return av_false;
	}
	else
	{
		av_msg("router_ip = [%s]\n", router_ip);
		av_msg("mapping_name = [%s]\n", mapping_name);
		av_msg("mapped_port = [%d]\n", mapped_port);
		m_ExternalPort = mapped_port;;
	}

	return av_true;
}
av_bool CAvUpnp::Start(std::string LocalIp, std::string MapPort, UPNP_TRAN_MODE utm)
{
	int ret = 0;
	char mapping_name[20] = "avWare upnp";
	char router_ip[20];
	int  mapped_port;
	char mapping_protocol[5] = "TCP";

	switch (utm)
	{
		case UPNP_TCP:
			memcpy(mapping_protocol, "TCP", sizeof(mapping_protocol));
			break;
		case UPNP_UDP:
			memcpy(mapping_protocol, "UDP", sizeof(mapping_protocol));
			break;

		default:
			break;
	}

	ret = Ipcam_upnpc((char *)LocalIp.c_str(), atoi(MapPort.c_str()), mapping_name, mapping_protocol, router_ip, &mapped_port);
	if (ret < 0)
	{
		av_msg("upnp start error...\n");
		return av_false;
	}
	else
	{
		printf("router_ip = [%s]\n", router_ip);
		printf("mapping_name = [%s]\n", mapping_name);
		printf("mapped_port = [%d]\n", mapped_port);
		m_ExternalPort = mapped_port;
	}

	return av_true;
}
av_bool CAvUpnp::Stop()
{
	return av_true;
}

av_bool CAvUpnp::SetConf(std::string LocalIp, std::string MapPort, UPNP_TRAN_MODE utm)
{
	if (0 == LocalIp.size() || 0 == MapPort.size())
	{
		av_msg("input param is error...\n");
		return av_true;
	}

	m_LocalAddr = LocalIp;
	m_MapPort = MapPort;
	m_TranMode = utm;

	return av_true;
}

av_bool CAvUpnp::GetExternalPort(int &ExternalPort)
{
	ExternalPort = m_ExternalPort;

	return av_true;
}
