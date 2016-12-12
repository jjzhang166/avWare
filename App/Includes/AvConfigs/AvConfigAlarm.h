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
#ifndef _AVCONFIGALARM_H_
#define _AVCONFIGALARM_H_

#include "Apis/AvWareCplusplus.h"
#include "AvConfigs/AvConfigTable.h"

#define ConfMaxDayTimeSpan 6
#define ConfMaxWeekDaySpan 7



typedef struct {
	av_bool TimeEnable;
	av_u32  StartSec;
	av_u32  EndSec;
}C_TimeSpan;

typedef struct {
	av_bool DayEnable;
	C_TimeSpan TimeSpan[ConfMaxDayTimeSpan];
}C_DaySpan;


typedef struct{
	C_DaySpan DaySpan[ConfMaxWeekDaySpan];
}C_WeekSpan;



typedef struct {
	av_bool Enable;
	C_WeekSpan WeekSpan;
	av_bool EnableAlmOut;
	av_ushort AlmOutTimes;
	av_bool SendEmail;
	av_bool PtzAction;
}ConfigIoAlarmFormats;


typedef struct{
	av_ushort top;
	av_ushort buttom;
	av_ushort left;
	av_ushort rigth;
}C_AreaZone;


#define ConfMaxMdAreaNum 4
#define ConfMaxMdChunkLine 18
typedef struct {
	av_bool Enable;
	C_AreaZone Zone;
}C_MdAreaZone;
typedef struct {
	av_bool Enable;
	C_WeekSpan WeekSpan;
#ifndef ConfMaxMdChunkLine
	C_MdAreaZone MdAreaZone[ConfMaxMdAreaNum];
#else
	av_u32 Chunk[ConfMaxMdChunkLine];
#endif
	av_bool EnableAlmOut;
	av_ushort AlmOutTimes;
	av_bool SendEmail;
	av_bool PtzAction;
}ConfigMdAlarmFormats;

typedef TAvConfig<ConfigIoAlarmFormats, 0, ConfMaxIoAlarmIn> CAvConfigAlarmIo;

typedef TAvConfig<ConfigMdAlarmFormats, 0, ConfMaxIoAlarmIn> CAvConfigAlarmMd;











#endif
