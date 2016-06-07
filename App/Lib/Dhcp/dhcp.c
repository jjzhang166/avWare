
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
#define tracepoint() 			do {dh_trace("tracepoint: %s,%d\n",__FILE__,__LINE__); } while(0)

#define PRINTF(format, args...) do {printf(format, ##args);} while(0)
#define DEBUG_PRINTF(format, args...) PRINTF("DEBUG :" format, ##args);


int DhcpIp2Str(const unsigned long iIp, char *pStr)
{
	unsigned char i;
	unsigned char cIp[4];

	for (i=0; i<4; i++)
	cIp[i] = (unsigned char)( (iIp>>(i*8)) & (0xff) );

	sprintf(pStr, "%d.%d.%d.%d", cIp[0], cIp[1], cIp[2], cIp[3]);
	return 0;
}

int DhcpDNS2Str(const unsigned long long ip, char *pStr1, char *pStr2)
{
	unsigned char i;
	unsigned char cIp[8];

	for (i=0; i<8; i++)
		cIp[i] = (unsigned char)( (ip>>(i*8)) & (0xff) );

	sprintf(pStr1, "%d.%d.%d.%d", cIp[0], cIp[1], cIp[2], cIp[3]);
	sprintf(pStr2, "%d.%d.%d.%d", cIp[4], cIp[5], cIp[6], cIp[7]);
	return 0;
}

void PRINTPACKET(struct dhcpMessage packet)
{
    int iIndex= 0;
    char szDebug[64] = {0};
    char szIptmp[16]={0};
	
    DEBUG_PRINTF("DHCP Message------------------------\n");
    sprintf(szDebug, "op     :%d\n",packet.op);
    DEBUG_PRINTF("%s\n",szDebug);    
    sprintf(szDebug, "htype  :%d\n",packet.htype);
    DEBUG_PRINTF("%s\n",szDebug);      
    sprintf(szDebug, "hlen   :%d\n",packet.hlen);
    DEBUG_PRINTF("%s\n",szDebug);      
    sprintf(szDebug, "hops   :%d\n",packet.hops);
    DEBUG_PRINTF("%s\n",szDebug);      
    sprintf(szDebug, "xid    :%x\n", ntohl(packet.xid));
    DEBUG_PRINTF("%s\n",szDebug);      
    sprintf(szDebug, "secs   :%d\n", ntohs(packet.secs));
    DEBUG_PRINTF("%s\n",szDebug);      
    sprintf(szDebug, "flags  :%d\n", ntohs(packet.flags));
    DEBUG_PRINTF("%s\n",szDebug);   
	
	DhcpIp2Str(packet.ciaddr, szIptmp);
    sprintf(szDebug, "ciaddr :%s\n",szIptmp);
    DEBUG_PRINTF("%s\n",szDebug);      

	DhcpIp2Str(packet.yiaddr, szIptmp);
    sprintf(szDebug, "yiaddr :%s\n",szIptmp);
    DEBUG_PRINTF("%s\n",szDebug);      

	DhcpIp2Str(packet.siaddr, szIptmp);
    sprintf(szDebug, "siaddr :%s\n",szIptmp);	
    DEBUG_PRINTF("%s\n",szDebug);   

	DhcpIp2Str(packet.giaddr, szIptmp);
    sprintf(szDebug, "giaddr :%s\n",szIptmp);		
    DEBUG_PRINTF("%s\n",szDebug);      
    
    DEBUG_PRINTF("chaddr");
    for(iIndex =0; iIndex<15; iIndex ++)
    {
        sprintf(szDebug, ":%d",packet.chaddr[iIndex]);
        DEBUG_PRINTF("%s\n",szDebug);
    }
    DEBUG_PRINTF("\n");

    sprintf(szDebug, "cookie :%x\n",(packet.cookie));
    DEBUG_PRINTF("%s\n",szDebug);
	
    for(iIndex =0; iIndex<3;iIndex ++)
    {
        sprintf(szDebug, "options :%d\n",packet.options[iIndex]);
        DEBUG_PRINTF("%s\n",szDebug);
    }
	//printf("options = %d\n",sizeof(u_int8_t));
}

u_char DhcpGetOption(u_char *src,u_char opt, void *ptr, u_char size)
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

u_char *DhcpSetOption(u_char * dst, u_char opt, u_char *src, u_char size)
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
/********************************************************************/
/* Constuct a ip/udp header for a packet, 
and specify the source and dest hardware address */
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
	
	/*local variables*/
	int fd;
	int result;
	struct sockaddr_ll dest;
	struct udp_dhcp_packet packet;

	/*initialize local variables*/
	memset(&dest, 0, sizeof(dest));
	memset(&packet, 0, sizeof(packet));

	dest.sll_family = AF_PACKET;
	dest.sll_protocol = htons(ETH_P_IP);
	dest.sll_ifindex = ifindex;
	dest.sll_halen = 6;
	memcpy(dest.sll_addr, szDestMacAddr, 6);

	/*create packet socket*/
	if ((fd = socket(PF_PACKET, SOCK_DGRAM, htons(ETH_P_IP))) < 0) 
	{
	    return -1;
	}

	/*bind the socket*/
	if (bind(fd, (struct sockaddr *)&dest, sizeof(struct sockaddr_ll)) < 0) 
	{
#ifdef __linux__	    
				close(fd);
#else		
				closesocket(fd);
#endif

	    return -1;
	}

	/*fill in  each fields*/
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

	/*send this raw packet*/
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

	//printf("DHCP:send raw packet--------------------\n");
    	//PRINTPACKET(*pDhcppacket);	

	return result;
}
/*
读取网络接口配置信息
addr;arp hwaddr；ifindex 输出,
interface, eth0输入
*/
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

    clinetID[0] = 1;/*type -eth*/
    memcpy(tmp + 1, szMac, 6);/*data*/
	
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


/* 创建随机数*/
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

/* return -1 on errors that are fatal for the socket, -2 for those that aren't */
int UdpInput( int sockfd,struct dhcpMessage *pRecvPacket)
{
    int bytes;
    struct udp_dhcp_packet packet;
    u_int32_t source, dest;
    u_int16_t check;

	//struct timeval timeout;
	//fd_set readset;
	//int nRet;

	if (NULL == pRecvPacket )
	{
		return -1;
    }
	
    memset(&packet, 0, sizeof(struct udp_dhcp_packet));

	//FD_ZERO(&readset);
	//FD_SET(sockfd, &readset);

	//timeout.tv_sec = 1;
	//timeout.tv_usec = 0;

	//nRet = select(sockfd + 1, &readset, NULL, NULL, &timeout);
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

    /* ignore any extra garbage bytes */
    bytes = ntohs(packet.ip.tot_len);

    /* Make sure its the right packet for us, and that it passes sanity checks */
    if (packet.ip.protocol != IPPROTO_UDP 
		|| packet.ip.version != IPVERSION 
		|| packet.ip.ihl != sizeof(packet.ip) >> 2 
		|| packet.udp.dest != htons(CLIENT_PORT) 
		|| bytes > (int) sizeof(struct udp_dhcp_packet)
		|| ntohs(packet.udp.len) != (short) (bytes - sizeof(packet.ip))) 
    {
        return -2;
    }

    /* check IP checksum */
    check = packet.ip.check;
    packet.ip.check = 0;
    if (check != checksum(&(packet.ip), sizeof(packet.ip)))
    {
        return -1;
    }

    /* verify the UDP checksum by replacing the header with a psuedo header */
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

    //printf("DHCP:recv raw packet--------111111------------\n");

    //PRINTPACKET(packet.data);
    
    return bytes - (sizeof(packet.ip) + sizeof(packet.udp));

}

/*
申请ip
1:DHCP客户机初始化TCP/IP，
通过UDP端口67向网络中发送一个DHCPDISCOVER广播包，
请求租用IP地址。
该 广播包中的源IP地址为0.0.0.0，
目标IP地址为255.255.255.255；
包中还包含客户机的MAC地址和计算机名。
注意:花费1秒钟的时间等待DHCP服务器的回应，
如果1秒钟没有服务器的回应，
它会将这一广播包重新广播四次（以2，4，8和16秒为间隔，
加上1~1000毫秒之间随机长度的时间）。四次之后，
如果仍未能收到服务器的回应，则DHCP客户机将无法获得IP地址。
DHCP客户机仍然每隔5分钟重新广播一次，
如果收到某个服务器的回应，则继续IP租用过程。

2:DHCP服务器，都会通过UDP端口68给客户机回应一个DHCPOFFER广播包，
提供一个IP地址。
该广播包的源IP地址为DCHP服务器IP，
目标IP地址为255.255.255.255；
包中还包含提供的IP地址、子网掩码及租期等信息。

3:客户选择IP租用报文，客户机从不止一台DHCP服务器接收到提供之后，
会选择第一个收到的DHCPOFFER包，
并向网络中广播一个 DHCPREQUEST消息包，
表明自己已经接受了一个DHCP服务器提供的IP地址。
该广播包中包含所接受的IP地址和服务器的IP地址。 
所有其他的DHCP服务器撤消它们的提供以便将IP地址提供给下一次IP租用请求。

4:DHCP服务器发出IP租用确认报文，
被客户机选择的DHCP服务器在收到DHCPREQUEST广播后，
会广播返回给客户机一个DHCPACK消息包，
表明已经接受客户机的选择，
并将这一IP地址的合法租用以及其他的配置信息都放入该广播包发给客户机。 
客户配置成功后发出的公告报文，
客户机在收到DHCPACK包，会使用该广播包中的信息来配置自己的TCP/IP，
则租用过程完成，客户机可以在网络中通信。



租约变更
（1）在当前租期已过去50%时，
DHCP客户机直接向为其提供IP地址的DHCP服务器发送DHCPREQUEST消息包。
如果客户机接收到该服务器回应的DHCPACK消息包，
客户机就根据包中所提供的新的租期以及其它已经更新的TCP/IP参数，
更新自己的配置，IP租用更新完成。如果没收到该服务器的回复，
则客户机继续使用现有的 IP地址，因为当前租期还有50%

（2）如果在租期过去50%时未能成功更新，
则客户机将在当前租期过去87.5%时再次向为其提供IP地址的DHCP联系。
如果联系不成功，则重新开始IP租用过程。

 （3）如果DHCP客户机重新启动时，
 它将尝试更新上次关机时拥有的IP租用。
 如果更新未能成功，客户机将尝试联系现有IP租用中列出的缺省网关。
 如果联系成功且租用尚未到期，客户机则认为自己仍然位于
 与它获得现有IP租用时相同的子网上（没有被移走）继续使用现有IP地址。 
 如果未能与缺省网关联系成功，客户机则认为自己已经被移到不同的子网上，
 将会开始新一轮的IP租用过程。

*/

//struct dhcpMessage dhcpsendpacket; 
//struct dhcpMessage dhcprecvpacket; 

int DHCP_Discover(DHCP_CLIENT_T * pdhcp_client_in)
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

	/*
	 * Add DHCP option for discover message.
	 */
	bp->cookie= htonl(DHCP_MAGIC);
	iMessageType = DHCPDISCOVER;
	cp = DhcpSetOption(bp->options, DHCP_MESSAGE_TYPE, &iMessageType, 1);
	cp = DhcpSetOption(cp, DHCP_CLIENT_ID, (u_char *)pdhcp_client_in->dhcp_session_ctx.ClientId, 7);	

	cp = DhcpSetOption(cp, DHCP_REQUESTED_IP, (u_char *)MAC_BCAST_ADDR, 4);/*必须*/

	cp = DhcpSetOption(cp, DHCP_PARAM_REQ, (u_char *)paramreq, 6);	

	cp = DhcpSetOption( cp, DHCP_HOST_NAME, (u_char *)pdhcp_client_in->dhcp_host_name, strlen(pdhcp_client_in->dhcp_host_name) );	
	
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

int DHCP_Release(DHCP_CLIENT_T * pdhcp_client_in)
{
	return -1;
}

int DHCP_Request(u_long serverid, DHCP_CLIENT_T * pdhcp_client_in)
{
	struct dhcpMessage *bp;

	u_char iMessageType;

	u_char *cp;
	
	if ( NULL == pdhcp_client_in)
	{
		return -1;
	}
	
	bp = &(pdhcp_client_in->dhcpsendpacket);/*重新使用discover头包括xid*/

	char paramreq[6];

	paramreq[0] = 0x01;/*subnet*/
	paramreq[1] = 0x03;/*gateway*/
	paramreq[2] = 0x06;/*dns*/
	paramreq[3] = 0x1c;/*brdcast*/
	paramreq[4] = 0x0f;/*domain*/
	paramreq[5] = 0x0c;/*hostname*/	

	/*
	* Add DHCP option for discover message.
	*/
	bp->cookie= htonl(DHCP_MAGIC);

	iMessageType = DHCPREQUEST;
	cp = DhcpSetOption(bp->options, DHCP_MESSAGE_TYPE, &iMessageType, 1);

	cp = DhcpSetOption(cp, DHCP_CLIENT_ID, (u_char *)pdhcp_client_in->dhcp_session_ctx.ClientId, 7);	

	cp = DhcpSetOption(cp, DHCP_REQUESTED_IP, (u_char *)&(pdhcp_client_in->dhcprecvpacket.yiaddr), 4);/*必须*/
	cp = DhcpSetOption(cp, DHCP_SERVER_ID, (u_char *)&serverid, 4);  /*必须*/
	cp = DhcpSetOption(cp, DHCP_PARAM_REQ, (u_char *)paramreq, 6);	
	cp = DhcpSetOption( cp, DHCP_HOST_NAME, (u_char *)pdhcp_client_in->dhcp_host_name, strlen(pdhcp_client_in->dhcp_host_name) );	

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

int DHCP_Receive( int iRawsocket, u_char xtype, DHCP_CLIENT_T * pdhcp_client_in, int time_out_in )
{
     int rlen = -1;
     int type = 1;
	int nRet;

	struct timeval timeout;
	fd_set readset;

	time_t tmp_time_now_old = time(NULL);
	time_t time_exceed = tmp_time_now_old+time_out_in;	//!计算出超时时间
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

		if( tmp_time_now_old >= time_exceed )	//!time out
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
			&& DhcpGetOption(pdhcp_client_in->dhcprecvpacket.options,DHCP_MESSAGE_TYPE, &type, 1) == 1 
			&&  type == xtype) 
			{
				//printf("type[%d] xid[%u]\n",type,dhcprecvpacket.xid);
				return 0;
			}
		}
		else
		{
			return -2;
		}
		//printf("--failed----type[%d] xid[%u]\n",type,dhcprecvpacket.xid);
	}

}

extern int Dhcp_InitSessionCtx(DHCP_SESSION_CTX * pclientctx, char * eth_name)
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

	ret = DhcpGetOption(pdhcp_client_in->dhcprecvpacket.options,DHCP_SUBNET, &(pdhcp_client_in->dhcp_proto_config.my_netmask), 4);
	if(ret <= 0)
	{
		trace("dhcp>: DhcpGetOption netmask failed!\n");
	}
	ret = DhcpGetOption(pdhcp_client_in->dhcprecvpacket.options,DHCP_ROUTER, &(pdhcp_client_in->dhcp_proto_config.my_gateway), 4);
	if(ret <= 0)
	{
		trace("dhcp>: DhcpGetOption DHCP_ROUTER failed!\n");
	}
	ret = DhcpGetOption(pdhcp_client_in->dhcprecvpacket.options,DHCP_LEASE_TIME, &tmpdata, 4);
	if(ret <= 0)
	{
		trace("dhcp>: DhcpGetOption DHCP_LEASE_TIME failed!\n");
	}

	ret = DhcpGetOption(pdhcp_client_in->dhcprecvpacket.options,DHCP_DNS_SERVER, &(pdhcp_client_in->dhcp_proto_config.server_DNS), 8); //解析DNS
	if(ret <= 0)
	{
		trace("dhcp>: DhcpGetOption DHCP_DNS_SERVER failed!\n");
	}
	
	pdhcp_client_in->dhcp_proto_config.lease = ntohl(tmpdata);
	
	ret = DhcpGetOption(pdhcp_client_in->dhcprecvpacket.options,DHCP_T1, &tmpdata, 4);
	pdhcp_client_in->dhcp_proto_config.t1 = ntohl(tmpdata);

	ret = DhcpGetOption(pdhcp_client_in->dhcprecvpacket.options,DHCP_T2, &tmpdata, 4);
	pdhcp_client_in->dhcp_proto_config.t2 = ntohl(tmpdata);	/*字节序转化*/
	
	return 0;
}

extern int DhcpClient(int iRawsocket, DHCP_CLIENT_T * pdhcp_client_in)
{
	int DHCPRetryTimer = 0;
	u_long serverid =0;
	
	if (NULL == pdhcp_client_in )
	{
		return -1;
	}
	
	memset(&(pdhcp_client_in->dhcpsendpacket),0,sizeof(struct dhcpMessage));
	memset(&(pdhcp_client_in->dhcprecvpacket),0,sizeof(struct dhcpMessage));
	
	DHCP_Discover(pdhcp_client_in);
	
	while (1) 
	{
		if (0==DHCP_Receive(iRawsocket,DHCPOFFER, pdhcp_client_in, 5)) 
		{
			DhcpGetOption(pdhcp_client_in->dhcprecvpacket.options,DHCP_SERVER_ID, &serverid, 4);
			pdhcp_client_in->dhcp_proto_config.serverid = serverid;
			DHCPRetryTimer=0;
			DHCP_Request(serverid, pdhcp_client_in);
			while (1) 
			{
				if (0==DHCP_Receive(iRawsocket,DHCPACK, pdhcp_client_in, 5)) 
				{ 
					Dhcp_BackInfo(pdhcp_client_in);
					return 0;
				}

				DHCPRetryTimer++;
				
				if (DHCPRetryTimer>5) //timeout for ACK listen
				{
					DHCPRetryTimer=20; //ready for next DHCP_Discover
					break;
				}
			}

			tracepoint();
			break;
		}
		else 
		{
			if (DHCPRetryTimer>5) //timeout for OFFER listen
			{
				tracepoint();
				sleep(3);
				DHCPRetryTimer=0;
				memset(&(pdhcp_client_in->dhcpsendpacket),0,sizeof(struct dhcpMessage));
				memset(&(pdhcp_client_in->dhcprecvpacket),0,sizeof(struct dhcpMessage));				
				DHCP_Discover(pdhcp_client_in);
			}
			else
			{
				DHCPRetryTimer++;
			}
		}
	}

	return -1;
	
}
/************************************************************************/

int DHCP_AddLease(int iRawsocket, DHCP_CLIENT_T * pdhcp_client_in)
{
	printf("dhcp:>DHCP_AddLease\n");
	DHCP_Request(pdhcp_client_in->dhcp_proto_config.serverid, pdhcp_client_in);

	if (0==DHCP_Receive(iRawsocket,DHCPACK, pdhcp_client_in, 5)) 
	{ 
		Dhcp_BackInfo(pdhcp_client_in);
		printf("DHCP_AddLease success!\n");
		return 0;
	}
	return -1;
}
#if 0/*for test*/
int main()
{
	int irawsocket = -1;
	DHCP_CLIENT_T dhcp_client_in;

	DHCP_SESSION_CTX clientctx;
	memset(&clientctx,0,sizeof(DHCP_SESSION_CTX));
	
	Dhcp_InitSessionCtx(&clientctx, "eth0");

	
	memcpy(&dhcp_client_in.dhcp_session_ctx, &clientctx, sizeof(DHCP_PROTO_CONFIG));

	irawsocket = UdpRawSocket(clientctx.IfIndex);
	if(irawsocket<0)
	{
		return -1;
	}
	
     	int iret = DhcpClient(irawsocket,&dhcp_client_in);
	if (iret !=0)
	{
		close(irawsocket);
	        return 0;
	}

    char szDebug[64]={0};
    char szIptmp[16]={0};
	char szIptmp1[16]={0};

    DhcpIp2Str(dhcp_client_in.dhcp_proto_config.my_ip, szIptmp);
    sprintf(szDebug, "my_ip:%s\n",szIptmp);
	
    DEBUG_PRINTF("%s\n",szDebug);    

    DhcpIp2Str(dhcp_client_in.dhcp_proto_config.server_ip, szIptmp);
    sprintf(szDebug, "server_ip:%s\n",szIptmp);
	
    DEBUG_PRINTF("%s\n",szDebug);    
	
    DhcpIp2Str(dhcp_client_in.dhcp_proto_config.my_netmask, szIptmp);
    sprintf(szDebug, "my_netmask:%s\n",szIptmp);
	
    DEBUG_PRINTF("%s\n",szDebug);   
	
    DhcpIp2Str(dhcp_client_in.dhcp_proto_config.my_gateway, szIptmp);
    sprintf(szDebug, "my_gateway:%s\n",szIptmp);
	
    DEBUG_PRINTF("%s\n",szDebug);   

	//DhcpIp2Str(dhcp_client_in.dhcp_proto_config.server_DNS, szIptmp);
	//sprintf(szDebug, "my_server_DNS:%s\n",szIptmp);

	DhcpDNS2Str(dhcp_client_in.dhcp_proto_config.server_DNS, szIptmp,szIptmp1);
	sprintf(szDebug, "my_server_DNS:%s\n",szIptmp);
    DEBUG_PRINTF("%s\n",szDebug);

	sprintf(szDebug, "my_server_DNS1:%s\n",szIptmp1);
    DEBUG_PRINTF("%s\n",szDebug);

    printf("Lease[%lu],T1[%lu],T2[%lu]\n",dhcp_client_in.dhcp_proto_config.lease,dhcp_client_in.dhcp_proto_config.t1,dhcp_client_in.dhcp_proto_config.t2);

	//PRINTPACKET(dhcp_client_in.dhcprecvpacket);

    close(irawsocket);
    return 0;
	

}
 #endif
