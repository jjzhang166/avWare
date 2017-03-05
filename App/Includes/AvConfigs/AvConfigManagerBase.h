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
#ifndef _AVCONFIG_MANAGER_BASE_H_
#define _AVCONFIG_MANAGER_BASE_H_

#include <map>
#include <string>
#include "AvConfigs/AvConfigTable.h"

class CAvConfigBase;
class CAvConfigManagerBase:public CAvObject
{
public:
	CAvConfigManagerBase(const std::string & DefConfPath, const std::string &DefCongPathBak);
	virtual ~CAvConfigManagerBase();
	virtual void Initialize() = 0;

	//TODO:参数导入、导出待测试
	virtual int ExportConfig(void *buf, int &len);
	virtual int ImportConfig(void *buf, const int len);

protected:
	av_bool SetAvConfigPath(const std::string &ConfPath, const std::string &ConfFile, const std::string &ConfFileBak);
	av_bool LoadConfigFromFile();
	void SetUpConfig(const char *name, CAvConfigBase &conf, const int index);
	int RestoreDefault(const char *ConfName, const int index = -1);
	int RestoreDefaultAll();
	av_int WriteFile();
private:
	//id 配置表TAvConfig模板类实例化后的id
	//index 配置类型索引，参考AvConfigIndex
	//name 配置类型别名，序列化成json后充当key
	//table 配置类型对象实例
	std::map<int, std::string> m_id_name;
	std::map<std::string, CAvConfigBase*> m_name_table;
	std::map<std::string, int> m_name_index;

private:
	const char* IdToName(int id);
	CAvConfigBase* NameToTable(const char *name);
	av_bool ReadConfigFile(const char *path, std::string &conf);
	void CBConfigChange(CAvConfigBase *conf, int &req);

private:
	std::string m_ConfigFullPath;
	std::string m_ConfigFullPathBak;

	AvConfigValue m_total;
	static CMutex m_mutex;
	std::string m_config_data;
};

#endif //_AVCONFIG_MANAGER_BASE_H_