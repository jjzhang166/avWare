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
#include "AvEnum.h"
#include "AvSystemStruct.h"

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
	avMediaUp_IoAlm,
	avMediaUp_MdAlm,
	avMediaUp_RealT,
	avMediaUp_RecordIoAlm,
	avMediaUp_RecordMdAlm,
	avMediaUp_RecordTimer,
	avMediaUp_RecordManual,
}av_media_property;



typedef enum {
	E_Capture_VideoNONE,
	E_Capture_VideoStart,
	E_Capture_VideoRunning,
	E_Capture_VideoStop,
	E_Capture_VideoErr,
	E_Capture_VideoNr,
}E_CaptureSynchronizeStat;



av_bool AvImageSize2ImageValue(CaptureSize ImageSize, av_u32 *ImageValueW, av_u32 *ImageValueH);
av_bool AvImageValue2ImageSize(CaptureSize *ImageSize, av_u32 ImageValueW, av_u32 ImageValueH);


av_bool AvCaptureCreate(av_uchar Channel);
av_bool AvCaptureDestroy(av_uchar Channel);

av_bool AvCaptureStart(av_uchar Channel, av_char Slave);
av_bool AvCaptureStop(av_uchar Channel, av_char Slave);


av_bool AvEncodeGetCaps(av_uchar Channel, C_EncodeCaps *EncodeCaps);
av_bool AvEncodeSetProfile(av_uchar Channel, av_uchar Slave, C_EnCodeProfile *Profile);

av_bool AvCaptureGetCaps(av_char Channel, C_CaptureCaps *CaptureCaps);

av_bool AvCaptureSetProfile(av_uchar channel, C_CaptureProfile *Profile);
av_bool AvCaptureGetBuffer(av_uchar Channel, av_uchar Slave, StreamContent sc, av_buf *buf);
av_bool AvCaptureForceKeyFrame(av_uchar Channel, av_uchar Slave);
E_CaptureSynchronizeStat AvCaptureSynchronize(av_uchar Channel);
//视频遮盖
av_bool AvCoverCreate(av_uchar Channel);
av_bool AvCoverDestroy(av_uchar Channel);
av_bool AvCoverSetProfile(av_uchar Channel, C_CoverProfile *CoverFormats);

av_bool AvOverLayGetCaps(av_uchar Channel, C_OverLayCaps *OverLayCaps);
av_bool AvOverLayCreate(av_uchar Channel);
av_bool AvOverLayDestory(av_uchar Channel);
av_bool AvOverLaySetProfile(av_uchar Channel, C_OverLayProfile *OverLayProfile);



//image
av_bool AvImageCaps(av_uchar Channel, C_ImageCaps *ImageCaps);
av_bool AvImageSetProfile(av_uchar Channel, C_ImageProfile *ImageProfile);


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
	AvComp			comp;
	avFrameT		Ft;
	av_u32			FrameRate;
	av_u32			ImageWidth;
	av_u32			ImageHeigh;
}C_AvVideoHead;

typedef struct{
	AvComp			comp;
	av_u32			sampleRate;
	av_u32			sampleBits;
}C_AvAudioHead;

typedef struct{
	av_u32 Channel;
	av_u32 Slave;
	av_u32 PlayLoadLength;
	av_u64 pts;
	StreamContent Sc;
	av_u16 MediaPropertyMask;
	av_u32 IndexCode;
	union{
		C_AvVideoHead VHead;
		C_AvAudioHead AHead;
	};
}C_AvMediaHead;

av_bool AvMediaHeadFiller(av_uchar *Fillerdata, C_AvMediaHead *headInfo, av_u32 *lseek);
av_bool AvMediaHeadParse(av_uchar *Fillerdata, C_AvMediaHead *headInfo, av_u32 *lseek);
av_bool AvMediaHeadConvert(av_uchar *Fillerdata, av_u32 LocalChannel);



#ifdef __cplusplus
}
#endif

#endif


