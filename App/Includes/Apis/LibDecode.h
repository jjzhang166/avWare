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
#ifndef _LIBDECODE_H_
#define _LIBDECODE_H_

#ifdef __cplusplus
extern "C" {
#endif


//音频解码
av_bool AvAudioDecodeCreate();
av_bool AvAudioDecodeDestroy();
av_bool AvAudioDecodeStart();
av_bool AvAudioDecodeStop();
av_bool AvAudioDecodeSetFormat();
av_bool AvAudioDecodeBuffer();


typedef struct{
	av_u32	 CompMask;
	av_u32	 ImageSizeMask;// CHL_MAIN_T
	av_uchar ExtChannel;
	av_u32   ExtImageSizeMask[DisplaySize_NR];
	av_uchar MaxOutFrameRate;
}C_DecodeCaps;

av_bool AvGetDecodeCaps(av_uchar Channel, C_DecodeCaps *DecodeCaps);





#ifdef __cplusplus
}
#endif

#endif
