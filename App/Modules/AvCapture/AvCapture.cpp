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
#include "AvCapture/AvCapture.h"
#include "Apis/AvEnum.h"
Capture::Capture() 
{
	m_NetProtoHandle = NULL;
}

Capture::~Capture()
{

}
av_bool  Capture::StartNetCapture(CAvNetProto *Handle)
{
	m_NetProtoHandle = Handle;
	return av_true;
}
av_bool  Capture::StopNetCapture()
{
	m_NetProtoHandle = NULL;
	return av_true;
}

CAvNetProto *Capture::AvNetProtoHandle()
{
	return m_NetProtoHandle;
}