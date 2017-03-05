/*******************************************************************
|  Copyright(c) 2015-2016 Graceport Technology Development Co.,Ltd
|  All rights reserved.
|
|  版本: 1.0
|  作者: FootMan [FootMan@graceport.cn]
|  日期: 2016年6月6日
|  说明:
|
|  版本: 1.1
|  作者:
|  日期:
|  说明:
******************************************************************/
#ifndef __NETWORK_H__
#define __NETWORK_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#include <sys/types.h>

#include <sys/stat.h>
#include "pthread.h"
#include "common.h"
#include "list.h"

#define DF_NET_MSGBUFLEN 20*1024	


typedef struct _NetCliInfo_S{
	char 	fromIP[16];
	int		fromPort;
	int		recvSocket;
	int 	recvLen;
	char	recvmsg[DF_NET_MSGBUFLEN];
	void	*usrdef;
}NetCliInfo_S;

typedef int (*NetTcpSerReadCbFun)(NetCliInfo_S *cli);
typedef int (*NetTcpSerCheckEndCbFun)(NetCliInfo_S *cli);

typedef struct _NetWorkSerHandle_S{
	char				mulicastip[16];//组播地址
	int					mulicastPort;
	int 				serverport;
	int					sockfd;
	NetTcpSerReadCbFun 	readcb;
	NetTcpSerCheckEndCbFun checkendcb;
}NetWorkSerHandle_S;


int NetCreateTcpSer(NetWorkSerHandle_S *handle);
int NetCloseSerSocket(NetWorkSerHandle_S *handle);
int NetTcpSendMsg(int sockfd,char *msg,int len);
int NetCreateUdpMulicast(NetWorkSerHandle_S *handle);
int NetUdpSendMsg(char *ip, int port, char *msg, int len);

#endif

