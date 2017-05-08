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
#ifndef _AV_ALARMLOCAL_H_
#define _AV_ALARMLOCAL_H_



#include "AvThread/AvThread.h"
#include "Apis/LibSystem.h"
#include "AvThread/AvQmsg.h"
#include "Apis/LibLocalAlm.h"
#include "AvAlarm/AvAlarm.h"

class CAvAlmLocal :public CThread
{

public:
	SINGLETON_DECLARE(CAvAlmLocal);
	av_bool Initialize();
	av_bool AlarmOutCtrl(av_bool bOpen = av_true);
private:
	CAvAlmLocal();
	~CAvAlmLocal();

private:
	av_bool CheckConfIo();
	av_bool CheckConfMd();




private:
	av_bool SendAlmMsg(C_AlmMsg  &msg);
	av_bool PacketAlmIo(av_u32 Result, av_bool bOpen = av_false);
	av_bool PacketAlmMd(av_u32 Result[], av_bool bOpen = av_false);


public:
	C_AlarmCaps m_AlarmCaps;

private:
	void ThreadProc();
};


#define g_AvAlmLocal (*CAvAlmLocal::instance())

#endif
