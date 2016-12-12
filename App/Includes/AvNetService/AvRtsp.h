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
#ifndef _AV_RTSP_H_
#define _AV_RTSP_H_
#include "Apis/AvWareCplusplus.h"
#include "CAvObject.h"
#include "Rtsp/LibRtspServer.h"
#include "AvPacket/AvPacket.h"


class CAvRtspServerMedia:public CRtspMediaSer
{
public:
	CAvRtspServerMedia();
	~CAvRtspServerMedia();

	bool StartMedia();
	bool StopMedia();

	av_bool SetMediaInfo(av_u32 Channel, av_u32 Slave);
private:
	av_u32		m_Channel;
	av_u32		m_Slave;

private:
	av_void OnStream(av_int Channel, av_int Slave, CAvPacket *AvPacket);
};



class CAvRtspServer:public CRtspServer
{
public:
	SINGLETON_DECLARE(CAvRtspServer)
	CAvRtspServer();
	~CAvRtspServer();
	av_bool Start(int ServicePort = 554);
	av_bool Stop();

};


#define g_AvRtspServer (*CAvRtspServer::instance())




#endif
