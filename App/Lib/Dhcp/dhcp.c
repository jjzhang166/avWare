
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <unistd.h>
#include <fcntl.h>
#include <features.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <time.h>

#if __GLIBC__ >=2 && __GLIBC_MINOR >= 1
dsdas
#include <netpacket/packet.h>
#include <net/ethernet.h>
#else
//#include <asm/types.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#endif

#include "dhcp.h"

typedef unsigned char u_char;
typedef unsigned int u_int;

#define dh_trace printf
#define trace printf
#define tracepoint() 			do {dh_trace("dhcp tracepoint: %s,%d\n",__FILE__,__LINE__); } while(0)

#define PRINTF(format, args...) do {printf(format, ##args);} while(0)
#define DEBUG_PRINTF(format, args...) PRINTF("DEBUG :" format, ##args);


int AVDhcpIp2Str(const unsigned long iIp, char *pStr)
{
	unsigned char i;
	unsigned char cIp[4];

	for (i=0; i<4; i++)
	cIp[i] = (unsigned char)( (iIp>>(i*8)) & (0xff) );

	sprintf(pStr, "%d.%d.%d.%d", cIp[0], cIp[1], cIp[2], cIp[3]);
	return 0;
}

int AVDhcpDNS2Str(const unsigned long long ip, char *pStr1, char *pStr2)
{
	unsigned char i;
	unsigned char cIp[8];

	for (i=0; i<8; i++)
		cIp[i] = (unsigned char)( (ip>>(i*8)) & (0xff) );

	sprintf(pStr1, "%d.%d.%d.%d", cIp[0], cIp[1], cIp[2], cIp[3]);
	sprintf(pStr2, "%d.%d.%d.%d", cIp[4], cIp[5], cIp[6], cIp[7]);
	return 0;
}

u_char AVDhcpGetOption(u_char *src,u_char opt, void *ptr, u_char size)
{
    u_char *cp;
    u_char i;
	
    if (NULL == ptr  
		|| NULL == src)
    {
		return -1;
    }
	
    cp = src;
    for (;;) 
    {
        if (*cp == DHCP_PAD)
		{
            cp++;
            continue;
        }
		
        if (*cp == DHCP_END)
        {
            break;
        }
		
        if (*cp == opt) 
		{
            if (*(cp + 1) < size)
            {
                size = *(cp + 1);
            }
			
            for (i = 0; i < size; i++)
            {
                *(((u_char *) ptr) + i) = *(cp + i + 2);
            }
			
            return *(cp + 1);
        }
        cp++;
        cp += *cp;
        cp++;
    }
    return 0;
}

u_char *AVDhcpSetOption(u_char * dst, u_char opt, u_char *src, u_char size)
{
	if (NULL == dst  
		|| NULL == src)
	{
		return NULL;
	}
	
    *dst++ = opt;
    *dst++ = size;
    while (size--)
    {
        //*dst++ = *((u_char *) src)++;
		*dst++ = *((u_char *)src);
		(u_char *)src++;
    }
	
    *dst = DHCP_END;

    return dst;
}

int UdpOutput(struct dhcpMessage *pDhcppacket, 
			   u_int32_t source_ip, 
			   int source_port,
			   u_int32_t dest_ip, 
			   int dest_port, 
			   unsigned char *szDestMacAddr, 
			   int ifindex)
{
	if (NULL == pDhcppacket  
		|| NULL == szDestMacAddr)
	{
		return -1;
	}
	
	int fd;
	int result;
	struct sockaddr_ll dest;
	struct udp_dhcp_packet packet;

	memset(&dest, 0, sizeof(dest));
	memset(&packet, 0, sizeof(packet));

	dest.sll_family = AF_PACKET;
	dest.sll_protocol = htons(ETH_P_IP);
	dest.sll_ifindex = ifindex;
	dest.sll_halen = 6;
	memcpy(dest.sll_addr, szDestMacAddr, 6);

	if ((fd = socket(PF_PACKET, SOCK_DGRAM, htons(ETH_P_IP))) < 0) 
	{
	    return -1;
	}

	if (bind(fd, (struct sockaddr *)&dest, sizeof(struct sockaddr_ll)) < 0) 
	{
#ifdef __linux__	    
				close(fd);
#else		
				closesocket(fd);
#endif

	    return -1;
	}

	packet.ip.protocol = IPPROTO_UDP;
	packet.ip.saddr = source_ip;
	packet.ip.daddr = dest_ip;

	packet.udp.source = htons(source_port);
	packet.udp.dest = htons(dest_port);
	packet.udp.len = htons(sizeof(packet.udp) + sizeof(struct dhcpMessage)); 

	packet.ip.tot_len = packet.udp.len;
	memcpy(&(packet.data), pDhcppacket, sizeof(struct dhcpMessage));
	packet.udp.check = checksum(&packet, sizeof(struct udp_dhcp_packet));

	packet.ip.tot_len = htons(sizeof(struct udp_dhcp_packet));
	packet.ip.ihl = sizeof(packet.ip) >> 2;
	packet.ip.version = IPVERSION;
	packet.ip.ttl = IPDEFTTL;
	packet.ip.check = checksum(&(packet.ip), sizeof(packet.ip));

	result = sendto(fd, &packet, sizeof(struct udp_dhcp_packet), 0, (struct sockaddr *) &dest, sizeof(dest));
	if (result <= 0) 
	{
		printf("DHCP:send raw packet, failed!\n");
	}

#ifdef __linux__	    
		close(fd);
#else		
		closesocket(fd);
#endif

	return result;
}

int NetIfInfo(char *ifname, 
                    int *ifindex, 
                    u_int32_t *ipaddr,
                    unsigned char *szMac,
                    unsigned char *clinetID)
{
    int sockfd;
    struct ifreq ifr;
    struct sockaddr_in *our_ip;
    unsigned char *tmp = clinetID;	
    int iret = -1;

    memset(&ifr, 0, sizeof(struct ifreq));
    sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
	
    if(sockfd < 0)
    {
        return -1;
    }
	
    ifr.ifr_addr.sa_family = AF_INET;
    strcpy(ifr.ifr_name, ifname);

    if (NULL == ipaddr
	||NULL == ifindex
	||NULL == szMac
	||NULL == clinetID) 
    { 
    	return -1;
    }
	
    iret = ioctl(sockfd, SIOCGIFADDR, &ifr) ;
    if (iret !=0)
    {
#ifdef __linux__	    
			close(sockfd);
#else		
			closesocket(sockfd);
#endif

	return -1;
    }

    our_ip = (struct sockaddr_in *) &ifr.ifr_addr;
    *ipaddr = our_ip->sin_addr.s_addr;

     iret = ioctl(sockfd, SIOCGIFINDEX, &ifr) ;
    if (iret !=0)
   {
#ifdef __linux__	    
			close(sockfd);
#else		
			closesocket(sockfd);
#endif

	return -1;
    }
		
    *ifindex = ifr.ifr_ifindex;
	
    iret = ioctl(sockfd, SIOCGIFHWADDR, &ifr) ;
    if (iret !=0)
    {
#ifdef __linux__	    
			close(sockfd);
#else		
			closesocket(sockfd);
#endif

	return -1;
    }
    memcpy(szMac, ifr.ifr_hwaddr.sa_data, 6);

    clinetID[0] = 1;
    memcpy(tmp + 1, szMac, 6);
	
#ifdef __linux__	    
	close(sockfd);
#else		
	closesocket(sockfd);
#endif
    return 0;
}
extern int UdpRawSocket(int ifindex)
{
    int sockfd;
    struct sockaddr_ll sock; 

    sockfd = socket(PF_PACKET, SOCK_DGRAM, htons(ETH_P_IP));
    if (sockfd < 0) 
    {
        return -1;
    }

    sock.sll_family = AF_PACKET;
    sock.sll_protocol = htons(ETH_P_IP); 
    sock.sll_ifindex = ifindex;

    if (bind(sockfd, (struct sockaddr *) &sock, sizeof(sock)) < 0)
    {
#ifdef __linux__	    
	close(sockfd);
#else		
	closesocket(sockfd);
#endif
        return -1;
    }
    return sockfd;
}
u_int16_t checksum(void *addr, int count)
{
    if (NULL == addr)
    {
		return -1;
    }
	
    int32_t sum = 0;
   u_int16_t tmp = 0;
   
    u_int16_t *source = (u_int16_t *) addr;

    while (count > 1)  
    {
        sum += *source++;
        count -= 2;
    }

    if (count > 0) 
    {
         *(unsigned char *) (&tmp) = * (unsigned char *) source;
        sum += tmp;
    }

    while (sum >> 16)
    sum = (sum & 0xffff) + (sum >> 16);

    return ~sum;
}


unsigned long random_xid(unsigned char *szMac)
{
    time_t current = 0; 
    unsigned short result1 = 0;  
    unsigned short result2 = 0;  
    unsigned char tmp[6]={""};  
	
    if (NULL == szMac)
    {
		return -1;
    }
	
    memcpy(tmp,szMac,6); 
    
    time(&current);  
    
    result1 = checksum(tmp, 6);  
    result2 = checksum((unsigned short *)(&current), 2);  
    
    return((result1 << 16) + result2);

}

int UdpInput( int sockfd,struct dhcpMessage *pRecvPacket)
{
    int bytes;
    struct udp_dhcp_packet packet;
    u_int32_t source, dest;
    u_int16_t check;

	if (NULL == pRecvPacket )
	{
		return -1;
    }
	
    memset(&packet, 0, sizeof(struct udp_dhcp_packet));

	bytes = read(sockfd, &packet, sizeof(struct udp_dhcp_packet));
	if (bytes < 0) 
	{
		printf("couldn't read on raw listening socket -- ignoring, sleep 500 MSEL\n");
		usleep(500000);
		return -1;
	}	

    if (bytes < (int) (sizeof(struct iphdr) + sizeof(struct udphdr)))
    {
        return -2;
    }

    if (bytes < ntohs(packet.ip.tot_len))
    {
        return -2;
    }

    bytes = ntohs(packet.ip.tot_len);

    if (packet.ip.protocol != IPPROTO_UDP 
		|| packet.ip.version != IPVERSION 
		|| packet.ip.ihl != sizeof(packet.ip) >> 2 
		|| packet.udp.dest != htons(CLIENT_PORT) 
		|| bytes > (int) sizeof(struct udp_dhcp_packet)
		|| ntohs(packet.udp.len) != (short) (bytes - sizeof(packet.ip))) 
    {
        return -2;
    }

    check = packet.ip.check;
    packet.ip.check = 0;
    if (check != checksum(&(packet.ip), sizeof(packet.ip)))
    {
        return -1;
    }

    source = packet.ip.saddr;
    dest = packet.ip.daddr;
    check = packet.udp.check;
    packet.udp.check = 0;
    memset(&packet.ip, 0, sizeof(packet.ip));

    packet.ip.protocol = IPPROTO_UDP;
    packet.ip.saddr = source;
    packet.ip.daddr = dest;
    packet.ip.tot_len = packet.udp.len; 
    if (check && check != checksum(&packet, bytes)) 
    {
        return -2;
    }

    memcpy(pRecvPacket, &(packet.data), bytes - (sizeof(packet.ip) + sizeof(packet.udp)));

    return bytes - (sizeof(packet.ip) + sizeof(packet.udp));

}

int AVDHCP_Discover(DHCP_CLIENT_T * pdhcp_client_in)
{
	struct dhcpMessage *bp;

	u_char iMessageType;

	u_char *cp;
	
	if (NULL == pdhcp_client_in)
	{
		return -1;
	}
	
	bp = &(pdhcp_client_in->dhcpsendpacket);
	bp->op = BOOTREQUEST;
	bp->xid = random_xid(pdhcp_client_in->dhcp_session_ctx.LocalMacAddr);
	
	bp->htype = ETH_10MB;
	bp->hlen = ETH_10MB_LEN;
	memcpy(bp->chaddr, pdhcp_client_in->dhcp_session_ctx.LocalMacAddr, 6);

	char paramreq[6];

	paramreq[0] = 0x01;/*subnet*/
	paramreq[1] = 0x03;/*gateway*/
	paramreq[2] = 0x06;/*dns*/
	paramreq[3] = 0x1c;/*brdcast*/
	paramreq[4] = 0x0f;/*domain*/
	paramreq[5] = 0x0c;/*hostname*/	

	bp->cookie= htonl(DHCP_MAGIC);
	iMessageType = DHCPDISCOVER;
	cp = AVDhcpSetOption(bp->options, DHCP_MESSAGE_TYPE, &iMessageType, 1);
	cp = AVDhcpSetOption(cp, DHCP_CLIENT_ID, (u_char *)pdhcp_client_in->dhcp_session_ctx.ClientId, 7);	

	cp = AVDhcpSetOption(cp, DHCP_REQUESTED_IP, (u_char *)MAC_BCAST_ADDR, 4);

	cp = AVDhcpSetOption(cp, DHCP_PARAM_REQ, (u_char *)paramreq, 6);	

	cp = AVDhcpSetOption( cp, DHCP_HOST_NAME, (u_char *)pdhcp_client_in->dhcp_host_name, strlen(pdhcp_client_in->dhcp_host_name) );	
	
	int iret = -1;
	iret = UdpOutput(&(pdhcp_client_in->dhcpsendpacket), 
				    INADDR_ANY, 
				    CLIENT_PORT, 
				    INADDR_BROADCAST,
				    SERVER_PORT, 
				    MAC_BCAST_ADDR, 
				    pdhcp_client_in->dhcp_session_ctx.IfIndex);

	if (iret <=0)
	{
	    return -1;
	}

	return iret;
}

int AVDHCP_Release(DHCP_CLIENT_T * pdhcp_client_in)
{
	return -1;
}

int AVDHCP_Request(u_long serverid, DHCP_CLIENT_T * pdhcp_client_in)
{
	struct dhcpMessage *bp;

	u_char iMessageType;

	u_char *cp;
	
	if ( NULL == pdhcp_client_in)
	{
		return -1;
	}
	
	bp = &(pdhcp_client_in->dhcpsendpacket);

	char paramreq[6];

	paramreq[0] = 0x01;/*subnet*/
	paramreq[1] = 0x03;/*gateway*/
	paramreq[2] = 0x06;/*dns*/
	paramreq[3] = 0x1c;/*brdcast*/
	paramreq[4] = 0x0f;/*domain*/
	paramreq[5] = 0x0c;/*hostname*/	

	bp->cookie= htonl(DHCP_MAGIC);

	iMessageType = DHCPREQUEST;
	cp = AVDhcpSetOption(bp->options, DHCP_MESSAGE_TYPE, &iMessageType, 1);

	cp = AVDhcpSetOption(cp, DHCP_CLIENT_ID, (u_char *)pdhcp_client_in->dhcp_session_ctx.ClientId, 7);	

	cp = AVDhcpSetOption(cp, DHCP_REQUESTED_IP, (u_char *)&(pdhcp_client_in->dhcprecvpacket.yiaddr), 4);/*±ÿ–Î*/
	cp = AVDhcpSetOption(cp, DHCP_SERVER_ID, (u_char *)&serverid, 4); 
	cp = AVDhcpSetOption(cp, DHCP_PARAM_REQ, (u_char *)paramreq, 6);	
	cp = AVDhcpSetOption( cp, DHCP_HOST_NAME, (u_char *)pdhcp_client_in->dhcp_host_name, strlen(pdhcp_client_in->dhcp_host_name) );	

	int iret = -1;
	iret = UdpOutput(&(pdhcp_client_in->dhcpsendpacket), 
		    INADDR_ANY, 
		    CLIENT_PORT, 
		    INADDR_BROADCAST,
		    SERVER_PORT, 
		    MAC_BCAST_ADDR, 
		    pdhcp_client_in->dhcp_session_ctx.IfIndex);

	if (iret <=0)
	{
		return -1;
	}

	return iret;	
}

int AVDHCP_Receive( int iRawsocket, u_char xtype, DHCP_CLIENT_T * pdhcp_client_in, int time_out_in )
{
     int rlen = -1;
     int type = 1;
	int nRet;

	struct timeval timeout;
	fd_set readset;

	time_t tmp_time_now_old = time(NULL);
	time_t time_exceed = tmp_time_now_old+time_out_in;
	while(1)
	{
		FD_ZERO(&readset);
		FD_SET(iRawsocket, &readset);

		time_t time_now;
		time_now = time(NULL);
		if(time_now < tmp_time_now_old )
		{
			printf("dhcp check system time change, dhcp failed!\n");
			return -1;
		}
		else
		{
			tmp_time_now_old = time_now ;
		}

		if( tmp_time_now_old >= time_exceed )
		{
			return -2;
		}
		timeout.tv_sec = time_exceed-tmp_time_now_old;
		timeout.tv_usec = 0;

		nRet = select(iRawsocket + 1, &readset, NULL, NULL, &timeout);
		if( nRet > 0 )
		{
			rlen = UdpInput(iRawsocket, &(pdhcp_client_in->dhcprecvpacket));
			if (rlen<= 0) 
			{
			    continue;
			}

			if (pdhcp_client_in->dhcpsendpacket.xid ==pdhcp_client_in->dhcprecvpacket.xid 
			&& AVDhcpGetOption(pdhcp_client_in->dhcprecvpacket.options,DHCP_MESSAGE_TYPE, &type, 1) == 1 
			&&  type == xtype) 
			{
				return 0;
			}
		}
		else
		{
			return -2;
		}
	}

}

extern int AVDhcp_InitSessionCtx(DHCP_SESSION_CTX * pclientctx, char * eth_name)
{
	int iret = NetIfInfo( eth_name, &(pclientctx->IfIndex),(u_int32_t *)&(pclientctx->LocalIpAddr), pclientctx->LocalMacAddr,pclientctx->ClientId); 	

	return iret;
}

int Dhcp_BackInfo(DHCP_CLIENT_T * pdhcp_client_in)
{
	int ret = 0;
	unsigned long tmpdata = 0;
	
	if (NULL == pdhcp_client_in )
	{
		return -1;
	}
	
	pdhcp_client_in->dhcp_proto_config.my_ip = pdhcp_client_in->dhcprecvpacket.yiaddr;
	pdhcp_client_in->dhcp_proto_config.server_ip = pdhcp_client_in->dhcprecvpacket.siaddr;

	ret = AVDhcpGetOption(pdhcp_client_in->dhcprecvpacket.options,DHCP_SUBNET, &(pdhcp_client_in->dhcp_proto_config.my_netmask), 4);
	if(ret <= 0)
	{
		trace("dhcp>: DhcpGetOption netmask failed!\n");
	}
	ret = AVDhcpGetOption(pdhcp_client_in->dhcprecvpacket.options,DHCP_ROUTER, &(pdhcp_client_in->dhcp_proto_config.my_gateway), 4);
	if(ret <= 0)
	{
		trace("dhcp>: DhcpGetOption DHCP_ROUTER failed!\n");
	}
	ret = AVDhcpGetOption(pdhcp_client_in->dhcprecvpacket.options,DHCP_LEASE_TIME, &tmpdata, 4);
	if(ret <= 0)
	{
		trace("dhcp>: DhcpGetOption DHCP_LEASE_TIME failed!\n");
	}

	ret = AVDhcpGetOption(pdhcp_client_in->dhcprecvpacket.options,DHCP_DNS_SERVER, &(pdhcp_client_in->dhcp_proto_config.server_DNS), 8); //Ω‚ŒˆDNS
	if(ret <= 0)
	{
		trace("dhcp>: DhcpGetOption DHCP_DNS_SERVER failed!\n");
	}
	
	pdhcp_client_in->dhcp_proto_config.lease = ntohl(tmpdata);
	
	ret = AVDhcpGetOption(pdhcp_client_in->dhcprecvpacket.options,DHCP_T1, &tmpdata, 4);
	pdhcp_client_in->dhcp_proto_config.t1 = ntohl(tmpdata);

	ret = AVDhcpGetOption(pdhcp_client_in->dhcprecvpacket.options,DHCP_T2, &tmpdata, 4);
	pdhcp_client_in->dhcp_proto_config.t2 = ntohl(tmpdata);
	
	return 0;
}

extern int AVDhcpClient(int iRawsocket, DHCP_CLIENT_T * pdhcp_client_in)
{
	int DHCPRetryTimer = 0;
	u_long serverid =0;
	
	if (NULL == pdhcp_client_in )
	{
		return -1;
	}
	
	memset(&(pdhcp_client_in->dhcpsendpacket),0,sizeof(struct dhcpMessage));
	memset(&(pdhcp_client_in->dhcprecvpacket),0,sizeof(struct dhcpMessage));
	
	AVDHCP_Discover(pdhcp_client_in);
	
	while (1) 
	{
		if (0==AVDHCP_Receive(iRawsocket,DHCPOFFER, pdhcp_client_in, 5)) 
		{
			AVDhcpGetOption(pdhcp_client_in->dhcprecvpacket.options,DHCP_SERVER_ID, &serverid, 4);
			pdhcp_client_in->dhcp_proto_config.serverid = serverid;
			DHCPRetryTimer=0;
			AVDHCP_Request(serverid, pdhcp_client_in);
			while (1) 
			{
				if (0==AVDHCP_Receive(iRawsocket,DHCPACK, pdhcp_client_in, 5)) 
				{ 
					AVDhcp_BackInfo(pdhcp_client_in);
					return 0;
				}

				DHCPRetryTimer++;
				
				if (DHCPRetryTimer>5)
				{
					DHCPRetryTimer=20;
					break;
				}
			}

			tracepoint();
			break;
		}
		else 
		{
			if (DHCPRetryTimer>5)
			{
				tracepoint();
				sleep(3);
				DHCPRetryTimer=0;
				memset(&(pdhcp_client_in->dhcpsendpacket),0,sizeof(struct dhcpMessage));
				memset(&(pdhcp_client_in->dhcprecvpacket),0,sizeof(struct dhcpMessage));				
				AVDHCP_Discover(pdhcp_client_in);
			}
			else
			{
				DHCPRetryTimer++;
			}
		}
	}

	return -1;
	
}

int AVDHCP_AddLease(int iRawsocket, DHCP_CLIENT_T * pdhcp_client_in)
{
	AVDHCP_Request(pdhcp_client_in->dhcp_proto_config.serverid, pdhcp_client_in);

	if (0==AVDHCP_Receive(iRawsocket,DHCPACK, pdhcp_client_in, 5)) 
	{ 
		AVDhcp_BackInfo(pdhcp_client_in);
		return 0;
	}
	return -1;
}
