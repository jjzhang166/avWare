/*******************************************************************
|  Copyright(c) 2015-2016 Graceport Technology Development Co.,Ltd
|  All rights reserved.
|
|  版本: 1.0
|  作者: Xman [xman@graceport.cn]
|  日期: 2016年6月6日
|  说明:
|
|  版本: 1.1
|  作者:
|  日期:
|  说明:
******************************************************************/
#ifndef AVWARE_AVCONFIG_MANAGER_H_
#define AVWARE_AVCONFIG_MANAGER_H_

#include <map>
#include "AvConfigs/AvConfigUserManager.h"
#include "AvConfigs/AvConfigDevice.h"
#include "AvConfigs/AvConfigNetService.h"
#include "AvConfigs/AvConfigCapture.h"
#include "AvConfigs/AvConfigAlarm.h"

void ConfigUnitTest(void);

class CTimer;

std::string getString(const AvConfigValue &val);

class CAvConfigManager : public CAvObject
{
public:
	SINGLETON_DECLARE(CAvConfigManager);
	CAvConfigManager();
	virtual ~CAvConfigManager();
	void Initialize();
	void Save();
	//TODO:恢复默认，配置表导入导出
	int RestoreDefault();
	int ExportConfig();
	int ImportConfig();

public:
	av_bool SetAvConfigPath(std::string &ConfPatch);
private:
	std::string m_ConfigFullPatch;
	std::string m_ConfigFullPatchBu;

private:
	std::map<int, std::string> m_config_id_name;
	std::map<std::string, CAvConfigBase*> m_config_name_confclass;
	std::map<std::string, int> m_name_configIndex;
	typedef std::map<std::string, int>::value_type ConfIndexValueType;
	const char* IdToName(int id);
	CAvConfigBase* NameToConfClass(const char *name);

	av_bool LoadConfigFromFile(const char *path, std::string &conf);
	void LoadConfig(const char *name, CAvConfigBase &conf);
	void CBConfigChange(CAvConfigBase *conf, int &req);

	av_bool m_change;
	AvConfigValue m_total;
	std::string m_config_data;
	CTimer *m_timer;

	static CMutex m_mutex;

	CAvConfigEncode				m_ConfigEncode;
	CAvConfigCover				m_ConfigCover;
	CAvConfigWaterMarking		m_ConfigWaterMark;
	CAvConfigAlarmIo			m_ConfigAlarmIo;
	CAvConfigAlarmMd			m_ConfigAlarmMd;
	CAvConfigNetComm			m_ConfigNetComm;
	CAvConfigProtocol			m_ConfigNetProtocol;
	CAvConfigImage				m_ConfigImage;
	CAvConfigAudio				m_ConfigAudio;

	CAvConfigCapture			m_ConfigCapture;

	CAvConfigUart				m_config_device_uart;
	CAvConfigUserManager		m_config_user_manager;

	
	CAvConfigNetSerSmtp			m_confignet_smtp;
	CAvConfigNetFtp				m_confignet_ftp;
	CAvConfigNetDdns			m_confignet_ddns;
	CAvConfigNetUpnp			m_confignet_upnp;
	CAvConfigNetNtp				m_confignet_ntp;
	
};

#define g_AvConfigManager (*CAvConfigManager::instance())
#endif //AVWARE_AVCONFIG_MANAGER_H_