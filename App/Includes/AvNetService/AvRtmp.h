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
#ifndef _AVRTMP_H_
#define _AVRTMP_H_
#include "Apis/AvWareType.h"
#include "CAvObject.h"
#include "AvPacket/AvPacket.h"
#include "AvThread/AvThread.h"
#include "Rtmp/rtmp.h"
class AvRtmp:public CThread
{
public:
	typedef enum{
		RTMP_PULL,
		RTMP_PUSH,
	}E_RTMP_MODE;
public:
	AvRtmp(E_RTMP_MODE mode);
	~AvRtmp();

	av_bool Start(av_uchar Channel, av_uchar Slave, std::string url);
	av_bool Stop();
private:
	av_int  PushMediaData(CAvPacket *AvPacket);
	av_void OnStream(av_uchar Channel, av_uchar Slave, CAvPacket *AvPacket);
	void ThreadProc(void);
	
	av_bool RtmpPullTask();
	av_bool RtmpPushTask();
private:
	av_int SendMetaPacket(av_comp_t Vcomp, av_int PicWidth, av_int PicHight, av_int FrameRate, 
						  av_comp_t Acomp, av_int ASampleRate, av_int ABits);
	av_int SendRtmpData(unsigned int nPacketType,	unsigned char *predata,  unsigned int presize, 
													unsigned char *loaddata, unsigned int loaddatasize,
													unsigned char headerType = RTMP_PACKET_SIZE_LARGE);
	av_int SendRtmpPacket(unsigned int nPacketType, av_nal_unit_type nalType, CAvPacket *AvPacket);
private:
	av_int SendAacSpac();
	av_int SendAudioData(CAvPacket *Packet);
private:
	av_u32		m_AudioCfgSampleRate;
	av_u32		m_AudioCfgSampleBits;
	av_comp_t	m_AudioComp;
	av_u32		m_AudioChannels;


private:
	av_bool			m_GetIFrame;
	RTMP  *			m_RtmpHandle;
	E_RTMP_MODE		m_RtmpMode;
	av_uchar		m_Channel;
	av_uchar		m_Slave;
	av_char			m_Url[128];
	av_char		*	m_CacheBuffer;
	av_int			m_CacheBufferLen;
	av_int			m_PicWidth;
	av_int			m_PicHeigh;
	av_uint			m_nVTimestamp;
	av_uint			m_nATimestamp;

	av_u64			m_nVBasePts;
	av_u64			m_nABasePts;
private:
	std::queue<CAvPacket *> m_Avpacket;
	CMutex	m_Mutex;
};







#endif