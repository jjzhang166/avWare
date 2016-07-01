/*******************************************************************
|  Copyright(c) 2015-2016 Graceport Technology Development Co.,Ltd
|  All rights reserved.
|
|  版本: 1.0
|  作者: ironman [ironman@graceport.cn]
|  日期: 2016年6月6日
|  说明:
|
|  版本: 1.1
|  作者:
|  日期:
|  说明:
******************************************************************/
#ifndef __AV_NTPCLI_H__
#define __AV_NTPCLI_H__

#include "AvThread/AvTimer.h"
#include "AvConfigs/AvConfigNetService.h"

class CAvNtpCli:public CTimer
{
public:
	SINGLETON_DECLARE(CAvNtpCli);
private:
	CAvNtpCli();
	virtual ~CAvNtpCli();

public:
	av_bool Start();
	av_bool Stop();
private:
	av_void OnTime();
	av_void AvConfigNtp(CAvConfigNetNtp* obj, int &num);

	CAvConfigNetNtp m_AvConfigNetNtp;
};

#define g_AvNtpCli (*CAvNtpCli::instance())

#endif /*__AV_NTPCLI_H__*/