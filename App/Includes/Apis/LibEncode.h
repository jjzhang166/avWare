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

#if 0
显示器
标屏			分辨率				宽屏			分辨率
QVGA		320×240				WQVGA		400×240
VGA			640×480				WVGA		800×480
SVGA		800×600				WSVGA		1024×600
XGA			1024×768			WXGA		1280×720/1280×768/1280×800
XGA+		1152×864			WXGA+		1366×768
SXGA		1280×1024/1280×960	WSXGA		1440×900
SXGA+		1400×1050			WSXGA+		1680×1050
UXGA		1600×1200			WUXGA		1920×1200
QXGA		2048×1536			WQXGA		2560×1600

照相机
百万像素		4:3分辨率			16 : 9分辨率
0.3M		640×480				720×405
0.5M		800×600				960×540
0.6M		1024×576
0.7M		960×720
0.8M		1024×768
0.9M		1280×720

1M			1152×864			1366×768
1.2M		1280×960
1.5M		1400×1050			1600×900
2M			1600×1200			1920×1080
2.4M		1792×1344			2048×1152
3.2M		2048×1536
4M			2304×1728			2560×1440
5M			2592×1944			3072×1728
6M			2816×2112
7M			3072×2304
7.5M		3200×2400
8M			3264×2448			3840×2160
9M			3456×2592
9.5M		3600×2700			4096×2304
10M			3672×2754
11M			3840×2880
12M			4000×3000			4608×2592
12.5M		4096×3072
15M			4800×3600			5120×2880
20M			5120×3840
30M			6400×4800
#endif

typedef enum {
	CaptureSize_Self,
	CaptureSize_QVGAEX,	//320*180
	CaptureSize_QVGA,	//320*240
	CaptureSize_VGAEX,	//640*360
	CaptureSize_VGA,	//640*480
	CaptureSize_D1,		//720*576
	CaptureSize_720P,
	CaptureSize_960P,
	CaptureSize_1080P,
	CaptureSize_300W,	//2048*1536
	CaptureSize_400W,	//2560*1440
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
	CHL_ACAP_T = 0,
	CHL_APLY_T = 1, 
	CHL_A_NUM = 2,
}E_AUDIO_CHL;

typedef enum {
	AvComp_MJPEG,
	AvComp_H264,
	AvComp_H265,
	AvComp_JPEG,
	AvComp_PCM,
	AvComp_G711A,
	AvComp_G711U,
	AvComp_AAC,
	AvComp_AMR_NB,
	AvComp_AMR_WB,
	AvComp_MP3,
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
typedef enum {
	avMediaUp_IoAlm,
	avMediaUp_MdAlm,
	avMediaUp_RealT,
	avMediaUp_RecordIoAlm,
	avMediaUp_RecordMdAlm,
	avMediaUp_RecordTimer,
	avMediaUp_RecordManual,
}av_media_property;

typedef enum {
	nal_unit_type_h264_nr = 0,
	nal_unit_type_h264_p = 1,
	nal_unit_type_h264_dataA = 2,
	nal_unit_type_h264_dataB = 3,
	nal_unit_type_h264_dataC = 4,
	nal_unit_type_h264_idr = 5,
	nal_unit_type_h264_sei = 6,
	nal_unit_type_h264_sps = 7,
	nal_unit_type_h264_pps = 8,
	nal_unit_type_h264_delimiter = 9,
	nal_unit_type_h264_nalend = 10,
	nal_unit_type_h264_streamend = 11,
	nal_unit_type_h264_pading = 12,
}av_nal_unit_type_h264;

typedef enum {
	nal_unit_type_hevc_trail_n	= 0,
	nal_unit_type_hevc_trail_r	= 1,
	nal_unit_type_hevc_idr		= 19,
	nal_unit_type_hevc_vps		= 32,
	nal_unit_type_hevc_sps		= 33,
	nal_unit_type_hevc_pps		= 34,
	nal_unit_type_hevc_sei		= 39,
}av_nal_unit_type_hevc;

typedef enum{
	avStreamT_V,
	avStreamT_A,
	avStreamT_Nr,
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

typedef enum {
	ASampleRate_8000,
	ASampleRate_16000,
	ASampleRate_11025,
	ASampleRate_22050,
	ASampleRate_32000,
	ASampleRate_44100,
	ASampleRate_47250,
	ASampleRate_48000,
	ASampleRate_50000,
	ASampleRate_96000,
	ASampleRate_192000,
	ASampleRate_2822400,
}E_SampleRate;

typedef enum {
	ASampleBits_8,
	ASampleBits_16,
	ASampleBits_24,
	ASampleBits_32,
}E_SampleBits;

typedef struct {
	av_u32 nMaxChannels;
	av_u32 CompMask;
	av_u32 SampleRateMask[AvComp_NR];
	av_u32 SampleBitsMask[AvComp_NR];
}C_AudioCaps;

typedef struct {
	av_comp_t		Comp;
	av_u32			Volume;
	av_u32			Channels;
	E_SampleRate	SampleRate;
	E_SampleBits	Samplebits;
}C_AudioProfile;

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
	WM_CHANNEL_NAME,
	WM_DATE_TIME,
	WM_TYPE_Nr,
}E_WaterMarkingType;

typedef enum {
	WM_YMD_HMS,
	WM_YMD_W_HMS,
	WM_HMS_YMD,
}E_WaterMarkingStyle;

typedef struct {
	av_uchar index;
	av_bool  enable;
	C_RECT   rect;
	av_u32	 color;
	av_char  WaterMarkString[128];
	E_WaterMarkingStyle  type;
}C_WaterMarkingFormats;

typedef struct {
	av_u32  VideoInWidth;
	av_u32	VideoInHeigh;
	av_u32	VideoInFrame;
	
	av_u32  IrCutModeMask;
	av_u32  VideoReverseMask;
	av_u32  VideoRoRateMask;
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
}E_VideoReverse;

typedef enum{
	VIDEOROTATE_0,
	VIDEOROTATE_90,
	VIDEOROTATE_180,
	VIDEOROTATE_270,
}E_VideoRotate;



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
	av_bool bMirror;
	av_bool bFilp;
}C_CapReverseAttr;

typedef struct {
	E_VideoRotate mode;
}C_CapRotateAttr;

typedef struct {
	E_IrCutMode mode;
	//for Timer IRCUT
	av_u32		tOpen;
	av_u32		tClose;
}C_CapIrCutAttr;
typedef struct {
	E_AntiFlckerMode mode;

}C_CapAntiFlckerAttr;
typedef struct {
	E_WhiteBalanceMode mode;
	//for AvWhiteBalanceMode_MANUAL
	av_u32			 value;
}C_CapWhiteBalanceAttr;
typedef struct {
	E_ExposureMode mode;
	av_int		   max;
	av_int		   min;
}C_CapExposureAttr;

typedef struct {
	C_CapReverseAttr		ReverseAttr;
	C_CapRotateAttr			RotateAttr;
	C_CapIrCutAttr			IrCutAttr;
	C_CapAntiFlckerAttr		AntiFlckerAttr;
	C_CapWhiteBalanceAttr	WhiteBalanceAttr;
	C_CapExposureAttr		ExposureAttr;
}C_CaptureInFormats;

typedef enum {
	E_Capture_VideoNONE,
	E_Capture_VideoStart,
	E_Capture_VideoRunning,
	E_Capture_VideoStop,
	E_Capture_VideoErr,
	E_Capture_VideoNr,
}E_CaptureSynchronizeStat;



av_bool AvImageSize2ImageValue(av_capture_size ImageSize, av_u32 *ImageValueW, av_u32 *ImageValueH);
av_bool AvImageValue2ImageSize(av_capture_size *ImageSize, av_u32 ImageValueW, av_u32 ImageValueH);


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
av_bool AvACaptureCaps(E_AUDIO_CHL AChl, C_AudioCaps *ACaps);
av_bool AvACreate(av_uint Channel);
av_bool AvADestroy(av_uint Channel);
av_bool AvAStart(av_uint Channel, E_AUDIO_CHL AChl);
av_bool AvAStop(av_uint Channel, E_AUDIO_CHL AChl);
av_bool AvASetProfile(av_uint Channel, E_AUDIO_CHL AChl, C_AudioProfile *AProfile);
av_bool AvAPlayPutBuffer(av_uchar *data, av_int datalen);

//数据帧头
typedef struct {
	av_comp_t		comp;
	av_frame_type	frametype;
	av_u32			FrameRate;
	av_u32			ImageWidth;
	av_u32			ImageHeigh;
}C_AvVideoHead;

typedef struct{
	av_comp_t		comp;
	av_u32			sampleRate;
	av_u32			sampleBits;
}C_AvAudioHead;

typedef struct{
	av_u32 Channel;
	av_u32 Slave;
	av_u32 PlayLoadLength;
	av_u64 pts;
	av_stream_type StreamType;
	av_u16 MediaPropertyMask;
	av_u32 IndexCode;
	union{
		C_AvVideoHead VHead;
		C_AvAudioHead AHead;
	};
}C_AvMediaHead;

av_bool AvMediaHeadFiller(av_uchar *Fillerdata, C_AvMediaHead *headInfo, av_u32 *lseek);
av_bool AvMediaHeadParse(av_uchar *Fillerdata, C_AvMediaHead *headInfo, av_u32 *lseek);




#ifdef __cplusplus
}
#endif

#endif


