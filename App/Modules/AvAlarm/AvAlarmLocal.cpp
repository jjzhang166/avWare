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
#include "AvAlarm/AvAlarmLocal.h"
#include "Apis/LibEncode.h"
#include "CAvObject.h"
#include "AvDevice/AvDevice.h"
#include "Apis/AvEnuminline.h"


SINGLETON_IMPLEMENT(CAvAlmLocal)
CAvAlmLocal::CAvAlmLocal() :CThread(__FUNCTION__)
{
}

CAvAlmLocal::~CAvAlmLocal()
{
}

av_bool CAvAlmLocal::CheckConfIo()
{
	return av_true;
}
av_bool CAvAlmLocal::CheckConfMd()
{
	return av_true;
}

av_bool CAvAlmLocal::SendAlmMsg(C_AlmMsg  &msg)
{
	CAvQmsg AlarmMsgQueue(ALARM_QUEUE_MSG_NAME);
	av_u32 msglen = sizeof(C_AlmMsg);
	return AlarmMsgQueue.QmSnd((av_char *)&msg, msglen);
}

av_bool CAvAlmLocal::PacketAlmIo(av_u32 Result, av_bool bOpen)
{
	//是否开启了almio in 配置

	C_AlmMsg msg;
	msg.AlarmTime = (av_u32)time(NULL);
	msg.AlarmEventName = AlarmEvent_PORT_In;
	msg.Channel = -1;
	msg.Slave = CHL_NR_T;

	msg.AlarmStatus = bOpen == av_false ? AlarmStat_Stop : AlarmStat_Start;
	msg.IoResult = Result;

	return SendAlmMsg(msg);
}

av_bool CAvAlmLocal::PacketAlmMd(av_u32 Result[], av_bool bOpen)
{
	C_AlmMsg msg;
	msg.AlarmTime = (av_u32)time(NULL);
	msg.AlarmEventName = AlarmEvent_VIDEO_MotionDetection;
	msg.Channel = -1;
	msg.Slave = CHL_NR_T;

	msg.AlarmStatus = bOpen == av_false ? AlarmStat_Stop : AlarmStat_Start;
	memcpy(msg.MdResult, Result, sizeof(msg.MdResult));

	return SendAlmMsg(msg);
}

void CAvAlmLocal::ThreadProc()
{
	av_bool bRet = av_false;
	av_u32  AlmIoMask = 0;

	av_u32 AlmIoResult = 0, AlmIoLastResult = 0;

	av_u32  AlmMdResult[ConfMotionDetectionLine] = {0};
	av_u32	AlmMdLastResult[ConfMotionDetectionLine] = {0};
	av_u32	AlmMdZeroResult[ConfMotionDetectionLine] = {0};
	av_bool	AlmMdlastbOpen = av_false;
	time_t	LastAlmTime = 0;
	

	while (av_true == m_Loop){
		if (0 != m_AlarmCaps.MaxAlmIn && av_true == CheckConfIo()){
			bRet = AvGetIoAlmStatus(&AlmIoResult);
			if (bRet == av_true && AlmIoResult != AlmIoLastResult){
				PacketAlmIo(AlmIoResult, AlmIoResult == 0?av_false:av_true);
				AlmIoLastResult = AlmIoResult;
			}
		}

		if (av_true == CheckConfMd()  && time(NULL) - LastAlmTime  > 1 && (m_AlarmCaps.AlarmEventMask & AvMask(AlarmEvent_VIDEO_MotionDetection))){
			bRet = avGetMdAlmStatus(AlmMdResult);
			if (bRet == av_true && AlmMdlastbOpen == av_false && 0 != memcmp(AlmMdResult, AlmMdLastResult, sizeof(AlmMdLastResult))){
				PacketAlmMd(AlmMdResult, av_true);
				memcpy(AlmMdLastResult, AlmMdResult, sizeof(AlmMdLastResult));
				AlmMdlastbOpen = av_true;
				LastAlmTime = time(NULL);
			}
			else if (bRet == av_true && AlmMdlastbOpen  == av_true && 0 == memcmp(AlmMdResult, AlmMdZeroResult, sizeof(AlmMdZeroResult))){
				PacketAlmMd(AlmMdZeroResult, av_false);
				memcpy(AlmMdLastResult, AlmMdZeroResult, sizeof(AlmMdZeroResult));
				AlmMdlastbOpen = av_false;
				LastAlmTime = time(NULL);
			}
		}

		av_msleep(40);
	}

	avAlarmDestory();
}

#include <cmath>
av_bool CAvAlmLocal::Initialize()
{
	av_bool bRet = av_false;

	bRet = AvAlarmCaps(&m_AlarmCaps);

	CThread::ThreadStart();

	//md
	CAvConfigAlarm ConfigAlarm;
	ConfigAlarm.Update();

	av_bool bVaildFactoryInfo = av_false;
	std::string bVaild;

	CAvDevice::GetEnv(std::string(EKey_BvalidFactoryInfo), bVaild);
	if (bVaild == std::string(EnumNameav_bool(av_true))){
		bVaildFactoryInfo = av_true;
		avAlarmCreate();
	}
	else{
		av_error("FactoryInfo is  Invalid CAvAlmLocal Task not Initialize\n");
	}

	av_msg("m_AlarmCaps.AlarmEventMask[0x%x]\n", m_AlarmCaps.AlarmEventMask);
	if (bVaildFactoryInfo == av_true && (m_AlarmCaps.AlarmEventMask & AvMask(AlarmEvent_VIDEO_MotionDetection))){
		ConfigAlarmProfile &Profile = ConfigAlarm.GetConfig();
		av_msg("AlarmEventMask[0x%x]\n", Profile.AlarmEventMask);
		if (Profile.AlarmEventMask & AvMask(AlarmEvent_VIDEO_MotionDetection)){
			av_bool ret = av_false;
			int i = 0;
			for (; i < 18; ++i) {
				av_msg("Profile.AlarmMotionArea[%d] = %d\n", i, Profile.AlarmMotionArea[i]);
			}
			
			ret = avSetMdProfile(Profile.AlarmMotionArea, Profile.AlarmMotionLevel);
			if (ret != av_true){
				av_error("avSetMdProfile return error \n");
			}
		}
	}

#if 0 //For MD Test 
	C_AlmMdParam param = {0};
	param.Enable = 1;
	param.Level = 6;
	int i = 0;
	for (; i < 18; i++) {
		//*
		//上半部区域
		if (i < 9) {
			param.Win[i] = 4294967295;
		} else {
			param.Win[i] = 0;
		}
		//*/
		
		/*
		//左半边区域
		int line_base = (int)pow(2, 11);
		line_base -= 1;
		line_base = line_base << 21;
		param.Win[i] = line_base;
		*/
	}
	AvMdAlmSetParameter(0, &param);
#endif

	return av_true;
}

av_bool CAvAlmLocal::AlarmOutCtrl(av_bool bOpen)
{
	return av_true;
}
