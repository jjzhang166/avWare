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
#include "AvNetService/AvUpnpCli.h"
#include "AvNetService/AvUpnp.h"


SINGLETON_IMPLEMENT(CAvUpnpCli)

CAvUpnpCli::CAvUpnpCli()
{
	m_AvConfigChanged = 0;
}

CAvUpnpCli::~CAvUpnpCli()
{

}

av_bool CAvUpnpCli::Start()
{
	int ret = 0;
	memcpy(m_LocalIP, CAvConfigNetComm::GetEnvConfig().LanAttr.IpAddr, 16);
	//memcpy(m_InternalPort, CAvConfigNetUpnp::GetEnvConfig().LocalPort, sizeof(m_InternalPort));
	
	m_AvConfigNetUpnp.Attach(this ,(AvConfigCallBack)&CAvUpnpCli::AvConfigUpnp);
	//StartTimer(1*1000, 5*1000, av_true, av_true);
	AvConfigUpnp(&m_AvConfigNetUpnp, ret);
	return av_true;
}

av_bool CAvUpnpCli::Stop()
{
	//StopTimer();
	m_AvConfigNetUpnp.Detach(this ,(AvConfigCallBack)&CAvUpnpCli::AvConfigUpnp);
	return av_true;
}

av_void CAvUpnpCli::OnTime()
{
	if (m_AvConfigChanged > 0)
	{
		m_Mutex.Enter();
		m_AvConfigChanged = 0;	
		m_Mutex.Leave();
		UpnpUpdateConfig();
	}
}

av_bool CAvUpnpCli::UpnpUpdateConfig()
{
#if 0
	int i = 0;
	int ret = 0;
	int upnp_status[Mapping_NR] = {0};

	m_AvConfigNetUpnp.Update();
	ConfigNetUpnp &upnp_config = m_AvConfigNetUpnp.GetConfig();

	if (upnp_config.bEnable == av_true)
	{
		CAvConfigNetComm AvConfigNetComm;
		AvConfigNetComm.Update();
		int mapped_port;
		if (strncmp(m_LocalIP, AvConfigNetComm.GetConfig().LanAttr.IpAddr, 15) != 0)
		{
			memcpy(m_LocalIP, AvConfigNetComm.GetConfig().LanAttr.IpAddr, 16);
			upnp_status[Mapping_RTSP] = 1;
			upnp_status[Mapping_Web] = 1;
			upnp_status[Mapping_SelfProtol] = 1;

		}
		for (i = Mapping_RTSP; i < Mapping_NR; i++)
		{
			if (m_InternalPort[i] != upnp_config.LocalPort[i])
			{
				m_InternalPort[i] = upnp_config.LocalPort[i];
				upnp_status[i] = 1;
			}
		}

		for (i = Mapping_RTSP; i < Mapping_NR; i++)
		{
			if (upnp_status[i] == 1)
			{
				CAvUpnp AvUpnp;
				std::string LocalPort;
				sprintf((char *)LocalPort.c_str(), "%d", upnp_config.LocalPort[i]);
				ret = AvUpnp.SetConf(m_LocalIP, LocalPort, (CAvUpnp::UPNP_TRAN_MODE)upnp_config.TranMode);
				if (ret == av_false)
				{
					av_msg("CAvUpnpCli::UpnpUpdateConfig SetConf fail...\n");
					continue;
				}
				AvUpnp.Start();
				AvUpnp.GetExternalPort(mapped_port);
				upnp_config.RouterPort[i] = mapped_port;
			}
		}
		//提交
		m_AvConfigNetUpnp.SettingUp();
	}
#endif
	return av_true;
}

av_void CAvUpnpCli::AvConfigUpnp(CAvConfigNetUpnp* obj, int &num)
{
	m_Mutex.Enter();
	m_AvConfigChanged++;	
	m_Mutex.Leave();
}

