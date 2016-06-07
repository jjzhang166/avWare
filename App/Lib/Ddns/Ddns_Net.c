#include "Ddns_Global.h"
#include "Ddns_Net.h"
#include "base64.h"

char * Ddns_NetGetServerIp(char *ServerName)
{ 
	char *ip = NULL;
	struct hostent *h; 

	if(NULL == ServerName)  return NULL;
	if((h=gethostbyname(ServerName))== NULL)
	{ 
		perror("gethostbyname:");
		return NULL;
	} 
	ip =(char *)inet_ntoa(*((struct in_addr*)(h->h_addr)));
		
	return ip;
}


static int Ddns_CheckSock(int SocketFD, int Contrl, int timeoutsec)
{
	fd_set			test_set;
	struct timeval  wtimeout;
	int				Ret	 = 0;
	
	FD_ZERO(&test_set);
	FD_SET(SocketFD, &test_set);
	wtimeout.tv_sec			= timeoutsec;
	wtimeout.tv_usec 		= 100L;
	if (Contrl == 0)
	{
		Ret = select(SocketFD + 1, &test_set, NULL, NULL, &wtimeout);//read
	}
	else
	{
		Ret = select(SocketFD + 1, NULL, &test_set, NULL, &wtimeout);//write
	}
	
	if (Ret <= 0)  	return -1;
	return 0;
}

int Ddns_SendPack(int SocketFd, char *buf, int buflen, int waitsec)
{
	int		RetTS			= 1;
	int		HavedSend		= 0;
	int		SendOnceByte	= 0;
	
SEND_AGAIN:
	if(0 == (RetTS = Ddns_CheckSock(SocketFd, 1, waitsec)))
	{
		if(0 < (SendOnceByte = send(SocketFd, &buf[HavedSend], buflen - HavedSend, 0)))
		{
			HavedSend += SendOnceByte;
			if (HavedSend == buflen)
			{
				return HavedSend;
			}
			else
			{
				SendOnceByte = 0;
				goto SEND_AGAIN;
			}
		}
	}
	return FALSE;
}


int Ddns_RecvPack(int SocketFd, char *buf, int buflen, int waitsec)
{
	int 	RetTS			= 1;
	int 	HavedRecv		= 0;
	int 	RecvOnceByte	= 0;
RECV_AGAIN:
	if(0 == (RetTS = Ddns_CheckSock(SocketFd, 0, waitsec)))
	{
		if(0 < (RecvOnceByte = recv(SocketFd, &buf[HavedRecv], buflen - HavedRecv, 0)))
		{
			HavedRecv += RecvOnceByte;
			if(HavedRecv == buflen)
			{
				return HavedRecv;
			}
			else
			{
				RecvOnceByte = 0;
				goto RECV_AGAIN;
			}
		} 
	} 
	return FALSE;
}

int Ddns_GetOutIP(char *pOutIp)
{
	int    fd 	= -1;
	int    ret 	= -1;
	char   *p0 	= NULL;
	char   *p1	= NULL;
	struct sockaddr_in sin;
	struct hostent *   host; 
	char   buffer[1024] = {0};
	char   *head="GET /ic.asp HTTP/1.1\r\nHOST:iframe.ip138.com\r\nConnection: Close\r\n\r\n";
	
	if(!(host = gethostbyname("iframe.ip138.com")))
	{ 
		perror("gethostbyname");
		return -1;
	}
	
	if((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket");
		return -1;
	}
	DDNS_DEBUG("create socket ok sock=%d",fd);

	memset(&sin, 0, sizeof(struct sockaddr_in));
	sin.sin_family      = AF_INET;
	sin.sin_port        = htons (80);
	sin.sin_addr.s_addr = *((unsigned long*)host->h_addr);
	if(connect(fd, (struct sockaddr *)&sin, sizeof(sin)) < 0)
	{
		perror("connect");
		ret = -1;
		goto GETOUTIP_END;
	}
	
	if(send(fd,head,strlen(head),0) < 0)
	{
		perror("send");
		ret = -1;
		goto GETOUTIP_END;
	}
	if(recv(fd,buffer,1024,0) < 0)
	{
		perror("recv");
		ret = -1;
		goto GETOUTIP_END;
	}

	if(NULL == (p0 = strstr(buffer,"["))||
	   NULL == (p1 = strstr(p0,"]")))
	{
		ret = -1;
		goto GETOUTIP_END;
	}
	*p1 ='\0';
	strcpy(pOutIp, p0+1);
	ret = 0;
	
GETOUTIP_END:
#ifdef __linux__
	close(fd);
#else
	closesocket(fd);
#endif
	return ret;
}

