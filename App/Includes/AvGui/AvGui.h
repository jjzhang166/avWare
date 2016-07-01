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
#ifndef _AVGUI_H_
#define _AVGUI_H_

#include "Apis/AvWareType.h"
#include "Apis/LibSystem.h"
#include "CAvObject.h"


class CAvGui
{
public:
	SINGLETON_DECLARE(CAvGui);
	CAvGui();
	av_bool Initialize();
	av_bool exec();
private:
	~CAvGui();

private:

};

#define g_AvGui (*CAvGui::instance())







#endif