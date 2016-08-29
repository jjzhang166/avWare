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
#include "AvConfigs/AvConfigTable.h"

#include <stdio.h>

CMutex CAvConfigBase::m_mutex;

CAvConfigBase::CAvConfigBase(int &cur_index, int max_index)
:m_status(CONF_STATUS_CHECK),
m_valid(av_true),
m_env_used_index(cur_index),
m_use_index(max_index),
m_max_index(max_index)
{
}

CAvConfigBase::~CAvConfigBase()
{
}

AvConfigValue& CAvConfigBase::GetValue(AvConfigKey key, AvConfigValue &table)
{
	if (AvConfigKey::KIND_INDEX == key.m_kind) {
		return table[key.m_value.index];
	} else if (AvConfigKey::KIND_NAME == key.m_kind) {
		return table[key.m_value.name];
	}

	av_error("AvConfigKey not have this type %d\n", key.m_kind);
	return table;
}

void CAvConfigBase::GetValueBool(AvConfigKey key, AvConfigValue &table, av_bool &val, av_bool def)
{
	AvConfigValue &table_tmp = GetValue(key, table);
	if (table_tmp.isInt() && (table_tmp.asInt()>=0) && (table_tmp.asInt()<=1)) {
		val = (1 == table_tmp.asInt() ? av_true : av_false);
	} else {
		val = def;
	}
}

void CAvConfigBase::GetValueInt(AvConfigKey key, AvConfigValue &table, int &val, int def)
{
	AvConfigValue &table_tmp = GetValue(key, table);
	val = table_tmp.isInt() ? table_tmp.asInt() : def;
}

/*
void CAvConfigBase::GetValueInt64(AvConfigKey key, AvConfigValue &table, av_64 &val, av_64 def)
{
	AvConfigValue &table_tmp = GetValue(key, table);
	val = table_tmp.isInt64() ? table_tmp.asInt64() : def;
}
*/

const char* CAvConfigBase::GetValueString(AvConfigKey key, AvConfigValue &table, const char *def)
{
	AvConfigValue &table_tmp = GetValue(key, table);
	return table_tmp.isString() ? table_tmp.asCString() : def;
}

//参数中的bool型会保存成json里的整形，节省数据表空间。
void CAvConfigBase::Process(AvConfigKey key, AvConfigValue &table, av_bool &val, av_bool def)
{
	switch(m_status)
	{
	case CONF_STATUS_GET:
		GetValueBool(key, table, val, def);
		break;
	case CONF_STATUS_SET:
		GetValue(key, table) = (av_true == val ? 1 : 0);
		break;
	case CONF_STATUS_DEFAULT:
		val = def;
		break;
	case CONF_STATUS_CHECK:
		//bool型合法性检测在GetValue里面完成
		break;
	default:
		break;
	}
}

inline void CAvConfigBase::Process(AvConfigKey key, AvConfigValue &table, bool &val, bool def)
{
	av_bool val_tmp = val ? av_true : av_false;
	av_bool def_tmp = val ? av_true : av_false;
	Process(key, table, val_tmp, def_tmp);
}

void CAvConfigBase::Process(AvConfigKey key, AvConfigValue &table, int &val, int def, int min, int max)
{
	switch(m_status)
	{
	case CONF_STATUS_GET:
		GetValueInt(key, table, val, def);
		if ((val < min) || (val > max)){
			val = def;
		}

		break;
	case CONF_STATUS_SET:
		GetValue(key, table) = val;
		break;
	case CONF_STATUS_DEFAULT:
		val = def;
		break;
	case CONF_STATUS_CHECK:
		if (av_true == valid()) {
			assert(max >= min);
			if ((min > val) || (max < val)) {
				set_valid(av_false);
			}
		}
		break;
	default:
		break;
	}
}

void CAvConfigBase::Process(AvConfigKey key, AvConfigValue &table, char &val,
								char def, char min, char max)
{
	switch(m_status)
	{
	case CONF_STATUS_GET:
		int val_tmp;
		GetValueInt(key, table, val_tmp, def);
		if ((val_tmp < min) || (val_tmp > max)) {
			val = def;
		} else {
			val = val_tmp;
		}
		break;
	case CONF_STATUS_SET:
		GetValue(key, table) = val;
		break;
	case CONF_STATUS_DEFAULT:
		val = def;
		break;
	case CONF_STATUS_CHECK:
		if (av_true == valid()) {
			assert(max >= min);
			if ((min > val) || (max < val)) {
				set_valid(av_false);
			}
		}
		break;
	default:
		break;
	}
}

void CAvConfigBase::Process(AvConfigKey key, AvConfigValue &table, unsigned char &val,
								unsigned char def, unsigned char min, unsigned char max)
{
	switch(m_status)
	{
	case CONF_STATUS_GET:
		int val_tmp;
		GetValueInt(key, table, val_tmp, def);
		if ((val_tmp < min) || (val_tmp > max)) {
			val = def;
		} else {
			val = val_tmp;
		}
		break;
	case CONF_STATUS_SET:
		GetValue(key, table) = val;
		break;
	case CONF_STATUS_DEFAULT:
		val = def;
		break;
	case CONF_STATUS_CHECK:
		if (av_true == valid()) {
			assert(max >= min);
			if ((min > val) || (max < val)) {
				set_valid(av_false);
			}
		}
		break;
	default:
		break;
	}
}


void CAvConfigBase::Process(AvConfigKey key, AvConfigValue &table, unsigned int &val,
								unsigned int def, unsigned int min, unsigned int max)
{
	switch(m_status)
	{
	case CONF_STATUS_GET:
		//av_64 val_tmp;
		//GetValueInt64(key, table, val_tmp, def);
		av_int val_tmp;
		GetValueInt(key, table, val_tmp, def);
		if ((val_tmp < 0)) {
			val = def;
		} else {
			val = static_cast<unsigned int>(val_tmp);
		}
		break;
	case CONF_STATUS_SET:
		//GetValue(key, table) = (Json::Int64)val;
		GetValue(key, table) = val;
		break;
	case CONF_STATUS_DEFAULT:
		val = def;
		break;
	case CONF_STATUS_CHECK:
		if (valid()) {
			assert(max >= min);
			if ((min > val) || (max < val)) {
				set_valid(av_false);
			}
		}
		break;
	default:
		break;
	}
}


void CAvConfigBase::Process(AvConfigKey key, AvConfigValue &table, unsigned short &val,
								unsigned short def, unsigned short min, unsigned short max)
{
	switch(m_status)
	{
	case CONF_STATUS_GET:
		int val_tmp;
		GetValueInt(key, table, val_tmp, def);
		if (val_tmp < 0) {
			val = def;
		} else {
			val = static_cast<unsigned short>(val_tmp);
		}
		break;
	case CONF_STATUS_SET:
		//GetValue(key, table) = (Json::Int64)val;
		GetValue(key, table) = val;
		break;
	case CONF_STATUS_DEFAULT:
		val = def;
		break;
	case CONF_STATUS_CHECK:
		if (valid()) {
			assert(max >= min);
			if ((min > val) || (max < val)) {
				set_valid(av_false);
			}
		}
		break;
	default:
		break;
	}
}


void CAvConfigBase::Process(AvConfigKey key, AvConfigValue &table, std::string& val, const std::string& def)
{
	switch(m_status)
	{
	case CONF_STATUS_GET:
		val = GetValueString(key, table, def.c_str());
		break;
	case CONF_STATUS_SET:
		GetValue(key, table) = val;
		break;
	case CONF_STATUS_DEFAULT:
		val = def;
		break;
	case CONF_STATUS_CHECK:
	default:
		break;
	}
}

void CAvConfigBase::Process(AvConfigKey key, AvConfigValue &table, char* val, const char* def)
{
	switch(m_status)
	{
	case CONF_STATUS_GET:
		strcpy(val, GetValueString(key, table, def));
		break;
	case CONF_STATUS_SET:
		GetValue(key, table) = val;
		break;
	case CONF_STATUS_DEFAULT:
		strcpy(val, def);
		break;
	case CONF_STATUS_CHECK:
	default:
		break;
	}
}


//获取配置表类中的配置，并放到参数table里面
void CAvConfigBase::GetConfigTable(AvConfigValue &table)
{
	m_mutex.Enter();
	set_status(CONF_STATUS_GET);

	m_use_index = table.size();
	m_use_index = m_use_index > m_max_index ? m_max_index : m_use_index;

	for (int i = 0; i < m_use_index; i++) {
		ProcessValue(table[i], i);
	}
	m_mutex.Leave();
}

//将参数table的配置设置到配置表类中
void CAvConfigBase::SetConfigTable(AvConfigValue &table)
{
	m_mutex.Enter();
	set_status(CONF_STATUS_SET);
	table.resize(m_use_index);

	for (int i = 0; i < m_use_index; i++) {
		ProcessValue(table[i], i);
	}
	m_mutex.Leave();
}

void CAvConfigBase::Update(int index)
{
	m_mutex.Enter();
	if (-1 == index) {
		for (int i = 0; i < m_max_index; i++){
			ReadFromEnv(i);
		}
	} else {
		ReadFromEnv(index);
	}
	m_mutex.Leave();
}

void CAvConfigBase::GetDefault(int index)
{
	m_mutex.Enter();
	set_status(CONF_STATUS_DEFAULT);
	AvConfigValue tmp;
	if (-1 == index) {
		for (int i = 0; i < m_max_index; i++) {
			ProcessValue(tmp, i);
		}
	} else {
		ProcessValue(tmp, index);
	}
	m_mutex.Leave();
}

av_bool CAvConfigBase::Check(int index)
{
	m_mutex.Enter();
	//必须先设置合法否则在当前状态为不合法时不会进行检测
	set_valid(av_true);
	set_status(CONF_STATUS_CHECK);
	AvConfigValue tmp;
	if (-1 == index) {
		for (int i = 0; i < m_max_index; i++) {
			ProcessValue(tmp, i);
		}
	} else {
		ProcessValue(tmp, index);
	}
	m_mutex.Leave();
	return valid();
}

//检查配置是否变化（包括数量）->合法性检查->通知观察者（包含配置生效和保存到配置文件中）
int CAvConfigBase::SettingUp(int index, int require)
{
	CGuard lock(m_mutex);
	int req = require;
	AvConfigValue tmp;
	int i = 0;
	//检查配置参数是否变化
	if (m_env_used_index == m_use_index) {
		if (-1 == index) {
			for (i = 0; i < m_max_index; ++i) {
				if (!Compare(i)) {
					break;
				}
			}
			if (i == m_max_index) {
				return req;
			}
		} else {
			if (Compare(index)) {
				return req;
			}
		}
	}

	set_valid(av_true);
	set_status(CONF_STATUS_CHECK);

	if (-1 == index) {
		for (i = 0; i < m_use_index; ++i) {
			ProcessValue(tmp, i);
		}
	} else {
		ProcessValue(tmp, index);
	}

	if (valid()) {
		//更新配置类中的环境信息
		if (-1 == index) {
			for (int j = 0; j < m_max_index; j++) {
				WriteToEnv(j);
			}
		} else {
			WriteToEnv(index);
		}
		m_env_used_index = m_use_index;

		//参数有效，先通知观察者
		m_mutex.Leave();
		Notify(this, require);
		m_mutex.Enter();

	} else {
		req |= CONF_REQ_CHECK_INVALID;
	}
	return req;
}