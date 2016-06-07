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
#include "Apis/AvWareType.h"
#include "CObject.h"
#include "WebService/LibWebService.h"

class CAvWebSer:public CWebService
{
public:
	PATTERN_SINGLETON_DECLARE(CAvWebSer);
	av_bool Start();
	av_bool Stop();

private:
	CAvWebSer();
	~CAvWebSer();

private:
	int WebCallBackProc(std::string InBuf, std::string &OutBuf);

};


#define g_AvWebSer (*CAvWebSer::instance())

