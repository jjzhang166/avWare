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
#ifndef _RTSPPDU_H_
#define _RTSPPDU_H_
#include <string>
#include "Http.h"

typedef enum {
	rMethod_Options,
	rMethod_Describe,
	rMethod_Setup,
	rMethod_Play,
	rMethod_Record,
	rMethod_Pause,
	rMethod_GetParameter,
	rMethod_SetParameter,
	rMethod_Teardown,
	rMethod_Num,
}RTSP_METHOD_E;


typedef enum {
	Rtsp_Server,
	Rtsp_Client,
}RTSP_SER_CLI_E;

#define RTSP_SESSION_RTP_OVER_TCP "RTP/AVP/TCP;unicast"
#define RTSP_SESSION_RTP_OVER_UDP "RTP/AVP;unicast"
#define RTSP_SESSION_RTP_OVER_MUL "RTP/AVP;multicast"
#define RTP_OVER_MULTICAST_ADDR "230.0.0.2"

class CRtspPdu
{
public:
	CRtspPdu(RTSP_SER_CLI_E sce = Rtsp_Server);
	virtual ~CRtspPdu();
	
	int parseBody(const std::string data);
	int packetAckBody(std::string &AckString);
	int packetAckAppend(std::string &src, std::string &value);


public:
	RTSP_METHOD_E	m_Method;
	std::string		m_Session;
	std::string		m_Ssrc;
	std::string		m_Url;
	std::string		m_UserAgent;
	std::string		m_Version;
	std::string		m_Cseq;
	std::string		m_sdp;
	std::string		m_TrackId;
	std::string		m_TransPtl;
	std::string		m_TransMulticastAddr;
	std::string		m_CliRtpPort;
	std::string		m_CliRtcpPort;
	std::string		m_SerRtpPort;
	std::string		m_SerRtcpPort;
	std::string		m_MediaEncodec;

private:

	RTSP_SER_CLI_E  m_ServiceType;

};








#endif