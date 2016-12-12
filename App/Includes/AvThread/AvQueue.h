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
#ifndef _AVQUEUE_H_
#define _AVQUEUE_H_
#include "Apis/AvWareCplusplus.h"
#include "AvThread/AvThread.h"

class CQueue
{
public:
	CQueue(av_u32 QueueSize = 8);
	virtual ~CQueue();

	av_bool Empty();
	av_bool Full();

	av_bool PushBackPointer(av_void *data, av_u32 len = 0);
	av_bool PushFrontPointer(av_void *data, av_u32 len = 0);
	av_bool PopBackPointer(av_void *&data, av_u32 &len);
	av_bool PopFrontPointer(av_void *&data, av_u32 &len);

	av_u32	Size();
	av_bool Size(av_u32 size);

	av_u32  Length();

private:
	std::list<av_void *> m_Queue;
	CMutex		m_Mutex;
	av_u32      m_QueueSize;
};



#endif