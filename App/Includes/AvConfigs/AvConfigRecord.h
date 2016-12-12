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

#ifndef _AVCONFIGRECORD_H_
#define _AVCONFIGRECORD_H_

#include "Apis/AvWareCplusplus.h"
#include "AvConfigs/AvConfigTable.h"

typedef enum {
	EAvRecord_NONE,
	EAvRecord_Timer,
	EAvRecord_Manul,
	EAvRecord_Alarm,
	EAvRecord_LAST,
}EAvRecordType;

typedef struct{
	av_bool bEnable;
	av_u32  tStart;
	av_u32  tEnd;
}RecordTime;

#define RECORD_MAX_TIME_RANGE 3



typedef struct {
	av_uint			RecordTypeMask;
	av_uint			RecordChlMask;
	RecordTime		RecordTm[RECORD_MAX_TIME_RANGE];
}ConfigRecordFormats;

typedef TAvConfig<ConfigRecordFormats, 0, SYS_CHN_NUM> CAvConfigRecord;


typedef enum{
	EAvRecord_LimitNONE,
	EAvRecord_LimitTime,
	EAvRecord_LimitSize,
	EAvRecord_LimitLAST,
}EAvRecordLimitType;


#define EAvRecordMaxRecordFileSize (500*1024*1024)
#define EAvRecordMaxPrerecordSec   15

// 是否是循环录相，还是存满停止录相
typedef struct {
	EAvRecordLimitType	LimitType;
	av_uint				LimitArgs;//按时间限制时，此参数为一个录相文件的秒数;按大小限制时，此参数为录相文件的字节数;
	av_uint				LimitPrerecordSec;
}ConfigRecordCtrlFormats;

typedef TAvConfig<ConfigRecordCtrlFormats> CAvConfigRecordCtrl;


#endif