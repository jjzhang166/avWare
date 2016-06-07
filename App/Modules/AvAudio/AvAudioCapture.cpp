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
#include "AvAudio/AvAudio.h"
#include "Apis/LibEncode.h"
PATTERN_SINGLETON_IMPLEMENT(CAudioCapture);

CAudioCapture::CAudioCapture() :CThread("CAudioCapture")
{

}

CAudioCapture::~CAudioCapture()
{
	
}

av_bool CAudioCapture::Initialize()
{
	AudioSetFormat();
	//run();
	return av_true;
}

av_void CAudioCapture::ThreadProc()
{
	av_buf buf;
	av_bool bret = av_false;
	while (av_true == m_Loop)
	{
		bret = AvCaptureGetBuffer(0, 0, avStreamT_A, &buf);
		if (bret == av_true){
			m_StreamSignal(0, 0, buf);
			free(buf.base);
		}
		av_msleep(40);
	}
}

av_bool CAudioCapture::AudioCreate()
{
	return AvAudioEncodeCreate();
}
av_bool CAudioCapture::AudioDestroy()
{
	return AvAudioEncodeDestroy();
}
av_bool CAudioCapture::AudioStart()
{
	return AvAudioEncodeStart();
}
av_bool CAudioCapture::AudioStop()
{
	return AvAudioEncodeStop();
}
av_bool CAudioCapture::AudioSetFormat()
{
	return AvAudioEncodeSetFormat();
}


av_bool CAudioCapture::StreamStart(av_uchar Slave, CObject *obj, OnStreamSigNalFunc proc)
{
	int ret = 0;
	ret = m_StreamSignal.Attach(obj, proc);
	if (ret < 0) {
		return av_false;
	}
	else{
		return av_true;
	}
}
av_bool CAudioCapture::StreamStop(av_uchar Slave, CObject *obj, OnStreamSigNalFunc proc)
{
	int ret = 0;
	ret = m_StreamSignal.Detach(obj, proc);
	if (ret < 0) {
		return av_false;
	}
	else{
		return av_true;
	}
}