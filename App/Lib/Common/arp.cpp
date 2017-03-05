#include "arp.h"
#include "Apis/AvWareType.h"

#define EPT_IP   0x0800    /* type: IP */
#define EPT_ARP   0x0806    /* type: ARP */
#define EPT_RARP 0x8035    /* type: RARP */
#define ARP_HARDWARE 0x0001    /* Dummy type for 802.3 frames */
#define ARP_REQUEST 0x0001    /* ARP request */
#define ARP_REPLY 0x0002    /* ARP reply */

//定义以太网首部
typedef struct ehhdr
{
	unsigned char eh_dst[6];   /* destination ethernet addrress */
	unsigned char eh_src[6];   /* source ethernet addresss */
	unsigned short eh_type;   /* ethernet pachet type */
}EHHDR, *PEHHDR;

//定义以太网arp字段
typedef struct arphdr
{
	//arp首部
	unsigned short arp_hrd;    /* format of hardware address */
	unsigned short arp_pro;    /* format of protocol address */
	unsigned char arp_hln;    /* length of hardware address */
	unsigned char arp_pln;    /* length of protocol address */
	unsigned short arp_op;     /* ARP/RARP operation */

	unsigned char arp_sha[6];    /* sender hardware address */
	unsigned long arp_spa;    /* sender protocol address */
	unsigned char arp_tha[6];    /* target hardware address */
	unsigned long arp_tpa;    /* target protocol address */
}ARPHDR, *PARPHDR;

//定义整个arp报文包，总长度42字节
typedef struct arpPacket
{
	EHHDR ehhdr;
	ARPHDR arphdr;
} ARPPACKET, *PARPPACKET;

CArp::CArp()
{
	m_RawSock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
	if (m_RawSock <= 0){
		av_error("socket IPPROTO Error  ret = %d\n", m_RawSock);
	}
	m_RawSock = -1;
}
CArp::~CArp()
{

}

bool CArp::BroadCaseArp(const char *LocalIpAddr, const char *LocalMac)
{
	ARPPACKET ArpPack;
	ArpPack.ehhdr.eh_type = EPT_ARP;
	//sendto(m_RawSock, (char *)&ArpPack, sizeof(ARPPACKET), 0, );
	return true;
}
bool CArp::WaitArpAck(int TmOutMs, C_ConflictInfo ConflitInfo[], int &ConflitCount)
{
	return true;
}
