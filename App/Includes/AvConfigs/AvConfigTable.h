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
#ifndef AVWARE_AVCONFIG_AVCONFIGTABLE_H_
#define AVWARE_AVCONFIG_AVCONFIGTABLE_H_

#include <string.h>
#include <string>
#include <vector>
#include "jsoncpp/json.h"
#include"CAvObject.h"
#include "Apis/AvWareType.h"
#include "AvThread/AvThread.h"

typedef Json::Value AvConfigValue;
class CAvConfigBase;

class AvConfigObserver
{
	friend class CAvConfigBase;
public:
	//注意 为了把回调的结果取回来 这里第二个参数int是引用
	typedef void (CAvObject::*CallBack)(void *, int&);
	AvConfigObserver() :m_pObj(NULL), m_cb(NULL){ };
	
	void Call(CAvObject *pObj , int arg){ (m_pObj->*m_cb)(pObj, arg); };

	CAvObject * m_pObj;
	CallBack m_cb;
};
typedef AvConfigObserver::CallBack AvConfigCallBack;

class AvConfigKey
{
	friend class CAvConfigBase;
public:
	AvConfigKey(int index)
	{
		m_kind = KIND_INDEX;
		m_value.index = index;
	};

	AvConfigKey(const char* name)
	{
		m_kind = KIND_NAME;
		m_value.name = name;
	};

private:
	enum Kind
	{
		KIND_NONE = 0,
		KIND_INDEX,
		KIND_NAME
	}m_kind;

	union
	{
		int index;
		const char *name;
	}m_value;
};

//提交设置的状态，按位表示
enum AvConfigReqState{
	CONF_REQ_OK					= 0,		//状态正常
	CONF_REQ_CHECK_INVALID		= 0x1 << 0, //参数不合法
	CONF_REQ_ERROR				= 0x1 << 1, //其他错误
	CONF_REQ_ERROR_FILE			= 0x1 << 2, //配置文件出错
	CONF_REQ_APPLY_NOT_SAVE		= 0x1 << 3, //将参数设置到系统中但是不保存配置表
};

class CAvConfigBase : public CAvObject
{
public:
	typedef enum teConfigValueStatus{
		CONF_STATUS_CHECK = 0,
		CONF_STATUS_DEFAULT,
		CONF_STATUS_GET,
		CONF_STATUS_SET,
	}ConfigValueStatus;

	CAvConfigBase(int cur_index,int max_index);
	virtual ~CAvConfigBase();
	
	inline ConfigValueStatus status(void)	const { return m_status; }
	inline void			set_status(ConfigValueStatus status) { m_status = status; }

	inline av_bool		valid(void) const { return m_valid; }
	inline void			set_valid(av_bool valid) { m_valid = valid; }

	//参数与配置表数据之间的转换,包含参数有合法性检查和参数默认值定义
	void				Process(AvConfigKey key, AvConfigValue &table, int		&val, int			def, int				min = -2147483647, int				max = 2147483647);
	void				Process(AvConfigKey key, AvConfigValue &table, int		&val,			int			def,			std::list<int> ValidValueList);
	void				Process(AvConfigKey key, AvConfigValue &table, bool		&val,			bool		def																						);
	void				Process(AvConfigKey key, AvConfigValue &table, char		&val,			char		def,			char			min = -128,			char			max = 127			);
	void				Process(AvConfigKey key, AvConfigValue &table, char		*val,			const char* def																						);
	void				Process(AvConfigKey key, AvConfigValue &table, short	&val,			short		def,			short			min = -32768,		short			max = 32767			);
	void				Process(AvConfigKey key, AvConfigValue &table, av_bool	&val,			av_bool		def																						);
	void				Process(AvConfigKey key, AvConfigValue &table, unsigned int		&val,	unsigned int		def,	unsigned int	min = 0,			unsigned int	max = 4294967295	);
	void				Process(AvConfigKey key, AvConfigValue &table, unsigned	char	&val,	unsigned char		def,	unsigned char	min = 0,			unsigned char	max = 255			);
	void				Process(AvConfigKey key, AvConfigValue &table, unsigned short	&val,	unsigned short		def,	unsigned short	min = 0,			unsigned short	max = 65535			);
	void				Process(AvConfigKey key, AvConfigValue &table, std::string		&val,	const std::string  &def																				);

	void				Update		(int index = -1);
	void				GetDefault	(int index = -1);
	av_bool				Check		(int index = -1);
	int					SettingUp	(int index = -1, int require = CONF_REQ_OK);

public:
	inline int			get_use_index(void)		{ return m_use_index; }
	inline void			set_use_index(int num)	{ m_use_index = num; }
	inline int			get_max_index(void)		{ return m_max_index; }

	//获取配置表类中的配置，并放到参数table里面
	void				GetConfigTable(AvConfigValue &table);

	//将参数table的配置设置到配置表类中
	void				SetConfigTable(AvConfigValue &table);

	//用内存地址做id 如果用64位系统int型可能要修改
	virtual int			get_id(void) = 0;
	virtual void		ProcessValue(AvConfigValue &val, int index) = 0;

	virtual void		Attach(CAvObject *pObj, AvConfigCallBack cb) = 0;
	virtual void		Detach(CAvObject *pObj, AvConfigCallBack cb) = 0;
	virtual av_bool		IsAttached(CAvObject *pObj, AvConfigCallBack cb) = 0;
	virtual void		Notify(CAvConfigBase *pObj, int require) = 0;

private:
	AvConfigValue&		GetValue		(AvConfigKey key, AvConfigValue &table);
	void				GetValueBool	(AvConfigKey key, AvConfigValue &table, av_bool &val,	av_bool def		);
	void				GetValueInt		(AvConfigKey key, AvConfigValue &table, int &val,		int def			);
	void				GetValueInt64	(AvConfigKey key, AvConfigValue &table, av_64 &val,		av_64 def		);
	const char*			GetValueString	(AvConfigKey key, AvConfigValue &table,	const char* def					);

	virtual void		WriteToEnv(int index) = 0;
	virtual void		ReadFromEnv(int index) = 0;
	virtual av_bool		Compare(int index) = 0;



private:
	static CMutex		m_mutex;
	ConfigValueStatus	m_status;
	av_bool				m_valid;
	int					m_max_index;
	int				   &m_env_used_index;
	int					m_use_index;
};

template <class T> void ProcessValue(CAvConfigBase &av_conf, AvConfigValue &val, T &config, int index, int diff);

//T 参数结构体类型
//diff 当参数结构相同时用此参数来实例化不同的配置类
//max_num 配置表支持的结构体组数目。IPC一般是1，NVR一般和通道数相关。
//compare 配置比较使用的方法，用void表示使用内存比较函数来比较
template <class T, int diff = 0, int max_num = 1, class compare = void> class TAvConfig : public CAvConfigBase
{
public:
	TAvConfig() :CAvConfigBase(m_use_index, max_num) { }
	virtual ~TAvConfig() { }

	virtual void ProcessValue(AvConfigValue &val, int index)
	{
		::ProcessValue(*(CAvConfigBase*)this, val, GetConfig(index), index, diff);
	}
	virtual int get_id(void) { return (int)m_env_config; }

	inline static const T& GetEnvConfig(int index = 0) { return m_env_config[index]; }
	
	inline T& GetConfig(int index = 0) { return m_use_config[index]; }
	inline T& operator[](int index){ return m_use_config[index]; }

	inline virtual void WriteToEnv(int index) { m_env_config[index] = m_use_config[index]; }
	inline virtual void ReadFromEnv(int index) { m_use_config[index] = m_env_config[index]; }

	inline av_bool Compare(const void *env, const void *use)
	{
		return (0 == memcmp(env, use, sizeof(T))) ? av_true : av_false;
	}
	inline av_bool Compare(const T *env, const T *use)
	{
		return (*env == *use) ? av_true : av_false;
	}
	inline virtual av_bool Compare(int index) 
	{
		return Compare((compare*)&m_env_config[index], (compare*)&m_use_config[index]);
	}

	virtual void  Attach(CAvObject *pObj, AvConfigCallBack cb)
	{
		m_mutex.Enter();
		std::vector<AvConfigObserver>::iterator it;
		for (it = m_observer.begin(); it != m_observer.end(); it++) {
			if ((it->m_pObj == pObj)
				&& (memcmp(&(it->m_cb), &cb, sizeof(AvConfigCallBack)) == 0)) {
				break;
			}
		}
		AvConfigObserver node;
		node.m_cb = cb;
		node.m_pObj = pObj;
		m_observer.push_back(node);
		m_mutex.Leave();	
	}

	virtual void Detach(CAvObject *pObj, AvConfigCallBack cb)
	{
		m_mutex.Enter();
		std::vector<AvConfigObserver>::iterator it;
		for (it = m_observer.begin(); it != m_observer.end(); it++) {
			if ((it->m_pObj == pObj)
				&& (memcmp(&(it->m_cb), &cb, sizeof(AvConfigCallBack)) == 0)) {
				m_observer.erase(it);
				break;
			}
		}
		m_mutex.Leave();	
	}

	virtual av_bool IsAttached(CAvObject *pObj, AvConfigCallBack cb)
	{
		av_bool ret = av_false;
		m_mutex.Enter();
		std::vector<AvConfigObserver>::iterator it;
		for (it = m_observer.begin(); it != m_observer.end(); it++) {
			if ((it->m_pObj == pObj)
				&& (memcmp(&(it->m_cb), &cb, sizeof(AvConfigCallBack)) == 0)) {
				ret = av_true;
				break;
			}
		}
		m_mutex.Leave();
		return ret;
	}

	virtual void Notify(CAvConfigBase *pObj, int require)
	{
		std::vector<AvConfigObserver>::iterator it;
		for (it = m_observer.begin(); it != m_observer.end(); it++) {
			it->Call(pObj, require);
		}
	}

private:
	//当前配置类中的配置
	T m_use_config[max_num];
	static int m_use_index;
	//系统环境中的配置
	static T m_env_config[max_num];
	static std::vector<AvConfigObserver> m_observer;
	static CMutex		m_mutex;
};

template <class T, int diff, int max_num, class compare>
T TAvConfig<T, diff, max_num, compare>::m_env_config[max_num];

template <class T, int diff, int max_num , class compare>
int TAvConfig<T, diff, max_num, compare>::m_use_index = max_num;

template <class T, int diff, int max_num, class compare>
CMutex TAvConfig<T, diff, max_num, compare>::m_mutex;

template <class T, int diff, int max_num , class compare>
std::vector<AvConfigObserver> TAvConfig<T, diff, max_num, compare>::m_observer;

#endif //AVWARE_AVCONFIG_AVCONFIGTABLE_H_