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
#include "AvExternInterfaceTest.h"
#include "AvCapture/AvCapture.h"
PATTERN_SINGLETON_IMPLEMENT(CAvExtInterFaceTest)


av_bool CAvExtInterFaceTest::Start()
{
	CAvCapture *pAvCapture = g_MCapture.GetAvCaptureInstance(0);
	pAvCapture->StreamStart(1, this, (CAvCapture::OnStreamSigNalFunc)&CAvExtInterFaceTest::OnStream);
	pAvCapture->StreamStart(0, this, (CAvCapture::OnStreamSigNalFunc)&CAvExtInterFaceTest::OnStream);
	return av_true;
}
av_bool CAvExtInterFaceTest::Stop()
{
	return av_true;
}

av_void CAvExtInterFaceTest::OnStream(av_uchar Channel, av_uchar Slave, CPacket &packet)
{
	av_msg("CAvExtInterFaceTest::OnStream have data %d %d\n", Channel, Slave);
	if (0 && packet.GetSlave() == CHL_MAIN_T){
		static FILE *fp = NULL;
		if (fp == NULL) fp = fopen("avExtinterText.h264", "wb");
		fwrite(packet.GetRawData(), packet.GetRawDataLen(), 1, fp);
	}


	return ;
}


CAvExtInterFaceTest::CAvExtInterFaceTest()
{

}
CAvExtInterFaceTest::~CAvExtInterFaceTest()
{

}