#include "AvCapture/AvDevCapture.h"
#include "AvAlarm/AvAlarm.h"
#include "AvAudio/AvAudio.h"
#include "AvDevice/AvDevice.h"
#include "Apis/LibEncode.h"
CAvDevCapture::CAvDevCapture()
{
	CThread::SetThreadName(__FUNCTION__);
	m_Channel = 0;
	m_LastCaptureSyncStat = E_Capture_VideoNONE;
	
	m_Snap = NULL;

	m_ConfigEncode.Update(m_Channel);
	m_ConfigEncode.Attach(this, (AvConfigCallBack)&CAvDevCapture::OnConfigEncodeModify);

	m_ConfigImage.Update(m_Channel);
	m_ConfigImage.Attach(this, (AvConfigCallBack)&CAvDevCapture::OnConfigImageModify);

	m_ConfigCover.Update(m_Channel);
	m_ConfigCover.Attach(this, (AvConfigCallBack)&CAvDevCapture::OnConfigCoverModify);

	m_ConfigWaterMark.Update(m_Channel);
	m_ConfigWaterMark.Attach(this, (AvConfigCallBack)&CAvDevCapture::OnConfigWaterMarkingModify);

	m_ConfigAudio[CHL_ACAP_T].Update(CHL_ACAP_T);
	m_ConfigAudio[CHL_ACAP_T].Attach(this, (AvConfigCallBack)&CAvDevCapture::OnConfigAudio);

	m_ConfigAudio[CHL_APLY_T].Update(CHL_APLY_T);
	m_ConfigAudio[CHL_APLY_T].Attach(this, (AvConfigCallBack)&CAvDevCapture::OnConfigAudio);
}
CAvDevCapture::~CAvDevCapture()
{

	m_ConfigCapture.Detach(this, (AvConfigCallBack)&CAvDevCapture::OnConfigCaptureModify);
	m_ConfigEncode.Detach(this, (AvConfigCallBack)&CAvDevCapture::OnConfigEncodeModify);
	m_ConfigImage.Detach(this, (AvConfigCallBack)&CAvDevCapture::OnConfigImageModify);
	m_ConfigCover.Detach(this, (AvConfigCallBack)&CAvDevCapture::OnConfigCoverModify);
	m_ConfigWaterMark.Detach(this, (AvConfigCallBack)&CAvDevCapture::OnConfigWaterMarkingModify);

}
av_bool CAvDevCapture::Initialize(av_int Channel)
{
	m_Channel = Channel;
	CaptureCreate();

	if (av_true != CThread::run()){
		av_error("%s Start Thread Error\n", __FUNCTION__);
	}
	C_EncodeCaps EncodeCaps;
	CAvDevice::GetCaptureCaps(m_Channel, EncodeCaps);
	for (int i = CHL_MAIN_T; i < CHL_NR_T; i++){
		if (!(AvMask(i) & EncodeCaps.ExtChannel))continue;
		Start(i);
	}
	{
		C_AudioCaps AudioCaps;
		CAvDevice::GetACaptureCaps(CHL_ACAP_T, AudioCaps);
		if (AudioCaps.nMaxChannels != 0){
			AStart(CHL_ACAP_T);
		}
		CAvDevice::GetACaptureCaps(CHL_APLY_T, AudioCaps);
		if (AudioCaps.nMaxChannels != 0){
			AStart(CHL_APLY_T);
		}
	}

	LoadConfigs();
	return av_true;
}

av_bool CAvDevCapture::Start(av_int Slave)
{
	return VStart(Slave);
}
av_bool CAvDevCapture::Stop(av_int Slave)
{

	return VStop(Slave);
}

av_bool CAvDevCapture::Start(av_int Slave, CAvObject *obj, SIG_PROC_ONDATA pOnData)
{
	int ret = 0;
	ret = m_StreamSignal[Slave].Attach(obj, pOnData);
	if (ret < 0) {
		return av_false;
	}
	else{
		return SetIFrame(Slave);
	}
}
av_bool CAvDevCapture::Stop(av_int Slave, CAvObject *obj, SIG_PROC_ONDATA pOnData)
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

av_bool CAvDevCapture::SetProfile(av_int Slave, C_EncodeFormats &Formats)
{
	return av_true;
}
av_bool CAvDevCapture::GetProfile(av_int Slave, C_EncodeFormats &Fromats)
{
	return av_true;
}
av_bool CAvDevCapture::GetCaps(C_EncodeCaps &Caps)
{
	return av_true;
}

av_bool CAvDevCapture::SetTime(av_timeval &atv)
{
	return av_true;
}
av_bool CAvDevCapture::SetIFrame(av_int Slave)
{
	return AvCaptureForceKeyFrame((av_char)m_Channel, (av_char)Slave);
}

CAvPacket * CAvDevCapture::GetSnap(av_int Slave)
{
	CGuard m(m_SnapMutex);
	if (NULL != m_Snap)m_Snap->AddRefer();
	return m_Snap;
}



av_bool CAvDevCapture::CaptureCreate()
{
	av_bool ret = av_true;
	ret = AvCaptureCreate(m_Channel);
	if (ret != av_true){
		m_LastCaptureSyncStat = E_Capture_VideoErr;
		return av_false;
	}
	m_LastCaptureSyncStat = E_Capture_VideoStart;

	AvCoverCreate(m_Channel);
	AvCoverStart(m_Channel);

	AvWaterMarkingCreate(m_Channel);
	AvWaterMarkingStart(m_Channel);

	AvACreate(m_Channel);

	return av_true;
}
av_bool CAvDevCapture::CaptureDestroy()
{
	av_bool ret = av_true;

	VStop(m_Channel);

	AStop(CHL_ACAP_T);
	AStop(CHL_APLY_T);

	AvWaterMarkingStop(m_Channel);
	AvWaterMarkingDestory(m_Channel);

	AvCoverStop(m_Channel);
	AvCoverDestroy(m_Channel);

	ret = AvCaptureDestroy(m_Channel);
	if (ret != av_true){
		return av_false;
	}
	return av_true;
}

av_bool CAvDevCapture::VStart(av_uchar Slave)
{
	return AvCaptureStart(m_Channel, Slave);
}
av_bool CAvDevCapture::VStop(av_uchar Slave)
{
	return AvCaptureStop(m_Channel, Slave);
}

av_bool CAvDevCapture::AStart(E_AUDIO_CHL chl)
{
	return AvAStart(m_Channel, chl);
	}
av_bool CAvDevCapture::AStop(E_AUDIO_CHL chl)
{
	return AvAStop(m_Channel, chl);
}
av_bool CAvDevCapture::APlayPutBuffer(CAvPacket *Packet)
{
	return AvAPlayPutBuffer((av_uchar *)Packet->GetRawBuffer(), Packet->GetRawLength());
	}
av_bool CAvDevCapture::ASetProfile(E_AUDIO_CHL CHL, C_AudioProfile &aProfile)
{
	return AvASetProfile(m_Channel, CHL, &aProfile);
}

av_bool CAvDevCapture::LoadConfigs()
{
	C_EncodeCaps EncodeCaps;
	CAvDevice::GetCaptureCaps(m_Channel, EncodeCaps);

	ConfigEncodeFormats &Formats = m_ConfigEncode.GetConfig(m_Channel);

	for (int i = CHL_MAIN_T; i < CHL_NR_T; i++){
		if (!(AvMask(i) & EncodeCaps.ExtChannel)){
			continue;
		}

		AvEncodeSetFormat(m_Channel, i, &(Formats.CHLFormats[i].Formats));
	}
	ConfigCaptureFormats &CaptureFormats = m_ConfigCapture.GetConfig(m_Channel);
	AvCaptureInSetFormat(m_Channel, &CaptureFormats);

	ConfigImageFormats &ImageFormats = m_ConfigImage.GetConfig(m_Channel);
	AvImageSet(m_Channel, &(ImageFormats));

	C_EncodeCaps CaptureCaps;
	CAvDevice::GetCaptureCaps(m_Channel, CaptureCaps);

	ConfigCoverFormats &CoverFormats = m_ConfigCover.GetConfig(m_Channel);
	for (int i = 0; i < CaptureCaps.MaxCover; i++){
		AvCoverSetFormat(m_Channel, &(CoverFormats.CHLFormats[i]));
	}

	ConfigWaterMarkingFormats &WaterMarkingFormats = m_ConfigWaterMark.GetConfig(m_Channel);
	for (int i = 0; i < CaptureCaps.MaxWaterMaring; i++){
		AvWaterMarkingSetFormat(m_Channel, &(WaterMarkingFormats.CHLFormats[i]));
	}

	C_AudioCaps AudioCaps;
	CAvDevice::GetACaptureCaps(CHL_ACAP_T, AudioCaps);
	if (AudioCaps.nMaxChannels > 0){
		ConfigAudioFormats &AudioCapFromat = m_ConfigAudio[CHL_ACAP_T].GetConfig(CHL_ACAP_T);
		ASetProfile(CHL_ACAP_T, AudioCapFromat);
	}
	CAvDevice::GetACaptureCaps(CHL_APLY_T, AudioCaps);
	if (AudioCaps.nMaxChannels > 0){
		ConfigAudioFormats &AudioCapFromat = m_ConfigAudio[CHL_APLY_T].GetConfig(CHL_APLY_T);
		ASetProfile(CHL_APLY_T, AudioCapFromat);
	}


	return av_true;

}

void CAvDevCapture::ThreadProc()
{
	av_buf buf;
	av_bool ret;
	av_u32 cnt = 0;
	CAvPacket *AvPacket = NULL;
	E_CaptureSynchronizeStat ViStatus;
	av_bool isSleep = av_false;
	int i = 0;
	
	C_EncodeCaps EncodeCaps;
	CAvDevice::GetCaptureCaps(m_Channel, EncodeCaps);
	av_msg("%s Task Running StreamMark = %x\n", __FUNCTION__, EncodeCaps.ExtChannel);
	while (av_true == m_Loop){
		isSleep = av_true;
		//#pragma omp parallel for
		for (i = CHL_MAIN_T; i < CHL_NR_T; i++){
			if (!(AvMask(i) & EncodeCaps.ExtChannel))continue;
			ret = AvCaptureGetBuffer(m_Channel, i, avStreamT_V, &buf);
			if (ret == av_true){
				isSleep = av_false;
				AvPacket = g_AvPacketManager.GetAvPacket(buf.len);
				AvPacket->PutBuffer((av_uchar *)buf.base, buf.len);
				AvFree(buf.base);
				AvPacket->PutBufferOver();
				m_StreamSignal[i](m_Channel, i, AvPacket);

				if (i == CHL_JPEG_T){ 
					m_SnapMutex.Enter();
					if (NULL != m_Snap)m_Snap->Release();
				
					m_Snap = AvPacket;
					m_Snap->AddRefer();
					
					m_SnapMutex.Leave();
				}

				AvPacket->Release();
				
			}
		}

		do
		{
			ret = AvCaptureGetBuffer(m_Channel, 0, avStreamT_A, &buf);
			if (ret == av_true){
				AvPacket = g_AvPacketManager.GetAvPacket(buf.len);
				AvPacket->PutBuffer((av_uchar *)buf.base, buf.len);
				AvFree(buf.base);
				AvPacket->PutBufferOver();
				isSleep = av_false;
				for (i = CHL_MAIN_T; i < CHL_NR_T; i++){
					if (!(AvMask(i) & EncodeCaps.ExtChannel))continue;
					m_StreamSignal[i](m_Channel, i, AvPacket);
				}
				AvPacket->Release();
				
			}
		} while (ret == av_true);

		if (isSleep == av_true){
#if defined(WIN32)
			av_msleep(1);
#else
			usleep(1);
#endif
		}

		//以下是为了动态输入
		if (1 && (cnt++ % 100) == 0){
			ViStatus = AvCaptureSynchronize(m_Channel);
			CAvAlarm::AvAlarmStat MsgStatus = CAvAlarm::AvAlm_Stat_NONE;
			switch (ViStatus){
			case 	E_Capture_VideoNONE:
				av_error("Why AvCaptureSynchronize return E_Capture_VideoNONE\n");
				break;
			case E_Capture_VideoStart:
			{
				if (m_LastCaptureSyncStat != E_Capture_VideoStart){
					av_warning("Video In Start Captures \n");
					CaptureCreate();
					for (int i = CHL_MAIN_T; i < CHL_NR_T; i++){
						if (!(AvMask(i) & EncodeCaps.ExtChannel))continue;
						Start(i);
					}
					AStart(CHL_ACAP_T);
					AStart(CHL_APLY_T);
					MsgStatus = CAvAlarm::AvAlm_Stat_Close;
					m_LastCaptureSyncStat = E_Capture_VideoStart;
				}

			}
			break;
			case E_Capture_VideoRunning:
				m_LastCaptureSyncStat = E_Capture_VideoRunning;
				break;
			case E_Capture_VideoStop:
			{
				if (m_LastCaptureSyncStat != E_Capture_VideoStop){
					av_warning("Video In Stop Captures \n");
					for (int i = CHL_MAIN_T; i < CHL_NR_T; i++){
						if (!(AvMask(i) & EncodeCaps.ExtChannel))continue;
						Stop(i);
					}
					CaptureDestroy();
					MsgStatus = CAvAlarm::AvAlm_Stat_Open;
					m_LastCaptureSyncStat = E_Capture_VideoStop;
				}
			}
			break;
			case E_Capture_VideoErr:
				av_error("Why AvCaptureSynchronize return E_Capture_VideoErr\n");
				break;
			case E_Capture_VideoNr:
				av_error("Why AvCaptureSynchronize return E_Capture_VideoNr\n");
				break;
			default:
				av_error("Why AvCaptureSynchronize return default\n");
				break;
			}

			if (1 && MsgStatus != CAvAlarm::AvAlm_Stat_NONE){
				CAvQmsg AlarmMsgQueue(ALARM_QUEUE_MSG_NAME);
				CAvAlarm::AlmMsg MsgData;
				MsgData.AlmStatus = MsgStatus;
				MsgData.AlmTmSec = (av_u32)time(NULL);
				MsgData.Channel = m_Channel;
				MsgData.Slave = CHL_NR_T;
				MsgData.AlmType = CAvAlarm::AvAlmT_VIDEO_Lost;
				av_u32 MsgDatalen = sizeof(CAvAlarm::AlmMsg);
				AlarmMsgQueue.QmSnd((av_char *)&MsgData, MsgDatalen);
			}
		}
	}
	av_msg("%s Task Stop\n", __FUNCTION__);
}



av_void CAvDevCapture::OnConfigWaterMarkingModify(CAvConfigWaterMarking *ConfigWaterMarking, int &result)
{
	ConfigWaterMarkingFormats &OldWaterMarkingFormats = m_ConfigWaterMark.GetConfig(m_Channel);
	ConfigWaterMarkingFormats &NewWaterMarkingFormats = ConfigWaterMarking->GetConfig(m_Channel);

	av_bool ret = av_false;
	C_EncodeCaps CaptureCaps;
	CAvDevice::GetCaptureCaps(m_Channel, CaptureCaps);
	for (int i = 0; i < CaptureCaps.MaxWaterMaring; i++)
	{
		if (0 != memcmp(&(OldWaterMarkingFormats.CHLFormats[i]), &(NewWaterMarkingFormats.CHLFormats[i]), sizeof(C_WaterMarkingFormats))){
			ret = AvWaterMarkingSetFormat(m_Channel, &(NewWaterMarkingFormats.CHLFormats[i]));
			if (ret == av_true){
				OldWaterMarkingFormats.CHLFormats[i] = NewWaterMarkingFormats.CHLFormats[i];
				result = 0;
			}
			else{
				av_error("AvWaterMarkingSetFormat  return Error\n");
				result = -1;
			}
		}
	}
}
av_void CAvDevCapture::OnConfigCoverModify(CAvConfigCover *ConfigCover, int &result)
{
	ConfigCoverFormats &OldFormats = m_ConfigCover.GetConfig(m_Channel);
	ConfigCoverFormats &newFormats = ConfigCover->GetConfig(m_Channel);

	av_bool ret = av_false;
	C_EncodeCaps CaptureCaps;
	CAvDevice::GetCaptureCaps(m_Channel, CaptureCaps);
	for (int i = 0; i < CaptureCaps.MaxCover; i++)
	{
		if (0 != memcmp(&OldFormats.CHLFormats[i], &newFormats.CHLFormats[i], sizeof(C_CoverFormats))){
			ret = AvCoverSetFormat(m_Channel, &(newFormats.CHLFormats[i]));
			if (ret == av_true){
				OldFormats.CHLFormats[i] = newFormats.CHLFormats[i];
				result = 0;
			}
			else{
				av_error("AvCoverSetFormat  return Error\n");
				result = -1;
			}
		}
	}

}
av_void CAvDevCapture::OnConfigImageModify(CAvConfigImage *ConfigImage, int &result)
{
	ConfigImageFormats &OldFormats = m_ConfigImage.GetConfig(m_Channel);
	ConfigImageFormats &NewFormats = ConfigImage->GetConfig(m_Channel);
	av_bool ret = av_false;
	if (0 != memcmp(&OldFormats, &NewFormats, sizeof(ConfigImageFormats))){
		ret = AvImageSet(m_Channel, &NewFormats);
		if (ret == av_true){
			OldFormats = NewFormats;
			result = 0;
		}
		else{
			av_error("AvImageSet  return Error\n");
			result = -1;
		}
	}
}
av_void CAvDevCapture::OnConfigEncodeModify(CAvConfigEncode *ConfigEncode, int &result)
{
	av_warning("OnConfigEncodeModify Modify\n");
	ConfigEncodeFormats &OldFormats = m_ConfigEncode.GetConfig(m_Channel);
	ConfigEncodeFormats &NewFormats = ConfigEncode->GetConfig(m_Channel);
	av_bool ret;
	for (int i = 0; i < CHL_NR_T; i++){
		if (0 != memcmp(&(NewFormats.CHLFormats[i].Formats), &(OldFormats.CHLFormats[i].Formats), sizeof(C_EncodeFormats))){
			ret = AvEncodeSetFormat(m_Channel, i, &(NewFormats.CHLFormats[i].Formats));
			if (ret == av_true){
				OldFormats.CHLFormats[i].Formats = NewFormats.CHLFormats[i].Formats;
				result = 0;
			}
			else{
				av_error("AvEncodeSetFormat  return Error\n");
				result = -1;
			}
		}
	}
	av_warning("OnConfigEncodeModify Modify\n");
}
av_void CAvDevCapture::OnConfigCaptureModify(CAvConfigCapture *ConfigCapture, int &result)
{
	av_warning("OnConfigCaptureModify Modify\n");
	ConfigCaptureFormats &OldFormats = m_ConfigCapture.GetConfig(m_Channel);
	ConfigCaptureFormats &NewFormats = ConfigCapture->GetConfig(m_Channel);
	av_bool ret = av_false;
	if (0 != memcmp(&OldFormats, &NewFormats, sizeof(ConfigCaptureFormats))){
		ret = AvCaptureInSetFormat(m_Channel, &NewFormats);
		if (ret == av_true){
			OldFormats = NewFormats;
			result = 0;
		}
		else{
			av_error("AvCaptureInSetFormat return Error\n");
			result = -1;
		}
	}
}
av_void CAvDevCapture::OnConfigAudio(CAvConfigAudio *ConfigAudio, int &result)
{

}

av_void CAvDevCapture::OnTest(void  *args, int &result)
{

}