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
#ifndef _AVALARM_H_
#define _AVALARM_H_

#include "Signals.h"
#include "Apis/AvWareType.h"
#include "AvThread/AvThread.h"
#include "Apis/LibSystem.h"
#include "AvThread/AvQmsg.h"
#include "Apis/LibLocalAlm.h"
#include "AvThread/AvTask.h"
#include "AvConfigs/AvConfigNetService.h"
#include "AvConfigs/AvConfigAlarm.h"


#define ALARM_QUEUE_MSG_NAME "Alm_Msg_Queue"

class CAvAlarm:public CThread
{
public:
	PATTERN_SINGLETON_DECLARE(CAvAlarm);
private:
	CAvAlarm();
	~CAvAlarm();
public:
	enum AvAlarmType
	{
		AvAlmT_NONE = 0,
		AvAlmT_PORT_In = 1,
		AvAlmT_PORT_Out = 2,
		///人脸识别
		AvAlmT_VIDEO_FaceRecognition = 3,
		///人脸检测
		AvAlmT_VIDEO_FaceDetection = 4,
		///移动帧测
		AvAlmT_VIDEO_MotionDetection = 5,
		///移动跟踪
		AvAlmT_VIDEO_MotionFollow = 6,
		///车牌识别
		AvAlmT_VIDEO_PlatenumberRecognition = 7,
		///设备掉线
		AvAlmT_VIDEO_DevOffline = 8,
		///视频丢失
		AvAlmT_VIDEO_Lost = 9,
		///视频遮挡
		AvAlmT_VIDEO_Blind = 10,
		///物品遗失
		AvAlmT_VIDEO_SomethingLoss = 11,
		///硬盘
		AvAlmT_DISK_ERROR = 12,
		AvAlmT_DISK_LOST = 13,
		AvAlmT_LAST = 14
	};
	inline static std::string GetCAvAlarmAlarmTypeString(AvAlarmType e)
	{
		static std::string sCAvAlarmAlarmTypeString[AvAlmT_LAST + 1] =
		{ "None", "Port In", "Port Out", "FaceRecognition", "FaceDetection", "MotionDetection", "MotionFollow", "PlatenumberRecognition",
		"DevOffline", "VIDEO_Lost", "VIDEO_Blind", "SomethingLoss",
		"DISK_ERROR", "DISK_LOST" };
		return sCAvAlarmAlarmTypeString[e];
	};
	enum AvAlarmStat{
		AvAlm_Stat_NONE,
		AvAlm_Stat_Open,
		AvAlm_Stat_Close,
		AvAlm_Stat_Ongoing,
		AvAlm_Stat_NR,
	};

	typedef struct {
		CAvAlarm::AvAlarmType	AlmType;
		av_u32					AlmTmSec;
		AvAlarmStat				AlmStatus;//av_ture open; av_false close
		av_uchar				Channel;
		av_uchar				Slave;

		union 
		{
			C_AlmMdResult MdResult;
			C_AlmIoResult IoResult;
		};

	}AlmMsg;

public:
	typedef TSignal1<AlmMsg &>::SigProc OnAvAlarmSigNalFunc;
	av_bool Initialize();

	av_bool Start(CObject *obj, OnAvAlarmSigNalFunc proc);
	av_bool Stop(CObject *obj, OnAvAlarmSigNalFunc proc);

private:
	av_void ThreadProc();
	CAvQmsg m_AlarmMsg;

	TSignal1<AlmMsg &> m_AlarmSignal;


};



#define g_AvAlarm (*CAvAlarm::instance())



class CAvAlmTask:public CAvTask
{
public:
	CAvAlmTask();
	~CAvAlmTask();
	

	av_void TaskJob();
	av_bool SetAlmMsg(CAvAlarm::AlmMsg m_AlmMsg);
	av_bool SetalmSignal(TSignal1<CAvAlarm::AlmMsg &> *signal);
private:

	av_void OnSendEmail();
	av_void OnFtpSend();
	av_bool CheckEmailStartUp();
	av_bool CheckTimeSection(C_WeekSpan *pWeekSpan);
	
	CAvAlarm::AlmMsg m_AlmMsg;
	TSignal1<CAvAlarm::AlmMsg &> *m_AlarmSignal;
};



#endif

