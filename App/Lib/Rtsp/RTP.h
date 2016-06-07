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
#ifndef _RTP_H_
#define _RTP_H_

class CRTP
{
public:
#define RTP_MAX_RTP_PACKET 10
	typedef enum {
		RTP_OVER_TCP,
		RTP_OVER_UDP,
		RTP_OVER_MUL,
	}RTP_OVER_E;
#pragma pack(1)
	typedef struct {
		char *base;
		int	  len;
	}RTP_PACKET_T;
#if 0
	typedef struct {
		unsigned char	Magic;
		unsigned char	Channel;
		unsigned short	Length;
	}Interleaved;
#endif
	typedef struct {
		unsigned short CsrcCount : 4;
		unsigned short Extension : 1;
		unsigned short Padding : 1;
		unsigned short Version : 2;

		unsigned short PayloadType : 7;
		unsigned short Marker : 1;

		unsigned short SequenceNm;
		unsigned long  Timestamp;
		unsigned long  Ssrc;
	}RtpHead;
#pragma pack()

public:
	CRTP();
	virtual ~CRTP();
public:
#define RTPFuIndicator(F,NRI,TYPE) (unsigned char)((F&0x01)<<7 | (NRI&0x03) << 5|(TYPE&0x1f))
#define RTPH264FuHeader(S,E,R,TYPE) (unsigned char)((S&0x01) << 7 | (E&0x01)<<6 | (R&0x01) << 5|(TYPE&0x1f))
#define RTPH264NALUTYPE(nalu) (unsigned char)(nalu&0x1f)

#define RTPH265FuIndicator(F,TYPE,LAYERID,TID) (unsigned short)((F&0x01)<<15|(TYPE&0x3f)<<9 | (LAYERID&0x1f)<<3 | (TID&0x07))
#define RTPH265NALUTYPE(nalu) (unsigned char)((nalu>>1)&0x3f)

	void FuIndicatorHeader(unsigned char Indicator/*F-NRI--TYPE-----*/, unsigned char FuHead/*S-E-R-TYPE-----*/);
	void FuIndicatorHeader(unsigned short Indicator/*F-TYPE------LAYERID-----TID---*/, unsigned char FuHead/*S-E-R-TYPE-----*/);
	void FuIndicatorHeader(unsigned char siglePacketFuHead);
	void SequenceNmAdd();
	void SetRtpOver(RTP_OVER_E roe);
	void SetRtpPort(int RtpPort);
	void SetRtpSock(int sock);
	void SetInterleaved(unsigned char Channel, unsigned short length);
	void SetMarker(unsigned char Marker);
	void SetTimestamp(unsigned long timestamp);
	void SetPayload(int Payload);
	void SetRtpHeadOver();
	void LoadData(char *data, int len);
	void RtpBufReset();

public:
	virtual int SendRtpPacket(const RTP_PACKET_T *packet, int packetcnt);
	virtual int OnFrameRecv(unsigned char *data, int len);
public:
	
	virtual int RtpPacketH264Fua(const char *nalu_data, int nalu_len);
	virtual int RtpPacketH265Fua();
	virtual int RtpPacketMjpegFua();
	virtual int RtpPakcetJpegFua();
	virtual int RtpPacketAudioFua();

	virtual int RtpUnPacketH264Fua();
	virtual int RtpUnPacketH265Fua();
	virtual int RtpUnPacketMjpegFua();
	virtual int RtpUnPakcetJpegFua();
	virtual int RtpUnPacketAudioFua();

protected:
	unsigned int m_Interleaved;
	RtpHead		 m_RtpHead;
	RTP_OVER_E   m_RtpOver;
	RTP_PACKET_T m_RtpBuf[RTP_MAX_RTP_PACKET];
	int			 m_RtpBufCnt;
	int			 m_RtpPort;
	int			 m_RtpSock;

	unsigned char m_FuIndicatorHeader[3];
	unsigned int m_FuIndicatorHeaderLen;

	unsigned int m_TempSeq;
	unsigned int m_TempTimeStamp;
};


typedef enum _NUT{
	NUT_NR = 0,
	NUT_STAP_A = 24,
	NUT_STAP_B = 25,
	NUT_MTAP16 = 26,
	NUT_MTAP24 = 27,
	NUT_FU_A = 28,
	NUT_FU_B = 29,
	NUT_FU_HEVC = 49,
}NUT_T;

typedef enum RTP_PT_E
{
	RTP_PT_ULAW = 0,        /* mu-law */
	RTP_PT_GSM = 3,        /* GSM */
	RTP_PT_G723 = 4,        /* G.723 */
	RTP_PT_ALAW = 8,        /* a-law */
	RTP_PT_G722 = 9,        /* G.722 */
	RTP_PT_S16BE_STEREO = 10,       /* linear 16, 44.1khz, 2 channel */
	RTP_PT_S16BE_MONO = 11,       /* linear 16, 44.1khz, 1 channel */
	RTP_PT_MPEGAUDIO = 14,       /* mpeg audio */
	RTP_PT_JPEG = 26,       /* jpeg */
	RTP_PT_H261 = 31,       /* h.261 */
	RTP_PT_MPEGVIDEO = 32,       /* mpeg video */
	RTP_PT_MPEG2TS = 33,       /* mpeg2 TS stream */
	RTP_PT_H263 = 34,       /* old H263 encapsulation */

	//RTP_PT_PRIVATE          = 96,       
	RTP_PT_H264 = 96,       /* hisilicon define as h.264 */
	RTP_PT_G726 = 97,       /* hisilicon define as G.726 */
	RTP_PT_ADPCM = 98,       /* hisilicon define as ADPCM */

	RTP_PT_INVALID = 127
}RTP_PT_E;


#define MTU_FRAME (1500)
#define MTU_TRANS (MTU_FRAME-40)
/*UDP 中裸数据长度*/
#define MTU_PLAYLOAD (MTU_TRANS-sizeof(unsigned int) - sizeof(CRTP::RtpHead) - sizeof(unsigned char) - sizeof(unsigned char))




#endif
