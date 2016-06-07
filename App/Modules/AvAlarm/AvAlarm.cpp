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
#include "AvAlarm/AvalarmLocal.h"
#include "AvNetService/AvSmtp.h"
#include "AvNetService/AvFtp.h"
#include "AvDevice/AvDevice.h"


PATTERN_SINGLETON_IMPLEMENT(CAvAlarm)

CAvAlarm::CAvAlarm() :CThread(__FUNCTION__), m_AlarmMsg(ALARM_QUEUE_MSG_NAME)
{

}
CAvAlarm::~CAvAlarm()
{

}
av_bool CAvAlarm::Initialize()
{
	CThread::run();

	g_AvAlmLocal.Initialize();

	return av_true;
}

av_bool CAvAlarm::Start(CObject *obj, OnAvAlarmSigNalFunc proc)
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
av_bool CAvAlarm::Stop(CObject *obj, OnAvAlarmSigNalFunc proc)
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
	
	av_msg("Get Alarm Type [%s]\n", CAvAlarm::GetCAvAlarmAlarmTypeString(m_AlmMsg.AlmType).c_str());
	switch (m_AlmMsg.AlmType)
	{
	case CAvAlarm::AvAlmT_DISK_ERROR:		
		break;
	case CAvAlarm::AvAlmT_DISK_LOST:
		break;
	case CAvAlarm::AvAlmT_LAST:
		break;
	case CAvAlarm::AvAlmT_NONE:
		break;
	case CAvAlarm::AvAlmT_PORT_In:
		break;
	case CAvAlarm::AvAlmT_PORT_Out:
		break;
	case CAvAlarm::AvAlmT_VIDEO_Blind:
		break;
	case CAvAlarm::AvAlmT_VIDEO_DevOffline:
		break;
	case CAvAlarm::AvAlmT_VIDEO_FaceDetection:
		break;
	case CAvAlarm::AvAlmT_VIDEO_FaceRecognition:
		break;
	case CAvAlarm::AvAlmT_VIDEO_Lost:
		break;
	case CAvAlarm::AvAlmT_VIDEO_MotionDetection:
		//获取报警配置 若发送邮件使能 则发送邮件
		OnSendEmail();
		break;
	case CAvAlarm::AvAlmT_VIDEO_MotionFollow:
		break;
	case CAvAlarm::AvAlmT_VIDEO_PlatenumberRecognition:
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

av_bool CAvAlmTask::CheckEmailStartUp()
{
	switch (m_AlmMsg.AlmType)
	{
		case CAvAlarm::AvAlmT_VIDEO_MotionDetection:
		{
			CAvConfigAlarmMd AvConfigAlarmMd;
			AvConfigAlarmMd.Update();
			ConfigMdAlarmFormats MdAlarmConfig = AvConfigAlarmMd.GetConfig();
			if (MdAlarmConfig.SendEmail == av_false)
			{
				return av_false;
			}

			if (CheckTimeSection(&MdAlarmConfig.WeekSpan) != av_true)
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
	CAvConfigNetSerSmtp AvConfigNetSmtp;
	AvConfigNetSmtp.Update();
	ConfigNetSerSmtp smtp_config;
	smtp_config = AvConfigNetSmtp.GetConfig();

	if (smtp_config.enable == av_true)
	{
		CAvSmtp Smtp;
		std::string port;
		std::string Content;

		if (CheckEmailStartUp() != av_true)
		{
			av_msg("CheckEmail fail...\n");
			return;
		}
		
		sprintf((char *)port.c_str(), "%d", smtp_config.port);
		Smtp.SetServer(smtp_config.smtp_server, port, (CAvSmtp::E_SMTP_EN_MODE)smtp_config.save_mode);
		Smtp.SetSender(smtp_config.sender);
		for (i = 0; i < MAX_EMAIL_RECEIVER; i++)
		{
			Smtp.SetAddRecver(smtp_config.receiver[i]);
		}
		Smtp.SetAuthorization(smtp_config.account, smtp_config.password);
		Smtp.SetTitle(smtp_config.title);
		Content = CAvAlarm::GetCAvAlarmAlarmTypeString(m_AlmMsg.AlmType);
		Smtp.SetContent(Content);
		Smtp.Start();
	}
}

av_void CAvAlmTask::OnFtpSend()
{

}
