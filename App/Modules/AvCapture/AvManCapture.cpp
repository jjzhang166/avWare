#include "AvCapture/AvManCapture.h"
#include "AvDevice/AvDevice.h"
#include "AvConfigs/AvConfigCapture.h"
#include "Apis/AvWareCplusplus.h"
#include "Apis/AvEnuminline.h"
#include "AvNetService/AvProtoMoon.h"
#include "AvNetService/AvOnvifClient.h"


SINGLETON_IMPLEMENT(CManCapture)
CManCapture::CManCapture()
{
	m_MaxCaptureChannels = 0;
	m_ManCaptureMap.clear();
	m_ManCaptureMap.clear();

}
CManCapture::~CManCapture()
{

}
av_bool CManCapture::Initialize()
{
	C_DspCaps DspCaps;
	CAvDevice::GetDspCaps(DspCaps);
	int i = 0;
	Capture *pCapture = NULL;
#if defined(_AV_WARE_M_HAVE_UI_CLOSE_CAPUTRE)
	DspCaps.nMaxEncodeChannel = 0;
#endif

	av_bool bVaildFactoryInfo = av_false;
	std::string bVaild;

	CAvDevice::GetEnv(std::string(EKey_BvalidFactoryInfo), bVaild);
	if (bVaild == std::string(EnumNameav_bool(av_true))){
		bVaildFactoryInfo = av_true;
	}
	else{
		av_error("FactoryInfo is  Invalid Capture Task not Initialize\n");
	}

	for (i = 0; i < (int)DspCaps.nMaxEncodeChannel; i++){
		pCapture = new CAvDevCapture;
		if (av_true == bVaildFactoryInfo){
			pCapture->Initialize((av_int)i);
		}
		m_ManCaptureMap.insert(std::pair<av_int, Capture*>(i, pCapture));
	}
	for (; i < SYS_CHN_NUM; i++){
		pCapture = new CAvNetCapture;
		if (av_true == bVaildFactoryInfo){
			pCapture->Initialize((av_int)i);
		}
		m_ManCaptureMap.insert(std::pair<av_int, Capture*>(i, pCapture));
	}

	m_MaxCaptureChannels = i;
	return av_true;
}

Capture *CManCapture::GetAvCaptureInstance(av_int iChannel)
{
	assert(iChannel >= 0);
	assert(iChannel < SYS_CHN_NUM);
	return m_ManCaptureMap[iChannel];
}

av_u32   CManCapture::GetAvCaptureTotal()
{
	return m_MaxCaptureChannels;
}

av_bool CManCapture::NetCaptureSearch(LinkProtocol ProType, av_bool bClearCache)
{
	switch (ProType)
	{
	case LinkProtocol_Moon:
	{
		CAvProtoMoon::SearchCmdStart(bClearCache);
	}
		break;
	case LinkProtocol_Onvif:
	{
		CAvOnvifClient::SearchCmdStart(bClearCache);
	}
		break;
	case LinkProtocol_RTSP:
	case LinkProtocol_GBT28181:
	default:
		break;
	}

	return av_true;
}
av_bool CManCapture::NetCaptureSearchResult(LinkProtocol ProType, std::list<C_DevSearch> &SearchList)
{
	switch (ProType)
	{
	case LinkProtocol_Moon:
	{
		CAvProtoMoon::SearchResult(SearchList);
	}
	break;
	case LinkProtocol_Onvif:
	{
		CAvOnvifClient::SearchResult(SearchList);
	}
		break;
	case LinkProtocol_RTSP:
	case LinkProtocol_GBT28181:
	default:
		break;
	}
	return av_true;
}


av_bool CManCapture::NetCaptureAddDevice(int Channel, C_ProtoFormats &ProtoFormats)
{
	CAvConfigProtocol AvConfigProtocol;
	AvConfigProtocol.Update();
	ConfigProtoFormats &ConfigProtocol = AvConfigProtocol.GetConfig(Channel);
	ConfigProtocol = ProtoFormats;
	AvConfigProtocol.SettingUp(Channel);
	return av_true;
}

av_bool CManCapture::NetCaptureDelDevice(int Channel)
{
	CAvConfigProtocol AvConfigProtocol;
	AvConfigProtocol.Update();
	ConfigProtoFormats &ConfigProtocol = AvConfigProtocol.GetConfig(Channel);
	ConfigProtocol.IsEnable = av_false;
	AvConfigProtocol.SettingUp(Channel);
	return av_true;
}

av_bool CManCapture::MoonDeviceSetNetProfile(C_NetWorkProfile &NetWorkProfile)
{
	return CAvProtoMoon::RemoteNetCommSetProfile(NetWorkProfile) == IRet_succeed?av_true:av_false;
}
av_bool CManCapture::MoonDeviceSetFactoryInfo(C_ManufacturerInfo &ManufacturerInfo)
{
	return CAvProtoMoon::RemoteFactoryInfoSet(ManufacturerInfo) == IRet_succeed ? av_true : av_false;
}

av_bool CManCapture::NetCaptureProtocolsMask(av_u32 &ProtocolMask)
{
	ProtocolMask = AvMask(LinkProtocol_GBT28181)
		| AvMask(LinkProtocol_Moon)
		| AvMask(LinkProtocol_Onvif)
		| AvMask(LinkProtocol_RTSP);

	return av_true;
}
int		 CManCapture::NetCaptureGetEmptyChannel()
{
	for (int i = 0; i < (int)m_ManCaptureMap.size(); i++){
		if (m_ManCaptureMap[i]->GetCaptureStatus() == Capture::EAvCapture_STOP){
			return i;
		}
	}
	return -1;
}

Capture::EAvCaptureStatus CManCapture::CaptureStatus(int Channel)
{
	return m_ManCaptureMap[Channel]->GetCaptureStatus();
}



static std::list <CAvProtoMoon *> g_CAvProtoMoonUpgrade;

av_bool CManCapture::NetCaptureProtoUpgrade(std::list <C_FirmWareUpgrade> *FirmWareUpgrade)
{
	CAvProtoMoon *pAvProtoMoon = NULL;
	C_ProtoFormats ProtoFormats;
	ProtoFormats.IsEnable = av_true;
	ProtoFormats.ProtoMode = ProtocolMoon;
	C_FirmWareUpgrade LocalFrimWareUpgrade;
	std::list<C_FirmWareUpgrade>::iterator ilist;
	for (ilist = FirmWareUpgrade->begin(); ilist != FirmWareUpgrade->end(); ilist++){
		if (ilist->_status == ProgressStatus_HaveNoRightResource){
			continue;
		}
		sprintf(ProtoFormats.UsrName, ilist->Usrname);
		sprintf(ProtoFormats.Passwd, ilist->Passwd);
		sprintf(ProtoFormats.MoonFormats.Url, ilist->IpAddr);
		ProtoFormats.MoonFormats.Port = ilist->Port;

		pAvProtoMoon = new CAvProtoMoon(ProtoFormats);
		pAvProtoMoon->Connect();
		ilist->_status = ProgressStatus_None;
		ilist->_value = 0;

		LocalFrimWareUpgrade = *ilist;
		pAvProtoMoon->RemoteFirmWareUpgrade(LocalFrimWareUpgrade);

		g_CAvProtoMoonUpgrade.push_back(pAvProtoMoon);

	}

	return av_true;
}

av_bool CManCapture::NetCaptureProtoUpgradeProgress(std::list <C_FirmWareUpgrade> *FirmWareUpgrade)
{
	std::list<C_FirmWareUpgrade>::iterator ilist;
	std::list <CAvProtoMoon *>::iterator iProtoList;
	int Progress;
	for (ilist = FirmWareUpgrade->begin(), iProtoList = g_CAvProtoMoonUpgrade.begin();
		ilist != FirmWareUpgrade->end(); ilist++){

		if (ilist->_status == ProgressStatus_HaveNoRightResource){
			continue;
		}
		
		(*iProtoList)->RemoteFirmWareUpgradeGetProgress(Progress);
		ilist->_status = (ProgressStatus)((Progress >> 16) & 0xffff);
		ilist->_value = Progress & 0xff;

		iProtoList++;
	}

	return av_true;
}

av_bool CManCapture::NetCaptureProtoUpgradeOver()
{
	std::list <CAvProtoMoon *>::iterator iProtoList;
//	int Progress;
	CAvProtoMoon *pAvPrtoMoon;
	for (iProtoList = g_CAvProtoMoonUpgrade.begin();iProtoList != g_CAvProtoMoonUpgrade.end();){
		pAvPrtoMoon = *iProtoList;
		
		iProtoList = g_CAvProtoMoonUpgrade.erase(iProtoList);
		av_error("relase over\n");
		pAvPrtoMoon->Disconnect();
//		delete pAvPrtoMoon;
		//why delete error
		
	}

	return av_true;
}

