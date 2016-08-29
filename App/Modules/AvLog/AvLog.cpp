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
#include "AvLog/AvLog.h"
#include "Apis/LibSystem.h"

SINGLETON_IMPLEMENT(CAvLog)

#pragma pack(push)
#pragma pack(4)
typedef struct {
	av_u32 size;  //日志最大行数
	av_u32 total; //有效日志行数
	av_u32 index; //起始日志位置
}C_LogHead;
#pragma pack(pop)

CMutex CAvLog::m_logFileMutex;

CAvLog::CAvLog()
{
	m_item = NULL;
	m_itemCount = 0;
	m_Search_sOffset = -1;
	m_Search_eOffset = LOGMAXLINE;
	m_Search_lt = (LogType)0xffffffff;
	m_Search_st = 0;
	m_Search_et = 0xffffffff;

}
CAvLog::~CAvLog()
{
	if (m_item != NULL) free(m_item);
	m_itemCount = 0;
}


av_bool CAvLog::LogLoad()
{
	bool bret; 
	av_u32 uret = 0;
	m_logFileMutex.Enter();
	{
		bret = m_logFile.Open(LOG_PATH, CFile::modeReadWrite | CFile::modeNoTruncate);
		if (bret != true){
			return av_false;
		}
		C_LogHead LogHead;
		m_logFile.Seek(0, CFile::begin);
		uret = m_logFile.Read(&LogHead, sizeof(LogHead));
		if (uret != sizeof(LogHead)){
			av_error("read LogHead Error\n");
			m_logFile.Close();
			m_logFileMutex.Leave();
			return av_false;
		}
		m_item = (LogItem *)malloc(sizeof(LogItem)*LogHead.total);
		av_u32 readindex = 0;
		readindex = LogHead.index;
		do 
		{
			readindex = (readindex + 1) % LogHead.total;
			m_logFile.Seek(sizeof(C_LogHead) + sizeof(LogItem)*readindex, CFile::begin);
			m_logFile.Read(&m_item[m_itemCount], sizeof(LogItem));
			m_itemCount++;
			if (m_itemCount >= LogHead.total){
				break;
			}
		} while (1);
		m_logFile.Close();
	}
	m_logFileMutex.Leave();
	return av_true;
}

av_bool CAvLog::LogUnload()
{
	if (NULL != m_item) free(m_item);
	m_itemCount = 0;
	return av_true;
}

av_bool CAvLog::Initialize()
{
	LogLoad();
	return av_true;
}
av_bool CAvLog::Append(LogType Type, av_char *data, av_u16 len)
{
	av_u32 ret;
	av_u32 WriteIndex = 0;
	m_logFileMutex.Enter();
	{
		C_LogHead LogHead;
		LogItem   Logitem;
		Logitem.t = time(NULL);
		Logitem.lt = Type;
		memset(Logitem.context, 0x00, sizeof(Logitem.context));
		len == 0 ? NULL : memcpy(Logitem.context, data, len > 11 ? 11:len);
		m_logFile.Open(LOG_PATH, CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate);
		ret = m_logFile.Read(&LogHead, sizeof(C_LogHead));
		if (ret != sizeof(C_LogHead)){
			LogHead.size	= LOGMAXLINE;
			LogHead.total	= 1;
			LogHead.index	= 0;
			WriteIndex = LogHead.index;
		}
		else{
			LogHead.index = (LogHead.index + 1) % LogHead.size;
			if (LogHead.total < LogHead.size){
				LogHead.total++;
			}
			WriteIndex = LogHead.index;
		}

		m_logFile.Seek(sizeof(C_LogHead) + sizeof(LogItem)*WriteIndex, CFile::begin);
		m_logFile.Write(&Logitem, sizeof(LogItem));
		

		m_logFile.Seek(0, CFile::begin);
		m_logFile.Write(&LogHead, sizeof(C_LogHead));
		m_logFile.Flush();
		m_logFile.Close();
	}
	m_logFileMutex.Leave();
	return av_true;
}
av_bool CAvLog::Clear(const char *usrname)
{
	m_logFileMutex.Enter();
	m_logFile.Remove(LOG_PATH);
	Append(LOG_LOG_CLEAR, (char *)usrname, strlen(usrname));
	m_logFileMutex.Leave();
	return av_true;
}

av_bool CAvLog::SetLogfilter(LogType &lt)
{
	m_Search_lt = lt;
	return av_true;
}
av_bool CAvLog::SetLogfilter(time_t st, time_t et)
{
	m_Search_st = st;
	m_Search_et = et;
	return av_true;
}
av_bool CAvLog::SetLogfilter(av_16 sOffset, av_16 eOffset)
{
	m_Search_sOffset = sOffset;
	m_Search_eOffset = eOffset;
	return av_true;
}
av_bool CAvLog::GetLogContext(av_16 &offset, LogItem &li)
{
	return av_true;
}

