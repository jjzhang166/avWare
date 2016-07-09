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
#ifndef _PROTOSTREAM_H_
#define _PROTOSTREAM_H_

#include "ProtoStruct.h"

#ifdef __MOONPROTO_ //FOR VFWARE
#include "system/Object.h"
#include "system/Signals.h"
#else
#include "CAvObject.h"
#include "Signals.h"
#endif


#ifdef PROTOSHARE_EXPORTS

#if defined (WIN32)

#if defined (USE_STATIC_LIB)
#define PROTOSHARE_API
#else
#define PROTOSHARE_API __declspec(dllexport)
#endif

#else //else define win32
#define PROTOSHARE_API __attribute__((visibility("default")))
#endif //end define win32

#else //else PROTOSHARE_EXPORTS

#if defined(WIN32)
#if defined (USE_STATIC_LIB)
#define PROTOSHARE_API
#else
#define PROTOSHARE_API __declspec(dllimport)
#endif

#else
#define PROTOSHARE_API
#endif

#endif //end PROTOSHARE_EXPORTS



class PROTOSHARE_API CStreamPacket
{
public:
	CStreamPacket();
	~CStreamPacket();
public:
	int RefLock();
	int RefUnLock();
	int ReSize(int totallen);
	int StreamPacketGet(char *&pbase, int &len);
	int StreamPacketRelease();
	int StreamPacketAppend(char *base, int len);
	bool IsFrameOver();


	int GetChannel();
	int GetSlave();
	int GetFrameRate();
	Resolution GetResolutionSize();
	StreamCompress GetCompress();
	VideoSliceType GetVideoSliceType();
	int GetPackNum();
	int GetPackCnt();
	unsigned int GetPacketSec();
	unsigned int GetPacketUsec();
	unsigned int GetPacketSeqNum();
	int GetLen();
	unsigned int GetDownCtrl();


	int SetDownCtrl(unsigned int CtrlSingle);
	int SetChannel(int Channel);
	int SetSlave(int Slave);
	int SetFrameRate(int FrameRate);
	int SetResolutionSize(Resolution Size);
	int SetCompress(StreamCompress Compress);
	int SetVideoSliceType(VideoSliceType SliceType);
	int SetPackNum(int num);
	int SetPackCnt(int cnt);
	int SetPackSec(unsigned int sec);
	int SetPackUsec(unsigned int usec);
	int SetPacketSeqNum(unsigned int SeqNum);
	int SetLen(int totallen);


private:
	
	int  m_Channel;
	int  m_Slave;
	int  m_FrameRate;
	Resolution m_Size;
	StreamCompress m_Compress;
	VideoSliceType m_SliceType;
	int   m_PackNum;
	int	  m_PackCnt;
	unsigned int m_PacketSec;
	unsigned int m_PacketUsec;
	unsigned int m_PacketSeqNum;

	char *m_data;
	int   m_datasize;
	int   m_datalen;

	int	  m_ref;
	void *m_mutex;
	
	unsigned int m_DownloadCtrl;

};

typedef struct _C_AudioAttr{
	int Channel;
	int Slave;
}C_AudioAttr;
typedef struct _C_VideoAttr{
	int Channel;
	int Slave;
	Resolution		Size;
	Profile			EnCodeProfile;
	BitRateCtrl		Brc;
	StreamCompress	EnCodeType;
	VideoSliceType  SliceType;
	int				FrameRate;
	int				FrameCount;
	unsigned int	FrameSec;
	unsigned int	FrameUsec;
}C_VideoAttr;

typedef int(*OpenStreamFunc)(int Channel, int Slave, StreamContent AudioVideo);
typedef int(*CloseStreamFunc)(int Channel, int Slave, StreamContent AudioVideo);


class PROTOSHARE_API CStreamManager :public CAvObject
{
public:
	CStreamManager();
	~CStreamManager();
	SINGLETON_DECLARE(CStreamManager);
public:
	typedef TSignal1<CStreamPacket *>::SigProc OnStreamSigNalFunc;
	int RegisterOpenStream(OpenStreamFunc OpenStream);
	int RegisterCloseStream(CloseStreamFunc CloseStream);
	
	int StreamAudioAttr(int Channel, int Slave, C_AudioAttr &AudioAttr);
	int StreamAudioReset(int Channel, int Slave);
	int StreamAudioAppend(int Channel, int Slave, char *buf, int len, int marked);
	int StreamVideoAttr(int Channel, int Slave, C_VideoAttr &VideoAttr);
	int StreamVideoReset(int Channel, int Slave);
	int StreamVideoAppend(int Channel, int Slave, char *buf, int len, int marked);
	int StreamVideoLen(int Channel, int Slave);
	int StreamPush(CStreamPacket *Packet);
	int Start(int Channel, int Slave, CAvObject *obj, OnStreamSigNalFunc proc);
	int End(int Channel, int Slave, CAvObject *obj, OnStreamSigNalFunc proc);
private:
	int				 m_ChannelStatus[PROTO_MAX_CHANNEL][Proto_MAX_SLAVE];
	CStreamPacket   *mp_ChannelPacket[PROTO_MAX_CHANNEL][Proto_MAX_SLAVE];

private:
	TSignal1<CStreamPacket *> m_OnStreamSignal[PROTO_MAX_CHANNEL][Proto_MAX_SLAVE];

	OpenStreamFunc	m_OpenStream;
	CloseStreamFunc	m_CloseStream;
};



#define g_StreamManager (*CStreamManager::instance())


#endif
