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
#include "AvGui/AvGui.h"



PATTERN_SINGLETON_IMPLEMENT(CAvGui);
int m_AppArgc = 0;
CAvGui::CAvGui()
{

}
CAvGui::~CAvGui()
{

}
av_bool CAvGui::Initialize()
{
	av_msg("%s Started\n", __FUNCTION__);
	return av_true;
}

av_bool CAvGui::exec()
{
	while (1) av_msleep(1000);
	return av_true;
}