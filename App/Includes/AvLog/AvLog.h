/*******************************************************************
|  Copyright(c) 2015-2016 Graceport Technology Development Co.,Ltd
|  All rights reserved.
|
|  版本: 1.0
|  作者: Knight [knight@graceport.cn]
|  日期: 2016年6月6日
|  说明:
|
|  版本: 1.1
|  作者:
|  日期:
|  说明:
******************************************************************/
#ifndef _AVLOG_H_
#define _AVLOG_H_
#include "Apis/AvWareCplusplus.h"
#include "CAvObject.h"
#include "AvThread/AvThread.h"
#include "AvFile/AvFile.h"

#define LOG_PATH "./log/Log.l"
#define LOGMAXLINE 1000
class CAvLog
{
public:
	enum LogType
	{
		LOG_SYS				= 0x10,
		LOG_SYS_REBOOT,
		LOG_SYS_START,

		LOG_CONF			= 0X20,
		LOG_CONF_SAVE,
		LOG_CONF_LOAD,
		LOG_CONF_DEFAULT,

		LOG_HDD				= 0x40,
		LOG_HDD_WERR,
		LOG_HDD_RERR,
		LOG_HDD_FORMAT,
		LOG_HDD_NOSPACE,
		LOG_HDD_NODISK,
		LOG_HDD_NOSD,
		LOG_HDD_ERR,

		LOG_ALM				= 0x80,
		LOG_ALM_PORT_IN,
		LOG_ALM_PORT_END,
		LOG_ALM_MOTION_IN,
		LOG_ALM_MOTION_END,

		LOG_NET				= 0x100,
		LOG_NET_ABORT,
		LOG_NET_ABORT_RESUME,
		LOG_NET_IP_CONFLICT,

		LOG_RECORD			= 0x200,
		LOG_RECORD_MANUAL,
		LOG_RECORD_CLOSED,
		LOG_RECORD_FILE_DOWNLOAD,
		LOG_RECORD_FILE_BACKUP,
		LOG_RECORD_FILE_PALY,
		LOG_RECORD_FILE_BACKUP_ERR,

		LOG_USER			= 0x400,
		LOG_USER_LOGIN,
		LOG_USER_LOGOUT,
		LOG_USER_ADD,
		LOG_USER_DELETE,
		LOG_USER_MODIFY_USER,
		LOG_USER_ADD_GROUP,
		LOG_USER_DELETE_GROUP,
		LOG_USER_MODIFY_GROUP,
		LOG_USER_MODIFY_PASSWORD,
		LOG_USER_LOCKED,

		LOG_LOG				= 0x800,
		LOG_LOG_CLEAR,
		LOG_LOG_SEARCH,
	};

#pragma pack(push)
#pragma pack(4)
	typedef struct {
		time_t	t;	//8
		LogType lt;	//4
		av_char context[12];
	}LogItem;
#pragma pack(pop)

public:
	SINGLETON_DECLARE(CAvLog);

private:
	CAvLog();
	~CAvLog();

public:
	av_bool LogLoad();
	av_bool LogUnload();
private:
	LogItem *m_item;
	av_u32	 m_itemCount;

public://日志写入
	av_bool Initialize();
	av_bool Append(LogType Type, av_char *data = NULL, av_u16 len = 0);
	av_bool Clear(const char *usrname);


public://日志查询
	av_bool SetLogfilter(LogType &lt);
	av_bool SetLogfilter(time_t st = 0, time_t et = 0);
	av_bool SetLogfilter(av_16 sOffset = -1, av_16 eOffset = -1);
	av_bool GetLogContext(av_16 &offset, LogItem &li);

private:
	LogType m_Search_lt;
	time_t  m_Search_st;
	time_t  m_Search_et;
	av_16   m_Search_sOffset;
	av_16	m_Search_eOffset;

private:
	static CMutex m_logFileMutex;
	CFile		  m_logFile;
};



#define g_AvLog (*CAvLog::instance())










#endif


