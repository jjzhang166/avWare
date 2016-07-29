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
#ifndef _AVPACKET_H_
#define _AVPACKET_H_
#include "CAvObject.h"
#include "AvThread/AvThread.h"
#include "Apis/AvWareType.h"
#include "Apis/LibEncode.h"
#include "AvThread/AvTimer.h"

class CAvPacket
{
	friend class CAvPacketManager;
private:
	CAvPacket(av_u32 MaxLength);
	~CAvPacket();
private:
	static void *operator new (size_t size);
	static void operator delete(void *p);

public:
	av_bool			PutBuffer(av_uchar *Buffer, const av_u32 Length);
	av_bool			AppendBuffer(av_uchar *Buffer, const av_u32 Length);
	av_bool			PutBufferOver();

	av_bool			GetBuffer(av_uchar *Buffer, const av_u32 &Length/*IN BufferSize, Out BufferLength*/);
	const av_uchar *GetBuffer();
	av_u32			GetLength();

	av_bool			GetRawBuffer(av_uchar *Buffer, const av_u32 &Length/*IN BufferSize, Out BufferLength*/);
	const av_uchar * GetRawBuffer();
	av_u32			GetRawLength();
	
	av_u32			GetBufferSize();

	av_bool			IsVideoFrame();

public:
	av_bool			GetNaluSplit();
	av_u32			GetNaluCount();
	av_bool			GetNaluFrame(av_nal_unit_type naltype, av_uchar *&data, av_32 &datalen);
	av_bool			GetNaluFrame(av_32 index, av_nal_unit_type &naltype, av_uchar *&data, av_32 &datalen);
private:
	typedef struct {
		av_uchar *data;
		av_int	 len;
		av_nal_unit_type type;
	}AvPacketNaluInfo;

	av_u32			 m_NaluInfoCount;
	AvPacketNaluInfo m_NaluInfo[10];
	av_bool			 m_IsSplitNalu;

private:
	av_bool			ReSet();

private:
	av_uchar *		m_Buffer;
	av_u32			m_BufferLength;
	av_u32			m_Size;
	CMutex			m_Mutex;

	av_uchar *		m_RawBuffer;
	av_u32			m_RawBufferLength;
public:
	av_u32			AddRefer();
	av_u32			Release();
	av_u32			Refer();
private:
	av_u32			m_Refer;
public:
	av_stream_type  StreamType();
public:
	av_u64			TimeStamp();
	av_u32			ImageWidth();
	av_u32			ImageHeigh();
	av_u32			Channel();
	av_u32			Slave();
	av_comp_t		Comp();
	av_frame_type   FrameType();
	av_ushort		FrameRate();
public:
	av_u32			SampleRate();
	av_u32			SampleBits();
	av_u32			MediaPropertyMask();
private:
	av_stream_type	m_StreamType;
	av_u64			m_TimeStamp;
	av_short		m_Channel;
	av_short		m_Slave;

	av_comp_t		m_Comp;
	av_u32			m_ImageWidth;
	av_u32			m_ImageHeigh;
	av_frame_type	m_FrameType;
	av_ushort		m_FrameRate;

	av_u32			m_SampleRate;
	av_u32			m_SampleBits;
	av_u32			m_MediaPropertyMask;
};



typedef struct {
	CMutex mutex;
	av_u32 total;
	std::list <CAvPacket *> LeftPacket;
}C_PacketNode;

#define AVPACKET_MAX_FRAME	(500*1024)
#define AVPACKET_UNIT		(20*1024)

class CAvPacketManager:public CTimer
{

private:
	CAvPacketManager();
	~CAvPacketManager();
public:
	SINGLETON_DECLARE(CAvPacketManager);
	av_bool Initialize();

	CAvPacket *GetAvPacket(av_u32 NewLen = AVPACKET_MAX_FRAME);
	av_bool PutAvPacket(CAvPacket *Packet);
	av_bool Dump();
	av_void OnTime();
private:
	C_PacketNode m_AvPacketNodeInfo[AVPACKET_MAX_FRAME / AVPACKET_UNIT];

};








#define g_AvPacketManager (*CAvPacketManager::instance())








#endif
