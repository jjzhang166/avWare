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
#ifndef _LIBRTSPCLIENT_H_
#define _LIBRTSPCLIENT_H_

#include "Session.h"


class CRtspClient;

class CRtspClientsManager:public CThread
{

public:
	CRtspClientsManager();
	~CRtspClientsManager();
public:
	av_bool Init();
	av_bool DeInit();

public:
	av_bool Add(CRtspClient *pClient);
	av_bool Del(CRtspClient *pClient);

private:
	void ThreadProc();
	void Processor();

private:
	CMutex		m_Mutex;
	std::map<int, CRtspClient *>	m_RtspClients;
	char		m_Buffer[8 * 1024];
};




class CRtspClient :public CRtspSessionCli
{

	friend class CRtspClientsManager;
public:
	CRtspClient();
	~CRtspClient();


public:
	av_bool Connect(const char *Url);
	av_bool DisConnect();

	av_void GetStreamPacket(CAvPacket *Packet);
private:
	std::queue <CAvPacket *>	m_PacketQueue;

private:
	

private:
	static av_bool			   m_bRtspClientsManInited;
	static CRtspClientsManager m_s_RtspClientsManager;
};






#endif
