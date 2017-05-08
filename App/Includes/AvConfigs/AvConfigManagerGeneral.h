/*******************************************************************
|  Copyright(c) 2015-2016 Graceport Technology Development Co.,Ltd
|  All rights reserved.
|
|  版本: 1.0
|  作者: Xman [xman@graceport.cn]
|  日期: 2016年12月5日
|  说明:
|
******************************************************************/
#ifndef _AVCONFIG_MANAGER_GENERAL_H_
#define _AVCONFIG_MANAGER_GENERAL_H_

#include "AvConfigs/AvConfigManagerBase.h"
#include "AvConfigs/AvConfigManagerUser.h"
#include "AvConfigs/AvConfigDevice.h"
#include "AvConfigs/AvConfigNetService.h"
#include "AvConfigs/AvConfigCapture.h"
#include "AvConfigs/AvConfigAlarm.h"
#include "AvConfigs/AvConfigRecord.h"

class CAvConfigManagerGeneral :public CAvConfigManagerBase
{
public:
	SINGLETON_DECLARE(CAvConfigManagerGeneral);
	CAvConfigManagerGeneral();
	~CAvConfigManagerGeneral();
	virtual void Initialize();

	int RestoreDefaultAllGeneralConfig();

private:
	CAvConfigEncode					m_ConfigEncode;
	CAvConfigCover					m_ConfigCover;
	CAvConfigOverLay				m_ConfigOverLay;
	CAvConfigAlarm					m_ConfigAlarm;
	CAvConfigCapture				m_ConfigCapture;
	
	CAvConfigImage					m_ConfigImage;
	CAvConfigAudio					m_ConfigAudio;



	CAvConfigUart					m_config_device_uart;
	CAvConfigUserManager			m_config_user_manager;

	
	CAvConfigNetSmtp				m_confignet_smtp;
	CAvConfigNetFtp					m_confignet_ftp;
	CAvConfigNetDdns				m_confignet_ddns;
	CAvConfigNetUpnp				m_confignet_upnp;
	CAvConfigNetNtp					m_confignet_ntp;
	CAvConfigRtmp					m_config_rtmp;
	CAvConfigNetP2p					m_config_p2p;
	CAvConfigNetRtsp				m_config_rtsp;


	CAvConfigRecord					m_config_record;
	CAvConfigRecordCtrl				m_config_recordctrl;
	CAvConfigPtzCameraLensProfile	m_config_ptzCameraLens;
};

#define g_AvConfigMagagerGeneral (*CAvConfigManagerGeneral::instance())
#endif //_AVCONFIG_MANAGER_GENERAL_H_