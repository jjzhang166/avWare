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
#ifndef _AVCAPTURE_H_
#define _AVCAPTURE_H_
#include "AvPacket/AvPacket.h"
#include "Apis/AvWareType.h"
#include "Signals.h"
#include "CAvObject.h"


class Capture:public CThread
{
public:
	Capture() { };
	virtual ~Capture(){};

public:
	typedef TSignal3 <av_int, av_int, CAvPacket *>::SigProc SIG_PROC_ONDATA;
	virtual av_bool Initialize(av_int Channel) = 0;
	virtual av_bool Start(av_int Slave) = 0;
	virtual av_bool Stop(av_int Slave) = 0;

	virtual av_bool Start(av_int Slave, CAvObject *obj, SIG_PROC_ONDATA pOnData) = 0;
	virtual av_bool Stop(av_int Slave, CAvObject *obj, SIG_PROC_ONDATA pOnData) = 0;

	virtual av_bool SetProfile(av_int Slave, C_EncodeFormats &Formats) = 0;
	virtual av_bool GetProfile(av_int Slave, C_EncodeFormats &Fromats) = 0;
	virtual av_bool GetCaps(C_EncodeCaps &Caps) = 0;

	virtual av_bool SetTime(av_timeval &atv) = 0;
	virtual av_bool SetIFrame(av_int Slave = CHL_MAIN_T) = 0;

	virtual CAvPacket *GetSnap(av_int Slave = CHL_SUB1_T) = 0;
private:

};





#endif


