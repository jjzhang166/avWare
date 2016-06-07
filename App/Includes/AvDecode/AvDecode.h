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
#ifndef _AVDECODE_H_
#define _AVDECODE_H_
#include "Apis/AvWareType.h"
#include "CObject.h"

class CAvDecode
{
public:
	PATTERN_SINGLETON_DECLARE(CAvDecode);
	~CAvDecode();
	av_bool Initialize();

private:
	CAvDecode();
};

#define g_AvDecode (*CAvDecode::instance())


#endif


