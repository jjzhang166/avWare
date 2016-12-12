#include "AvCapture/AvDevCapture.h"
#include "AvAlarm/AvAlarm.h"
#include "AvAudio/AvAudio.h"
#include "AvDevice/AvDevice.h"
#include "Apis/LibEncode.h"
#include "AvUart/AvUart.h"
CAvDevCapture::CAvDevCapture()
{
	CThread::SetThreadName(__FUNCTION__);
	m_Channel = 0;
	m_LastCaptureSyncStat = E_Capture_VideoNONE;
	
	m_Snap = NULL;
	m_ConfigCapture.Update(m_Channel);
	m_ConfigCapture.Attach(this, (AvConfigCallBack)&CAvDevCapture::OnConfigCaptureModify);

	m_ConfigEncode.Update(m_Channel);
	m_ConfigEncode.Attach(this, (AvConfigCallBack)&CAvDevCapture::OnConfigEncodeModify);

	m_ConfigImage.Update(m_Channel);
	m_ConfigImage.Attach(this, (AvConfigCallBack)&CAvDevCapture::OnConfigImageModify);

	m_ConfigCover.Update(m_Channel);
	m_ConfigCover.Attach(this, (AvConfigCallBack)&CAvDevCapture::OnConfigCoverModify);

	m_ConfigWaterMark.Update(m_Channel);
	m_ConfigWaterMark.Attach(this, (AvConfigCallBack)&CAvDevCapture::OnConfigWaterMarkingModify);

	m_ConfigAudioCapture.Update();
	m_ConfigAudioCapture.Attach(this, (AvConfigCallBack)&CAvDevCapture::OnConfigAudioModify);


	for (int i = 0; i < CHL_NR_T; i++){
		m_RecvFrameNu[i] = 0;
		m_ContinuousErrFrameNu[i] = 0;
		
		m_CaptureStatus[i].Channel		= m_Channel;
		m_CaptureStatus[i].Slave		= i;
		m_CaptureStatus[i].Enable		= 1;
		m_CaptureStatus[i].CaptureIn	= 0;
		m_CaptureStatus[i].CaptureOut	= 0;
		m_CaptureStatus[i].CaptureError = 0;
		m_CaptureStatus[i].CaptureDrop	= 0;
		m_CaptureStatus[i].CaptureFps	= 0;
		m_CaptureStatus[i].CaptureWidth = 0;
		m_CaptureStatus[i].CaptureHeigh = 0;
		m_CaptureStatus[i].CaptureFormats[0] = '\0';
	}


}
CAvDevCapture::~CAvDevCapture()
{

	m_ConfigCapture.Detach(this, (AvConfigCallBack)&CAvDevCapture::OnConfigCaptureModify);
	m_ConfigEncode.Detach(this, (AvConfigCallBack)&CAvDevCapture::OnConfigEncodeModify);
	m_ConfigImage.Detach(this, (AvConfigCallBack)&CAvDevCapture::OnConfigImageModify);
	m_ConfigCover.Detach(this, (AvConfigCallBack)&CAvDevCapture::OnConfigCoverModify);
	m_ConfigWaterMark.Detach(this, (AvConfigCallBack)&CAvDevCapture::OnConfigWaterMarkingModify);
	m_ConfigAudioCapture.Detach(this, (AvConfigCallBack)&CAvDevCapture::OnConfigAudioModify);

}
av_bool CAvDevCapture::Initialize(av_int Channel)
{
	m_Channel = Channel;
	CaptureCreate();

	if (av_true != CThread::ThreadStart()){
		av_error("%s Start Thread Error\n", __FUNCTION__);
	}
	C_EncodeCaps EncodeCaps;
	CAvDevice::GetEncodeCaps(m_Channel, EncodeCaps);
	for (int i = CHL_MAIN_T; i < CHL_NR_T; i++){
		if (!(AvMask(i) & EncodeCaps.ExtChannelMask))continue;
		Start(i);
	}
	{
		C_AudioCaps AudioCaps;
		if (av_true == CAvDevice::GetACaptureCaps(CHL_ACAP_T, AudioCaps)){
			AStart(CHL_ACAP_T);
		}
		

		if (av_true == CAvDevice::GetACaptureCaps(CHL_APLY_T, AudioCaps)){
			AStart(CHL_APLY_T);
		}
		
	}

	AvCoverCreate(m_Channel);
	AvCoverStart(m_Channel);


	AvWaterMarkingCreate(m_Channel);
	AvWaterMarkingStart(m_Channel);
	
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

av_bool CAvDevCapture::SetTime(av_timeval &atv)
{
	return av_true;
}
av_bool CAvDevCapture::SetIFrame(av_int Slave)
{
	return AvCaptureForceKeyFrame((av_char)m_Channel, (av_char)Slave);
}
Capture::EAvCaptureStatus CAvDevCapture::GetCaptureStatus(av_int Slave)
{
	if (Slave == -1){
		for (int i = 0; i < CHL_NR_T; i++){
			if (m_RecvFrameNu[i] != 0){
				return EAvCapture_ING;
			}
		}

		return EAvCapture_STOP;
	}
	return m_RecvFrameNu[Slave] == 0 ? EAvCapture_STOP : EAvCapture_ING;
}

CAvPacket * CAvDevCapture::GetSnap(av_int Slave)
{
	CGuard m(m_SnapMutex);
	if (NULL != m_Snap)m_Snap->AddRefer();
	return m_Snap;
}


av_bool CAvDevCapture::CaptureGetCaps(C_CaptureCaps &CaptureCaps)
{
	return AvCaptureGetCaps(m_Channel, &CaptureCaps);
}
av_bool CAvDevCapture::CaptureGetProfile(C_CaptureProfile &CaptureProfile)
{
	ConfigCaptureProfile &LocalCaptureProfile = m_ConfigCapture.GetConfig(m_Channel);
	CaptureProfile = LocalCaptureProfile;
	return av_true;
}
av_bool CAvDevCapture::CaptureSetProfile(C_CaptureProfile &CaptureProfile)
{
	CAvConfigCapture		ConfigCapture;
	ConfigCapture.Update();
	ConfigCaptureProfile &LocalCaptureProfile = ConfigCapture.GetConfig(m_Channel);
	LocalCaptureProfile = CaptureProfile;
	av_msg("CaptureSetProfile"
		"AntiFlcker = %d"
		"AntiFlckerValue = %d"
		"bOpenCvbs = %d"
		"Exposure = %d"
		"ExposureValue = %d"
		"IrCut = %d"
		"Start = %d"
		"End = %d"
		"MirrorMaskValue = %d"
		"WhiteBalance = %d"
		"WhiteBalanceValue = %d\n",
		CaptureProfile.AntiFlcker,
		CaptureProfile.AntiFlckerValue,
		CaptureProfile.bOpenCvbs,
		CaptureProfile.Exposure,
		CaptureProfile.ExposureValue,
		CaptureProfile.IrCut,
		CaptureProfile.IrCutTimer.Start,
		CaptureProfile.IrCutTimer.End,
		CaptureProfile.MirrorMaskValue, 
		CaptureProfile.WhiteBalance,
		CaptureProfile.WhiteBalanceValue);


	ConfigCapture.SettingUp();
	return av_true;

}

av_bool CAvDevCapture::EncodeGetCaps(C_EncodeCaps &EncodeCaps)
{
	return AvEncodeGetCaps(m_Channel, &EncodeCaps);
}
av_bool CAvDevCapture::EncodeGetProfile(int Slave, C_EnCodeProfile &EnCodeProfile)
{
	ConfigEncodeProfile &LocalEncodeProfile = m_ConfigEncode.GetConfig(m_Channel);
	EnCodeProfile = LocalEncodeProfile.CHLProfile[Slave].Profile;

	return av_true;
}
av_bool CAvDevCapture::EncodeSetProfile(int Slave, C_EnCodeProfile &EnCodeProfile)
{
	CAvConfigEncode ConfigEncode;
	ConfigEncode.Update();
	ConfigEncodeProfile &LocalEncodeProfile = ConfigEncode.GetConfig(m_Channel);
	LocalEncodeProfile.CHLProfile[Slave].Profile = EnCodeProfile;
	ConfigEncode.SettingUp();
	av_msg("Slave = %d, FrameRate = %d,  BitRateCtl = %d, Gop = %d, ImageSize= %d Qlevel = %d comp = %d, bitvalue = %d\n",Slave, EnCodeProfile.FrameRate, EnCodeProfile.BitRateCtl,
		EnCodeProfile.Gop, EnCodeProfile.ImageSize, EnCodeProfile.Qlevel, EnCodeProfile.Comp, EnCodeProfile.BitRateValue);
	return av_true;
}

av_bool  CAvDevCapture::AudioGetCaps(C_AudioCaps &AudioCaps)
{
	return AvACaptureCaps(CHL_ACAP_T, &AudioCaps);
}
av_bool  CAvDevCapture::AudioGetProfile(C_AudioProfile &AudioProfile)
{
	ConfigAudioFormats &LocalAudioProfile = m_ConfigAudioCapture.GetConfig();
	AudioProfile = LocalAudioProfile;
	return av_true;
}
av_bool  CAvDevCapture::AudioSetProfile(C_AudioProfile &AudioProfile)
{
	CAvConfigAudio ConfigAudio;
	ConfigAudio.Update();

	av_msg("CaptureDevice = %d, Comp = %d, SampleBits = %d, SampleRate = %d, SoundMode = %d, VoiceQualityMask = %d, VolumeCapture = %d,VolumePlay = %d\n",
		AudioProfile.CaptureDevice,
		AudioProfile.Comp,
		AudioProfile.SampleBits,
		AudioProfile.SampleRate,
		AudioProfile.SoundMode,
		AudioProfile.VoiceQualityMask,
		AudioProfile.VolumeCapture,
		AudioProfile.VolumePlay);
	ConfigAudioFormats &LocalAudioProfile = ConfigAudio.GetConfig();
	LocalAudioProfile = AudioProfile;

	ConfigAudio.SettingUp();

	return av_true;
}

av_bool CAvDevCapture::ImageGetCaps(C_ImageCaps &ImageCaps)
{
	return AvImageCaps(m_Channel, &ImageCaps);
}
av_bool CAvDevCapture::ImageGetProfile(C_ImageProfile &ImageProfile)
{
	
	ConfigImageProfile &LocalImageProfile = m_ConfigImage.GetConfig(m_Channel);
	ImageProfile = LocalImageProfile;
	return av_true;
}
av_bool CAvDevCapture::ImageSetProfile(C_ImageProfile &ImageProfile)
{
	CAvConfigImage			ConfigImage;
	ConfigImage.Update();

	ConfigImageProfile &LocalImageProfile = ConfigImage.GetConfig(m_Channel);
	LocalImageProfile = ImageProfile;
	ConfigImage.SettingUp();
	return av_true;
}

av_bool CAvDevCapture::PtzGetCaps(C_PtzCaps &PtzCaps)
{
	return g_AvUart.PtzGetCaps(PtzCaps);
}
av_bool CAvDevCapture::PtzGetProfile(C_PtzProfile &PtzProfile)
{
	return g_AvUart.PtzGetProfile(PtzProfile);
}
av_bool CAvDevCapture::PtzSetProfile(C_PtzProfile &PtzProfile)
{
	return g_AvUart.PtzSetProfile(PtzProfile);
}
av_bool CAvDevCapture::PtzSetCommand(C_PtzCmd &PtzCmd)
{
	return g_AvUart.PtzSetCommand(PtzCmd);
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
	CAvDevice::GetEncodeCaps(m_Channel, EncodeCaps);

	//视频采集
	ConfigCaptureProfile &CaptureProfile = m_ConfigCapture.GetConfig(m_Channel);
	AvCaptureSetProfile(m_Channel, &CaptureProfile);

	//视频编码
	ConfigEncodeProfile &Formats = m_ConfigEncode.GetConfig(m_Channel);
	for (int i = CHL_MAIN_T; i < CHL_NR_T; i++){
		if (!(AvMask(i) & EncodeCaps.ExtChannelMask)){
			continue;
		}

		AvEncodeSetProfile(m_Channel, i, &(Formats.CHLProfile[i].Profile));
	}

	//图像参数
	ConfigImageProfile &ImageProfile = m_ConfigImage.GetConfig(m_Channel);
	AvImageSetProfile(m_Channel, &(ImageProfile));

	//音频参数
	ConfigAudioFormats &AudioCapFromat = m_ConfigAudioCapture.GetConfig();
	ASetProfile(CHL_ACAP_T, AudioCapFromat);


	//C_EncodeCaps CaptureCaps;
	//CAvDevice::GetCaptureCaps(m_Channel, CaptureCaps);

	ConfigCoverFormats &CoverFormats = m_ConfigCover.GetConfig(m_Channel);
	for (int i = 0; i < EncodeCaps.MaxCover; i++){
		AvCoverSetFormat(m_Channel, &(CoverFormats.CHLFormats[i]));
	}

	ConfigWaterMarkingFormats &WaterMarkingFormats = m_ConfigWaterMark.GetConfig(m_Channel);
	for (int i = 0; i < EncodeCaps.MaxWaterMaring; i++){
		AvWaterMarkingSetFormat(m_Channel, &(WaterMarkingFormats.CHLFormats[i]));
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
	CAvDevice::GetEncodeCaps(m_Channel, EncodeCaps);
	av_msg("%s Task Running StreamMark = %x\n", __FUNCTION__, EncodeCaps.ExtChannelMask);
	while (av_true == m_Loop){
		isSleep = av_true;
		//#pragma omp parallel for
		for (i = CHL_MAIN_T; i < CHL_NR_T; i++){
			if (!(AvMask(i) & EncodeCaps.ExtChannelMask))continue;
			ret = AvCaptureGetBuffer(m_Channel, i, StreamContent_VIDEO, &buf);
			if (ret == av_true){
				m_RecvFrameNu[i]++;
				m_ContinuousErrFrameNu[i] = 0;

				isSleep = av_false;
				AvPacket = g_AvPacketManager.GetAvPacket(buf.len);
				AvPacket->PutBuffer((av_uchar *)buf.base, buf.len);
				AvFree(buf.base);
				AvPacket->PutBufferOver();
				m_StreamSignal[i](m_Channel, i, AvPacket);

				if (i == CHL_JPEG_T){ 
					m_SnapMutex.Enter();
					if (NULL != m_Snap){
						m_Snap->Release();
					}
					m_Snap = AvPacket;
					m_Snap->AddRefer();
					m_SnapMutex.Leave();
				}
#if defined(_AV_WARE_M_HAVE_PROC)
				{
					m_CaptureStatus[i].CaptureIn++;
					m_CaptureStatus[i].CaptureHeigh = AvPacket->ImageHeigh();
					m_CaptureStatus[i].CaptureWidth = AvPacket->ImageWidth();
					m_CaptureStatus[i].CaptureFps = AvPacket->FrameRate();
					switch (AvPacket->Comp())
					{
					case AvComp_MJPEG:
						sprintf((char *)m_CaptureStatus[i].CaptureFormats, "MJPEG");
						break;
					case AvComp_H264:
						sprintf((char *)m_CaptureStatus[i].CaptureFormats, "H264");
						break;
					case AvComp_H265:
						sprintf((char *)m_CaptureStatus[i].CaptureFormats, "H265");
						break;
					case AvComp_JPEG:
						sprintf((char *)m_CaptureStatus[i].CaptureFormats, "JPEG");
						break;
					default:
						break;
					}

					if (AvPacket->FrameType() == avFrameT_I && i == CHL_MAIN_T){
						for (int c = 0; c < CHL_NR_T; c++){
							if (!(AvMask(c) & EncodeCaps.ExtChannelMask)) m_CaptureStatus[c].Enable = 0;
							AvProcSet(IOCTRL_CMD_SET_CAPSTAT, &m_CaptureStatus[c]);
						}
					}


				}
#endif
				AvPacket->Release();
				
			}
			else{
				m_ContinuousErrFrameNu[i]++;
				if (m_ContinuousErrFrameNu[i] >= 75){
					m_RecvFrameNu[i] = 0;
				}
			}
		}

		do
		{
			ret = AvCaptureGetBuffer(m_Channel, 0, StreamContent_AUDIO, &buf);
			if (ret == av_true){
				AvPacket = g_AvPacketManager.GetAvPacket(buf.len);
				AvPacket->PutBuffer((av_uchar *)buf.base, buf.len);
				AvFree(buf.base);
				AvPacket->PutBufferOver();
				isSleep = av_false;
				for (i = CHL_MAIN_T; i < CHL_NR_T; i++){
					if (!(AvMask(i) & EncodeCaps.ExtChannelMask))continue;
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
						if (!(AvMask(i) & EncodeCaps.ExtChannelMask))continue;
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
						if (!(AvMask(i) & EncodeCaps.ExtChannelMask))continue;
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
	C_EncodeCaps EncodeCaps;
	CAvDevice::GetEncodeCaps(m_Channel, EncodeCaps);
	for (int i = 0; i < EncodeCaps.MaxWaterMaring; i++)
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
	C_EncodeCaps EncodeCaps;
	CAvDevice::GetEncodeCaps(m_Channel, EncodeCaps);
	for (int i = 0; i < EncodeCaps.MaxCover; i++)
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
	ConfigImageProfile &OldProfile = m_ConfigImage.GetConfig(m_Channel);
	ConfigImageProfile &NewProfile = ConfigImage->GetConfig(m_Channel);
	av_bool ret = av_false;
	if (0 != memcmp(&OldProfile, &NewProfile, sizeof(ConfigImageProfile))){
		ret = AvImageSetProfile(m_Channel, &NewProfile);
		if (ret == av_true){
			OldProfile = NewProfile;
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
	ConfigEncodeProfile &OldProfile = m_ConfigEncode.GetConfig(m_Channel);
	ConfigEncodeProfile &NewProfile = ConfigEncode->GetConfig(m_Channel);
	av_bool ret;
	for (int i = 0; i < CHL_NR_T; i++){
		if (0 != memcmp(&(NewProfile.CHLProfile[i].Profile), &(OldProfile.CHLProfile[i].Profile), sizeof(C_EnCodeProfile))){
			ret = AvEncodeSetProfile(m_Channel, i, &(NewProfile.CHLProfile[i].Profile));
			if (ret == av_true){
				OldProfile.CHLProfile[i].Profile = NewProfile.CHLProfile[i].Profile;
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
	ConfigCaptureProfile &OldProfile = m_ConfigCapture.GetConfig(m_Channel);
	ConfigCaptureProfile &NewProfile = ConfigCapture->GetConfig(m_Channel);
	av_bool ret = av_false;
	if (0 != memcmp(&OldProfile, &NewProfile, sizeof(ConfigCaptureProfile))){
		ret = AvCaptureSetProfile(m_Channel, &NewProfile);
		if (ret == av_true){
			OldProfile = NewProfile;
			result = 0;
		}
		else{
			av_error("AvCaptureInSetFormat return Error\n");
			result = -1;
		}
	}
}
av_void CAvDevCapture::OnConfigAudioModify(CAvConfigAudio *ConfigAudio, int &result)
{
	av_warning("OnConfigAudioModify Modify\n");
	ConfigAudioFormats &OldProfile = m_ConfigAudioCapture.GetConfig();
	ConfigAudioFormats &NewProfile = ConfigAudio->GetConfig();

	av_bool ret = av_false;
	if (0 != memcmp(&OldProfile, &NewProfile, sizeof(ConfigCaptureProfile))){
		ret = AvASetProfile(m_Channel, CHL_ACAP_T, &NewProfile);
		if (ret == av_true){
			OldProfile = NewProfile;
			result = 0;
		}
		else{
			av_error("AvASetProfile return Error\n");
			result = -1;
		}
	}
}

