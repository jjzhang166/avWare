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
#include "AvGui/AvGui.h"
#include "AvPreview/AvPreview.h"
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
#include "AvIntelligentAnalysis/AvIntelligentAnalysis.h"
#include "AvF/AvF.h"

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
	av_warning("Version [%s]\n", CAvDevice::GetSoftVersionString().c_str());
	str_normalize_init();

	AvProcInit();


	g_AvDevice.Initialize();
	g_AvGui_Initialize();
	_AVGUI_LOADING_MSG(LoadingInitDevice, 5);

	g_AvThreadPool.Initialize(SYS_CHN_NUM + 10, av_true);

	g_AvTimer.Initialize();

	_AVGUI_LOADING_MSG(LoadingConfig, 10);
	g_AvConfigMagagerGeneral.Initialize();

	g_AvDevice.InitializeConfigs();

//	g_AvMemoryPool.Initialize();
	
	g_AvPacketManager.Initialize();


	_AVGUI_LOADING_MSG(LoadingLog, 15);
	g_AvLog.Initialize();
	

	g_AvUart.Initialize();

	_AVGUI_LOADING_MSG(LoadingCapture, 20);
	g_AvManCapture.Initialize();
	
	_AVGUI_LOADING_MSG(LoadingRecodService, 25);
	//g_AvRecordMan.Initialize();
	
	_AVGUI_LOADING_MSG(LoadingAlarmService, 50);
	g_AvAlarm.Initialize();
	
	_AVGUI_LOADING_MSG(LoadingNetService, 55);
	g_AvNetService.Initialize();

	_AVGUI_LOADING_MSG(LoadingIntelligentService, 60);
	g_AvIntelligentanalysis.Initialize();

	g_AvThreadPool.Dump();

	av_msg("AvWare Service Start Succeed\n");

	_AVGUI_LOADING_MSG(LoadingUiResource, 80);
	g_AvGui_exec();


	while (1) av_msleep(1000);

	return 0;

}

