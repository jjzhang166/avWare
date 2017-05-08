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
#ifndef _LIB_LOCALALM_H_
#define _LIB_LOCALALM_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "AvWareType.h"
#include "AvSystemStruct.h"



	av_bool AvAlarmCaps(C_AlarmCaps *AlarmCaps);
	av_bool AvGetIoAlmStatus(av_u32 *result);
	av_bool avSetIoAlmOut(av_uchar AlmOutNo, av_bool bOpen);
	av_bool avGetMdAlmStatus(av_u32 result[]);
	av_bool avSetMdProfile(av_u32 Area[], av_32 level);
	av_bool avAlarmCreate();
	av_bool avAlarmDestory();

#ifdef __cplusplus
}
#endif

#endif
