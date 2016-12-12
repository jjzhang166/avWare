#ifndef _AV_PREVIEW_H_
#define _AV_PREVIEW_H_
#include "Apis/AvWareCplusplus.h"
#include "CAvObject.h"
#include "AvPacket/AvPacket.h"
#include "AvAlarm/AvAlarm.h"
#include "AvCapture/AvManCapture.h"
#include "Apis/LibDecode.h"


class CAvPreview:public CAvObject
{

public:
//	CAvPreview(unsigned int hWid, int ScreenID);
	CAvPreview();
	~CAvPreview();

	av_bool Set(unsigned int hWid, int ScreenID);
	av_bool Start(int Channel, int Slave);
	av_bool GetChannleSlave(int &Channel, int &Slave);
	av_bool Stop();
	av_bool StartAudio(av_bool bOpen = av_true);
	av_bool SetStatistics(av_bool bOpen);
	av_bool GetStatistics(std::string &ImageSize, std::string &Comp, std::string &FrameRate, std::string &BitRate);
	av_void OnStream(int Channel, int Slave, CAvPacket *Pack);
	av_void OnTimer(CAvTimer &Timer);

private:
	av_bool			m_bStatistics;
	av_bool			m_bStartDecode;
	av_uint			m_StatisticsBitRate;
	av_uint			m_StatisticsFrameRate;
	av_uint			m_StatisticsGop;

	av_uint			m_BitRate;
	av_uint			m_FrameRate;
	av_uint			m_Gop;
	AvComp			m_Comp;
	av_uint			m_ImageWidth;
	av_uint			m_ImageHeigh;

private:
	unsigned int	m_hWid;
	int				m_ScreenID;
	int				m_Channel;
	int				m_Slave;
	av_bool			m_isStarted;
	Capture		*	m_Capture;
	av_bool			m_bAudioOpen;
	CAvTimer		m_Timer;
};




#endif

