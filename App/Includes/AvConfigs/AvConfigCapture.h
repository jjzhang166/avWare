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
#ifndef _AVCONFIGCAPTURE_H_
#define _AVCONFIGCAPTURE_H_
#include "Apis/AvWareType.h"
#include "AvConfigs/AvConfigTable.h"
#include "Apis/LibEncode.h"



typedef struct {
	av_bool Enable;
	C_EncodeFormats Formats;
}C_CHLEncodeFormats;

typedef struct{
	C_CHLEncodeFormats CHLFormats[CHL_NR_T];
}ConfigEncodeFormats;


typedef struct {
	C_CoverFormats CHLFormats[ConfMaxCover];
}ConfigCoverFormats;

typedef struct{
	C_WaterMarkingFormats CHLFormats[ConfMaxWaterMarking];
}ConfigWaterMarkingFormats;



typedef enum{
	ProtocolOnvif,
	ProtocolRtmp,
	ProtocolRtsp,
	ProtocolMoon,
}E_ProtoMode;

typedef struct{
	av_char MainUrl[128];
	av_char SubUrl[128];
}C_ProtoRtspFormats;

typedef struct {
	av_char MainUrl[128];
	av_char SubUrl[128];
}C_ProtoRtmpFormats;

typedef struct {
	av_char Url[128];
	av_int	Port;
}C_ProtoMoonFormats;

typedef struct {
	av_char Url[128];
}C_ProtoOnvifFormats;

typedef struct{
	av_bool IsEnable;
	av_char UsrName[64];
	av_char Passwd[64];
	av_int  DigitalChannel;
	av_int  ShowWindowsId;
	E_ProtoMode ProtoMode;
	union{
		C_ProtoOnvifFormats OnvifFormats;
		C_ProtoMoonFormats  MoonFormats;
		C_ProtoRtmpFormats  RtmpFormats;
		C_ProtoRtspFormats	RtspFormats;
	};
}ConfigProtoFormats;



typedef C_ImageFormats ConfigImageFormats;

typedef C_CaptureInFormats ConfigCaptureFormats;


typedef TAvConfig<ConfigEncodeFormats, 0, ConfMaxCaptureChn> CAvConfigEncode;

typedef TAvConfig<ConfigImageFormats, 0, ConfMaxCaptureChn> CAvConfigImage;

typedef TAvConfig<ConfigCaptureFormats, 0, ConfMaxCaptureChn> CAvConfigCapture;

typedef TAvConfig<ConfigCoverFormats, 0, ConfMaxCaptureChn>CAvConfigCover;

typedef TAvConfig<ConfigWaterMarkingFormats, 0, ConfMaxCaptureChn>CAvConfigWaterMarking;

typedef TAvConfig<ConfigProtoFormats, 0, ConfMaxDecodeChn>CAvConfigProtocol;









#endif


