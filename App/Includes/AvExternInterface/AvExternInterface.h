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
#ifndef _AVEXTERNINTERFACE_H_
#define _AVEXTERNINTERFACE_H_

#include "CObject.h"
#include "Apis/AvWareType.h"

class CAvExtInterFace
{
public:
	PATTERN_SINGLETON_DECLARE(CAvExtInterFace);
	
public:
	av_bool Initialize();
	av_bool Start();
	av_bool Stop();
	av_bool Restart();
	
private:
	CAvExtInterFace();
	~CAvExtInterFace();
};

#define g_AvExtInterFace (*CAvExtInterFace::instance())







#endif

