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
#ifndef _LIBRINGBUFFER_H_
#define _LIBRINGBUFFER_H_
#include "AvWareType.h"

typedef struct {
	av_u32  depth;
	av_u32  MaxSize;
	av_bool newDataPlace;
	av_bool mutex;
}AvRingBufferAttr;

typedef av_void* AvRingBufferHandle;

AvRingBufferHandle* AvRingBufferNew(AvRingBufferAttr *Attr);
av_bool AvRingBufferRelease(AvRingBufferHandle *Handle);
av_bool AvRingBufferPop(AvRingBufferHandle *Handle, av_char **data, av_u32 *datalen);
av_bool AvRingBufferPush(AvRingBufferHandle *Handle, av_char *data, av_u32 datalen);
av_bool AvRingBufferIsEmpty(AvRingBufferHandle *Handle);
av_bool AvRingBufferIsFull(AvRingBufferHandle *Handle);


#endif

