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
#ifndef _AV_DEVCAPTURE_H_
#define _AV_DEVCAPTURE_H_
#include "AvCapture/AvCapture.h"
#include "AvConfigs/AvConfigCapture.h"
#include "AvConfigs/AvConfigAlarm.h"
#include "AvProc/AvProc.h"

class CAvDevCapture:public Capture
{
public:
	CAvDevCapture();
	~CAvDevCapture();
public:
	av_bool Initialize(av_int Channel);
	av_bool Start(av_int Slave);
	av_bool Stop(av_int Slave);

	av_bool Start(av_int Slave, CAvObject *obj, SIG_PROC_ONDATA pOnData);
	av_bool Stop(av_int Slave, CAvObject *obj, SIG_PROC_ONDATA pOnData);

	EAvCaptureStatus GetCaptureStatus(av_int Slave = -1);

	//CAvPakcet over , usr call packet->Release();
	CAvPacket * Snapshot(av_bool bRealTime = av_false, av_uint SnapshotInterval = 0, av_uint ContinuousTimes = 0);

public:
	av_bool CaptureGetCaps(C_CaptureCaps &CaptureCaps);
	av_bool CaptureGetProfile(C_CaptureProfile &CaptureProfile);
	av_bool CaptureSetProfile(C_CaptureProfile &CaptureProfile);

	av_bool EncodeGetCaps(C_EncodeCaps &EncodeCaps);
	av_bool EncodeGetProfile(int Slave, C_EnCodeProfile &EnCodeProfile);
	av_bool EncodeSetProfile(int Slave, C_EnCodeProfile &EnCodeProfile);

	av_bool AudioGetCaps(C_AudioCaps &AudioCaps);
	av_bool AudioGetProfile(C_AudioProfile &AudioProfile);
	av_bool AudioSetProfile(C_AudioProfile &AudioProfile);

	av_bool ImageGetCaps(C_ImageCaps &ImageCaps);
	av_bool ImageGetProfile(C_ImageProfile &ImageProfile);
	av_bool ImageSetProfile(C_ImageProfile &ImageProfile);

	av_bool PtzGetCaps(C_PtzCaps &PtzCaps);
	av_bool PtzGetProfile(C_PtzProfile &PtzProfile);
	av_bool PtzSetProfile(C_PtzProfile &PtzProfile);
	av_bool PtzSetCommand(C_PtzCmd &PtzCmd);

	av_bool AdvancedSystemGetCaps(C_AdvancedSystemCaps &AdvancedSystemCaps);
	av_bool AdvancedSystemGetProfile(C_AdvancedSystemProfile &AdvancedSystemProfile);
	av_bool AdvancedSystemSetProfile(C_AdvancedSystemProfile &AdvancedSystemProfile);


private:
	av_bool CoverGetProfile(C_CoverProfile &CoverProfile);
	av_bool CoverSetProfile(C_CoverProfile &CoverProfile);
	av_bool OverLayGetCaps(C_OverLayCaps &OverLayCaps);
	av_bool OverLayGetProfile(C_OverLayProfile &OverlayProfile);
	av_bool OverLaySetProfile(C_OverLayProfile &OverlayProfile);
	av_bool AlarmGetCaps(C_AlarmCaps &AlarmCaps);
	av_bool AlarmSetProfile(C_AlarmProfile &AlmProfile);
	av_bool AlarmGetProfile(C_AlarmProfile &AlmProfile);
private:
	av_uint m_RecvFrameNu[CHL_NR_T];
	av_uint m_ContinuousErrFrameNu[CHL_NR_T];
	avWare_Capture_Status m_CaptureStatus[CHL_NR_T];

private:
	av_int m_Channel;
	E_CaptureSynchronizeStat m_LastCaptureSyncStat;
	
	CAvPacket *	m_Snap;
	CMutex		m_SnapMutex;

public:
	av_bool CaptureCreate();
	av_bool CaptureDestroy();
	av_bool VStart(av_uchar Slave);
	av_bool VStop(av_uchar Slave);
	av_bool LoadConfigs();

	av_bool AStart(E_AUDIO_CHL chl);
	av_bool AStop(E_AUDIO_CHL chl);
	av_bool APlayPutBuffer(CAvPacket *Packet);
	av_bool ASetProfile(E_AUDIO_CHL CHL, C_AudioProfile &aProfile);


private:
	TSignal3<av_int, av_int, CAvPacket *> m_StreamSignal[CHL_NR_T];
private:
	void ThreadProc();

	
private:
	av_void OnConfigEncodeModify(CAvConfigEncode *ConfigEncode, int &result);
	av_void OnConfigImageModify(CAvConfigImage *ConfigImage, int &result);
	av_void OnConfigCoverModify(CAvConfigCover *ConfigCover, int &result);
	av_void OnConfigOverLayModify(CAvConfigOverLay *ConfigOverLay, int &result);
	av_void OnConfigCaptureModify(CAvConfigCapture *ConfigCapture, int &result);
	av_void OnConfigAudioModify(CAvConfigAudio *ConfigAudio, int &result);
	av_void OnConfigAlarmModify(CAvConfigAlarm *Configalarm, int &result);
private:
	CAvConfigEncode			m_ConfigEncode;
	CAvConfigImage			m_ConfigImage;
	CAvConfigCover			m_ConfigCover;
	CAvConfigOverLay		m_ConfigOverLay;
	CAvConfigCapture		m_ConfigCapture;
	CAvConfigAudio			m_ConfigAudioCapture;
	CAvConfigAlarm			m_ConfigAlarm;
};









#endif


