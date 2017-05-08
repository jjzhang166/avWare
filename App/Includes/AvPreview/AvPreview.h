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
	CAvPreview();
	~CAvPreview();



	av_bool Set(unsigned int hWid, int ScreenID);
	av_bool Start(int Channel, int Slave);
	av_bool GetChannleSlave(int &Channel, int &Slave);
	av_uint GetWindowsID();
	av_int  GetScreenID();
	C_RECT  &GetWindowsRect();

	av_bool Stop();
	av_bool Pause(av_bool bPause = av_false);
	av_bool RenderResize(C_RECT &RenderRect);

	av_bool StartAudio(av_bool bOpen = av_true);
	av_bool SetStatistics(av_bool bOpen);
	av_bool GetStatistics(std::string &ImageSize, std::string &Comp, std::string &FrameRate, std::string &BitRate);
	av_void OnStream(int Channel, int Slave, CAvPacket *Pack);
	av_void OnTimer(CAvTimer &Timer);


public:
	static  E_EncodeCHL					GetShowCHL(av_int Channel);
	static  av_void						SetSpiltScreen(int SpiltScreenNum);
	static	av_void						SplitScreenClear(int ScreenID = -1);
	static  av_uint						m_SpiltScreenNum;
	static  C_DecodeCaps				m_DecodeCaps;


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
	C_RECT			m_RenderRect;
	int				m_Channel;
	int				m_Slave;
	av_bool			m_bPause;
	av_bool			m_isStarted;
	Capture		*	m_Capture;
	av_bool			m_bAudioOpen;
	CAvTimer		m_Timer;
};




#endif

