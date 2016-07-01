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
#ifndef _AVUSER_H_
#define _AVUSER_H_
#include "Apis/AvWareType.h"
#include "CAvObject.h"

class CAvUser
{
public:
	SINGLETON_DECLARE(CAvUser)
private:
	CAvUser();
	~CAvUser();

public:
	av_bool AddGroup();
	av_bool DelGroup();
	av_bool ModifyGroup();
	av_bool GroupList();

	av_bool AddUser();
	av_bool DelUser();
	av_bool ModifyUser();
	av_bool CheckOut();
private:

};


#define g_AvUser (*CAvUser::instance())



#endif