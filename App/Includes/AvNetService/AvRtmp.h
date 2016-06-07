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
#ifndef _AVRTMP_H_
#define _AVRTMP_H_
#include "Apis/AvWareType.h"
#include "CObject.h"
#include "AvPacket/AvPacket.h"
#include "AvThread/AvThread.h"

class AvRtmp:public CThread
{
public:
	PATTERN_SINGLETON_DECLARE(AvRtmp);
	AvRtmp();
	~AvRtmp();

	av_bool Start();
	av_bool Stop();
private:
	av_void OnStream(av_uchar Channel, av_uchar Slave, CPacket &packet);
	void ThreadProc(void);
};

#define g_AvRtmp (*AvRtmp::instance())


#endif