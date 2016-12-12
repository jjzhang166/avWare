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
#include "AvGui/AvGui.h"
#include "AvSource/AvGuiApplication.h"
#include "AvDevice/AvDevice.h"
#include "AvPreview/AvPreview.h"

SINGLETON_IMPLEMENT(CAvGui);

CAvGui::CAvGui()
{

}
CAvGui::~CAvGui()
{

}
av_bool CAvGui::Initialize()
{

	av_msg("%s Started\n", __FUNCTION__);
	g_AvGuiApp.Initialize();
	return av_true;
}


av_bool CAvGui::exec()
{
	g_AvGuiApp.exec();
	return av_true;
}

std::string CAvGui::GetConfigsPath()
{
	std::string Value;
	CAvDevice::GetEnv(std::string(EKey_ConfigsPath), Value);
	return Value;
}

av_bool  CAvGui::NetCaptureSearch(LinkProtocol ProType, av_bool bClearCache)
{
	return g_AvManCapture.NetCaptureSearch(ProType, bClearCache);
}
av_bool  CAvGui::NetCaptureSearchResult(LinkProtocol ProType, std::list<C_DevSearch> &SearchList)
{
	return g_AvManCapture.NetCaptureSearchResult(ProType, SearchList);
}

av_bool CAvGui::NetCaptureSetNetProfile(C_NetWorkProfile &NetProfile)
{
	return g_AvManCapture.MoonDeviceSetNetProfile(NetProfile);
}

av_bool  CAvGui::NetCaptureSetFactoryInfo(C_ManufacturerInfo &ManufacturerInfo)
{
	return g_AvManCapture.MoonDeviceSetFactoryInfo(ManufacturerInfo);
}
int		 CAvGui::NetCaptureGetEmptyChannel()
{
	return g_AvManCapture.NetCaptureGetEmptyChannel();
}
av_bool CAvGui::NetCaptureProtocolsMask(av_uint &ProtocolsMask)
{
	return g_AvManCapture.NetCaptureProtocolsMask(ProtocolsMask);
}

av_bool CAvGui::NetCaptureAddDevice(int Channel, C_ProtoFormats &ProtoFormats)
{
	return g_AvManCapture.NetCaptureAddDevice(Channel, ProtoFormats);
}
av_bool	 CAvGui::NetCaptureDelDevice(int Channel)
{
	return g_AvManCapture.NetCaptureDelDevice(Channel);
}
int		 CAvGui::NetCaptureGetTotalNm()
{
	return g_AvManCapture.GetAvCaptureTotal();
}

Capture::EAvCaptureStatus CAvGui::CaptureStatus(int Channel)
{
	return g_AvManCapture.CaptureStatus(Channel);
}

 Capture *CAvGui::CaptureInstance(int Channel)
{
	return g_AvManCapture.GetAvCaptureInstance(Channel);
}

 av_bool CAvGui::NetCaptureProtoUpgrade(std::list <C_FirmWareUpgrade> *FirmWareUpgrade)
 {
	 return g_AvManCapture.NetCaptureProtoUpgrade(FirmWareUpgrade);
 }
 av_bool CAvGui::NetCaptureProtoUpgradeProgress(std::list <C_FirmWareUpgrade> *FirmWareUpgrade)
 {
	 return g_AvManCapture.NetCaptureProtoUpgradeProgress(FirmWareUpgrade);
 }
 av_bool CAvGui::NetCaptureProtoUpgradeOver()
 {
	 return g_AvManCapture.NetCaptureProtoUpgradeOver();
 }
