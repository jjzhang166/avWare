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
#include <cstdio>
#include "Apis/AvWareCplusplus.h"
#include "Apis/LibEncode.h"
#include "Apis/LibSystem.h"
#include "AvThread/AvQueue.h"
#include "AvThread/AvQmsg.h"
#include "AvThread/AvThreadPool.h"
#include "AvThread/AvTimer.h"
#include "AvNetService/AvSmtp.h"
#include "AvNetService/AvUpnp.h"
#include "AvNetService/AvDdns.h"
#include "AvNetService/AvFtp.h"
#include "AvNetService/AvNetService.h"
#include "AvPacket/AvPacket.h"
#include "AvDevice/AvDevice.h"
#include "AvWatchDog/AvWatchDog.h"
#ifdef _AV_WARE_M_HAVE_UI
#include "AvGui/AvGui.h"
#include "AvPreview/AvPreview.h"
#endif
#include "AvCapture/AvManCapture.h"
#include "AvUart/AvUart.h"
#include "AvLog/AvLog.h"
#include "AvAudio/AvAudio.h"
#include "AvAlarm/AvAlarm.h"
#include "AvMemoryPool/AvMemoryPool.h"
#include "AvThread/AvTask.h"
#include "AvConfigs/AvConfigManagerGeneral.h"
#include "AvRecord/AvRecord.h"
#include "AvLua/AvLua.h"
#include "AvProc/AvProc.h"
#include "Common/gtkutf8.h"

#ifdef FACE_DETECT
#include "AvIntelligentAnalysis/AvFaceDetectManager.h"
#endif //FACE_DETECT

int main(int argc, char *argv[])
{

#if defined( WIN32)
	WSADATA wsa = { 0 };
	WSAStartup(MAKEWORD(2,2),&wsa);
#else
	signal(SIGPIPE, SIG_IGN);
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGALRM);
	sigprocmask(SIG_BLOCK, &set, NULL);
#endif
	/*
		以下函数运行序列，不得自行调整。
	*/
	av_msg("AvWare Runing Start\n");
	str_normalize_init();
	AvProcInit();
	g_AvDevice.Initialize();
	g_AvThreadPool.Initialize(SYS_CHN_NUM + 10, av_true);
	g_AvTimer.Initialize();

	g_AvConfigMagagerGeneral.Initialize();
	g_AvDevice.InitializeConfigs();
//	g_AvMemoryPool.Initialize();
	
	g_AvPacketManager.Initialize();

#ifdef _AV_WARE_M_HAVE_UI
	g_AvGui.Initialize();
#endif
	g_AvLog.Initialize();
	g_AvUart.Initialize();
	
	g_AvAlarm.Initialize();

	g_AvManCapture.Initialize();
	//g_AvRecordMan.Initialize();
	
	g_AvNetService.Initialize();
	av_msg("AvWare Service Start Succeed\n");

	g_AvThreadPool.Dump();

#ifdef FACE_DETECT
	g_AvIS_FaceDetect.Start();
#endif //FACE_DETECT


#ifdef _AV_WARE_M_HAVE_UI
	g_AvGui.exec();
#endif

	while (1) av_msleep(1000);

	return 0;

}

