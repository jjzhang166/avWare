#ifndef _AVSYSTEMSTRUCT_H_
#define _AVSYSTEMSTRUCT_H_
#include "AvWareType.h"
#include "AvEnum.h"

typedef struct {
	av_u32 tv_sec;		//设置时间时必添 1900 至今的秒数
	av_u32 tv_usec;		//设置时间时必添

	av_u32 t_year;		//实际的年 2016
	av_u32 t_mon;		//实际的月 1-12
	av_u32 t_day;		//实际的日 1-31

	av_uchar t_week;		//rang 1-7
	av_uchar t_hour;
	av_uchar t_min;
	av_uchar t_sec;

	av_u32 t_todaysec;	//当天的秒数
}av_timeval;

typedef struct {
	av_int Start;
	av_int End;
}C_Region;

typedef struct{
	av_int Sx;
	av_int Sy;
	av_int Width;
	av_int Heigh;
}C_RECT;

typedef struct {
	av_u32 NetCommMask;
	av_u32 NetCommGetModeMask[8];
}C_NetCommCaps;


typedef struct {
	av_u32	 CompMask;
	av_u32	 ImageSizeMask;// CHL_MAIN_T
	av_uchar MaxCover;
	av_uchar MaxWaterMaring;
	av_uchar BitRateCtrlMask;

	av_uchar ExtChannelMask;
	av_u32   ExtImageSizeMask[CaptureSize_LAST];
	av_u32   FrameRateMask[CaptureSize_LAST];
}C_EncodeCaps;

typedef struct{
	CaptureSize		ImageSize;
	av_u32			ImageSelfWidth;
	av_u32			ImageSelfHeigh;
	av_u32			FrameRate;
	av_u32			Gop;
	AvComp			Comp;
	BitRateCtrl		BitRateCtl;
	av_u32			BitRateValue;
	av_u32          Qlevel;
}C_EnCodeProfile;


typedef struct {
	av_u32 BrightnessRang;
	av_u32 ContrastRang;
	av_u32 HueRang;
	av_u32 SaturationRang;
}C_ImageCaps;

typedef struct {
	av_u32 Brightness;
	av_u32 Contrast;
	av_u32 Hue;
	av_u32 Saturation;
}C_ImageProfile;

typedef struct {
	av_u32	nMaxEncodePower;
	av_u32	nMaxEncodeChannel;
	av_u32	nMaxEncodeExtChannel;
	av_u32	nMaxDecodeChannel;
	av_u32	nMaxDecodeExtChannel;
}C_DspCaps;
typedef C_DspCaps C_DeviceCaps;

typedef struct {
	av_u32 IrCutMask;
	av_u32 MirrorMask;
	av_u32 ExposureMask;
	C_Region ExposureValueRegion;
	av_u32 WhiteBalanceMask;
	C_Region WhiteBalanceValueRegion;
	av_u32 AntiFlckerMask;
	C_Region AntiFlckerValueRegion;
	av_u32 CvbsMask;
}C_CaptureCaps;

typedef struct {
	IrCutMode 			IrCut;
	C_Region 			IrCutTimer;
	av_u32 				MirrorMaskValue;
	ExposureMode 		Exposure;
	av_int 				ExposureValue;
	WhiteBalanceMode 	WhiteBalance;
	av_int 				WhiteBalanceValue;
	AntiFlckerMode 		AntiFlcker;
	av_int 				AntiFlckerValue;
	av_bool 			bOpenCvbs;
}C_CaptureProfile;


typedef struct {
	av_uint CaptureDeviceMask;
	av_uint SoundModeMask;
	av_uint CompMask;
	av_uint SampleRateMask[AvComp_LAST];
	av_uint SampleBitsMask[AvComp_LAST];
	av_uint VolumeRang;
	av_uint VoiceQualityMask;
}C_AudioCaps;

typedef struct{
	AvComp					Comp;
	av_uint					VolumeCapture;
	av_uint					VolumePlay;
	AudioSampleRate			SampleRate;
	AudioSampleBits			SampleBits;
	AudioSoundMode			SoundMode;
	AudioCaptureDevice		CaptureDevice;
	av_uint					VoiceQualityMask;
}C_AudioProfile;
#endif

