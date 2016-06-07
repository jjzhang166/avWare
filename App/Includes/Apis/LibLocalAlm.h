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

	typedef struct {
		av_uchar MaxAlmOutNum;
		av_uchar MaxAlmInNum;
	}C_AlmIoCaps;

	typedef struct{
		av_u32 AlmIoCount;
		av_u32 AlmIoStatusMask;
	}C_AlmIoResult;

	av_bool AvGetIoAlmCaps(C_AlmIoCaps *Caps);
	av_bool AvGetIoAlmStatus(av_u32 AlmInNoMask, C_AlmIoResult *result);
	av_bool avSetIoAlmOut(av_uchar AlmOutNo, av_bool bOpen);

	typedef struct{
		av_uchar MaxRow;//最大16
		av_uchar MaxLine;//最大16
		av_uchar Maxlevel;
	}C_AlmMdCaps;

	typedef struct{
		av_u16 AlmAreaCount;
		av_u16 result[16];
	}C_AlmMdResult;
	av_bool avGetMdAlmCaps(C_AlmMdCaps *AlmMdCaps);
	av_bool avGetMdAlmStatus(C_AlmMdResult *result);

#ifdef __cplusplus
}
#endif

#endif