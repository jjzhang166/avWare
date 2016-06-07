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
#include <iostream>

#include "Apis/AvWareType.h"
#include "AvThread/AvTimer.h"
#include "AvConfigs/AvConfigIndex.h"
#include "AvConfigs/AvConfigTable.h"
#include "AvConfigs/AvConfigManager.h"


typedef Json::Reader AvConfigReader;

#ifdef WIN32
    typedef Json::StyledWriter AvConfigWriter;
#else
    typedef Json::FastWriter AvConfigWriter;
#endif

#ifdef WIN32
#define  AvConfigPath "./configs/avWare1.json"
#define  AvConfigPathBackUp "./configs/avWare2.json"
#else
#define  AvConfigPath "./configs/avWare1.conf"
#define  AvConfigPathBackUp "./configs/avWare2.conf"
#endif

class TimerConfManager:public CTimer
{
public:
	TimerConfManager(){}
	~TimerConfManager(){}
public: 
	void OnTime()
	{
		av_msg("TimerConfManager test\n");
	}
};

PATTERN_SINGLETON_IMPLEMENT(CAvConfigManager);

CMutex CAvConfigManager::m_mutex;

CAvConfigManager::CAvConfigManager()
{
	m_timer = nullptr;
}

CAvConfigManager::~CAvConfigManager()
{
	if (nullptr != m_timer)
		delete m_timer;
}

void CAvConfigManager::Initialize()
{
	m_timer = new TimerConfManager();
	m_change = av_false;
	m_config_data.reserve(512 * 1024);
	AvConfigReader reader;
	bool load_flag = false;


	if (av_true == LoadConfigFromFile(AvConfigPath, m_config_data)){
		load_flag = reader.parse(m_config_data, m_total);
	}
	if (false == load_flag) {
		av_msg("Load Configfile1 failed\n");
		if (av_true == LoadConfigFromFile(AvConfigPathBackUp, m_config_data)){
			load_flag = reader.parse(m_config_data, m_total);
		}
	}

	if (false == load_flag){
		av_error("Load all config file failed\n");
	}


	m_name_configIndex.clear();

	m_name_configIndex.insert(ConfIndexValueType("DeviceUart", CONF_DEVICE_UART));
	LoadConfig("DeviceUart", m_config_device_uart);

	
	m_name_configIndex.insert(ConfIndexValueType("Capture", CONF_CAPTURE_FORMATS));
	LoadConfig("Capture", m_ConfigCapture);

	m_name_configIndex.insert(ConfIndexValueType("Image", CONF_IMAGE_FORMATS));
	LoadConfig("Image", m_ConfigImage);

	m_name_configIndex.insert(ConfIndexValueType("Enocde", CONF_ENCODE_FORMATS));
	LoadConfig("Enocde", m_ConfigEncode);

	m_name_configIndex.insert(ConfIndexValueType("Cover", CONF_ENCODE_COVER));
	LoadConfig("Cover", m_ConfigCover);

	m_name_configIndex.insert(ConfIndexValueType("WaterMarking", CONF_ENCODE_WATERMARKING));
	LoadConfig("WaterMarking", m_ConfigWaterMark);

	m_name_configIndex.insert(ConfIndexValueType("AlarmIo", CONF_ALARM_IO));
	LoadConfig("AlarmIo", m_ConfigAlarmIo);
	
	m_name_configIndex.insert(ConfIndexValueType("AlarmMd", CONF_ALARM_MD));
	LoadConfig("AlarmMd", m_ConfigAlarmMd);

	m_name_configIndex.insert(ConfIndexValueType("NetComm", CONF_NET_COMM));
	LoadConfig("NetComm", m_ConfigNetComm);

	//User Manager
	m_name_configIndex.insert(ConfIndexValueType("UserManager", CONF_USER_MANAGER));
	LoadConfig("UserManager", m_config_user_manager);
	//Net Server Email
	m_name_configIndex.insert(ConfIndexValueType("NetServerSmtp", CONF_NET_SER_EMAIL));
	LoadConfig("NetServerSmtp", m_confignet_smtp);
	//ftp
	m_name_configIndex.insert(ConfIndexValueType("NetServerFtp", CONF_NET_SER_FTP));
	LoadConfig("NetServerFtp", m_confignet_ftp);
	m_name_configIndex.insert(ConfIndexValueType("NetServerDdns", CONF_NET_SER_DDNS));
	LoadConfig("NetServerDdns", m_confignet_ddns);
	m_name_configIndex.insert(ConfIndexValueType("NetServerUpnp", CONF_NET_SER_UPNP));
	LoadConfig("NetServerUpnp", m_confignet_upnp);
	m_name_configIndex.insert(ConfIndexValueType("NetServerNtp", CONF_NET_SER_NTP));
	LoadConfig("NetServerNtp", m_confignet_ntp);

	if (false == load_flag){
		Save();
	}
}

void CAvConfigManager::LoadConfig(const char *name, CAvConfigBase &conf)
{
	m_config_id_name.insert(std::map<int, std::string>::value_type(conf.get_id(), name));
	m_config_name_confclass.insert(std::map<std::string, CAvConfigBase*>::value_type(name, &conf));
	
	//装载配置
	conf.GetConfigTable(m_total[name]);
	if (!conf.valid() || 0 == conf.get_use_index()) {
		av_msg("Load Config from file faile restore default\n");
		conf.set_use_index(conf.get_max_index());
		conf.GetDefault();
	}
	conf.SetConfigTable(m_total[name]);
	m_change = av_true;
	//将配置参数设置生效但不要写入配置表
	conf.Attach(this, (AvConfigCallBack)(&CAvConfigManager::CBConfigChange));
	conf.SettingUp(-1, CONF_REQ_APPLY_NOT_SAVE);
}

av_bool CAvConfigManager::LoadConfigFromFile(const char *path, std::string &conf)
{
	FILE *fp = fopen(path, "r");
	if (NULL == fp) {
		av_error("%s open fail\n", path);
		return av_false;
	}
	conf.clear();
	char * tmp = new char[12 * 1024 + 1];
	int len = 0;
	while (!feof(fp)) {
		len = fread(tmp, sizeof(char), 12 * 1024, fp);
		//conf += tmp;
		conf.append(tmp, len);
	}
	delete[]tmp;
	fclose(fp);
	return av_true;
}

void CAvConfigManager::Save()
{
	CGuard lock(m_mutex);
	if (!m_change) return;

	m_change = av_false;
	AvConfigWriter writer;
	m_config_data = writer.write(m_total);

	remove(AvConfigPathBackUp);
	rename(AvConfigPath, AvConfigPathBackUp);

	//TODO将m_config_data 保存为文件
	FILE *fp = fopen(AvConfigPath, "w");
	if (NULL == fp) {
		perror("Save Config failed can`t create file:");
		return;
	}
	fwrite(m_config_data.c_str(), sizeof(char), m_config_data.size(), fp);
	fclose(fp);
}


const char* CAvConfigManager::IdToName(int id)
{
	std::map<int, std::string>::iterator it = m_config_id_name.find(id);
	if(it != m_config_id_name.end()) {
        return (*it).second.c_str();
    } else {
        av_error("IdToName failed id = %d.\n", id);
        return NULL;
    }
}

CAvConfigBase* CAvConfigManager::NameToConfClass(const char *name)
{
	std::map<std::string, CAvConfigBase*>::iterator it = m_config_name_confclass.find(name);
	if (it != m_config_name_confclass.end()) {
		return (*it).second;
	} else {
		av_error("NameToConfClass failed name[%s].\n", name);
	}
	return NULL;
}

void CAvConfigManager::CBConfigChange(CAvConfigBase *conf, int &req)
{
	const char *name = IdToName(conf->get_id());
	if (nullptr == name){
		return;
	}

	conf->SetConfigTable(m_total[name]);
	m_change = av_true;
	if (!(CONF_REQ_APPLY_NOT_SAVE & req)){
		Save();
	}


	return;
}
