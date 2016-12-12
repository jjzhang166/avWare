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
#ifndef _RTCP_H_
#define _RTCP_H_

#include "Apis/AvWareCplusplus.h"
#include "RTP.h"


typedef struct{
	unsigned char csrccount : 4;
	unsigned char extension : 1;
	unsigned char padding : 1;
	unsigned char version : 2;

	unsigned char payloadtype : 7;
	unsigned char marker : 1;

	unsigned short sequencenumber;
	unsigned int timestamp;
	unsigned int ssrc;
}RTCP_HEADER;


typedef struct
{
	unsigned short extid;
	unsigned short length;
}RTPExtensionHeader;

typedef struct
{
	unsigned int ssrc;
}RTPSourceIdentifier;

typedef struct
{
#ifdef RTP_BIG_ENDIAN
	unsigned char version : 2;
	unsigned char padding : 1;
	unsigned char count : 5;
#else // little endian
	unsigned char count : 5;
	unsigned char padding : 1;
	unsigned char version : 2;
#endif // RTP_BIG_ENDIAN

	unsigned char packettype;
	unsigned short length;
}RTCPCommonHeader;

typedef struct
{
	unsigned int  SSRC;
	unsigned int  ntptime_msw;
	unsigned int  ntptime_lsw;
	unsigned int  rtptimestamp;
	unsigned int packetcount;
	unsigned int octetcount;
}RTCPSenderReport;

typedef struct
{
	unsigned int	ssrc; // Identifies about which SSRC's data this report is...
	unsigned char	fractionlost;
	unsigned char	packetslost[3];
	unsigned int	exthighseqnr;
	unsigned int	jitter;
	unsigned int	lsr;
	unsigned int	dlsr;
}RTCPReceiverReport;

typedef struct
{
	unsigned char  sdesid;
	unsigned char length;
}RTCPSDESHeader;



class CRTCP
{
public:
	typedef enum {
		RTCP_SR		= 200,
		RTCP_RR		= 201,
		RTCP_SEDS	= 202,
		RTCP_BYE	= 203,
		RTCP_APP	= 204,
	}RTCP_TYPE;
	typedef enum {
		RTCP_SDES_END = 0,
		RTCP_SDES_CNAME,
		RTCP_SDES_NAME,
		RTCP_SDES_EMAIL,
		RTCP_SDES_PHONE,
		RTCP_SDES_LOC,
		RTCP_SDES_TOOL,
		RTCP_SDES_NOTE,
		RTCP_SDES_PRIV,
	}RTCP_SDES_TYPE;
private:

public:
	CRTCP();
	virtual ~CRTCP();
public:
	virtual av_bool RtcpGetPacket(RTCP_TYPE type, std::string &Packet);
	virtual RTCP_TYPE RtcpParsePacket(std::string &Packet);

public:
	void SetSock(int sock);
	int Sock();
private:
	int m_Sock;
};




#endif
