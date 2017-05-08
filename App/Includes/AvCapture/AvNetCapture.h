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
#ifndef _AVNETCAPTURE_H_
#define _AVNETCAPTURE_H_

#include "Apis/AvWareCplusplus.h"
#include "CAvObject.h"
#include "AvAlarm/AvAlarm.h"
#include "AvPacket/AvPacket.h"
#include "AvThread/AvThread.h"
#include "AvCapture/AvCapture.h"
#include "AvProtocol/AvNetProto.h"

class CAvNetCapture:public Capture
{
public:
	CAvNetCapture();
	virtual ~CAvNetCapture();

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

public:
	av_bool StartNetCapture(CAvNetProto *Handle);
	av_bool StopNetCapture();
	CAvNetProto *GetNetCaptureHandle(C_ProtoFormats &ProtoFormats);
public:
	av_bool LoadConfigs();
	av_bool SendAlmMsgToSystem(C_AlmMsg &Msg);

private:
	av_void OnConfigNetCaptureModify(CAvConfigProtocol *ConfigProtocol, int &result);
	CAvConfigProtocol	m_ConfigNetCapture;

private:
	TSignal3<av_int, av_int, CAvPacket *> m_StreamSignal[CHL_NR_T];
private:
	void ThreadProc();
	void OnTimer(CAvTimer &Timer);
	void PushAlmMsgToSys(av_uint Slave, AlarmStat AlmStat = AlarmStat_Start, AlarmEvent AlmE = AlarmEvent_VIDEO_Lost);

private:
	CAvTimer		m_Timer;
	av_uint			m_TimerLastRecFrameNm[CHL_NR_T];
	I_RET			m_LinkStatus;

private:
	av_int			m_Channel;
	av_int			m_RemoteChannel;
	av_uint			m_RecvFrameNm[CHL_NR_T];

private:
	av_bool			m_bMainChnOffLine;
	av_bool			m_bSubChnOffLine;
};




#endif
