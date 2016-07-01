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
#ifndef _AVEXTERNINTERFACETEST_H_
#define _AVEXTERNINTERFACETEST_H_


#include "AvPacket/AvPacket.h"
#include "Apis/AvWareType.h"
#include "CAvObject.h"
class CAvExtInterFaceTest :public CAvObject
{
public:
	SINGLETON_DECLARE(CAvExtInterFaceTest);
	
	av_bool Start();
	av_bool Stop();
private:
	av_void OnStream(av_uchar Channel, av_uchar Slave, CAvPacket *AvPacket);

private:
	CAvExtInterFaceTest();
	~CAvExtInterFaceTest();
};

#define g_AvExtInterFaceTest (*CAvExtInterFaceTest::instance())







#endif



