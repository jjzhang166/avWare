/*******************************************************************
|  Copyright(c) 2015-2016 Graceport Technology Development Co.,Ltd
|  All rights reserved.
|
|  版本: 1.0
|  作者: ironman [ironman@graceport.cn]
|  日期: 2016年6月6日
|  说明:
|
|  版本: 1.1
|  作者:
|  日期:
|  说明:
******************************************************************/
#include "Apis/AvWareType.h"
#include "AvNetService/AvNtp.h"

#define AV_NTP_PORT          123               /*NTP专用端口号字符串*/
#define NTP_SERVER_IP        "time.windows.com"
#define AV_NTP_PORT_STR      "123"          /*NTP专用端口号字符串*/
#define NTPV1                "NTP/V1"      /*协议及其版本号*/
#define NTPV2                "NTP/V2"
#define NTPV3                "NTP/V3"
#define NTPV4                "NTP/V4"
 
#define AV_NTP_PACK_LEN 48
#define AV_NTP_LI 0
#define AV_NTP_VN 3
#define AV_NTP_MODE 3
#define AV_NTP_STRATUM 0
#define AV_NTP_POLL 4
#define AV_NTP_PREC -6
 
#define AV_NTP_JAN_1970 0x83aa7e80 /* 1900年～1970年之间的时间秒数 */
#define AV_NTP_NTPFRAC(x)     (4294 * (x) + ((1981 * (x)) >> 11))
#define AV_NTP_USEC(x)         (((x) >> 12) - 759 * ((((x) >> 10) + 32768) >> 16))



typedef struct{
	unsigned int coarse;
	unsigned int fine;
}ntp_time;

struct ntp_packet{
	unsigned char leap_ver_mode;
	unsigned char startum;
	char poll;
	char precision;

	int root_delay;
	int root_dispersion;
	int reference_identifier;

	ntp_time reference_timestamp;
	ntp_time originage_timestamp;
	ntp_time receive_timestamp;
	ntp_time transmit_timestamp;
};

typedef struct{
	av_32 cur_tz;
	av_u16 update_interval;	//update time interval (1-65535 Min)
	av_u16 port;
	av_char ntp_domain[128];
}C_ConfigNetNtp;


CAvNtp::CAvNtp()
{
	m_NtpServer.clear();
	m_TimeZone = av_GMT_E8;
}
CAvNtp::~CAvNtp()
{

}

av_bool CAvNtp::Start()
{

	av_bool FuncRet = av_false;
	int NtpSocketFd;
	int ret;
	struct ntp_packet NtpPacket = { 0 };
	struct addrinfo RemoteAddr;
	struct addrinfo *NtpServer = NULL;
	memset(&RemoteAddr, 0, sizeof(struct addrinfo));
	RemoteAddr.ai_family = AF_UNSPEC;
	RemoteAddr.ai_socktype = SOCK_DGRAM;
	RemoteAddr.ai_protocol = IPPROTO_UDP;
	//TODO:获取配置表参数，并根据参数去创建套接字
	ret = getaddrinfo(m_NtpServer.c_str(), "123", &RemoteAddr, &NtpServer);
	if (ret != 0) {
		av_error("getaddrinfo ret = %d\n", ret);
		return av_false;
	}

	NtpSocketFd = socket(NtpServer->ai_family, NtpServer->ai_socktype, NtpServer->ai_protocol);
	if (NtpSocketFd < 0) {
		av_error("socket error \n");
		return av_false;
	}

	if (0 != GetNtpTime(NtpSocketFd, NtpServer, &NtpPacket)) {
		time_t t1, t2, t3, t4;
		t4 = time((time_t*)NULL) + AV_NTP_JAN_1970;
		t1 = NtpPacket.receive_timestamp.coarse;
		t2 = NtpPacket.originage_timestamp.coarse;
		t3 = NtpPacket.transmit_timestamp.coarse;

		time_t offset = ((t4 - t1) - (t3 - t2)) >> 1;
		//TODO:根据时区获取时间
		av_msg("NTP offset[%ld]\n", offset);
		FuncRet = av_true;
		{//set sys time
			
		}
	}
	else {
		av_msg("NTP Sync Time failed\n");
		FuncRet = av_false;
	}

#ifdef WIN32
	closesocket(NtpSocketFd);
#else
	ret = close(NtpSocketFd);
	if (!ret) {
		perror("close:");
	}
#endif
	return FuncRet;
}

av_bool CAvNtp::Stop()
{
	return av_true;
}

av_bool CAvNtp::SetTimeZone(av_timezone_t timezone)
{
	m_TimeZone = timezone;
	return av_true;
}

av_bool CAvNtp::SetNtpServer(std::string Server)
{
	m_NtpServer = Server;
	return av_true;
}

int CAvNtp::ConstructNtpPacket(char *packet)
{
	char version = 1;
    long tmp_wrd;
    int port;
    time_t curlocaltime;
    
	char protocol[32];
	memset(packet, 0, AV_NTP_PACK_LEN);
	port = AV_NTP_PORT;
	/*设置16字节的包头*/
   
	version = protocol[5]-0x30;
	tmp_wrd = htonl((AV_NTP_LI << 30)|(version << 27)
		|(AV_NTP_MODE << 24)|(AV_NTP_STRATUM << 16)|(AV_NTP_POLL << 8)|(AV_NTP_PREC & 0xff));

	memcpy(packet, &tmp_wrd, sizeof(tmp_wrd));

	/*设置Root Delay、Root Dispersion和Reference Indentifier */
	tmp_wrd = htonl(1<<16);

	memcpy(&packet[4], &tmp_wrd, sizeof(tmp_wrd));
	memcpy(&packet[8], &tmp_wrd, sizeof(tmp_wrd));

	/*设置Timestamp部分*/
	time(&curlocaltime);
	/*设置Transmit Timestamp coarse*/
	tmp_wrd = htonl(AV_NTP_JAN_1970 + (long)curlocaltime);
	memcpy(&packet[40], &tmp_wrd, sizeof(tmp_wrd));

	/*设置Transmit Timestamp fine*/
	tmp_wrd = htonl((long)AV_NTP_NTPFRAC(curlocaltime));
	memcpy(&packet[44], &tmp_wrd, sizeof(tmp_wrd));

	return AV_NTP_PACK_LEN;
}

int CAvNtp::GetNtpTime(int sk, struct addrinfo *addr, struct ntp_packet *ret_time)
{
	fd_set pending_data;
    struct timeval block_time;
	socklen_t socklen = sizeof(struct sockaddr);
    char data[AV_NTP_PACK_LEN * 2];
    int packet_len, count = 0, result;
	
    if (!(packet_len = ConstructNtpPacket(data))) {
        return 0;
    }

    /*客户端给服务器端发送NTP协议数据包*/
    if ((result = sendto(sk, data,
		packet_len, 0, addr->ai_addr, socklen)) < 0) {
        perror("sendto");
        return 0;
    }
   
    /*调用select()函数，并设定超时时间为3s*/
    FD_ZERO(&pending_data);
    FD_SET(sk, &pending_data);

    block_time.tv_sec=3;
    block_time.tv_usec=0;
    if (select(sk + 1, &pending_data, NULL, NULL, &block_time) > 0)
    {
        /*接收服务器端的信息*/
        if ((count = recvfrom(sk, data,
			AV_NTP_PACK_LEN * 8, 0, addr->ai_addr, &socklen)) < 0) {
            perror("recvfrom");
            return 0;
        }
        
       if (count < AV_NTP_PACK_LEN) {
            return 0;
        }


        /* 设置接收NTP包的数据结构 */
        ret_time->leap_ver_mode = (unsigned char)ntohl(data[0]);

		ret_time->startum = (unsigned char)ntohl(data[1]);
		ret_time->poll = (char)ntohl(data[2]);

		ret_time->precision = (char)ntohl(data[3]);

        ret_time->root_delay = ntohl(*(int*)&(data[4]));

        ret_time->root_dispersion = ntohl(*(int*)&(data[8]));

        ret_time->reference_identifier = ntohl(*(int*)&(data[12]));

        ret_time->reference_timestamp.coarse = ntohl(*(int*)&(data[16]));
        ret_time->reference_timestamp.fine = ntohl(*(int*)&(data[20]));

        ret_time->originage_timestamp.coarse = ntohl(*(int*)&(data[24]));
        ret_time->originage_timestamp.fine = ntohl(*(int*)&(data[28]));

        ret_time->receive_timestamp.coarse = ntohl(*(int*)&(data[32]));
        ret_time->receive_timestamp.fine = ntohl(*(int*)&(data[36]));

        ret_time->transmit_timestamp.coarse = ntohl(*(int*)&(data[40]));
        ret_time->transmit_timestamp.fine = ntohl(*(int*)&(data[44]));
        return 1;

    } /* end of if select */
	av_msg("select timeout\n");
    return 0;
}

