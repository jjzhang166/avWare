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
#ifndef _HTTP_H_
#define _HTTP_H_
#include <string>
#include <list>
#define HTTPHEAD_CONTENT_TYPE_XML	"text/xml"
#define HTTPHEAD_CONTENT_TYPE_SDP	"application/sdp"
#define HTTPHEAD_CONTENT_TYPE_HTTP	"application/http"
#define HTTPHEAD_CONTENT_TYPE_CGI	"application/x-httpd-cgi"


#define HTTPHEAD_VERSION_RTSP		"RTSP/1.0"
#define HTTPHEAD_VERSION_HTTP		"HTTP/1.1"
#define HTTPHEAD_VERSION_SIP		"SIP/2.0"

#define HTTPHEAD_CONTENT_TYPE		"Content-Type"
#define HTTPHEAD_USER_AGENT			"User-Agent"
#define HTTPHEAD_HOST				"Host"
#define HTTPHEAD_CLIETN_ADDRESS		"X-Client-Address"
#define HTTPHEAD_CONTENT_LENGTH		"Content-Length"
#define HTTPHEAD_CONTENT_LENGTH2	"Content-length"
#define HTTPHEAD_ACCEPT				"Accept"
#define HTTPHEAD_DATE				"Date"
#define HTTPHEAD_SERVER				"Server"
#define HTTPHEAD_SET_COOKIE			"Set-Cookie"
#define HTTPHEAD_COOKIE				"Cookie"
#define HTTPHEAD_FROM				"From"
#define HTTPHEAD_TO					"To"
#define HTTPHEAD_AUTHOR				"Authorization"
#define HTTPHEAD_CONTENT_BASE		"Content-Base"
#define HTTPHEAD_CONTENT_ENCODING	"Content-Encodeing"
#define HTTPHEAD_CONTENT_LANGUAGE	"Content-Language"
#define HTTPHEAD_ACCEPT_ENCODEING	"Accept-Encodeing"
#define HTTPHEAD_ACCEPT_LANGUAGE	"Accept-Language"


#define SDP_CONTROL					"a=control"
#define SDP_RTPMAP					"a=rtpmap"
#define SDP_FMTP					"a=fmtp"


#define HTTPHEAD_RTSP_RANGE			"Range"
#define HTTPHEAD_RTSP_CSEQ			"CSeq"
#define HTTPHEAD_RTSP_CSEQEX		"Cseq"
#define HTTPHEAD_RTSP_RTP_INFO		"RTP-Info"
#define HTTPHEAD_RTSP_SESSION		"Session"
#define HTTPHEAD_RTSP_TIMESTAMP		"Timestamp"
#define HTTPHEAD_RTSP_TRANSPORT		"Transport"


typedef enum {
	Rtsp_Server,
	Rtsp_Client,
}RTSP_SER_CLI_E;


class RtspHttp
{
public:
	RtspHttp();
	virtual ~RtspHttp();

public:
	static int SplitStr(const std::string data, std::list<std::string> &result);
	int parseHead(const std::string data);
	int parseLine(const std::string &data);
	int packetLine(std::string &data);


public:
	RTSP_SER_CLI_E _x_SerCli_Mode;
	std::string _x_accept;
	std::string _x_rtp_info;
	std::string _x_timestamp;
	std::string _x_range;
	std::string _x_scale;
	std::string _x_session;
	std::string _x_cseq;
	std::string _x_user_agent;
	std::string _x_server;
	std::string _x_date;
	std::string _x_content_type;
	std::string _x_content_length;
	
	std::string _x_url;
	std::string _x_version;
	std::string _x_method;
	std::string _x_authorization;
	std::string _x_status_code;
	std::string _x_transport;


	int		_x_sdp_index;
	std::string _x_sdp_control[2];
	std::string _x_sdp_rtpmap[2];
	std::string _x_sdp_fmtp[2];
	};


#endif
