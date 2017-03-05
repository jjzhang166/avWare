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
#ifndef AVWARE_AVCONFIG_AVCONFIGDEVICE_H_
#define AVWARE_AVCONFIG_AVCONFIGDEVICE_H_

#include "Apis/AvWareCplusplus.h"
#include "AvConfigs/AvConfigTable.h"
#include "Apis/LibSystem.h"
#include "Apis/AvSystemStruct.h"


typedef struct {
	av_bool			Enable;
	av_com_type		Type;
	C_SerialAttr	Attribute;
}ConfigUart;
typedef TAvConfig<ConfigUart, 0, ConfMaxSerial> CAvConfigUart;



typedef C_PtzAdvancedCameraLensProfile ConfigPtzCameraLensProfile;

typedef TAvConfig<ConfigPtzCameraLensProfile, 0, 1>CAvConfigPtzCameraLensProfile;

#endif //AVWARE_AVCONFIG_AVCONFIGDEVICE_H_