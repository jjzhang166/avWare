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
#ifndef _AV_MANCAPTURE_H_
#define _AV_MANCAPTURE_H_
#include "AvCapture/AvCapture.h"
#include "AvCapture/AvDevCapture.h"
#include "AvCapture/AvNetCapture.h"


class CManCapture
{
public:
	SINGLETON_DECLARE(CManCapture);
private:
	CManCapture();
	~CManCapture();
public:
	av_bool Initialize();
	Capture *GetAvCaptureInstance(av_int iChannel);
	av_u32   GetAvCaptureTotal();
private:
	std::map <av_int, Capture *>  m_ManCaptureMap;
	av_int m_MaxCaptureChannels;
};

#define g_AvManCapture (*CManCapture::instance())




#endif



