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
#include "AvAlarm/AvAlarm.h"
#include "AvAlarm/AvAlarmLocal.h"
#include "AvNetService/AvSmtp.h"
#include "AvNetService/AvFtp.h"
#include "AvDevice/AvDevice.h"
#include "Apis/AvEnuminline.h"

SINGLETON_IMPLEMENT(CAvAlarm)

CAvAlarm::CAvAlarm() :CThread(__FUNCTION__), m_AlarmMsg(ALARM_QUEUE_MSG_NAME)
{

}
CAvAlarm::~CAvAlarm()
{

}
av_bool CAvAlarm::Initialize()
{
	CThread::ThreadStart();

	g_AvAlmLocal.Initialize();

	return av_true;
}

av_bool CAvAlarm::Start(CAvObject *obj, OnAvAlarmSigNalFunc proc)
{
	int ret = 0;
	ret = m_AlarmSignal.Attach(obj, proc);
	if (ret < 0) {
		return av_false;
	}
	else{
		return av_true;
	}
}
av_bool CAvAlarm::Stop(CAvObject *obj, OnAvAlarmSigNalFunc proc)
{
	int ret = 0;
	ret = m_AlarmSignal.Detach(obj, proc);
	if (ret < 0) {
		return av_false;
	}
	else{
		return av_true;
	}
}

av_void CAvAlarm::ThreadProc()
{

	AlmMsg AlmMsgData;
	av_u32 len = sizeof(AlmMsg);

	av_bool AlarmMsgRet = av_false;
	while (m_Loop == av_true)
	{

		AlarmMsgRet = m_AlarmMsg.QmRcv((av_char *)&AlmMsgData, len, av_false);
		if (AlarmMsgRet == av_true){
			CAvAlmTask *AlmTask = new CAvAlmTask;
			AlmTask->SetAlmMsg(AlmMsgData);
			AlmTask->SetalmSignal(&m_AlarmSignal);
			AlmTask->SetTaskName(std::string("CAvAlarm"));
			AlmTask->StartTask(av_true);
		}
		else{
			av_msleep(30);
		}
	}
}




CAvAlmTask::CAvAlmTask()
{
}

CAvAlmTask::~CAvAlmTask()
{
}


av_void CAvAlmTask::TaskJob()
{
	(*m_AlarmSignal)(m_AlmMsg);
	
	av_msg("Get Alarm Type [%s]\n", EnumNameAlarmEvent(m_AlmMsg.AlmType));
	switch (m_AlmMsg.AlmType)
	{

	case AlarmEvent_PORT_In:
		OnSendEmail();
		break;
	case AlarmEvent_PORT_Out:
		break;
	case AlarmEvent_VIDEO_FaceRecognition:
		break;
	case AlarmEvent_VIDEO_FaceDetection:
		break;
	case AlarmEvent_VIDEO_MotionDetection:
		OnSendEmail();
		break;
	case AlarmEvent_VIDEO_MotionFollow:
		break;
	case AlarmEvent_VIDEO_PlatenumberRecognition:
		break;
	case AlarmEvent_VIDEO_DevOffline:
		break;
	case AlarmEvent_VIDEO_Lost:
		break;
	case AlarmEvent_VIDEO_Blind:
		break;
	case AlarmEvent_VIDEO_SomethingLoss:
		break;
	case AlarmEvent_DISK_ERROR:
		break;
	case AlarmEvent_DISK_LOST:
		break;
	default:
		break;
	}
}

av_bool CAvAlmTask::SetAlmMsg(CAvAlarm::AlmMsg AlmMsg)
{
	m_AlmMsg = AlmMsg;
	return av_true;
}
av_bool CAvAlmTask::SetalmSignal(TSignal1<CAvAlarm::AlmMsg &> *signal)
{
	m_AlarmSignal = signal;
	return av_true;
}
#if 0
av_bool CAvAlmTask::CheckTimeSection(C_WeekSpan *pWeekSpan)
{
	if (NULL == pWeekSpan)
	{
		av_error("input param pWeekSpan is error...\n");
		return av_false;
	}

	int n = 0;
	int current, start, end, week;
	av_timeval system_time;
	CAvDevice::GetSysTime(system_time);
	week = system_time.t_week % 7;
	current = (system_time.t_hour * 60 + system_time.t_min) * 60 + system_time.t_sec;

	if (pWeekSpan->DaySpan[week].DayEnable != av_true)
	{
		av_error("DaySpan %d is not enable...\n", week);
		return av_false; 
	}
	
	for (n = 0; n < ConfMaxDayTimeSpan; n++)
	{
		if (pWeekSpan->DaySpan[week].TimeSpan[n].TimeEnable != av_true)
		{
			continue;
		}
		start = pWeekSpan->DaySpan[week].TimeSpan[n].StartSec;
		end = pWeekSpan->DaySpan[week].TimeSpan[n].EndSec;
		if (current >= start && current < end)
		{
			break;
		}
	}

	if (n == ConfMaxDayTimeSpan)
	{
		av_error("DayTimeSpan is invalid...\n");
		return av_false;
	}
	
	return av_true;
}
#endif

av_bool CAvAlmTask::CheckEmailStartUp()
{
	switch (m_AlmMsg.AlmType)
	{
		case AlarmEvent_VIDEO_MotionDetection:
		{
			CAvConfigAlarm AvConfigAlarm;
			AvConfigAlarm.Update();
			ConfigAlarmProfile AlarmConfig = AvConfigAlarm.GetConfig();

			if (AlarmConfig.AlarmbLinkageEmail == av_false)
			{
				return av_false;
			}
			
			break;
		}

		default:
			av_error("AlmType %d is invalid...\n", m_AlmMsg.AlmType);
			return av_false;
	}
	return av_true;
}

av_void CAvAlmTask::OnSendEmail()
{
// 	check enable send email
// 	check send email time is valid
// 	CAvSmtp Smtp;
// 	Smtp.SetSender("sender@email.com");
// 	Smtp.SetServer("smtp.email.com");
// 	Smtp.SetAddRecver("recv@email.com");
// 
// 	Smtp.Start();
	int i = 0;
	CAvConfigNetSmtp AvConfigNetSmtp;
	AvConfigNetSmtp.Update();
	ConfigNetSmtp smtp_config;
	smtp_config = AvConfigNetSmtp.GetConfig();

	if (smtp_config.bEnable == av_true)
	{
		CAvSmtp Smtp;
		std::string port;
		std::string Content;

		if (CheckEmailStartUp() != av_true)
		{
			av_msg("CheckEmail fail...\n");
			return;
		}
		CAvSmtp::E_SMTP_EN_MODE EnMode;
		switch (smtp_config.EncodeType)
		{
		case	EmailEncodeType_TLS:
			EnMode = CAvSmtp::MAIL_ENTLS;
			break;
			
		case 	EmailEncodeType_SSL:
			EnMode = CAvSmtp::MAIL_ENSSL;
				break;
			
		case EmailEncodeType_NULL:
			EnMode = CAvSmtp::MAIL_ENNULL;
				break;
		default:
			break;
		}
		sprintf((char *)port.c_str(), "%d", smtp_config.ServicePort);
		Smtp.SetServer(smtp_config.ServerAddress, port, EnMode);
		Smtp.SetSender(smtp_config.Sender);
		if (0 != strlen(smtp_config.Receiver1)){
			Smtp.SetAddRecver(smtp_config.Receiver1);
		}
		if (0 != strlen(smtp_config.Receiver2)){
			Smtp.SetAddRecver(smtp_config.Receiver2);
		}
		Smtp.SetAuthorization(smtp_config.UserName, smtp_config.PassWord);
		Smtp.SetTitle(smtp_config.Title);
		Content = EnumNameAlarmEvent(m_AlmMsg.AlmType);
		Smtp.SetContent(Content);
		Smtp.Start();
	}
}

av_void CAvAlmTask::OnFtpSend()
{

}
