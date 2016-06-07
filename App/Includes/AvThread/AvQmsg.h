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
#ifndef _AVQMSG_H_
#define _AVQMSG_H_
#include "Apis/AvWareType.h"
#include "AvThread/AvThread.h"
#include "AvThread/AvQueue.h"

//释放 对象不能将消息队列释放掉，释放消息一定要使用QmRelease
//之所以为什么不将QmRelease放在 析构函数里，也正是因为如此
class CAvQmsg:public CObject
{
public:
	CAvQmsg(std::string QmsgName, av_u32 QmsgSize = 8);
	~CAvQmsg();
	av_bool QmRcv(av_char *msg, av_u32 &len,av_bool wait = av_true);
	av_bool QmSnd(av_char *msg, av_u32 &len);
	av_u32  QmCount();
	av_bool QmClear();
	av_bool QmRelease();

	static av_void QmDebug(std::string QmsgName = NULL);
private:
	CQueue			*m_Queue;
	std::string		m_QmsgName;
private:
	static std::map<std::string, CQueue *> s_QueueHandle;
	static CMutex s_QueueMutex;
};




#endif