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
#ifndef _AV_MANCAPTURE_H_
#define _AV_MANCAPTURE_H_
#include "AvCapture/AvCapture.h"
#include "AvCapture/AvDevCapture.h"
#include "AvCapture/AvNetCapture.h"
#include "Apis/AvSystemStruct.h"
//#include "AvProtocol/ProtoStruct.h"


class CManCapture:public CAvObject
{
public:
	SINGLETON_DECLARE(CManCapture);
private:
	CManCapture();
	~CManCapture();
public:
	av_bool Initialize();
	Capture *GetAvCaptureInstance(av_int iChannel);
	av_u32   GetAvCaptureTotal();
public:
	av_bool MoonDeviceSetNetProfile(C_NetWorkProfile &NetWorkProfile);
	av_bool MoonDeviceSetFactoryInfo(C_ManufacturerInfo &ManufacturerInfo);
public:
	av_bool NetCaptureProtocolsMask(av_u32 &ProtolMask);
	av_bool NetCaptureAddDevice(int Channel, C_ProtoFormats &ProtoFormats);
	av_bool NetCaptureDelDevice(int Channel);
	int		NetCaptureGetEmptyChannel();
	av_bool NetCaptureSearch(LinkProtocol ProType, av_bool bClearCache = av_true);
	av_bool NetCaptureSearchResult(LinkProtocol ProType, std::list<C_DevSearch> &SearchList);
	Capture::EAvCaptureStatus CaptureStatus(int Channel);


public:
	av_bool NetCaptureProtoUpgrade(std::list <C_FirmWareUpgrade> *FirmWareUpgrade);
	av_bool NetCaptureProtoUpgradeProgress(std::list <C_FirmWareUpgrade> *FirmWareUpgrade);
	av_bool NetCaptureProtoUpgradeOver();


public:
	av_void OnProtoSlots(LinkProtocol ProType, NetProtocolParam &_data);

private:
	std::map <av_int, Capture *>  m_ManCaptureMap;
	std::map<LinkProtocol, Capture *> m_SearchHandle;
	av_int m_MaxCaptureChannels;
};

#define g_AvManCapture (*CManCapture::instance())




#endif



