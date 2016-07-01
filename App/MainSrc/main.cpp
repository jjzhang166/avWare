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

#include "AvPacket/AvPacket.h"
#include "Apis/AvWareType.h"
#include "Apis/LibEncode.h"
#include "AvDevice/AvDevice.h"
#include "AvWatchDog/AvWatchDog.h"
#include "AvGui/AvGui.h"
#include "AvCapture/AvManCapture.h"
#include "AvNetService/AvNetService.h"
#include "Apis/LibSystem.h"
#include "AvDecode/AvDecode.h"
#include "AvUart/AvUart.h"
#include "AvExternInterface/AvExternInterface.h"
#include "AvLog/AvLog.h"
#include "AvAudio/AvAudio.h"
#include "AvThread/AvQueue.h"
#include "AvThread/AvQmsg.h"
#include "AvThread/AvThreadPool.h"
#include "AvThread/AvTimer.h"
#include "AvAlarm/AvAlarm.h"
#include "AvMemoryPool/AvMemoryPool.h"
#include "AvThread/AvTask.h"
#include "AvNetService/AvSmtp.h"
#include "AvNetService/AvUpnp.h"
#include "AvNetService/AvDdns.h"
#include "AvNetService/AvFtp.h"
#include "AvConfigs/AvConfigManager.h"
#include "AvRecord/AvRecord.h"



#include "AvConfigs/AvConfigCapture.h"

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
	av_msg("AvWare Runing Start\n");
	g_AvThreadPool.Initialize(10, av_true);
	g_AvConfigManager.Initialize();
	g_AvDevice.Initialize();
	g_AvMemoryPool.Initialize();
	g_AvTimerManager.Initialize();
	g_AvPacketManager.Initialize();
	g_AvGui.Initialize();
	g_AvLog.Initialize();
	g_AvUart.Initialize();
	g_AvDecode.Initialize();
	g_AvManCapture.Initialize();
	g_AvRecord.Initialize();
	g_AvAudioCapture.Initialize();
	g_AvAlarm.Initialize();
	g_AvNetService.Initialize();
	g_AvExtInterFace.Initialize();
	av_msg("AvWare Service Start Succeed\n");

	g_AvThreadPool.Dump();
	
	g_AvGui.exec();
	while (1) av_msleep(1000);

	return 0;

}

