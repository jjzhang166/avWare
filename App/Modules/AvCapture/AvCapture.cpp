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
#include "AvCapture/AvCapture.h"
#include "AvAlarm/AvAlarm.h"
#include "AvAudio/AvAudio.h"
#include "AvConfigs/AvConfigCapture.h"
#include "AvDevice/AvDevice.h"
CAvCapture::CAvCapture() :CThread(__FUNCTION__)
{
	memset(&m_EncodeCaps, 0x00, sizeof(m_EncodeCaps));
	memset(&m_CaptureInCaps, 0x00, sizeof(m_CaptureInCaps));
	m_Channel = 0x00;
	m_LastCaptureSyncStat = E_Capture_VideoNONE;
}
CAvCapture::~CAvCapture()
{

}
av_bool CAvCapture::Initialize(av_char Channel)
{
	av_msg("%s Channel %d Started\n", __FUNCTION__, Channel);
	m_Channel = Channel;
	AvCaptureGetCaps(m_Channel, &m_EncodeCaps);
	AvCaptureInCaps(m_Channel, &m_CaptureInCaps);


#if 0
	{
		m_ConfigCapture.Update(m_Channel);
		m_ConfigCapture.Attach(this, (AvConfigCallBack)&CAvCapture::OnConfigCaptureModify);

		m_ConfigEncode.Update(m_Channel);
		m_ConfigEncode.Attach(this, (AvConfigCallBack)&CAvCapture::OnConfigEncodeModify);

		m_ConfigImage.Update(m_Channel);
		m_ConfigImage.Attach(this, (AvConfigCallBack)&CAvCapture::OnConfigImageModify);

		m_ConfigCover.Update(m_Channel);
		m_ConfigCover.Attach(this, (AvConfigCallBack)&CAvCapture::OnConfigCoverModify);

		m_ConfigWaterMark.Update(m_Channel);
		m_ConfigWaterMark.Attach(this, (AvConfigCallBack)&CAvCapture::OnConfigCoverModify);
	}
#endif
	StartEnv();
	StartConf();

	return av_true;
}

av_bool CAvCapture::CaptureCreate()
{
	av_bool ret = av_true;
	ret = AvCaptureCreate(m_Channel);
	if (ret != av_true){
		m_LastCaptureSyncStat = E_Capture_VideoErr;
		return av_false;
	}
	m_LastCaptureSyncStat = E_Capture_VideoStart;

	AvCoverCreate(m_Channel);

	AvWaterMarkingCreate(m_Channel);

	return av_true;
}
av_bool CAvCapture::CaptureDestroy()
{
	av_bool ret = av_true;

	AvWaterMarkingDestory(m_Channel);

	AvCoverDestroy(m_Channel);

	ret = AvCaptureDestroy(m_Channel);
	if (ret != av_true){
		return av_false;
	}
	return av_true;
}
av_bool CAvCapture::CaptureStart(av_uchar Slave)
{
	av_bool ret = av_true;

	ret = AvCaptureStart(m_Channel, Slave);

	AvCoverStart(m_Channel);

	AvWaterMarkingStart(m_Channel);

	if (ret != av_true){
		return av_false;
	}
	return av_true;
}
av_bool CAvCapture::CaptureStop(av_uchar Slave)
{
	av_bool ret = av_true;

	AvWaterMarkingStop(m_Channel);
	
	AvCoverStop(m_Channel);

	ret = AvCaptureStop(m_Channel, Slave);
	if (ret != av_true){
		return av_false;
	}
	return av_true;
}

av_bool CAvCapture::CaptureGetDspInfo()
{
	av_bool ret = av_true;
	if (ret != av_true){
		return av_false;
	}
	return av_true;
}
av_bool CAvCapture::CaptureGetCaps()
{
	av_bool ret = AvCaptureGetCaps(m_Channel, &m_EncodeCaps);
	if (ret != av_true){
		return av_false;
	}
	return av_true;
}
av_u32 CAvCapture::CaptureGetChanannels()
{
	return 0;
}

av_bool CAvCapture::StreamStart(av_uchar Slave, CObject *obj, OnStreamSigNalFunc proc)
{
	int ret = 0;
	ret = m_StreamSignal[Slave].Attach(obj, proc);
	if (ret < 0) {
		return av_false;
	}
	else{
		AvCaptureForceKeyFrame(m_Channel, Slave);
		return av_true;
	}
}
av_bool CAvCapture::StreamStop(av_uchar Slave, CObject *obj, OnStreamSigNalFunc proc)
{
	int ret = 0;
	ret = m_StreamSignal[Slave].Detach(obj, proc);
	if (ret < 0) {
		return av_false;
	}
	else{
		return av_true;
	}
}

av_bool CAvCapture::StartEnv()
{
	CaptureCreate();
	
	if (av_true != CThread::run()){
		av_error("%s Start Thread Error\n", __FUNCTION__);
	}

	for (int i = CHL_MAIN_T; i < CHL_NR_T; i++){
		if (!(AvMask(i) & m_EncodeCaps.ExtChannel))continue;
		CaptureStart(i);
	}
	
	
	return av_true;
}

av_bool CAvCapture::StartConf(av_bool loadDefault)
{

	ConfigEncodeFormats &Formats = m_ConfigEncode.GetConfig(m_Channel);

	for (int i = CHL_MAIN_T; i < CHL_NR_T; i++){
		if (!(AvMask(i) & m_EncodeCaps.ExtChannel)){
			continue;
		}
		if (loadDefault == av_true){

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

	return av_true;  
}
av_void CAvCapture::OnConfigWaterMarkingModify(CAvConfigWaterMarking *ConfigWaterMarking, int &result)
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
av_void CAvCapture::OnConfigCoverModify(CAvConfigCover *ConfigCover, int &result)
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
av_void CAvCapture::OnConfigImageModify(CAvConfigImage *ConfigImage, int &result)
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
av_void CAvCapture::OnConfigEncodeModify(CAvConfigEncode *ConfigEncode, int &result)
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
}
av_void CAvCapture::OnConfigCaptureModify(CAvConfigCapture *ConfigCapture, int &result)
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

void CAvCapture::ThreadProc()
{
	av_buf buf;
	av_bool ret; 
	av_u32 cnt = 0;
	CPacket Packet;
	E_CaptureSynchronizeStat ViStatus;
	av_bool isSleep = av_false;
	int i = 0;
	av_msg("%s Task Running\n", __FUNCTION__);

	while (av_true == m_Loop){

		isSleep = av_true;
		for (i = CHL_MAIN_T; i < CHL_NR_T; i++){
			if (!(AvMask(i) & m_EncodeCaps.ExtChannel))continue;
			ret = AvCaptureGetBuffer(m_Channel, i, avStreamT_V, &buf);
			if (ret == av_true){
				isSleep = av_false;
				Packet.LoadData(buf.base, buf.len);
				m_StreamSignal[i](m_Channel, i, Packet);
				AvFree(buf.base);
			}
		}

		do 
		{
			ret = AvCaptureGetBuffer(m_Channel, 0, avStreamT_A, &buf);
			if (ret == av_true){
				Packet.LoadData(buf.base, buf.len);
				isSleep = av_false;
				for (i = CHL_MAIN_T; i < CHL_NR_T; i++){
					if (!(AvMask(i) & m_EncodeCaps.ExtChannel))continue;
					m_StreamSignal[i](m_Channel, i, Packet);
				}
				AvFree(buf.base);
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
			//av_msg("Get Vistatus = %d\n", ViStatus);
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
							if (!(AvMask(i) & m_EncodeCaps.ExtChannel))continue;
							CaptureStart(i);
						}

						StartConf(av_true);

						g_AvAudioCapture.AudioCreate();

						g_AvAudioCapture.AudioStart();

						g_AvAudioCapture.AudioSetFormat();

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
						g_AvAudioCapture.AudioStop();
						CaptureStop(m_Channel);

						g_AvAudioCapture.AudioDestroy();
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
				MsgData.AlmTmSec = time(NULL);
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




PATTERN_SINGLETON_IMPLEMENT(CMCapture)
CMCapture::CMCapture()
{

}
CMCapture::~CMCapture()
{

}
av_bool CMCapture::Initialize()
{
	C_DspCaps DspCaps;
	CAvDevice::GetDspCaps(DspCaps);
	m_MaxCaptureChannels = DspCaps.nMaxEncodeChannel;
	if (m_MaxCaptureChannels <= 0) return av_false;
	CAvCapture *avC = NULL;
	for (int i = 0; i < m_MaxCaptureChannels; i++){
		avC = new CAvCapture;
		avC->Initialize((av_uchar)i);
		m_MCaptureMap.insert(std::pair<av_uchar, CAvCapture*>(i, avC));
	}

	return av_true;
}
CAvCapture *CMCapture::GetAvCaptureInstance(av_uchar iChannel)
{
	return m_MCaptureMap[iChannel];
}
