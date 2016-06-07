
#ifndef _DHCP_H
#define _DHCP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#ifdef __linux__

#include <netinet/udp.h>
#include <netinet/ip.h>

#else
#include <winsock2.h>

typedef unsigned char u_int8_t;
typedef unsigned short u_int16_t;
typedef unsigned int u_int32_t;

#endif

struct dhcpMessage 
{
    u_int8_t op;
    u_int8_t htype;
    u_int8_t hlen;
    u_int8_t hops;
    u_int32_t xid;
    u_int16_t secs;
    u_int16_t flags;
    u_int32_t ciaddr;
    u_int32_t yiaddr;
    u_int32_t siaddr;
    u_int32_t giaddr;
    u_int8_t chaddr[16];
    u_int8_t sname[64];
    u_int8_t file[128];
    u_int32_t cookie;
    u_int8_t options[308]; /* 312 - cookie */ 
};

struct udp_dhcp_packet
{
    struct iphdr ip;
    struct udphdr udp;
    struct dhcpMessage data;
};

#define ETH_10MB              1
#define ETH_10MB_LEN          6

/*DHCP options start(部分列表)*/
#define DHCP_MAGIC          0x63825363

#define DHCP_PADDING        0x00
#define DHCP_SUBNET         0x01
#define DHCP_TIME_OFFSET    0x02
#define DHCP_ROUTER         0x03
#define DHCP_TIME_SERVER    0x04
#define DHCP_NAME_SERVER    0x05
#define DHCP_DNS_SERVER     0x06
#define DHCP_LOG_SERVER     0x07
#define DHCP_COOKIE_SERVER  0x08
#define DHCP_LPR_SERVER     0x09
#define DHCP_HOST_NAME      0x0c
#define DHCP_BOOT_SIZE      0x0d
#define DHCP_DOMAIN_NAME    0x0f
#define DHCP_SWAP_SERVER    0x10
#define DHCP_ROOT_PATH      0x11
#define DHCP_IP_TTL         0x17
#define DHCP_MTU            0x1a
#define DHCP_BROADCAST      0x1c
#define DHCP_NTP_SERVER     0x2a
#define DHCP_WINS_SERVER    0x2c
#define DHCP_REQUESTED_IP   0x32
#define DHCP_LEASE_TIME     0x33
#define DHCP_OPTION_OVER    0x34
#define DHCP_MESSAGE_TYPE   0x35
#define DHCP_SERVER_ID      0x36
#define DHCP_PARAM_REQ      0x37
#define DHCP_MESSAGE        0x38
#define DHCP_MAX_SIZE       0x39
#define DHCP_T1             0x3a
#define DHCP_T2             0x3b
#define DHCP_VENDOR         0x3c    /*版本1.0*/
#define DHCP_CLIENT_ID      0x3d

#define DHCP_PAD	    0
#define DHCP_END            0xFF
/*DHCP options end*/

#define BOOTREQUEST           1

/* DHCP协议-- see RFC 2131 */
#define SERVER_PORT         67
#define CLIENT_PORT         68

#define MAC_BCAST_ADDR            (unsigned char *) "\xff\xff\xff\xff\xff\xff"


#define DHCPDISCOVER            1
#define DHCPOFFER               2
#define DHCPREQUEST             3
#define DHCPDECLINE             4
#define DHCPACK                 5
#define DHCPNAK                 6
#define DHCPRELEASE             7
#define DHCPINFORM              8

typedef struct _dhcp_client_cfg
{
	unsigned long 	my_ip;
	unsigned long 	server_ip;
	unsigned long 	my_netmask;
	unsigned long 	my_gateway;
	unsigned long long 	server_DNS; //增加DNS ip地址
	unsigned long 	lease;
	unsigned long  	t1;
	unsigned long  	t2;
	unsigned long	serverid;	//保存serverid, 下次 启动的时侯,可以先直接尝试上次使用的ip
	unsigned char	isUseDhcp;
}DHCP_PROTO_CONFIG,*pDHCP_PROTO_CONFIG;

typedef struct _dhcp_clientsession_ctx
{
	int IfIndex;
	unsigned long LocalIpAddr;
	unsigned char  LocalMacAddr[6];
	unsigned char Reserved[2];
	unsigned char ClientId[7];
}DHCP_SESSION_CTX,*pDHCP_SESSION_CTX;

/**
*@description 此结构方便函数调用时传递
*/
typedef struct _DHCP_CLIENT_T
{
	DHCP_SESSION_CTX 	dhcp_session_ctx;
	DHCP_PROTO_CONFIG	dhcp_proto_config;
	char 				dhcp_host_name[256];
	struct dhcpMessage 	dhcpsendpacket; 
	struct dhcpMessage 	dhcprecvpacket;
}DHCP_CLIENT_T;

u_char DhcpGetOption(u_char *src,u_char opt, void *ptr, u_char size);

u_char *DhcpSetOption(u_char * dst, u_char opt, u_char *src, u_char size);
int UdpOutput(struct dhcpMessage *pDhcppacket, 
			   u_int32_t source_ip, 
			   int source_port,
			   u_int32_t dest_ip, 
			   int dest_port, 
			   unsigned char *szDestMacAddr, 
			   int ifindex);
int NetIfInfo(char *ifname, 
                    int *ifindex, 
                    u_int32_t *ipaddr,
                    unsigned char *szMac,
                    unsigned char *clinetID);
extern int UdpRawSocket(int ifindex);
u_int16_t checksum(void *addr, int count);
unsigned long random_xid(unsigned char *szMac);

extern int Dhcp_InitSessionCtx(DHCP_SESSION_CTX * pclientctx, char * eth_name);
int Dhcp_BackInfo(DHCP_CLIENT_T * pdhcp_client_in);
	
int UdpInput( int sockfd,struct dhcpMessage *pRecvPacket);
int DHCP_Receive(int iRawsocket,u_char xtype, DHCP_CLIENT_T * pdhcp_client_in, int time_out_in );
int DHCP_Discover(DHCP_CLIENT_T * pdhcp_client_in);
int DHCP_Request(u_long serverid,DHCP_CLIENT_T * pdhcp_client_in);
int DHCP_Release(DHCP_CLIENT_T * pdhcp_client_in);
//!0--success
//!-1 -- failed
int DHCP_AddLease(int iRawsocket, DHCP_CLIENT_T * pdhcp_client_in);
extern int DhcpClient(int iRawsocket, DHCP_CLIENT_T * pdhcp_client_in);

//static char dhcp_hostname[32];

#ifdef __cplusplus
};
#endif

#endif
