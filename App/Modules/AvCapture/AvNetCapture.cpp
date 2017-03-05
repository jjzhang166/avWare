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
	m_Timer.SetInterlvalMsec(10000);
	m_Timer.SetStartRunMSec(0);
	m_Timer.SetOnTimerProc(this, (CAvTimer::ONTIMER_PROC)&CAvNetCapture::OnTimer);

}
CAvNetCapture::~CAvNetCapture()
{
	m_LinkStatus = IRet_uninitialized;
}

av_bool CAvNetCapture::Initialize(av_int Channel)
{
	m_Channel = Channel;

	return av_true;
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
	if (Slave == -1){
		for (int i = CHL_MAIN_T; i < CHL_NR_T; i++){
			if (m_RecvFrameNm[i] != 0){
				return Capture::EAvCapture_ING;
			}
		}
		return Capture::EAvCapture_STOP;
	}
	return Capture::EAvCapture_STOP;
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
	CAvConfigProtocol NetProtocol;
	NetProtocol.Update();
	ConfigProtoFormats &Formats = NetProtocol.GetConfig(m_Channel);
	

	return av_true;
}

void CAvNetCapture::ThreadProc()
{
	CAvPacket *pAcket = NULL;
	av_bool bSleep = av_true;
	av_msg("Start NetCapture Channel %d\n", m_Channel);
	m_LinkStatus = IRet_linking;
	I_RET ret = m_NetProtoHandle->Connect();
	if (ret != IRet_succeed){
		av_error("Link Server Error exit NetCapture Thread\n");
		m_LinkStatus = IRet_droplinked;
		return;
	}
	m_LinkStatus = IRet_succeed;
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

		if (bSleep == av_true && m_Loop == av_true){
			av_msleep(20);
		}
	}

	Stop(CHL_MAIN_T);
	Stop(CHL_SUB1_T);
	Stop(CHL_JPEG_T);

	m_NetProtoHandle->Disconnect();
	m_LinkStatus = IRet_closeed;

	av_warning("Stop NetCapture Thread Channel %d m_NetProtoHandle not free\n", m_Channel);
	//delete m_NetProtoHandle;
	//m_NetProtoHandle = NULL;

	for (int i = CHL_MAIN_T; i < CHL_NR_T; i++){
		m_RecvFrameNm[i] = 0;
	}

}

void CAvNetCapture::OnTimer(CAvTimer &Timer)
{
	av_bool bMainStreamOff = av_false;
	av_bool bSubStreamOff = av_false;
	if (m_LinkStatus == IRet_succeed){
		if (m_TimerLastRecFrameNm[CHL_MAIN_T] == m_RecvFrameNm[CHL_MAIN_T]){
			bMainStreamOff = av_true;
		}

		if (m_TimerLastRecFrameNm[CHL_SUB1_T] == m_RecvFrameNm[CHL_SUB1_T]){
			bSubStreamOff = av_true;
		}

		if (bMainStreamOff == av_true && bSubStreamOff == av_true){
			CThread::ThreadStop(av_true, 2 * 1000);
		}
		else if (bMainStreamOff == av_true){
			Stop(CHL_MAIN_T);
			Start(CHL_MAIN_T);
		}
		else if (bSubStreamOff == av_true){
			Stop(CHL_SUB1_T);
			Start(CHL_SUB1_T);
		}
	}
	else if (m_LinkStatus != IRet_linking && NULL != m_NetProtoHandle){
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
	m_TimerLastRecFrameNm[CHL_MAIN_T] = m_RecvFrameNm[CHL_MAIN_T];
	m_TimerLastRecFrameNm[CHL_SUB1_T] = m_RecvFrameNm[CHL_SUB1_T];


}

av_bool CAvNetCapture::StartNetCapture(CAvNetProto *Handle)
{

	m_NetProtoHandle = Handle;
	for (int i = 0; i < CHL_NR_T; i++){
		m_RecvFrameNm[i] = 1;
	}
	CThread::ThreadStart();
	CAvTimer::StartTimer(m_Timer);
	
	return av_true;
}
av_bool CAvNetCapture::StopNetCapture()
{
	CThread::ThreadStop(av_true, 2000);
	CAvTimer::StopTimer(m_Timer);

	if (m_NetProtoHandle != NULL){
		av_msg("delete m_NetProtoHandle \n");
		delete m_NetProtoHandle;
	}
	m_NetProtoHandle = NULL;

	return av_true;
}