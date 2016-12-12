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
#include <cstdio>
#include <string>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>


#if defined (WIN32)
#include <WinSock2.h>
#include "ws2tcpip.h"
#include "io.h"
#else
#include <unistd.h>
#endif
#include "WebHttpTask.h"
#include "CAvObject.h"

#define DefWebServicePort 80
#define DefWebServiceRooDir "./webroot"
//#define DefWebServiceIndex  "index.html"
#define  DefWebServiceIndex "login.html"



class CWebService:public CAvObject
{
public:
	CWebService();
	virtual ~CWebService();

	bool WebStart();
	bool WebStop();


	bool WebSetPort(int port);
	bool WebSetRootDir(std::string RootDir);
	bool WebSetIndex(std::string IndexHtml);



public:
	int OpenHTTPDServer(int iHttpPort);
#ifdef _USE_IPV6	
	int OpenHTTPDServerIPv6(int iHttpPort);
#endif	
	void HttpProcLoop();
	int GetAcceptSock();
	static void* ThreadProc(void *arg);



public:
	virtual int WebCallBackDataProc(std::string InBuf, std::string &OutBuf);
	virtual int WebCallBackGetSnap(int Channel, std::string &SnapData);

private:
	std::string m_RootDir;
	std::string m_IndexHtml;
	int			m_ServicePort;
	bool		m_bEnableIpv6;

	int m_iSocket;//侦听的socket
	bool m_ThreadLoop;
	pthread_t m_iAcceptThread;
	CWebThreadPool m_ThreadPool;

};
