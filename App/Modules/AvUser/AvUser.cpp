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
#include "AvUser/AvUser.h"


SINGLETON_IMPLEMENT(CAvUser);

CAvUser::CAvUser()
{

}

CAvUser::~CAvUser()
{
}

av_bool CAvUser::AddGroup()
{
	return av_true;
}
av_bool CAvUser::DelGroup()
{
	return av_true;
}
av_bool CAvUser::ModifyGroup()
{
	return av_true;
}
av_bool CAvUser::GroupList()
{
	return av_true;
}

av_bool CAvUser::AddUser()
{
	return av_true;
}
av_bool CAvUser::DelUser()
{
	return av_true;
}
av_bool CAvUser::ModifyUser()
{
	return av_true;
}
av_bool CAvUser::CheckOut()
{
	return av_true;
}
