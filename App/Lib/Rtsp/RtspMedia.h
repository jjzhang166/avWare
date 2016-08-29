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
#ifndef _RTSP_H_
#define _RTSP_H_
#include <string>
#include "CAvObject.h"
#include "RTP.h"
#include "RSock.h"
#include "RTCP.h"


#if defined (_AV_WARE_)
#include "AvPacket/AvPacket.h"
#endif
typedef enum {
	nal_unit_type_nr = 0x01 << 0,
	nal_unit_type_p = 0x01 << 1,
	nal_unit_type_dataA = 0x01 << 2,
	nal_unit_type_dataB = 0x01 << 3,
	nal_unit_type_dataC = 0x01 << 4,
	nal_unit_type_idr = 0x01 << 5,
	nal_unit_type_sei = 0x01 << 6,
	nal_unit_type_sps = 0x01 << 7,
	nal_unit_type_pps = 0x01 << 8,
	nal_unit_type_delimiter = 0x01 << 9,
	nal_unit_type_nalend = 0x01 << 10,
	nal_unit_type_streamend = 0x01 << 11,
	nal_unit_type_pading = 0x01 << 12,
}nal_unit_type_rtsp;


typedef enum 
{
	NAL_UNIT_CODED_SLICE_TRAIL_N = 0,   // 0  
	NAL_UNIT_CODED_SLICE_TRAIL_R,   // 1  

	NAL_UNIT_CODED_SLICE_TSA_N,     // 2  
	NAL_UNIT_CODED_SLICE_TLA,       // 3   // Current name in the spec: TSA_R  

	NAL_UNIT_CODED_SLICE_STSA_N,    // 4  
	NAL_UNIT_CODED_SLICE_STSA_R,    // 5  

	NAL_UNIT_CODED_SLICE_RADL_N,    // 6  
	NAL_UNIT_CODED_SLICE_DLP,       // 7 // Current name in the spec: RADL_R  

	NAL_UNIT_CODED_SLICE_RASL_N,    // 8  
	NAL_UNIT_CODED_SLICE_TFD,       // 9 // Current name in the spec: RASL_R  

	NAL_UNIT_RESERVED_10,
	NAL_UNIT_RESERVED_11,
	NAL_UNIT_RESERVED_12,
	NAL_UNIT_RESERVED_13,
	NAL_UNIT_RESERVED_14,
	NAL_UNIT_RESERVED_15, 
	NAL_UNIT_CODED_SLICE_BLA,       // 16   // Current name in the spec: BLA_W_LP  
	NAL_UNIT_CODED_SLICE_BLANT,     // 17   // Current name in the spec: BLA_W_DLP  
	NAL_UNIT_CODED_SLICE_BLA_N_LP,  // 18  
	NAL_UNIT_CODED_SLICE_IDR,       // 19  // Current name in the spec: IDR_W_DLP  
	NAL_UNIT_CODED_SLICE_IDR_N_LP,  // 20  
	NAL_UNIT_CODED_SLICE_CRA,       // 21  
	NAL_UNIT_RESERVED_22,
	NAL_UNIT_RESERVED_23,

	NAL_UNIT_RESERVED_24,
	NAL_UNIT_RESERVED_25,
	NAL_UNIT_RESERVED_26,
	NAL_UNIT_RESERVED_27,
	NAL_UNIT_RESERVED_28,
	NAL_UNIT_RESERVED_29,
	NAL_UNIT_RESERVED_30,
	NAL_UNIT_RESERVED_31,

	NAL_UNIT_VPS,                   // 32  
	NAL_UNIT_SPS,                   // 33  
	NAL_UNIT_PPS,                   // 34  
	NAL_UNIT_ACCESS_UNIT_DELIMITER, // 35  
	NAL_UNIT_EOS,                   // 36  
	NAL_UNIT_EOB,                   // 37  
	NAL_UNIT_FILLER_DATA,           // 38  
	NAL_UNIT_SEI,                   // 39 Prefix SEI  
	NAL_UNIT_SEI_SUFFIX,            // 40 Suffix SEI  
	NAL_UNIT_RESERVED_41,
	NAL_UNIT_RESERVED_42,
	NAL_UNIT_RESERVED_43,
	NAL_UNIT_RESERVED_44,
	NAL_UNIT_RESERVED_45,
	NAL_UNIT_RESERVED_46,
	NAL_UNIT_RESERVED_47,
	NAL_UNIT_UNSPECIFIED_48,
	NAL_UNIT_UNSPECIFIED_49,
	NAL_UNIT_UNSPECIFIED_50,
	NAL_UNIT_UNSPECIFIED_51,
	NAL_UNIT_UNSPECIFIED_52,
	NAL_UNIT_UNSPECIFIED_53,
	NAL_UNIT_UNSPECIFIED_54,
	NAL_UNIT_UNSPECIFIED_55,
	NAL_UNIT_UNSPECIFIED_56,
	NAL_UNIT_UNSPECIFIED_57,
	NAL_UNIT_UNSPECIFIED_58,
	NAL_UNIT_UNSPECIFIED_59,
	NAL_UNIT_UNSPECIFIED_60,
	NAL_UNIT_UNSPECIFIED_61,
	NAL_UNIT_UNSPECIFIED_62,
	NAL_UNIT_UNSPECIFIED_63,
	NAL_UNIT_INVALID,
}NalUnitType;

struct SliceInfo{
	unsigned char *Start;
	unsigned int SliceLen;
	unsigned int nut;
};
typedef struct {
	unsigned int pic_width;
	unsigned int pic_high;
	unsigned char framerate;
	unsigned char profileidc;
	unsigned char levelidc;
	unsigned int  nut;
	unsigned char SliceNu;
	struct SliceInfo Sliceinfo[10];
}FrameSliceInfo;

int GetH264FrameSliceInfo(unsigned char *FrameData, unsigned int Datalen, FrameSliceInfo *SliceInfo);
int GetH265FrameSliceInfo(unsigned char *FrameData, unsigned int Datalen, FrameSliceInfo *SliceInfo);

//class CRtspMedia :public CAvObject, public CRTP, public CRTCP
class CRtspMedia :public CAvObject
{

public:
	CRtspMedia();
	virtual ~CRtspMedia();

public:
	typedef enum {
		RTSP_MEDIA_CLIENT,
		RTSP_MEDIA_SERVER,
	}RTSP_MEDIA_MODE;
	typedef enum{
		RTSP_MEDIA_AUDIO,
		RTSP_MEDIA_VIDEO,
		RTSP_MEDIA_Nr,
	}RTSP_MEDIA_TYPE_E;
	typedef enum{
		RTSP_MEDIA_ENCODEC_H264,
		RTSP_MEDIA_ENCODEC_H265,
		RTSP_MEDIA_ENCODEC_MJPEG,
		RTSP_MEDIA_ENCODEC_AAC,
		RTSP_MEDIA_ENCODEC_G711A,
		RTSP_MEDIA_ENCODEC_G711U,
		RTSP_MEDIA_ENCODEC_PCM,
		RTSP_MEDIA_ENCODEC_NR,
	}RTSP_MEDIA_ENCODEC;
	
	typedef enum {
		RTP_OVER_TCP,
		RTP_OVER_UDP,
		RTP_OVER_MUL,
		RTP_OVER_NR,
	}RTSP_RTP_OVER_E;

 	virtual bool StartMedia()		= 0;
 	virtual bool StopMedia()		= 0;



	
	

	virtual void StartSendMedia();

	void SetSequence(RTSP_MEDIA_TYPE_E type,unsigned int Seq);
	void SetStartTimeStamp(RTSP_MEDIA_TYPE_E type, unsigned int TmStamp);
	void SetTimeStampInterval(RTSP_MEDIA_TYPE_E type, unsigned int TmStamp);
	void SetSyncSourceIdentCode(RTSP_MEDIA_TYPE_E type, unsigned int ssrc);
	void SetRtpSocket(RTSP_MEDIA_TYPE_E type, int sock);
	void SetRtcpSocket(RTSP_MEDIA_TYPE_E type, int sock);
	void SetPayLoadType(RTSP_MEDIA_TYPE_E type, int payloadt);
	void SetInterChnPort(RTSP_MEDIA_TYPE_E type, int InterChnPort);
	
	void SetRtpDataOver(RTSP_RTP_OVER_E over);
	RTSP_RTP_OVER_E GetRtpDataOver();

public:
	bool GetMediaEncodec(RTSP_MEDIA_TYPE_E type, std::string &Ecodec);
	RTSP_MEDIA_ENCODEC GetMediaEncodec(RTSP_MEDIA_TYPE_E type);
private:
	RTSP_RTP_OVER_E m_RtpOver;

public:
	virtual bool GetMediaSdpInfo(RTSP_MEDIA_TYPE_E type, std::string &SdpInfo);
	virtual bool SetMediaSdpInfo(RTSP_MEDIA_TYPE_E type, std::string &SdpInfo);
	bool IsSetSdpOver(RTSP_MEDIA_TYPE_E type) { return m_SdpInfo[type].size() > 0 ? true : false; }

protected:
	bool m_SendMedia;
	RTSP_MEDIA_ENCODEC m_MediaEncodec[RTSP_MEDIA_Nr];
	std::string m_SdpInfo[RTSP_MEDIA_Nr];
#define IsSetSdp(type) (m_SdpInfo[type].size() > 0 ?true:false)
	
private:
	CRTP m_MediaRTP[RTSP_MEDIA_Nr];
	CRTCP m_MediaRTCP[RTSP_MEDIA_Nr];

public:
	virtual av_bool RtcpGetPacket(CRTCP::RTCP_TYPE type, std::string &Packet);
	virtual CRTCP::RTCP_TYPE RtcpParsePacket(std::string &Packet);

public:
	//异步发送
	virtual int SendRtpPacket(int Sock, const CRTP::RTP_PACKET_T *packet, int packetcnt);
	virtual int OnFrameRecv(unsigned char *data, int len);

	virtual int RtpPacketH264Fua(const char *nalu_data, int nalu_len);
	virtual int RtpPacketH265Fua(const char *nalu_data, int nalu_len);
	virtual int RtpPacketMjpegFua();
	virtual int RtpPakcetJpegFua();
	virtual int RtpPacketAudioFua(const char *AFrame, int FrameLen, RTSP_MEDIA_ENCODEC codeC);

	virtual int RtpUnPacketH264Fua();
	virtual int RtpUnPacketH265Fua();
	virtual int RtpUnPacketMjpegFua();
	virtual int RtpUnPakcetJpegFua();
	virtual int RtpUnPacketAudioFua();
};


class CRtspMediaSer:public CRtspMedia
{
public:
	CRtspMediaSer();
	virtual ~CRtspMediaSer();
	virtual bool StartMedia()		= 0;
	virtual bool StopMedia()		= 0;

	
#ifdef _AV_WARE_
public:
	int PushVideoStreamPacket(CAvPacket *AvPacket);

private:
	int PushVideostreamH264Packet(CAvPacket *AvPacket);
	int PushVideostreamH265Packet(CAvPacket *AvPacket);
	int PushVideostreamMjpegPacket(CAvPacket *AvPacket);
	int PushVideostreamG711aPacket(CAvPacket *AvPacket);
	int PushVideostreamG711uPacket(CAvPacket *AvPacket);
	int PushVideostreamAacPacket(CAvPacket *AvPacket);
	int PushVideostreamPcmPacket(CAvPacket *AvPacket);
#else
public:
	int PushStreamFrame(CRtspMedia::RTSP_MEDIA_ENCODEC codeC, const char *data, int len, int TimeInterl);
private:
	int PushH264VideoStreamFrame(const char *data,	int len, int TimeInterl);
	int PushH265VideoStreamFrame(const char *data,	int len, int TimeInterl);
	int PushMjpegVideoStreamFrame(const char *data, int len, int TimeInterl);
	int PushG711aAudioStreamFrame(const char *data, int len, int TimeInterl);
	int PushG711uAudioStreamFrame(const char *data, int len, int TimeInterl);
	int PushAacAudioStreamFrame(const char *data, int len, int TimeInterl);
	int PushPcmAudioStreamFrame(const char *data,	int len, int TimeInterl);
#endif
private:
	int m_FrameIndex;
	av_u64  m_MediaLastPts[RTSP_MEDIA_Nr];
};



class CRtspMediaCli:public CRtspMedia
{
public:
	CRtspMediaCli();
	virtual ~CRtspMediaCli();

private:

};


#endif
