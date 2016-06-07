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
#include "AvDecode/AvDecode.h"


PATTERN_SINGLETON_IMPLEMENT(CAvDecode);
CAvDecode::CAvDecode()
{
}
CAvDecode::~CAvDecode()
{

}
av_bool CAvDecode::Initialize()
{
	av_msg("%s Started\n", __FUNCTION__);
	return av_true;
}


