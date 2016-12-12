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
#ifndef _LIBRTSPSERVER_H_
#define _LIBRTSPSERVER_H_
#include "AvThread/AvThread.h"
#include "RtspMedia.h"
#include "Session.h"
#define  DEFAULT_RTSP_SERVICE_PORT 554




class CRtspServer: public CThread, public CRtspSessionSer
{
public:
	CRtspServer();
	virtual  ~CRtspServer();

	int RtspStart(int port = DEFAULT_RTSP_SERVICE_PORT);
	int RtspStop();


private:

	typedef std::map<int, CRtspSessionSer *> ClientInfoMap_t;
	typedef std::map<int, CRtspSessionSer *>::iterator ClientInfoMapIterator_t;

	void ThreadProc();
	void Processor();
	int OnConnect(const char *ip, const int port, const int sock);

	int  m_sock;
	int  m_port;

	ClientInfoMap_t	m_Clients;
};





#endif
