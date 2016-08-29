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

	av_bool SetProfile(av_int Slave, C_EncodeFormats &Formats);
	av_bool GetProfile(av_int Slave, C_EncodeFormats &Fromats);
	av_bool GetCaps(C_EncodeCaps &Caps);

	av_bool SetTime(av_timeval &atv);
	av_bool SetIFrame(av_int Slave = CHL_MAIN_T);

	CAvPacket *GetSnap(av_int Slave = CHL_SUB1_T);
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
public:
	av_bool LoadConfigs();

public:

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
	av_void OnConfigWaterMarkingModify(CAvConfigWaterMarking *ConfigWaterMarking, int &result);
	av_void OnConfigCaptureModify(CAvConfigCapture *ConfigCapture, int &result);
	av_void OnConfigAudio(CAvConfigAudio *ConfigAudio, int &result);
	av_void OnTest(void *args, int &result);
private:
	CAvConfigEncode			m_ConfigEncode;
	CAvConfigImage			m_ConfigImage;
	CAvConfigCover			m_ConfigCover;
	CAvConfigWaterMarking	m_ConfigWaterMark;
	CAvConfigCapture		m_ConfigCapture;
	CAvConfigAudio			m_ConfigAudio[CHL_A_NUM];
};









#endif


