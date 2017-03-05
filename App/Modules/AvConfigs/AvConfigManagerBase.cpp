#include "AvConfigs/AvConfigManagerBase.h"
#include "AvFile/AvFile.h"

typedef Json::Reader AvConfigReader;

#ifdef WIN32
    typedef Json::StyledWriter AvConfigWriter;
#else
    typedef Json::FastWriter AvConfigWriter;
#endif

CMutex CAvConfigManagerBase::m_mutex;
CAvConfigManagerBase::CAvConfigManagerBase(const std::string & DefConfPath, const std::string &DefCongPathBak)
	:m_ConfigFullPath(DefConfPath),
	m_ConfigFullPathBak(DefCongPathBak)
{
	m_id_name.clear();
	m_name_index.clear();
	m_name_table.clear();
}

CAvConfigManagerBase::~CAvConfigManagerBase()
{
}

av_bool CAvConfigManagerBase::SetAvConfigPath(const std::string &ConfPath, const std::string &ConfFile, const std::string &ConfFileBak)
{

	m_ConfigFullPath.clear();
	m_ConfigFullPathBak.clear();
	
	if (ConfPath.size() == 0){
		m_ConfigFullPath.assign(ConfFile);
		m_ConfigFullPathBak.assign(ConfFileBak);
	}
	else{
		m_ConfigFullPath = ConfPath;
		m_ConfigFullPath.append("/");
		m_ConfigFullPath.append(ConfFile);

		m_ConfigFullPathBak = ConfPath;
		m_ConfigFullPathBak.append("/");
		m_ConfigFullPathBak.append(ConfFileBak);
	}
	av_msg("ConfigsFullPath[%s] Backup[%s]\n", m_ConfigFullPath.c_str(), m_ConfigFullPathBak.c_str());
	return av_true;
}

av_bool CAvConfigManagerBase::ReadConfigFile(const char *path, std::string &conf)
{
	FILE *fp = fopen(path, "r");
	if (NULL == fp) {
		av_error("%s open fail\n", path);
		return av_false;
	}
	conf.clear();
	char * tmp = new char[4 * 1024];
	int len = 0;
	while (!feof(fp)) {
		len = fread(tmp, sizeof(char), (4 * 1024), fp);
		//conf += tmp;
		conf.append(tmp, len);
	}
	delete[]tmp;
	fclose(fp);
	return av_true;
}

av_bool CAvConfigManagerBase::LoadConfigFromFile()
{
	//需要根据序列化后json总大小调整字符串尺寸
	m_config_data.reserve(128 * 1024);
	AvConfigReader reader;
	bool load_flag = false;

	assert(!m_ConfigFullPath.empty());
	assert(!m_ConfigFullPathBak.empty());

	if (av_true == ReadConfigFile(m_ConfigFullPath.c_str(), m_config_data)) {
		load_flag = reader.parse(m_config_data, m_total);
	}

	if (false == load_flag) {
		av_msg("Load Configfile failed, Loading backup Configfile\n");
		if (av_true == ReadConfigFile(m_ConfigFullPathBak.c_str(), m_config_data)){
			load_flag = reader.parse(m_config_data, m_total);
		}
	}

	if (false == load_flag) {
		av_error("Load all config file failed\n");
		return av_false;
	}
	return av_true;
}

void CAvConfigManagerBase::SetUpConfig(const char *name, CAvConfigBase &conf, const int index)
{
	m_name_index.insert(std::map<std::string, int>::value_type(name, index));
	m_id_name.insert(std::map<int, std::string>::value_type(conf.get_id(), name));
	m_name_table.insert(std::map<std::string, CAvConfigBase*>::value_type(name, &conf));

	//装载配置
	conf.GetConfigTable(m_total[name]);
	if (!conf.valid() || 0 == conf.get_use_index()) {
		av_warning("Load Config[%15s] from file failed, restore default\n", name);
		conf.set_use_index(conf.get_max_index());
		conf.GetDefault();
	}
	conf.SetConfigTable(m_total[name]);
	//将配置参数设置生效但不要写入配置表
	conf.SettingUp(-1, CONF_REQ_APPLY_NOT_SAVE);
	
	conf.Attach(this, (AvConfigCallBack)(&CAvConfigManagerBase::CBConfigChange));
}

void CAvConfigManagerBase::CBConfigChange(CAvConfigBase *conf, int &req)
{
	const char *name = IdToName(conf->get_id());
	if (nullptr == name){
		return;
	}

	conf->SetConfigTable(m_total[name]);
	if (!(CONF_REQ_APPLY_NOT_SAVE & req)){
		WriteFile();
	}
	return;
}

const char *CAvConfigManagerBase::IdToName(int id)
{
	std::map<int, std::string>::iterator it = m_id_name.find(id);
	if(it != m_id_name.end()) {
        return (*it).second.c_str();
    } else {
        av_error("IdToName failed id = %d.\n", id);
        return nullptr;
    }
}

av_int CAvConfigManagerBase::WriteFile()
{
	CGuard lock(m_mutex);

	AvConfigWriter writer;
	m_config_data = writer.write(m_total);

	remove(m_ConfigFullPathBak.c_str());
	rename(m_ConfigFullPath.c_str(), m_ConfigFullPathBak.c_str());

	CFile::MakeDeepDirectory(m_ConfigFullPath.c_str());
	FILE *fp = fopen(m_ConfigFullPath.c_str(), "w");
	if (NULL == fp) {
		perror("fopen");
		av_error("Save Config failed ,can`t create file:[%s]\n", m_ConfigFullPath.c_str());
		return -1;
	}
	fwrite(m_config_data.c_str(), sizeof(char), m_config_data.size(), fp);
	fclose(fp);
	return 0;
}

CAvConfigBase* CAvConfigManagerBase::NameToTable(const char *name)
{
	auto it = m_name_table.find(name);
	if(it != m_name_table.end()) {
        return (*it).second;
    } else {
        av_error("NameToTable failed name = %s.\n", name);
        return nullptr;
    }
}

int CAvConfigManagerBase::RestoreDefault(const char *ConfName, const int index/*= -1*/)
{
	CAvConfigBase* ConfTable = NameToTable(ConfName);
	if (nullptr == ConfTable) {
		av_error("Not find config table [%s]\n", ConfName);
		return -1;
	}

	ConfTable->Update(index);
	ConfTable->GetDefault(index);
	return ConfTable->SettingUp(index);
}

int CAvConfigManagerBase::RestoreDefaultAll()
{
	int ret = 0;
	for (auto it : m_name_table) {
		it.second->GetDefault();
		ret |= it.second->SettingUp(-1, CONF_REQ_APPLY_NOT_SAVE);
	}

	if (ret) {
		WriteFile();
	}

	return ret;
}

int CAvConfigManagerBase::ExportConfig(void *buf, int &len)
{
	FileInfo file_info;

	if (false == CFile::Stat(m_ConfigFullPath.c_str(), &file_info)) {
		av_error("Export config failed, config file not exist\n");
		return -1;
	}

	if (file_info.size >= len) {
		av_error("Export config failed, buff size less then config file\n");
		return -1;
	}

	
	CGuard lock(m_mutex);
	CFile conf_read_file;
	if (false == conf_read_file.Open(m_ConfigFullPath.c_str())) {
		av_error("Export config failed, open config file failed\n");
		return -1;
	}

	len = conf_read_file.Read(buf, static_cast<int>(file_info.size));
	if (0 >= len) {
		av_error("Export config failed, read config file failed\n");
		return -1;
	}

	return 0;
}


int CAvConfigManagerBase::ImportConfig(void *buf, const int len)
{
	if (nullptr == buf || 0 >= len) return -1;

	std::string str_import(static_cast<char*>(buf));
	AvConfigValue all_cfg;
	Json::Reader conf_reader;
	if (false == conf_reader.parse(str_import, all_cfg)) {
		av_error("Import config failed, paser config file failed\n");
		return -1;
	}

	CGuard lock(m_mutex);
	Json::Value::Members meb = all_cfg.getMemberNames();
	for (auto it : meb) {
		m_total[it] = all_cfg[it];
	}

	m_mutex.Leave();
	WriteFile();
	m_mutex.Enter();

	return 0;
}
