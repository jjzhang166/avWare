#include "icmp.h"
#include <cstdio>
#include <stdlib.h>
#include <cstring>
#include "Apis/AvWareType.h"

#pragma pack(1) 
#define ICMP_ECHOREPLY  0  
#define ICMP_ECHOREQ    8 
// IP Header -- RFC 791  
typedef struct tagIPHDR
{
	unsigned char	VIHL;           // Version and IHL  
	unsigned char	TOS;            // Type Of Service  
	short			TotLen;         // Total Length  
	short			ID;             // Identification  
	short			FlagOff;        // Flags and Fragment Offset  
	unsigned char	TTL;            // Time To Live  
	unsigned char	Protocol;       // Protocol  
	unsigned char	Checksum;       // Checksum  
	struct  in_addr iaSrc;  // Internet Address - Source  
	struct  in_addr iaDst;  // Internet Address - Destination  
}IPHDR, *PIPHDR;

// ICMP Header - RFC 792  
typedef struct tagICMPHDR
{
	unsigned char	Type;           // Type  
	unsigned char	Code;           // Code  
	unsigned short	Checksum;       // Checksum  
	unsigned short	ID;             // Identification  
	unsigned short	Seq;            // Sequence  
	char			Data;           // Data  
}ICMPHDR, *PICMPHDR;


#define REQ_DATASIZE 32     // Echo Request Data size  

// ICMP Echo Request  
typedef struct tagECHOREQUEST
{
	ICMPHDR			icmpHdr;
	unsigned long   dwTime;
	char			cData[REQ_DATASIZE];
}ECHOREQUEST, *PECHOREQUEST;


// ICMP Echo Reply  
typedef struct tagECHOREPLY
{
	IPHDR   ipHdr;
	ECHOREQUEST echoRequest;
	char    cFiller[256];
}ECHOREPLY, *PECHOREPLY;
#pragma pop()


unsigned short in_cksum(unsigned short *addr, int len)
{
	register int nleft = len;
	register unsigned short *w = addr;
	register unsigned short answer;
	register int sum = 0;

	while (nleft > 1)  {
		sum += *w++;
		nleft -= 2;
	}

	if (nleft == 1) {
		unsigned short u = 0;

		*(unsigned char  *)(&u) = *(unsigned char  *)w;
		sum += u;
	}

	sum = (sum >> 16) + (sum & 0xffff); 
	sum += (sum >> 16);
	answer = ~sum;
	return (answer);
}

CICMPPing::CICMPPing()
{
	memset(m_HostIpAddr, 0x00, sizeof(m_HostIpAddr));
	m_TimeOutUs = 1;
	m_RawSocket = -1;
	m_nID = 1;
	m_nSeq = 1;
	m_RawSocket = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (m_RawSocket <= 0){
		av_error("socket IPPROTO Error  ret = %d\n", m_RawSocket);
	}

}
CICMPPing::~CICMPPing()
{
	if (m_RawSocket != -1){
#if defined(WIN32)
		closesocket(m_RawSocket);
#else
		close(m_RawSocket);
#endif
	}
	m_RawSocket = -1;
}

bool CICMPPing::SetRemoteHost(const char *Host)
{
	struct hostent *hent = NULL;
	hent = gethostbyname(Host);
	if (hent == NULL){
		return false;
	}
	char *ptr, **pptr;
// 	printf("official hostname:%s\n", hent->h_name);
// 	for (pptr = hent->h_aliases; *pptr != NULL; pptr++)
// 		printf(" alias:%s/n", *pptr);


	/* 根据地址类型，将地址打出来 */
	switch (hent->h_addrtype)
	{
	case AF_INET:
	{
		pptr = hent->h_addr_list;
		for (; *pptr != NULL; pptr++)
			inet_ntop(hent->h_addrtype, *pptr, m_HostIpAddr, sizeof(m_HostIpAddr));
	}
	break;

	default:
		printf("unknown address type/n");
		break;
	}
	return true;
}
bool CICMPPing::SetTimeOut(int us)
{
	assert(us > 0);
	m_TimeOutUs = us;
	return true;
}
int	 CICMPPing::Ping(int count)
{
	int ret = 0;
	for (int i = 0; i < count; i++){
		if (0 != (ret = SendICMP())){
			av_error("SendICMP Return Error ret = %d\n", ret);
			return -1;
		}
		ret += RecvICMP();
	}

	return ret;
}

int		CICMPPing::SendICMP()
{
	av_warning("CICMPPing %s [%s] %d bytes:\n", m_HostIpAddr, m_HostIpAddr, REQ_DATASIZE);
	struct sockaddr_in addr;

	memset(&addr,0x00, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(m_HostIpAddr);

	static ECHOREQUEST echoReq;
	echoReq.icmpHdr.Type = ICMP_ECHOREQ;
	echoReq.icmpHdr.Code = 0;
	echoReq.icmpHdr.Checksum = 0;
	echoReq.icmpHdr.ID = __LINE__;
	echoReq.icmpHdr.Seq = m_nSeq++;

	for (int nRet = 0; nRet < REQ_DATASIZE; nRet++)
		echoReq.cData[nRet] = ' ' + nRet;

	echoReq.dwTime = clock();
	echoReq.icmpHdr.Checksum = in_cksum((unsigned short *)&echoReq, sizeof(ECHOREQUEST));
                            
	int ret = sendto(m_RawSocket, (char *)&echoReq, sizeof(ECHOREQUEST), 0, (struct sockaddr *)&addr, sizeof(addr));
	if (ret != sizeof(ECHOREQUEST)){
		return -1;
	}
	return 0;
}
int		CICMPPing::RecvICMP()
{
	struct timeval timeo;
	// 设定TimeOut时间     
	timeo.tv_sec = m_TimeOutUs / (1000 * 1000);
	timeo.tv_usec = m_TimeOutUs % (1000 * 1000);
	fd_set readfds;

	struct sockaddr_in from;

	char ReadBuffer[2048];
	int  ReadLen = 0;

	FD_ZERO(&readfds);
	FD_SET(m_RawSocket, &readfds);
	int maxfds = m_RawSocket + 1;
	int n = select(maxfds, &readfds, NULL, NULL, &timeo);
	if (n <= 0){
		av_warning("ip:%s,Time out\n", m_HostIpAddr);
		return 0;
	}

	int fromlen = sizeof(from);
	n = recvfrom(m_RawSocket, ReadBuffer, sizeof(ReadBuffer), 0, (struct sockaddr *)&from, (socklen_t *)&fromlen);
	if (n < 1) {
		av_error("CICMPPing recvfrom func error ret = %d \n",n);
		return -1;
	}

	char *from_ip = (char *)inet_ntoa(from.sin_addr);
	// 判断是否是自己Ping的回复     
	if (strcmp(from_ip, m_HostIpAddr) != 0)
	{
		av_warning("Pingip:%s Fromip:%s\nNowPingip is not same to Fromip,so ping wrong!\n", m_HostIpAddr, from_ip);
		return -2;
	}

	

	IPHDR *iph = (IPHDR *)ReadBuffer;

	ICMPHDR *icmp = (ICMPHDR *)(ReadBuffer + (iph->VIHL << 2));

	int useus = m_TimeOutUs - (timeo.tv_sec * 1000 * 1000 + timeo.tv_usec);
	av_msg("recv [%s] Bytes [%d] Times [%02d]ms TTL=[%d]\n",
		m_HostIpAddr, iph->VIHL << 2, useus, iph->TTL);
	// 判断Ping回复包的状态     
	if (icmp->Type == ICMP_ECHOREPLY && icmp->ID == m_nID){
		return 1;
	}
	return 1;
}