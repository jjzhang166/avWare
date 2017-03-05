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
}C_DecodeCaps;


av_bool AvAudioDecodeBuffer(unsigned  char *buffer, int bufferlen);

av_bool AvDecodeSplit(int SplitNo);
av_bool AvDecodeGetCaps(C_DecodeCaps *DecodeCaps);

//以下4套接口，可能会成对调用。即AvDecodeStartByWid AvDecodeStopByWid,结合硬件平台，只实现一套即可。
//以下两个接口中给PC上显示时使用的，因为PC 多数是以wind 存在。
av_bool AvDecodeStartByWid(int ScreenID, unsigned int hWid);
av_bool AvDecodeStopByWid(int ScreenID);

//以下两个接口中给NVR 上 显示使用的，因为NVR多数是以坐标来计算的。
av_bool AvDecodeStartByPos(int ScreenID, C_RECT *pRenderRect);
av_bool AvDecodeStopByPos(int ScreenID);

//此接口，平台自行处理 分辨率 及 编码类型，结合自身平台，是否需要重置解码器。
av_bool AvDecodeRenderBuffer(int ScreenID, unsigned  char  *buffer, int bufferlen);


#ifdef __cplusplus
}
#endif

#endif
