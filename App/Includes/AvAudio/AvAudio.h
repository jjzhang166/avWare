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
#ifndef _AVAUDIO_H_
#define _AVAUDIO_H_
#include "Apis/AvWareType.h"
#include "AvThread/AvThread.h"
#include "CAvObject.h"
#include "Signals.h"

class CAudioPlay
{
public:
	SINGLETON_DECLARE(CAudioPlay);
private:
	CAudioPlay();
	~CAudioPlay();


public:
	av_bool Initialize();
	av_bool AudioCreate();
	av_bool AudioDestroy();
	av_bool AudioStart();
	av_bool AudioStop();
	av_bool AudioSetFormat();
	av_bool AudioPutBuffer();
private:

};

class CAudioCapture:public CThread
{
public:
	SINGLETON_DECLARE(CAudioCapture);
private:
	CAudioCapture();
	~CAudioCapture();

public:
	av_bool Initialize();
	av_void ThreadProc();

	av_bool AudioCreate();
	av_bool AudioDestroy();
	av_bool AudioStart();
	av_bool AudioStop();
	av_bool AudioSetFormat();


public:
	typedef TSignal3<av_uchar, av_uchar, av_buf &>::SigProc OnStreamSigNalFunc;
	av_bool StreamStart(av_uchar Slave, CAvObject *obj, OnStreamSigNalFunc proc);
	av_bool StreamStop(av_uchar Slave, CAvObject *obj, OnStreamSigNalFunc proc);
private:
	TSignal3<av_uchar, av_uchar, av_buf &> m_StreamSignal;


private:

};

#define g_AvAudioCapture (*CAudioCapture::instance())
#define g_AvAudioPlay	 (*CAudioPlay::instance())


#endif