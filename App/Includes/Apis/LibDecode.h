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

#include "AvWareType.h"
#include "AvSystemStruct.h"
typedef struct{
	unsigned int CompMask;
	unsigned int ImageSizeMask;// CHL_MAIN_T
	unsigned int MaxOutFrameRate;
	unsigned int MaxChannels;
	unsigned int MainStreamMode[64];
}C_DecodeCaps;


av_bool AvDecodeSplit(int SplitNum);
av_bool AvDecodeClears(int ScreenID);


av_bool AvAudioDecodeBuffer(unsigned  char *buffer, int bufferlen);


av_bool AvDecodeGetCaps(C_DecodeCaps *DecodeCaps);


av_bool AvDecodeStart(int ScreenID);


//此接口，平台自行处理 分辨率 及 编码类型，结合自身平台，是否需要重置解码器。
av_bool AvDecodeRenderBuffer(int ScreenID, unsigned  char  *buffer, int bufferlen);


av_u32		 avWareGetWindowsIdByScreenID(int SrceenID);
C_RECT		*avWareGetWindowsRectByScreenID(int SrceenID);


#ifdef __cplusplus
}
#endif

#endif
