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
#ifndef AVWARE_AVCONFIG_AVCONFIGUSERMANAGER_H_
#define AVWARE_AVCONFIG_AVCONFIGUSERMANAGER_H_

#include "AvConfigs/AvConfigTable.h"

typedef enum {
	AuthSuperAdmin,
	AuthAdmin,
	AuthCommon,
	AuthPreview,
	AuthNr,
}AvUserAuthorityType;

#define MAX_USER_STRING_LEN 64
typedef struct {
	char account[MAX_USER_STRING_LEN];
	char passwd[MAX_USER_STRING_LEN];
	char creator[MAX_USER_STRING_LEN];
	char group[MAX_USER_STRING_LEN];
	int auth_type;
}AvConfigUserManager;

typedef TAvConfig<AvConfigUserManager, 0, ConfMaxUser> CAvConfigUserManager;
#endif //AVWARE_AVCONFIG_AVCONFIGUSERMANAGER_H_