#include "Ddns_Global.h"
#include "Ddns_Net.h"
#include "Base64.h"
#include "NetDdns.h"

#define FORMAT_DYNDNS 		"GET /nic/update?hostname=%s&myip=&wildcard=NOCHG&mx=NOCHG&backmx=NOCHG HTTP/1.0\r\n" \
							"Host: members.dyndns.org\r\n" \
							"Authorization: Basic %s\r\n" \
							"User-Agent:inadyn/1.96.2 inarcis2002@hotpop.com\r\n\r\n"  

#define FORMAT_3322         "GET /dyndns/update?system=dyndns&hostname=%s HTTP/1.0\r\n"\
							"Host: www.3322.org\r\n"\
							"Authorization: Basic %s\r\n\r\n"

#define FORMAT_DNSDYNAMIC   "GET /nic/update?system=dyndns&hostname=%s HTTP/1.0\r\n" \
							"Host: www.dnsdynamic.org\r\n" \
				  			"Authorization: Basic %s\r\n" \
				  			"User-Agent: ddclient/3.8.0-r127\r\n" \
				  			"Connection: close\r\n\r\n"
static char strOutIP[16] = {0};

static int DDNS_Active(ENU_DDNS nType, char *pDoamain,char *pName, char *pPwd)
{
	int                 nRet           =  0;
	int                 nEncLen        =  0;
	int 				nSock		   = -1;
	char				Buffer[1024]   = {0};
	char				UserInfo[128]  = {0};
	char				EncInfo[128]   = {0};
	char                ServerName[64] = {0};
	//char				*pServerIp	   = NULL;
	char 				pServerIp[16]  = {0};
	struct hostent		*pHos		   ;
	struct sockaddr_in	SockAddr       ;
	
	//msgbuf 
	snprintf(UserInfo,sizeof(UserInfo),"%s:%s", pName, pPwd);
	DDNS_DEBUG("=====ddns=========0========\n");
	#if 0
	if(-1 == ddns_base64_encode((unsigned char *)EncInfo, (unsigned char *)UserInfo, sizeof(EncInfo)))
	{
		DDNS_DEBUG("BASE64 FAILED");
		return -1;
	}
	#endif
	if(-1 == base64_encode((unsigned char *)UserInfo, strlen(UserInfo),(unsigned char *)EncInfo, &nEncLen))
	{
		DDNS_DEBUG("BASE64 FAILED");
		return -1;
	}
	DDNS_DEBUG("=====ddns=========00========\n");

	if(DDNS_3322 == nType)
	{
		sprintf(Buffer,FORMAT_3322,pDoamain,EncInfo);
		snprintf(ServerName, sizeof(ServerName), "%s", "www.3322.org");
	}
	else if(DDNS_DYNDNS == nType)
	{
		sprintf(Buffer,FORMAT_DYNDNS,pDoamain,EncInfo);
		snprintf(ServerName, sizeof(ServerName), "%s", "www.dyndns.org");
	}
	else if(DDNS_DNSDYNMAIC == nType)
	{
		sprintf(Buffer,FORMAT_DNSDYNAMIC,pDoamain,EncInfo);
		snprintf(ServerName, sizeof(ServerName), "%s", "www.dnsdynamic.org");
	}
	else
	{
		return -1;
	}
	//create socket
	if((nSock = socket(AF_INET, SOCK_STREAM, 0))<0)
	{
		return -1;
	}
	DDNS_DEBUG("=====ddns=========1========\n");

	pHos   = gethostbyname(ServerName);
	if(NULL != pHos)
	{
		//pServerIp=(char *)inet_ntoa(*(struct in_addr*)pHos->h_addr_list[0]);
		memcpy(pServerIp, inet_ntoa(*(struct in_addr*)pHos->h_addr_list[0]), 16);
	}
	else
	{
		goto  DDNS_END;
	}
	memset(&SockAddr, 0, sizeof(SockAddr));
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_port = htons(80);
	SockAddr.sin_addr.s_addr = inet_addr(pServerIp);

	DDNS_DEBUG("=====ddns=========2========\n");
	if(0 > connect(nSock, (struct sockaddr*)&SockAddr, sizeof(SockAddr)))
	{
		SockAddr.sin_port = htons(8080);
		if(0 > connect(nSock, (struct sockaddr*)&SockAddr, sizeof(SockAddr)))
		{
			goto  DDNS_END;
		}
	}
	//send recv
	send(nSock,Buffer,strlen(Buffer),0);
	//DDNS_DEBUG("[COMM]: send DDNS URL=%s\n",Buffer);
	memset(Buffer,0,sizeof(Buffer));
	recv(nSock,Buffer,sizeof(Buffer),0);
	DDNS_DEBUG("recv [%s]\n",Buffer);
	if((strstr(Buffer,"good")!=NULL)||(strstr(Buffer,"nochg")!=NULL))
	{
		nRet = 0;
	}
	else
	{
		nRet = -1;
	}
	
DDNS_END:
#ifdef __linux__
	close(nSock);
#else
	closesocket(nSock);
#endif
	
	return nRet;
}

///////////////////app/////////////////////
int DDNS_Enable(ENU_DDNS nType, char *pDoamain,char *pName, char *pPwd)
{
	char OutIp[16] = {0};
	int ret = 0;
	if(NULL == pDoamain || NULL == pName || NULL == pPwd)
	{
		return -1;
	}
	
	if(-1== Ddns_GetOutIP(OutIp))
	{
		return -1;
	}
	
	//DDNS_DEBUG("***OutIp=%s, saveOutIP=%s\n\n",OutIp,strOutIP);
	//if(0 == strlen(strOutIP)||0 != strncmp(OutIp,strOutIP,15))
	//{
		//DDNS_DEBUG("***OutIp=%s\n\n",OutIp);
		ret = DDNS_Active(nType, pDoamain,pName, pPwd);
		
		if(0 == ret)
		{
			DDNS_DEBUG("ddns_active ok");
			strncpy(strOutIP,OutIp,15);
			return 0;
		}
		return -1;
	//}
	//return 1;
}


