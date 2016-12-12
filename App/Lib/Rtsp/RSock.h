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
#ifndef _RSOCK_H_
#define _RSOCK_H_

#include "Apis/AvWareCplusplus.h"




class CRSock
{
public:
	typedef enum{
		eSockT_UDP,
		eSockT_TCP,
		eSockT_Multicast,
		eSockT_Broadcast,
	}eSockT;

public:
	CRSock();
	virtual ~CRSock();
	static int Rsock(eSockT type);
	static int Rbind(int sock, struct sockaddr *addr, socklen_t *addrlen);
	static int Rlisten(int sock);
	static int Raccept(int sock, struct sockaddr *addr, socklen_t *addrlen);
	static int Rsetopt(int sock, int sndBufSize = 320 * 1024, int rcvBufSize = 320 * 1024);
	static int Rconnect(int sock, struct sockaddr *addr, socklen_t *addrlen);
	static int Rclosesock(int sock);


public:
	static int CreateMediaSock(int localPort, const char *remoteAddr, const int remotePort, bool bMulticast = false);
	
private:

};


#endif