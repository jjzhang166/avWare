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
#include "AvCapture/AvNetCapture.h"
#include "AvConfigs/AvConfigCapture.h"
#include "Common/icmp.h"
#include "AvThread/AvTimer.h"

#include "AvNetService/AvProtoMoon.h"
#include "AvNetService/AvOnvifClient.h"

CAvNetCapture::CAvNetCapture()
{
	CThread::SetThreadName(__FUNCTION__);
	m_NetProtoHandle = NULL;
	m_RemoteChannel = 0;
	m_Channel = 0;
	m_LinkStatus  = IRet_uninitialized;

	for (int i = CHL_MAIN_T; i < CHL_NR_T; i++){
		m_RecvFrameNm[i] = 0;
		m_TimerLastRecFrameNm[i] = 0;
	}

 	m_Timer.SetContinual(av_true);
	m_Timer.SetInterlvalMsec(5000);
	m_Timer.SetStartRunMSec(0);
	m_Timer.SetOnTimerProc(this, (CAvTimer::ONTIMER_PROC)&CAvNetCapture::OnTimer);
	m_bMainChnOffLine = av_false;
	m_bSubChnOffLine = av_false;

	m_ConfigNetCapture.Update();
	m_ConfigNetCapture.Attach(this, (AvConfigCallBack)&CAvNetCapture::OnConfigNetCaptureModify);


}
CAvNetCapture::~CAvNetCapture()
{
	m_LinkStatus = IRet_uninitialized;
	m_ConfigNetCapture.Detach(this, (AvConfigCallBack)&CAvNetCapture::OnConfigNetCaptureModify);

}

av_bool CAvNetCapture::Initialize(av_int Channel)
{
	m_Channel = Channel;
	return LoadConfigs();
}
av_bool CAvNetCapture::Start(av_int Slave)
{
	return m_NetProtoHandle->RemoteStreamStart(m_RemoteChannel, Slave) == IRet_succeed ? av_true : av_false;
}
av_bool CAvNetCapture::Stop(av_int Slave)
{
	return m_NetProtoHandle->RemoteStreamStop(m_RemoteChannel, Slave) == IRet_succeed ? av_true : av_false;
}

av_bool CAvNetCapture::Start(av_int Slave, CAvObject *obj, SIG_PROC_ONDATA pOnData)
{
	int ret = 0;
	ret = m_StreamSignal[Slave].Attach(obj, pOnData);
	if (ret < 0) {
		return av_false;
	}
	else{
		return av_true;
		///return m_ProtoHandle->ForceIFrame(m_CaptureRemoteChannel, Slave);
	}
}
av_bool CAvNetCapture::Stop(av_int Slave, CAvObject *obj, SIG_PROC_ONDATA pOnData)
{
	int ret = 0;
	ret = m_StreamSignal[Slave].Detach(obj, pOnData);
	if (ret < 0) {
		return av_false;
	}
	else{
		return av_true;
	}
}
CAvPacket * CAvNetCapture::Snapshot(av_bool bRealTime, av_uint SnapshotInterval, av_uint ContinuousTimes)
{
	if (m_NetProtoHandle == NULL) return NULL;
	return m_NetProtoHandle->RemoteSnapshot(m_RemoteChannel, bRealTime, SnapshotInterval, ContinuousTimes);
}


Capture::EAvCaptureStatus CAvNetCapture::GetCaptureStatus(av_int Slave)
{
	if (m_LinkStatus == IRet_linking){
		return Capture::EAvCapture_ING;
	}
	else{
		return Capture::EAvCapture_STOP;
	}
}


av_bool CAvNetCapture::CaptureGetCaps(C_CaptureCaps &CaptureCaps)
{
	if (m_NetProtoHandle == NULL) return av_false;
	return m_NetProtoHandle->RemoteCaptureGetCaps(m_RemoteChannel, CaptureCaps) == IRet_succeed ? av_true : av_false;
}
av_bool CAvNetCapture::CaptureGetProfile(C_CaptureProfile &CaptureProfile)
{
	if (m_NetProtoHandle == NULL) return av_false;
	return m_NetProtoHandle->RemoteCaptureGetProfile(m_RemoteChannel, CaptureProfile) == IRet_succeed ? av_true : av_false;
}
av_bool CAvNetCapture::CaptureSetProfile(C_CaptureProfile &CaptureProfile)
{
	if (m_NetProtoHandle == NULL) return av_false;
	return m_NetProtoHandle->RemoteCaptureSetProfile(m_RemoteChannel, CaptureProfile) == IRet_succeed ? av_true : av_false;
}

av_bool CAvNetCapture::EncodeGetCaps(C_EncodeCaps &EncodeCaps)
{
	if (m_NetProtoHandle == NULL) return av_false;
	return m_NetProtoHandle->RemoteEncodeGetCaps(m_RemoteChannel, EncodeCaps) == IRet_succeed ? av_true : av_false;
}
av_bool CAvNetCapture::EncodeGetProfile(int Slave, C_EnCodeProfile &EnCodeProfile)
{
	if (m_NetProtoHandle == NULL) return av_false;
	return m_NetProtoHandle->RemoteEncodeGetProfile(m_RemoteChannel, Slave, EnCodeProfile) == IRet_succeed ? av_true : av_false;
}
av_bool CAvNetCapture::EncodeSetProfile(int Slave, C_EnCodeProfile &EnCodeProfile)
{
	if (m_NetProtoHandle == NULL) return av_false;
	return m_NetProtoHandle->RemoteEncodeSetProfile(m_RemoteChannel, Slave, EnCodeProfile) == IRet_succeed ? av_true : av_false;
}
av_bool CAvNetCapture::AudioGetCaps(C_AudioCaps &AudioCaps)
{
	if (m_NetProtoHandle == NULL) return av_false;
	return m_NetProtoHandle->RemoteAudioGetCaps(m_RemoteChannel, AudioCaps) == IRet_succeed ? av_true : av_false;
}
av_bool CAvNetCapture::AudioGetProfile(C_AudioProfile &AudioProfile)
{
	if (m_NetProtoHandle == NULL) return av_false;
	return m_NetProtoHandle->RemoteAudioGetProfile(m_RemoteChannel, AudioProfile) == IRet_succeed ? av_true : av_false;
}
av_bool CAvNetCapture::AudioSetProfile(C_AudioProfile &AudioProfile)
{
	if (m_NetProtoHandle == NULL) return av_false;
	return m_NetProtoHandle->RemoteAudioSetProfile(m_RemoteChannel, AudioProfile) == IRet_succeed ? av_true : av_false;
}

av_bool CAvNetCapture::ImageGetCaps(C_ImageCaps &ImageCaps)
{
	if (m_NetProtoHandle == NULL) return av_false;
	return m_NetProtoHandle->RemoteImageGetCaps(m_RemoteChannel, ImageCaps) == IRet_succeed ? av_true : av_false;
}
av_bool CAvNetCapture::ImageGetProfile(C_ImageProfile &ImageProfile)
{
	if (m_NetProtoHandle == NULL) return av_false;
	return m_NetProtoHandle->RemoteImageGetProfile(m_RemoteChannel, ImageProfile) == IRet_succeed ? av_true : av_false;
}
av_bool CAvNetCapture::ImageSetProfile(C_ImageProfile &ImageProfile)
{
	if (m_NetProtoHandle == NULL) return av_false;
	return m_NetProtoHandle->RemoteImageSetProfile(m_RemoteChannel, ImageProfile) == IRet_succeed ? av_true : av_false;
}

av_bool CAvNetCapture::PtzGetCaps(C_PtzCaps &PtzCaps)
{
	if (m_NetProtoHandle == NULL) return av_false;
	return m_NetProtoHandle->RemotePtzGetCaps(m_RemoteChannel, PtzCaps) == IRet_succeed ? av_true : av_false;
}
av_bool CAvNetCapture::PtzGetProfile(C_PtzProfile &PtzProfile)
{
	if (m_NetProtoHandle == NULL) return av_false;
	return m_NetProtoHandle->RemotePtzGetProfile(m_RemoteChannel, PtzProfile) == IRet_succeed ? av_true : av_false;
}
av_bool CAvNetCapture::PtzSetProfile(C_PtzProfile &PtzProfile)
{
	if (m_NetProtoHandle == NULL) return av_false;
	return m_NetProtoHandle->RemotePtzSetProfile(m_RemoteChannel, PtzProfile) == IRet_succeed ? av_true : av_false;
}
av_bool CAvNetCapture::PtzSetCommand(C_PtzCmd &PtzCmd)
{
	if (m_NetProtoHandle == NULL) return av_false;
	return m_NetProtoHandle->RemotePtzSetCommand(m_RemoteChannel, PtzCmd) == IRet_succeed ? av_true : av_false;
}
av_bool  CAvNetCapture::AdvancedSystemGetCaps(C_AdvancedSystemCaps &AdvancedSystemCaps)
{
	if (m_NetProtoHandle == NULL) return av_false;
	return m_NetProtoHandle->RemoteAdvancedSystemGetCaps(m_RemoteChannel, AdvancedSystemCaps) == IRet_succeed ? av_true : av_false;
}
av_bool  CAvNetCapture::AdvancedSystemGetProfile(C_AdvancedSystemProfile &AdvancedSystemProfile)
{
	if (m_NetProtoHandle == NULL) return av_false;
	return m_NetProtoHandle->RemoteAdvancedSystemGetProfile(m_RemoteChannel, AdvancedSystemProfile) == IRet_succeed ? av_true : av_false;
}
av_bool  CAvNetCapture::AdvancedSystemSetProfile(C_AdvancedSystemProfile &AdvancedSystemProfile)
{
	if (m_NetProtoHandle == NULL) return av_false;
	return m_NetProtoHandle->RemoteAdvancedSystemSetProfile(m_RemoteChannel, AdvancedSystemProfile) == IRet_succeed ? av_true : av_false;

}
av_bool CAvNetCapture::LoadConfigs()
{
	m_ConfigNetCapture.Update();
	ConfigProtoFormats &Formats = m_ConfigNetCapture.GetConfig(m_Channel);
	if (Formats.IsEnable != av_true){
		return av_true;
	}


	return StartNetCapture(GetNetCaptureHandle(Formats));
}
av_bool CAvNetCapture::SendAlmMsgToSystem(C_AlmMsg &msg)
{
	CAvQmsg AlarmMsgQueue(ALARM_QUEUE_MSG_NAME);
	av_u32 msglen = sizeof(C_AlmMsg);
	return AlarmMsgQueue.QmSnd((av_char *)&msg, msglen);
}

void CAvNetCapture::ThreadProc()
{
	CAvPacket	*pAcket = NULL;
	av_bool		bSleep = av_true;
	C_AlmMsg	AlarmMsg;
	m_LinkStatus = IRet_linking;
	av_msg("Start NetCapture Channel %d Remote IpAddr[%s]\n", m_Channel, m_NetProtoHandle->ProtoFromats().CheckAliveAddress);
	
	I_RET ret = m_NetProtoHandle->Connect();
	if (ret != IRet_succeed){
		av_error("Link Server Error exit NetCapture Thread\n");
		m_LinkStatus = IRet_droplinked;
		return;
	}
	Start(CHL_MAIN_T);
	Start(CHL_SUB1_T);
	//Start(CHL_JPEG_T);


	while (m_Loop == av_true){
		bSleep = av_true;
		for (int i = CHL_MAIN_T; i < CHL_NR_T && m_Loop == av_true; i++){
			pAcket = m_NetProtoHandle->RemoteStreamGet(m_RemoteChannel, i);
			if (pAcket != NULL){
				bSleep = av_false;
				m_RecvFrameNm[i]++;
				pAcket->ConvertLocalChannel(m_Channel);
				m_StreamSignal[i](m_Channel, i, pAcket);
				pAcket->Release();
			}
		}
		
		while (IRet_succeed == m_NetProtoHandle->RemoteGetAlarmMsg(AlarmMsg))
		{
			AlarmMsg.Channel = m_Channel;
			SendAlmMsgToSystem(AlarmMsg);
		}

		if (bSleep == av_true && m_Loop == av_true){
			av_msleep(10);
		}
	}

	Stop(CHL_MAIN_T);
	Stop(CHL_SUB1_T);
	//Stop(CHL_JPEG_T);

	m_NetProtoHandle->Disconnect();
	m_LinkStatus = IRet_closeed;

	av_warning("Stop NetCapture Thread Channel %d m_NetProtoHandle not free\n", m_Channel);
	//delete m_NetProtoHandle;
	//m_NetProtoHandle = NULL;

	for (int i = CHL_MAIN_T; i < CHL_NR_T; i++){
		m_RecvFrameNm[i] = 0;
		m_TimerLastRecFrameNm[i] = 0;
	}

}

void CAvNetCapture::OnTimer(CAvTimer &Timer)
{
	av_bool bMainStreamOff = av_false;
	av_bool bSubStreamOff = av_false;
	if (m_LinkStatus == IRet_linking && NULL != m_NetProtoHandle){
		if (m_TimerLastRecFrameNm[CHL_MAIN_T] == m_RecvFrameNm[CHL_MAIN_T] && m_RecvFrameNm[CHL_MAIN_T] != 0){
			bMainStreamOff = av_true;
		}
		else if (m_bMainChnOffLine == av_true){
			PushAlmMsgToSys(CHL_MAIN_T, AlarmStat_Stop);
			m_bMainChnOffLine = av_false;
		}

		if (m_TimerLastRecFrameNm[CHL_SUB1_T] == m_RecvFrameNm[CHL_SUB1_T] && m_RecvFrameNm[CHL_SUB1_T] != 0){
			bSubStreamOff = av_true;
		}
		else if (m_bSubChnOffLine == av_true){
			PushAlmMsgToSys(CHL_SUB1_T, AlarmStat_Stop);
			m_bSubChnOffLine = av_false;
		}

		if (bMainStreamOff == av_true && bSubStreamOff == av_true){
			if (m_bMainChnOffLine == av_false && m_bSubChnOffLine == av_false){
				//设备掉线
				m_bMainChnOffLine = av_true;
				m_bSubChnOffLine = av_true;
				PushAlmMsgToSys(CHL_MAIN_T);
				PushAlmMsgToSys(CHL_SUB1_T);
			}
			av_error("CAvNetCapture::OnTimer Channle[%d] Lost Device Relink it\n", m_Channel);
			CThread::ThreadStop(av_true, 2 * 1000);
		}
		else if (bMainStreamOff == av_true){
			m_bMainChnOffLine = av_true;
			PushAlmMsgToSys(CHL_MAIN_T);
			av_error("CAvNetCapture::OnTimer Stream[CHL_MAIN_T] Lost Relink it\n");
			Stop(CHL_MAIN_T);
			Start(CHL_MAIN_T);
		}
		else if (bSubStreamOff == av_true){
			m_bSubChnOffLine = av_true;
			PushAlmMsgToSys(CHL_SUB1_T);
			av_error("CAvNetCapture::OnTimer Stream[CHL_SUB1_T] Lost Relink it\n");
			Stop(CHL_SUB1_T);
			Start(CHL_SUB1_T);
		}

		m_TimerLastRecFrameNm[CHL_MAIN_T] = m_RecvFrameNm[CHL_MAIN_T];
		m_TimerLastRecFrameNm[CHL_SUB1_T] = m_RecvFrameNm[CHL_SUB1_T];

	}
	else if (m_LinkStatus != IRet_linking && NULL != m_NetProtoHandle){
		if (m_bMainChnOffLine == av_false && m_bSubChnOffLine == av_false){
			//设备掉线
			m_bMainChnOffLine = av_true;
			m_bSubChnOffLine = av_true;
			PushAlmMsgToSys(CHL_MAIN_T);
			PushAlmMsgToSys(CHL_SUB1_T);
		}
		CICMPPing ICMPPing;
		C_ProtoFormats &ProtoFormats = m_NetProtoHandle->ProtoFromats();
		ICMPPing.SetRemoteHost(ProtoFormats.CheckAliveAddress);
		ICMPPing.SetTimeOut(200 * 1000);
		

		if (1 == ICMPPing.Ping()){
			av_warning("ping [%s] is ok relink server CThread::ThreadStart\n", ProtoFormats.CheckAliveAddress);
			CThread::ThreadStart();
		}
	}
	else{

	}



}

void CAvNetCapture::PushAlmMsgToSys(av_uint Slave, AlarmStat AlmStat, AlarmEvent AlmE)
{
	CAvQmsg AlarmMsgQueue(ALARM_QUEUE_MSG_NAME);
	C_AlmMsg MsgData;
	MsgData.AlarmStatus = AlmStat;
	MsgData.AlarmTime = (av_u32)time(NULL);
	MsgData.Channel = m_Channel;
	MsgData.Slave = Slave;
	MsgData.AlarmEventName = AlmE;
	av_u32 MsgDatalen = sizeof(C_AlmMsg);
	AlarmMsgQueue.QmSnd((av_char *)&MsgData, MsgDatalen);
}



av_bool CAvNetCapture::StartNetCapture(CAvNetProto *Handle)
{
	if (NULL == Handle){
		av_error("Handle is NULL\n");
		return av_false;
	}
	m_NetProtoHandle = Handle;

	CThread::ThreadStart();
	CAvTimer::StartTimer(m_Timer);
	
	return av_true;
}
av_bool CAvNetCapture::StopNetCapture()
{
	CAvTimer::StopTimer(m_Timer);
	CThread::ThreadStop(av_true, 2000);
	
	if (m_NetProtoHandle != NULL){
		m_NetProtoHandle->Disconnect();
		av_msg("delete m_NetProtoHandle \n");
		delete m_NetProtoHandle;
	}
	m_NetProtoHandle = NULL;

	return av_true;
}
CAvNetProto *CAvNetCapture::GetNetCaptureHandle(C_ProtoFormats &ProtoFormats)
{
	CAvNetProto *ProtoHandle = NULL;
	switch (ProtoFormats.ProtoMode)
	{
	case ProtocolMoon:
	{
		ProtoHandle = new CAvProtoMoon(ProtoFormats);
	}
	break;
	case ProtocolOnvif:
	{
		ProtoHandle = new CAvOnvifClient();
	}
	break;
	case ProtocolRtsp:
		break;

	default:
		av_error("Unkown this Protocol [%d]\n", ProtoFormats.ProtoMode);
		break;
	}

	return ProtoHandle;
}

av_void CAvNetCapture::OnConfigNetCaptureModify(CAvConfigProtocol *ConfigProtocol, int &result)
{
	ConfigProtoFormats &OldConfigProtoProfile = m_ConfigNetCapture.GetConfig(m_Channel);
	ConfigProtoFormats &NewConfigProtoProfile = ConfigProtocol->GetConfig(m_Channel);
	result = 0;

	av_bool bChange = av_false;

	{
		if (OldConfigProtoProfile.IsEnable != NewConfigProtoProfile.IsEnable ||
			0 != strcmp(OldConfigProtoProfile.UsrName, NewConfigProtoProfile.UsrName) ||
			0 != strcmp(OldConfigProtoProfile.Passwd, NewConfigProtoProfile.Passwd) ||
			0 != strcmp(OldConfigProtoProfile.CheckAliveAddress, NewConfigProtoProfile.CheckAliveAddress) ||
			OldConfigProtoProfile.ProtoMode != NewConfigProtoProfile.ProtoMode){
			bChange = av_true;
		}
		else{
			switch (OldConfigProtoProfile.ProtoMode)
			{
			case ProtocolMoon:
			{
				if (OldConfigProtoProfile.MoonFormats.Port != NewConfigProtoProfile.MoonFormats.Port ||
					0 != strcmp(OldConfigProtoProfile.MoonFormats.Url, NewConfigProtoProfile.MoonFormats.Url)){
					bChange = av_true;
				}
			}
				break;
			case ProtocolOnvif:
				break;
			case ProtocolRtmp:
				break;
			case ProtocolRtsp:
				break;

			default:
				return;
				break;
			}
		}
	}

	//if (0 == memcmp(&OldConfigProtoProfile, &NewConfigProtoProfile, sizeof(ConfigProtoFormats))){
	if (bChange == av_false){
		return ;
	}
	else{
		if (m_NetProtoHandle != NULL){
			StopNetCapture();
		}

		if (NewConfigProtoProfile.IsEnable == av_true){
			StartNetCapture(GetNetCaptureHandle(NewConfigProtoProfile));
		}
		OldConfigProtoProfile = NewConfigProtoProfile;
		av_warning("NetCapture new Config\n");
	}

}