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
//#include "AvProtocol/AvNetProto.h"
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

// 	av_bool SetProfile(av_int Slave, C_EnCodeProfile &Profile);
// 	av_bool GetProfile(av_int Slave, C_EnCodeProfile &Profile);
// 	av_bool GetCaps(C_EncodeCaps &Caps);

	av_bool SetTime(av_timeval &atv);
	av_bool SetIFrame(av_int Slave = CHL_MAIN_T);
	EAvCaptureStatus GetCaptureStatus(av_int Slave = -1);
	CAvPacket *GetSnap(av_int Slave = CHL_SUB1_T);


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

public:
	av_bool StartNetCapture(CAvNetProto *Handle);
	av_bool StopNetCapture();
public:
	av_bool LoadConfigs();
private:
	TSignal3<av_int, av_int, CAvPacket *> m_StreamSignal[CHL_NR_T];
private:
	void ThreadProc();
	void OnTimer(CAvTimer &Timer);
private:
	CAvTimer		m_Timer;
	av_uint			m_TimerLastRecFrameNm[CHL_NR_T];
	av_bool			m_Linked;
private:
	av_int			m_Channel;
	av_int			m_RemoteChannel;
	av_uint			m_RecvFrameNm[CHL_NR_T];
};




#endif
