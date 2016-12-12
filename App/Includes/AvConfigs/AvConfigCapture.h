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
#include "Apis/AvWareCplusplus.h"
#include "AvConfigs/AvConfigTable.h"
#include "AvProtocol/AvNetProto.h"
#include "Apis/LibEncode.h"



typedef struct {
	av_bool Enable;
	C_EnCodeProfile Profile;
}C_CHLEncodeProfile;

typedef struct{
	C_CHLEncodeProfile CHLProfile[CHL_NR_T];
}ConfigEncodeProfile;


typedef struct {
	C_CoverFormats CHLFormats[ConfMaxCover];
}ConfigCoverFormats;

typedef struct{
	C_WaterMarkingFormats CHLFormats[ConfMaxWaterMarking];
}ConfigWaterMarkingFormats;





typedef C_ProtoFormats ConfigProtoFormats;

typedef C_AudioProfile ConfigAudioFormats;

typedef C_ImageProfile ConfigImageProfile;

typedef C_CaptureProfile ConfigCaptureProfile;


typedef TAvConfig<ConfigEncodeProfile, 0, SYS_LOCALCAPTURE_CHN> CAvConfigEncode;

typedef TAvConfig<ConfigImageProfile, 0, SYS_LOCALCAPTURE_CHN> CAvConfigImage;

typedef TAvConfig<ConfigCaptureProfile, 0, SYS_LOCALCAPTURE_CHN> CAvConfigCapture;

typedef TAvConfig<ConfigCoverFormats, 0, SYS_LOCALCAPTURE_CHN>CAvConfigCover;

typedef TAvConfig<ConfigWaterMarkingFormats, 0, SYS_LOCALCAPTURE_CHN>CAvConfigWaterMarking;

typedef TAvConfig<ConfigProtoFormats, 0, SYS_REMOTECAPTURE_CHN>CAvConfigProtocol;

typedef TAvConfig<ConfigAudioFormats, 0, 1>CAvConfigAudio;






#endif


