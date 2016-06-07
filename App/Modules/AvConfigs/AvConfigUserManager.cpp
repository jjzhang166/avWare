/*******************************************************************
|  Copyright(c) 2015-2016 Graceport Technology Development Co.,Ltd
|  All rights reserved.
|
|  版本: 1.0
|  作者: Xman [xman@graceport.cn]
|  日期: 2016年6月6日
|  说明:
|
|  版本: 1.1
|  作者:
|  日期:
|  说明:
******************************************************************/
#include "AvConfigs/AvConfigUserManager.h"

template<> void ProcessValue<AvConfigUserManager>(CAvConfigBase &av_conf, AvConfigValue &val, AvConfigUserManager &config, int index, int diff)
{
	if (index == 0){
		av_conf.Process("Account", val, config.account, "admin");
		av_conf.Process("Password", val, config.passwd, "admin");
		av_conf.Process("Creator", val, config.creator, "system");
		av_conf.Process("Group", val, config.group, "admin");
		av_conf.Process("AuthType", val, config.auth_type, (int)AuthSuperAdmin, (int)AuthSuperAdmin, (int)AuthSuperAdmin);
	}
	else{
		av_conf.Process("Account", val, config.account, "");
		av_conf.Process("Password", val, config.passwd, "");
		av_conf.Process("Creator", val, config.creator, "");
		av_conf.Process("Group", val, config.group, "");
		av_conf.Process("AuthType", val, config.auth_type, (int)AuthPreview, (int)AuthAdmin, (int)AuthPreview);

	}
}