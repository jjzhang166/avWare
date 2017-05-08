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
#ifndef _AVGUI_H_
#define _AVGUI_H_

#include "Apis/AvWareCplusplus.h"
#include "Apis/LibSystem.h"
#include "CAvObject.h"
#include "AvThread/AvThread.h"
#include "AvCapture/AvManCapture.h"
#include "config.h"

#ifdef AVGUI_SHARE_LIB
#define AVWARE_SHARE_EXPORT
#else
//#define AVWARE_SHARE_IMPORT
#endif

#include "Apis/AvWareLibDef.h"
#include "Apis/AvWareCplusplus.h"
#include "CAvObject.h"

typedef enum {

	LoadingConfig,
	LoadingInitDevice,
	LoadingLog,
	LoadingCapture,
	LoadingRecodService,
	LoadingAlarmService,
	LoadingNetService,
	LoadingIntelligentService,
	LoadingUiResource,
	LoadingInitOver,
}LoadingMessage;

class CAvGui:public CAvObject
{
public:
	SINGLETON_DECLARE(CAvGui);
	CAvGui();
	av_bool Initialize();
	av_bool exec();

public:
	static	std::string GetConfigsPath();
	static	av_bool  NetCaptureProtocolsMask(av_uint &ProtocolsMask);
	static  av_bool  NetCaptureSearch(LinkProtocol ProType, av_bool bClearCache = av_true);
	static  av_bool  NetCaptureSearchResult(LinkProtocol ProType, std::list<C_DevSearch> &SearchList);
	static	av_bool  NetCaptureSetNetProfile(C_NetWorkProfile &NetProfile);
	static  av_bool  NetCaptureSetFactoryInfo(C_ManufacturerInfo &ManufacturerInfo);
	static  int		 NetCaptureGetEmptyChannel();
	static  av_bool  NetCaptureAddDevice(int Channel, C_ProtoFormats &ProtoFormats);
	static  av_bool	 NetCaptureDelDevice(int Channel);
	static  int		 NetCaptureGetTotalNm();
	static  Capture::EAvCaptureStatus CaptureStatus(int Channel);
	static  Capture *CaptureInstance(int Channel);
	static av_bool NetCaptureProtoUpgrade(std::list <C_FirmWareUpgrade> *FirmWareUpgrade);
	static av_bool NetCaptureProtoUpgradeProgress(std::list <C_FirmWareUpgrade> *FirmWareUpgrade);
	static av_bool NetCaptureProtoUpgradeOver();
	static av_bool LoadingMsg(LoadingMessage m, int progress);
public:
	void OnSystemAlarmMsg(C_AlmMsg &Msg);
private:
	~CAvGui();
};

#define g_AvGui (*CAvGui::instance())




#if defined(_AV_WARE_M_HAVE_UI)

#define _AVGUI_LOADING_MSG(msg, progress)\
	g_AvGui.LoadingMsg(msg, progress)

#define g_AvGui_Initialize()\
	g_AvGui.Initialize()

#define g_AvGui_exec()\
	g_AvGui.exec();

#else

#define _AVGUI_LOADING_MSG(msg, progress)
#define g_AvGui_Initialize()
#define g_AvGui_exec()

#endif




#endif