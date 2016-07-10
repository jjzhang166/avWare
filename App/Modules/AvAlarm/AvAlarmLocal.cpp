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

av_bool CAvAlmLocal::SendAlmMsg(CAvAlarm::AlmMsg  &msg)
{
	CAvQmsg AlarmMsgQueue(ALARM_QUEUE_MSG_NAME);
	av_u32 msglen = sizeof(CAvAlarm::AlmMsg);
	return AlarmMsgQueue.QmSnd((av_char *)&msg, msglen);
}

av_bool CAvAlmLocal::PacketAlmIo(C_AlmIoResult *Result)
{
	//是否开启了almio in 配置

	CAvAlarm::AlmMsg msg;
	msg.AlmTmSec = (av_u32)time(NULL);
	msg.AlmType = CAvAlarm::AvAlmT_PORT_In;
	msg.Channel = -1;
	msg.Slave = CHL_NR_T;

	msg.AlmStatus = Result->AlmIoCount == 0 ? CAvAlarm::AvAlm_Stat_Close:CAvAlarm::AvAlm_Stat_Open;
	msg.IoResult = *Result;

	return SendAlmMsg(msg);
}
av_bool CAvAlmLocal::PacketAlmMd(C_AlmMdResult *Result)
{
	CAvAlarm::AlmMsg msg;
	msg.AlmTmSec = (av_u32)time(NULL);
	msg.AlmType = CAvAlarm::AvAlmT_VIDEO_MotionDetection;
	msg.Channel = -1;
	msg.Slave = CHL_NR_T;

	msg.AlmStatus = Result->AlmAreaCount == 0 ? CAvAlarm::AvAlm_Stat_Close : CAvAlarm::AvAlm_Stat_Open;
	msg.MdResult = *Result;

	return SendAlmMsg(msg);
}

void CAvAlmLocal::ThreadProc()
{
	av_bool bRet = av_false;
	av_u32  AlmIoMask = 0;

	C_AlmIoResult AlmIoResult;
	C_AlmIoResult AlmIoLastResult;
	memset(&AlmIoResult, 0x00, sizeof(C_AlmIoResult));
	memset(&AlmIoLastResult, 0x00, sizeof(C_AlmIoResult));

	av_bool bSuportMd = av_false;
	av_u32  AlmMdLastResultCount = 0;
	C_AlmMdResult AlmMdResult;
	memset(&AlmMdResult, 0x00, sizeof(C_AlmMdResult));

	if (m_AlmMdCaps.MaxLine > 0 && m_AlmMdCaps.MaxLine < 16 && m_AlmMdCaps.MaxLine > 0 && m_AlmMdCaps.MaxLine < 16){
		bSuportMd = av_true;
	}

	for (int i = 0; i < m_AlmIoCaps.MaxAlmInNum; i++){
		AlmIoMask = AlmIoMask << 1  | 0x01;
	}
	while (av_true == m_Loop){
		if (0 != m_AlmIoCaps.MaxAlmInNum && av_true == CheckConfIo()){
			bRet = AvGetIoAlmStatus(AlmIoMask, &AlmIoResult);
			if (bRet == av_true && AlmIoResult.AlmIoStatusMask != AlmIoLastResult.AlmIoStatusMask){
				//alm io
				PacketAlmIo(&AlmIoResult);
				AlmIoLastResult = AlmIoResult;
			}
		}

		if (av_true == bSuportMd && av_true == CheckConfMd()){
			bRet = avGetMdAlmStatus(&AlmMdResult);
			if (bRet == av_true && AlmMdResult.AlmAreaCount != AlmMdLastResultCount){
				//alm md
				PacketAlmMd(&AlmMdResult);
				AlmMdLastResultCount = AlmMdResult.AlmAreaCount;
			}
		}

		av_msleep(100);
	}
}


av_bool CAvAlmLocal::Initialize()
{
	av_bool bRet = av_false;
	bRet = AvGetIoAlmCaps(&m_AlmIoCaps);

	bRet = avGetMdAlmCaps(&m_AlmMdCaps);
	CThread::run();
	return av_true;
}

av_bool CAvAlmLocal::AlarmOutCtrl(av_bool bOpen)
{
	return av_true;
}