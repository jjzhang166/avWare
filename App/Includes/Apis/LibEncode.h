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
#ifndef _LIBENCODE_H_
#define _LIBENCODE_H_

#ifdef __cplusplus
extern "C" {
#endif
#include "AvWareType.h"


typedef enum {
	CaptureSize_Self,
	CaptureSize_QVGAEX,	//480*240
	CaptureSize_QVGA,	//480*360
	CaptureSize_VGAEX,	//640*360
	CaptureSize_VGA,	//640*480
	CaptureSize_D1,		//720*576
	CaptureSize_720P,
	CaptureSize_960P,
	CaptureSize_1080P,
	CaptureSize_300W,	//2048*1536
	CaptureSize_400W,	//2688*1520
	CaptureSize_500W,	//2592*1944
	CaptureSize_600W,	//3072*2048
	CaptureSize_800W,	//3840*2160
	CaptureSize_4K,		//4096*2160
	CaptureSize_8K,		//7680*4320
	CaptureSize_NR,
	DisplaySize_800_600,
	DisplaySize_1024_768,
	DisplaySize_1280_768,
	DisplaySize_1280_800,
	DisplaySize_1280_1024,
	DisplaySize_1366_768,
	DisplaySize_1400_900,
	DisplaySize_1440_900,
	DisplaySize_1400_1050,
	DisplaySize_1600_900,
	DisplaySize_1680_1050,
	DisplaySize_1920_1080,
	DisplaySize_4K,
	DisplaySize_NR,

}av_capture_size;

typedef enum {
	CHL_MAIN_T = 0,
	CHL_SUB1_T = 1,
	CHL_SUB2_T = 2,
	CHL_SUB3_T = 3,
	CHL_SUB4_T = 4,
	CHL_JPEG_T = 5,
	CHL_NR_T = 6,
}E_EncodeCHL;

typedef enum {
	AvComp_MJPEG,
	AvComp_H264,
	AvComp_H265,
	AvComp_JPEG,
	AvComp_PCM,
	AvComp_G711A,
	AvComp_G711U,
	AvComp_AAC,
	AvComp_NR,
}av_comp_t;

typedef enum {
	AvBitRate_CBR,
	AvBitRate_VBR,
}av_bitrate_ctrl;

typedef enum 
{
	avFrameT_P,
	avFrameT_B,
	avFrameT_I,
	avFrameT_Jpeg,
	avFrameT_Audio,
	avFrameT_NR,
}av_frame_type;

typedef enum{
	avStreamT_V,
	avStreamT_A,
}av_stream_type;
//视频采集


typedef struct {
	av_u32	 CompMask;
	av_u32	 ImageSizeMask;// CHL_MAIN_T
	av_uchar MaxCover;
	av_uchar MaxWaterMaring;
	av_uchar BitRateCtrlMask;

	av_uchar ExtChannel;
	av_u32   ExtImageSizeMask[DisplaySize_NR];
	av_uchar InputFrameRate;
}C_EncodeCaps;



typedef struct{
	av_capture_size ImageSize;
	av_u32			ImageSelfWidth;
	av_u32			ImageSelfHeigh;
	av_u32			FrameRate;
	av_u32			Gop;
	av_comp_t		Comp;
	av_bitrate_ctrl	BitRateCtrl;
	av_u32			BitRateValue;
	av_u32          Qlevel;
}C_EncodeFormats;

typedef struct {
	av_uchar Brightness;
	av_uchar Contrast;
	av_uchar Saturation;
	av_uchar Hue;
}C_ImageFormats;

typedef struct {
	av_uchar SupportMask;/*Brightness[0], Contrast[1], Saturation[2], Hue[3]*/
	av_uchar MaxRange;
}C_ImageQualityCaps;


typedef struct {
	av_u32 top;
	av_u32 bottom;
	av_u32 left;
	av_u32 right;
}C_RECT;


typedef struct {
	av_uchar index;
	av_bool  enable;
	C_RECT   rect;
	av_u32	 color;//argb
}C_CoverFormats;

typedef enum {
	WM_YMD_HMS,
	WM_YMD_W_HMS,
	WM_HMS_YMD,
}E_WaterMarkingType;
typedef struct {
	av_uchar index;
	av_bool  enable;
	C_RECT   rect;
	av_u32	 color;
	av_char  WaterMarkString[128];
	E_WaterMarkingType  type;
}C_WaterMarkingFormats;

typedef struct {
	av_u32  VideoInWidth;
	av_u32	VideoInHeigh;
	av_u32	VideoInFrame;
	
	av_u32  IrCutModeMask;
	av_u32  VideoReverseMask;
	av_u32  AntiFlckerMask;
	av_u32  ExposureMask;
	av_u32	WhiteBalanceMask;
}C_CaptureInCaps;

typedef enum {
	IRCUT_OPEN,
	IRCUT_CLOSE,
	IRCUT_AUTO,
	IRCUT_TIMER,
}E_IrCutMode;

typedef enum {
	VIDEOREVERSE_HOR,
	VIDEOREVERSE_VER,
	VIDEOREVERSE_90,
}E_VideoReverse;


typedef enum{
	AvAntiFlckerMode_NONE = 0,
	AvAntiFlckerMode_INDOOR_50HZ = 1,
	AvAntiFlckerMode_OUTDOOR_50HZ = 2,
	AvAntiFlckerMode_AUTO_50HZ = 3,
	AvAntiFlckerMode_INDOOR_60HZ = 4,
	AvAntiFlckerMode_OUTDOOR_60HZ = 5,
	AvAntiFlckerMode_AUTO_60HZ = 6,
	AvAntiFlckerMode_THEATER_50HZ = 7,
	AvAntiFlckerMode_FAST_50HZ = 8,
	AvAntiFlckerMode_THEATER_60HZ = 9,
	AvAntiFlckerMode_FAST_60HZ = 10,
	AvAntiFlckerMode_ANTI_FLICKER_CLOSED = 11,
	AvAntiFlckerMode_LAST = 12
}E_AntiFlckerMode;


typedef enum{
	AvWhiteBalanceMode_NONE = 0,
	AvWhiteBalanceMode_OFF = 1,
	AvWhiteBalanceMode_AUTO = 3,
	///室外白天
	AvWhiteBalanceMode_DAYTIME = 4,
	///室外黄昏
	AvWhiteBalanceMode_EVENING = 5,
	///室外多去
	AvWhiteBalanceMode_CLOUDY = 6,
	///室内办公室
	AvWhiteBalanceMode_OFFICE = 7,
	///荧光灯
	AvWhiteBalanceMode_FLUORESCENT = 8,
	/// 白炽灯
	AvWhiteBalanceMode_INCANDESCENT = 9,
	/// 手动调节
	AvWhiteBalanceMode_MANUAL = 10,
	AvWhiteBalanceMode_LAST = 11
}E_WhiteBalanceMode;

typedef enum {
	AvExposureMode_NONE = 0,
	AvExposureMode_AUTO = 1,
	AvExposureMode_MANUAL = 2,
	AvExposureMode_TRAFFIC = 3,
	AvExposureMode_LAST = 4
}E_ExposureMode;

typedef struct {
	av_bool			ReverseHor;
	av_bool			ReverseVer;
	av_bool			Reverse90;

	E_IrCutMode			IrCutMode;
	E_AntiFlckerMode	AntiFlckerMode;
	E_WhiteBalanceMode	WhiteBalanceMode;
	E_ExposureMode		ExposureMode;

}C_CaptureInFormats;


typedef enum {
	E_Capture_VideoNONE,
	E_Capture_VideoStart,
	E_Capture_VideoRunning,
	E_Capture_VideoStop,
	E_Capture_VideoErr,
	E_Capture_VideoNr,
}E_CaptureSynchronizeStat;





av_bool AvCaptureCreate(av_uchar Channel);
av_bool AvCaptureDestroy(av_uchar Channel);

av_bool AvCaptureStart(av_uchar Channel, av_char Slave);
av_bool AvCaptureStop(av_uchar Channel, av_char Slave);


av_bool AvCaptureGetCaps(av_uchar Channel, C_EncodeCaps *EncodeCaps);
av_bool AvEncodeSetFormat(av_uchar Channel, av_uchar Slave, C_EncodeFormats *Formats);

av_bool AvCaptureInCaps(av_char Channel, C_CaptureInCaps *CaptureInCaps);
av_bool AvCaptureInSetFormat(av_uchar channel, C_CaptureInFormats *Formats);
av_bool AvCaptureGetBuffer(av_uchar Channel, av_uchar Slave, av_stream_type ast, av_buf *buf);
av_bool AvCaptureForceKeyFrame(av_uchar Channel, av_uchar Slave);
E_CaptureSynchronizeStat AvCaptureSynchronize(av_uchar Channel);
//视频遮盖
av_bool AvCoverCreate(av_uchar Channel);
av_bool AvCoverDestroy(av_uchar Channel);
av_bool AvCoverStart(av_uchar Channel);
av_bool AvCoverStop(av_uchar Channel);
av_bool AvCoverSetFormat(av_uchar Channel, C_CoverFormats *CoverFormats);


//osd
av_bool AvWaterMarkingCreate(av_uchar Channel);
av_bool AvWaterMarkingDestory(av_uchar Channel);
av_bool AvWaterMarkingStart(av_uchar Channel);
av_bool AvWaterMarkingStop(av_uchar channel);
av_bool AvWaterMarkingSetFormat(av_uchar Channel, C_WaterMarkingFormats *VmFormats);

//image
av_bool AvImageCaps(av_uchar Channel, C_ImageQualityCaps *ImageCaps);
av_bool AvImageSet(av_uchar Channel,C_ImageFormats *ImageFormats);


//音频编码
av_bool AvAudioEncodeCreate();
av_bool AvAudioEncodeDestroy();
av_bool AvAudioEncodeStart();
av_bool AvAudioEncodeStop();
av_bool AvAudioEncodeSetFormat();



//取音视频
av_bool AvEncodeGetBuffer();









#ifdef __cplusplus
}
#endif

#endif


