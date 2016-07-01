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
#include "AvMoon.h"
#include "AvCapture/AvManCapture.h"
#include "AvUart/AvUart.h"
#include "AvConfigs/AvConfigNetService.h"
#include "AvPacket/AvPacket.h"
#include "AvDevice/AvDevice.h"
#include "Apis/AvWareType.h"



SINGLETON_IMPLEMENT(CStream);
CStream::CStream()
{
	memset(m_VideoCount, 0x00, sizeof(m_VideoCount));
}

CStream::~CStream()
{
	av_msg("%s\n", __FUNCTION__);
}

int CStream::OnVideoData(av_uchar Channel, av_uchar Slave, CAvPacket *AvPacket)
{

	if (AvPacket->IsVideoFrame() == av_false){
		return 0;
	}
	C_VideoAttr VideoAttr = { 0 };
	VideoAttr.Channel = Channel;
	VideoAttr.Slave = Slave;
	VideoAttr.Brc = BitRateCtrl_NONE;
	VideoAttr.EnCodeProfile = Profile_Main;
	VideoAttr.EnCodeType = StreamCompress_H264;
	VideoAttr.FrameCount = 0;
	VideoAttr.FrameRate = 25;
	VideoAttr.Size = Resolution_Res1280_720;
	VideoAttr.SliceType = VideoSliceType_IDR;
	VideoAttr.FrameSec = 0;
	VideoAttr.FrameUsec = 0;



	g_StreamManager.StreamVideoReset(Channel, Slave);
	g_StreamManager.StreamVideoAttr(Channel, Slave, VideoAttr);
	g_StreamManager.StreamVideoAppend(Channel, Slave, (char *)AvPacket->GetRawBuffer(), AvPacket->GetRawLength(), 0x01);

	return 0;
}



int OpenStream(int Channel, int Slave, StreamContent AudioVideo)
{
	av_msg("%s\n", __FUNCTION__);
	Capture *pCapture = g_AvManCapture.GetAvCaptureInstance(Channel);

	pCapture->Start((av_uchar)Slave, &(g_LocalStream), (Capture::SIG_PROC_ONDATA)&CStream::OnVideoData);

	return 0;
}

int CloseStream(int Channel, int Slave, StreamContent AudioVideo)
{
	av_msg("%s\n", __FUNCTION__);
	Capture *pCapture = g_AvManCapture.GetAvCaptureInstance(Channel);

	pCapture->Stop((av_uchar)Slave, &(g_LocalStream), (Capture::SIG_PROC_ONDATA)&CStream::OnVideoData);

	return 0;
}



SINGLETON_IMPLEMENT(CAvMoon)

CAvMoon * CAvMoon::m_SearchService = NULL;
CAvMoon::CAvMoon()
{
	
	av_msg("%s\n", __FUNCTION__);

}
CAvMoon::~CAvMoon()
{
	av_msg("%s\n", __FUNCTION__);
}
int CAvMoon::StartMoon()
{
	av_msg("%s\n", __FUNCTION__);

	
	g_StreamManager.RegisterOpenStream(OpenStream);
	g_StreamManager.RegisterCloseStream(CloseStream);


	MoonService("0.0.0.0", 5000);
	m_SearchService = new CAvMoon;
	m_SearchService->MoonSearchService();

	return 0;
}
int CAvMoon::StopMoon()
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}

#if 0
av_bool CAvMoon::Connect(C_ConnectArgs &ConArgs)
{
	MoonConnect(ConArgs.address.c_str(), atoi(ConArgs.port.c_str()), ConArgs.username.c_str(), ConArgs.passwd.c_str());
	return av_true;
}
av_bool CAvMoon::DisConnect()
{
	return av_true;
}
av_bool CAvMoon::StartRemoteStream(av_ushort Channel, av_uchar Slave)
{
	C_StreamStart StreamArgs;
	StreamArgs.Channel = Channel;
	StreamArgs.Slave = Slave;
	StreamArgs.StreamContentMask = StreamContent_VIDEO | StreamContent_AUDIO;;
	StreamArgs.Tmode = TransportMode_PULL;
	RemoteGetStream(StreamArgs);
	return av_true;
}
av_bool CAvMoon::StopRemoteStream()
{
	return av_true;
}
av_bool CAvMoon::GetDeviceInfo(C_DeviceInfo &DeviceInfo)
{
	return av_true;
}
av_bool CAvMoon::SetDeviceInfo(C_DeviceInfo &DeviceInfo)
{
	return av_true;
}


//以下两个函数需要走搜索 相关接口。
av_bool CAvMoon::SearchDevice(std::list<C_DeviceList> &DeviceList)
{
	std::list<C_DevSearch > DevList;
	C_DeviceList NetCaptureDevice;
	char Temp[32];

	m_SearchService->MoonSearchStart(3, DevList);
	std::list<C_DevSearch>::iterator i;
	for (i = DevList.begin(); i != DevList.end(); i++){
		NetCaptureDevice.Address = std::string(i->NetWorkProfile.Ipv4);
		NetCaptureDevice.DeviceName = std::string(i->DevStatusInfo.devname);
		NetCaptureDevice.Dns1 = std::string(i->NetWorkProfile.DnsI);
		NetCaptureDevice.Dns2 = std::string(i->NetWorkProfile.DnsII);
		NetCaptureDevice.GateWay = std::string(i->NetWorkProfile.GateWay);
		sprintf(Temp, "%d", i->ManufacturerInfo.ChannelMax);
		NetCaptureDevice.MaxChannel = std::string(Temp);
		NetCaptureDevice.NetMask = std::string(i->NetWorkProfile.SubMask);
		sprintf(Temp, "%d", i->NetWorkProfile.ServicePort);
		NetCaptureDevice.ServicePort = std::string(Temp);
		DeviceList.push_back(NetCaptureDevice);
	}
	return av_true;
}
av_bool CAvMoon::SetNetParam(C_NetParam &NetParam)
{
	return av_true;
}
#endif


int CAvMoon::OnRemoteGetDeviceCaps(C_DeviceCaps &DeviceCaps, int status)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::OnRemoteGetDeviceStatus(C_DevStatusInfo &DevStatusInfo, int status)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::OnRemoteGetDeCodeProfile(int status)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::OnRemoteSetDeCodeProfile(int status)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}


int CAvMoon::OnRemoteGetEnCodeCaps(C_EnCodeCaps &Encodecaps, int status)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::OnRemoteGetImageCaps(C_ImageCaps &ImageCaps, int status)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::OnRemoteGetCaptureCaps(C_CaptureCaps &CaptureCaps, int status)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}

int CAvMoon::OnRemoteGetImageProfile(C_ImageProfile &ImageProfile, int status)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::OnRemoteGetCaptureProfile(C_CaptureProfile &CaptureProfile, int status)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::OnRemoteGetEnCodeProfile(C_EnCodeProfile &EnCodeProfile, int status)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}

int CAvMoon::OnRemoteSetImageProfile(C_ImageProfile ImageProfile, int status)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::OnRemoteSetCaptureProfile(C_CaptureProfile &CaptureProfile, int status)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::OnRemoteSetEnCodeProfile(C_EnCodeProfile &EnCodeProfile, int status)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}


int CAvMoon::OnRemoteGetAlarmCaps(int status)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::OnRemoteGetAlarmPorfile(int status)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::OnRemoteGetSysTimeProfile(int status)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::OnRemoteStreamData(CStreamPacket *pStream)
{
	av_msg("%s\n", __FUNCTION__);
	char *Streamdata;
	int  StreamLen;
	pStream->StreamPacketGet(Streamdata, StreamLen);


	CAvPacket *AvPacket = g_AvPacketManager.GetAvPacket(StreamLen);

	AvPacket->PutBuffer((av_uchar *)Streamdata, StreamLen);
	AvPacket->PutBufferOver();


	AvPacket->Release();
	return 0;
}
int CAvMoon::OnRemoteStreamStart(C_StreamStart &StreamStart, COMMAND_E ReqOrResp)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::OnRemoteStreamStop(C_StreamStop &StreamStop, COMMAND_E ReqOrResp)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}

int CAvMoon::OnRemoteFirmwareUpgradeProgress(C_FirmwareUpgradeProgress &FirmwareUpgradeProgress, int status)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}

int CAvMoon::LocalGetDeviceCaps(C_DeviceCaps &DeviceCaps)
{
	av_msg("%s\n", __FUNCTION__);
	C_EncodeCaps EncodeCaps;
	CAvDevice::GetCaptureCaps(0, EncodeCaps);
	DeviceCaps.EnDeCode = false;
	DeviceCaps.EnEnCode = true;

	if (EncodeCaps.CompMask & AvMask(AvComp_MJPEG)){
		DeviceCaps.EncodeType |= ProtoMask(StreamCompress_MJPEG);
	}
	if (EncodeCaps.CompMask & AvMask(AvComp_H264)){
		DeviceCaps.EncodeType |= ProtoMask(StreamCompress_H264);
	}
	if (EncodeCaps.CompMask & AvMask(AvComp_H265)){
		DeviceCaps.EncodeType |= ProtoMask(StreamCompress_H265);
	}
	if (EncodeCaps.CompMask & AvMask(AvComp_JPEG)){
		DeviceCaps.EncodeType |= ProtoMask(StreamCompress_JPEG);
	}
	DeviceCaps.MaxChannel = 1;
	DeviceCaps.MaxSlave = 3;
	return 0;
}
int CAvMoon::LocalGetDeCodeCaps()
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::LocalGetDeCodeProfile()
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}



int CAvMoon::LocalGetImageCaps(C_ImageCaps &ImageCaps)
{
	av_msg("%s\n", __FUNCTION__);
	C_ImageQualityCaps ImageQualityCaps;
	CAvDevice::GetImageCaps(ImageCaps.Channel, ImageQualityCaps);

	ImageCaps.Brightness.start = 0;
	ImageCaps.Brightness.end = ImageQualityCaps.MaxRange;

	ImageCaps.Chroma.start = 0;
	ImageCaps.Chroma.end = ImageQualityCaps.MaxRange;

	ImageCaps.Contrast.start = 0;
	ImageCaps.Contrast.end = ImageQualityCaps.MaxRange;

	ImageCaps.Saturation.start = 0;
	ImageCaps.Saturation.end = ImageQualityCaps.MaxRange;
	return 0;
}
int CAvMoon::LocalGetImageProfile(C_ImageProfile &ImageProfile)
{
	av_msg("%s\n", __FUNCTION__);
	CAvConfigImage ConfigImage;
	ConfigImage.Update(ImageProfile.Channel);
	ConfigImageFormats &Fromats = ConfigImage.GetConfig(ImageProfile.Channel);
	ImageProfile.Bright = Fromats.Brightness;
	ImageProfile.Chroma = Fromats.Hue;
	ImageProfile.Contrast = Fromats.Contrast;
	ImageProfile.Saturation = Fromats.Saturation;
	return 0;
}
int CAvMoon::LocalSetImageProfile(C_ImageProfile &ImageProfile)
{
	av_msg("%s\n", __FUNCTION__);
	CAvConfigImage ConfigImage;
	ConfigImage.Update(ImageProfile.Channel);
	ConfigImageFormats &Fromats = ConfigImage.GetConfig(ImageProfile.Channel);

	Fromats.Brightness	= ImageProfile.Bright;
	Fromats.Contrast	= ImageProfile.Contrast;
	Fromats.Saturation	= ImageProfile.Saturation;
	Fromats.Hue			= ImageProfile.Chroma;
	
	ConfigImage.SettingUp();

	return 0;
}


int CAvMoon::LocalGetVideoCoverProfile(C_VideoCoverProfile &VideoCoverProfile)
{
	av_msg("%s\n", __FUNCTION__);
	CAvConfigCover ConfigCover;
	ConfigCover.Update(VideoCoverProfile.Channel);
	ConfigCoverFormats &Formats = ConfigCover.GetConfig();

	VideoCoverProfile.VideoCoverMax = 4;
	VideoCoverProfile.VideoCoverEnable = ProtoSwitch_OPEN;
	C_Zone Zone;
	for (int i = 0; i < ConfMaxCover; i++){
		if (Formats.CHLFormats[i].enable == av_true){
			Zone.start_x = Formats.CHLFormats[i].rect.left;
			Zone.start_y = Formats.CHLFormats[i].rect.top;
			Zone.end_x = Formats.CHLFormats[i].rect.right;
			Zone.end_y = Formats.CHLFormats[i].rect.bottom;
			VideoCoverProfile.VideoCoverZone.push_back(Zone);
		}

	}

	return 0;
}
int CAvMoon::LocalSetVideoCoverProfile(C_VideoCoverProfile &VideoCoverProfile)
{
	CAvConfigCover ConfigCover;
	ConfigCover.Update(VideoCoverProfile.Channel);
	ConfigCoverFormats &Formats = ConfigCover.GetConfig();
	
	std::list<C_Zone>::iterator iList;
	int i = 0;
	for (iList = VideoCoverProfile.VideoCoverZone.begin(); iList != VideoCoverProfile.VideoCoverZone.end(); iList++){
		Formats.CHLFormats[i].enable = av_true;
		Formats.CHLFormats[i].rect.left = iList->start_x;
		Formats.CHLFormats[i].rect.top = iList->start_y;
		Formats.CHLFormats[i].rect.right = iList->end_x;
		Formats.CHLFormats[i].rect.bottom = iList->end_y;
		i++;
	}

	for (; i < ConfMaxCover; i++){
		Formats.CHLFormats[i].enable = av_false;
	}
	ConfigCover.SettingUp();

	return 0;
}


int CAvMoon::LocalGetCaptureCaps(C_CaptureCaps &CaptureCaps)
{
	av_msg("%s\n", __FUNCTION__);
	C_CaptureInCaps CaptureInCaps;
	CAvDevice::GetCaputreInCaps(CaptureCaps.Channel, CaptureInCaps);

	if (CaptureInCaps.AntiFlckerMask & AvMask(AvAntiFlckerMode_INDOOR_50HZ)){
		CaptureCaps.AntiFlckerMask |= ProtoMask(AntiFlckerMode_INDOOR_50HZ);
	}
	if (CaptureInCaps.AntiFlckerMask & AvMask(AvAntiFlckerMode_OUTDOOR_50HZ)){
		CaptureCaps.AntiFlckerMask |= ProtoMask(AntiFlckerMode_OUTDOOR_50HZ);
	}
	if (CaptureInCaps.AntiFlckerMask & AvMask(AvAntiFlckerMode_AUTO_50HZ)){
		CaptureCaps.AntiFlckerMask |= ProtoMask(AntiFlckerMode_AUTO_50HZ);
	}
	if (CaptureInCaps.AntiFlckerMask & AvMask(AvAntiFlckerMode_INDOOR_60HZ)){
		CaptureCaps.AntiFlckerMask |= ProtoMask(AntiFlckerMode_INDOOR_60HZ);
	}
	if (CaptureInCaps.AntiFlckerMask & AvMask(AvAntiFlckerMode_OUTDOOR_60HZ)){
		CaptureCaps.AntiFlckerMask |= ProtoMask(AntiFlckerMode_OUTDOOR_60HZ);
	}
	if (CaptureInCaps.AntiFlckerMask & AvMask(AvAntiFlckerMode_AUTO_60HZ)){
		CaptureCaps.AntiFlckerMask |= ProtoMask(AntiFlckerMode_AUTO_60HZ);
	}
	if (CaptureInCaps.AntiFlckerMask & AvMask(AvAntiFlckerMode_THEATER_50HZ)){
		CaptureCaps.AntiFlckerMask |= ProtoMask(AntiFlckerMode_THEATER_50HZ);
	}
	if (CaptureInCaps.AntiFlckerMask & AvMask(AvAntiFlckerMode_FAST_50HZ)){
		CaptureCaps.AntiFlckerMask |= ProtoMask(AntiFlckerMode_FAST_50HZ);
	}
	if (CaptureInCaps.AntiFlckerMask & AvMask(AvAntiFlckerMode_THEATER_60HZ)){
		CaptureCaps.AntiFlckerMask |= ProtoMask(AntiFlckerMode_THEATER_60HZ);
	}
	if (CaptureInCaps.AntiFlckerMask & AvMask(AvAntiFlckerMode_FAST_60HZ)){
		CaptureCaps.AntiFlckerMask |= ProtoMask(AntiFlckerMode_FAST_60HZ);
	}
	if (CaptureInCaps.AntiFlckerMask & AvMask(AvAntiFlckerMode_ANTI_FLICKER_CLOSED)){
		CaptureCaps.AntiFlckerMask |= ProtoMask(AntiFlckerMode_ANTI_FLICKER_CLOSED);
	}

	if (CaptureInCaps.IrCutModeMask & AvMask(IRCUT_OPEN)){
		CaptureCaps.IrCutMask |= ProtoMask(IrCutMode_OPEN);
	}
	if (CaptureInCaps.IrCutModeMask & AvMask(IRCUT_CLOSE)){
		CaptureCaps.IrCutMask |= ProtoMask(IrCutMode_CLOSE);
	}

	if (CaptureInCaps.IrCutModeMask & AvMask(IRCUT_AUTO)){
		CaptureCaps.IrCutMask |= ProtoMask(IrCutMode_AUTO);
	}
	if (CaptureInCaps.IrCutModeMask & AvMask(IRCUT_TIMER)){
		CaptureCaps.IrCutMask |= ProtoMask(IrCutMode_TIMER);
	}

	if (CaptureInCaps.VideoReverseMask & AvMask(VIDEOREVERSE_HOR)){
		CaptureCaps.MirrorMask |= ProtoMask(MirrorMode_HOR);
	}
	if (CaptureInCaps.VideoReverseMask & AvMask(VIDEOREVERSE_VER)){
		CaptureCaps.MirrorMask |= ProtoMask(MirrorMode_VER);
	}
	if (CaptureInCaps.VideoReverseMask & AvMask(VIDEOREVERSE_90)){
		CaptureCaps.MirrorMask |= ProtoMask(MirrorMode_HALFMIRROR);
	}

	if (CaptureInCaps.ExposureMask & AvMask(AvExposureMode_AUTO)){
		CaptureCaps.ExposureMask |= ProtoMask(ExposureMode_AUTO);
	}
	if (CaptureInCaps.ExposureMask & AvMask(AvExposureMode_MANUAL)){
		 CaptureCaps.ExposureMask |= ProtoMask(ExposureMode_MANUAL);
	}
	if (CaptureInCaps.ExposureMask & AvMask(AvExposureMode_TRAFFIC)){
		 CaptureCaps.ExposureMask |= ProtoMask(ExposureMode_TRAFFIC);
	}

	if (CaptureInCaps.WhiteBalanceMask & AvMask(AvWhiteBalanceMode_OFF)){
		CaptureCaps.WhiteBalanceMask |= ProtoMask(WhiteBalanceMode_OFF);
	}
	if (CaptureInCaps.WhiteBalanceMask & AvMask(AvWhiteBalanceMode_AUTO)){
		 CaptureCaps.WhiteBalanceMask |= ProtoMask(WhiteBalanceMode_AUTO);
	}
	if (CaptureInCaps.WhiteBalanceMask & AvMask(AvWhiteBalanceMode_DAYTIME)){
		 CaptureCaps.WhiteBalanceMask |= ProtoMask(WhiteBalanceMode_DAYTIME);
	}
	if (CaptureInCaps.WhiteBalanceMask & AvMask(AvWhiteBalanceMode_EVENING)){
		 CaptureCaps.WhiteBalanceMask |= ProtoMask(WhiteBalanceMode_EVENING);
	}
	if (CaptureInCaps.WhiteBalanceMask & AvMask(AvWhiteBalanceMode_CLOUDY)){
		 CaptureCaps.WhiteBalanceMask |= ProtoMask(WhiteBalanceMode_CLOUDY);
	}
	if (CaptureInCaps.WhiteBalanceMask & AvMask(AvWhiteBalanceMode_OFFICE)){
		 CaptureCaps.WhiteBalanceMask |= ProtoMask(WhiteBalanceMode_OFFICE);
	}
	if (CaptureInCaps.WhiteBalanceMask & AvMask(AvWhiteBalanceMode_FLUORESCENT)){
		 CaptureCaps.WhiteBalanceMask |= ProtoMask(WhiteBalanceMode_FLUORESCENT);
	}
	if (CaptureInCaps.WhiteBalanceMask & AvMask(AvWhiteBalanceMode_INCANDESCENT)){
		 CaptureCaps.WhiteBalanceMask |= ProtoMask(WhiteBalanceMode_INCANDESCENT);
	}
	if (CaptureInCaps.WhiteBalanceMask & AvMask(AvWhiteBalanceMode_MANUAL)){
		 CaptureCaps.WhiteBalanceMask |= ProtoMask(WhiteBalanceMode_MANUAL);
	}

	CaptureCaps.WhiteBalanceValueRegion.start = 1;
	CaptureCaps.WhiteBalanceValueRegion.end = 100000;

	CaptureCaps.AntiFlckerValueRegion.start = 1;
	CaptureCaps.AntiFlckerValueRegion.end = 100000;

	CaptureCaps.ExposureValueRegion.start = 1;
	CaptureCaps.ExposureValueRegion.end = 100000;

	return 0;
}
int CAvMoon::LocalGetCaptureProfile(C_CaptureProfile &CaptureProfile)
{
	av_msg("%s\n", __FUNCTION__);
	CAvConfigCapture ConfigCapture;
	ConfigCapture.Update(CaptureProfile.Channel);
	ConfigCaptureFormats &Formats = ConfigCapture.GetConfig(CaptureProfile.Channel);
	switch (Formats.AntiFlckerMode)
	{
	case AvAntiFlckerMode_INDOOR_50HZ:
		CaptureProfile.AntiFlcker = AntiFlckerMode_INDOOR_50HZ;
		break;
	case AvAntiFlckerMode_OUTDOOR_50HZ:
		CaptureProfile.AntiFlcker = AntiFlckerMode_OUTDOOR_50HZ;
		break;
	case AvAntiFlckerMode_AUTO_50HZ:
		CaptureProfile.AntiFlcker = AntiFlckerMode_AUTO_50HZ;
		break;
	case AvAntiFlckerMode_INDOOR_60HZ:
		CaptureProfile.AntiFlcker = AntiFlckerMode_INDOOR_60HZ;
		break;
	case AvAntiFlckerMode_OUTDOOR_60HZ:
		CaptureProfile.AntiFlcker = AntiFlckerMode_OUTDOOR_60HZ;
		break;
	case AvAntiFlckerMode_AUTO_60HZ:
		CaptureProfile.AntiFlcker = AntiFlckerMode_AUTO_60HZ;
		break;
	case AvAntiFlckerMode_THEATER_50HZ:
		CaptureProfile.AntiFlcker = AntiFlckerMode_THEATER_50HZ;
		break;
	case AvAntiFlckerMode_FAST_50HZ:
		CaptureProfile.AntiFlcker = AntiFlckerMode_FAST_50HZ;
		break;
	case AvAntiFlckerMode_THEATER_60HZ:
		CaptureProfile.AntiFlcker = AntiFlckerMode_THEATER_60HZ;
		break;
	case AvAntiFlckerMode_FAST_60HZ:
		CaptureProfile.AntiFlcker = AntiFlckerMode_FAST_60HZ;
		break;
	case AvAntiFlckerMode_ANTI_FLICKER_CLOSED:
		CaptureProfile.AntiFlcker = AntiFlckerMode_ANTI_FLICKER_CLOSED;
		break;

	default:
		break;
	}

	switch (Formats.ExposureMode)
	{
	case AvExposureMode_AUTO:
		CaptureProfile.Exposure = ExposureMode_AUTO;
		break;
	case 	AvExposureMode_MANUAL:
		CaptureProfile.Exposure = ExposureMode_MANUAL;
		break;
	case 	AvExposureMode_TRAFFIC:
		CaptureProfile.Exposure = ExposureMode_TRAFFIC;
		break;

	default:
		break;
	}

	switch (Formats.WhiteBalanceMode)
	{
	case AvWhiteBalanceMode_OFF:
		CaptureProfile.WhiteBalance = WhiteBalanceMode_OFF;
		break;
	case AvWhiteBalanceMode_AUTO:
		CaptureProfile.WhiteBalance = WhiteBalanceMode_AUTO;
		break;
	case AvWhiteBalanceMode_DAYTIME:
		CaptureProfile.WhiteBalance = WhiteBalanceMode_DAYTIME;
		break;
	case AvWhiteBalanceMode_EVENING:
		CaptureProfile.WhiteBalance = WhiteBalanceMode_EVENING;
		break;
	case AvWhiteBalanceMode_CLOUDY:
		CaptureProfile.WhiteBalance = WhiteBalanceMode_CLOUDY;
		break;
	case AvWhiteBalanceMode_OFFICE:
		CaptureProfile.WhiteBalance = WhiteBalanceMode_OFFICE;
		break;
	case AvWhiteBalanceMode_FLUORESCENT:
		CaptureProfile.WhiteBalance = WhiteBalanceMode_FLUORESCENT;
		break;
	case AvWhiteBalanceMode_INCANDESCENT:
		CaptureProfile.WhiteBalance = WhiteBalanceMode_INCANDESCENT;
		break;
	case AvWhiteBalanceMode_MANUAL:
		CaptureProfile.WhiteBalance = WhiteBalanceMode_MANUAL;
		break;
	default:
		break;
	}

	if (Formats.Reverse90 == av_true){
		CaptureProfile.MirrorMaskValue |= ProtoMask(MirrorMode_HALFMIRROR);
	}
	if (Formats.ReverseHor == av_true){
		CaptureProfile.MirrorMaskValue |= ProtoMask(MirrorMode_HOR);
	}
	if (Formats.ReverseVer == av_true){
		CaptureProfile.MirrorMaskValue |= ProtoMask(MirrorMode_VER);
	}

	switch (Formats.IrCutMode)
	{
	case IRCUT_OPEN:
		CaptureProfile.IrCut = IrCutMode_OPEN;
		break;
	case IRCUT_CLOSE:
		CaptureProfile.IrCut = IrCutMode_CLOSE;
		break;
	case IRCUT_AUTO:
		CaptureProfile.IrCut = IrCutMode_AUTO;
		break;
	case IRCUT_TIMER:
		CaptureProfile.IrCut = IrCutMode_TIMER;
			break;
	default:
		break;
	}


	return 0;
}
int CAvMoon::LocalSetCaptureProfile(C_CaptureProfile &CaptureProfile)
{
	av_msg("%s\n", __FUNCTION__);

	CAvConfigCapture ConfigCapture;
	ConfigCapture.Update(CaptureProfile.Channel);
	ConfigCaptureFormats &Formats = ConfigCapture.GetConfig(CaptureProfile.Channel);
	switch (CaptureProfile.AntiFlcker)
	{
	case AntiFlckerMode_INDOOR_50HZ:
		Formats.AntiFlckerMode = AvAntiFlckerMode_INDOOR_50HZ;
		break;
	case AntiFlckerMode_OUTDOOR_50HZ:
		Formats.AntiFlckerMode = AvAntiFlckerMode_OUTDOOR_50HZ;
		break;
	case AntiFlckerMode_AUTO_50HZ:
		Formats.AntiFlckerMode = AvAntiFlckerMode_AUTO_50HZ;
		break;
	case AntiFlckerMode_INDOOR_60HZ:
		Formats.AntiFlckerMode = AvAntiFlckerMode_INDOOR_60HZ;
		break;
	case AntiFlckerMode_OUTDOOR_60HZ:
		Formats.AntiFlckerMode = AvAntiFlckerMode_OUTDOOR_60HZ;
		break;
	case AntiFlckerMode_AUTO_60HZ:
		Formats.AntiFlckerMode = AvAntiFlckerMode_AUTO_60HZ;
		break;
	case AntiFlckerMode_THEATER_50HZ:
		Formats.AntiFlckerMode = AvAntiFlckerMode_THEATER_50HZ;
		break;
	case AntiFlckerMode_FAST_50HZ:
		Formats.AntiFlckerMode = AvAntiFlckerMode_FAST_50HZ;
		break;
	case AntiFlckerMode_THEATER_60HZ:
		Formats.AntiFlckerMode = AvAntiFlckerMode_THEATER_60HZ;
		break;
	case AntiFlckerMode_FAST_60HZ:
		Formats.AntiFlckerMode = AvAntiFlckerMode_FAST_60HZ;
		break;
	case AntiFlckerMode_ANTI_FLICKER_CLOSED:
		Formats.AntiFlckerMode = AvAntiFlckerMode_ANTI_FLICKER_CLOSED;
		break;

	default:
		break;
	}

	switch (CaptureProfile.Exposure)
	{
	case ExposureMode_AUTO:
		Formats.ExposureMode = AvExposureMode_AUTO;
		break;
	case ExposureMode_MANUAL:
		Formats.ExposureMode = AvExposureMode_MANUAL;
		break;
	case ExposureMode_TRAFFIC:
		Formats.ExposureMode = AvExposureMode_TRAFFIC;
		break;

	default:
		break;
	}

	switch (CaptureProfile.WhiteBalance)
	{
	case WhiteBalanceMode_OFF:
		Formats.WhiteBalanceMode = AvWhiteBalanceMode_OFF;
		break;
	case WhiteBalanceMode_AUTO:
		Formats.WhiteBalanceMode = AvWhiteBalanceMode_AUTO;
		break;
	case WhiteBalanceMode_DAYTIME:
		Formats.WhiteBalanceMode = AvWhiteBalanceMode_DAYTIME;
		break;
	case WhiteBalanceMode_EVENING:
		Formats.WhiteBalanceMode = AvWhiteBalanceMode_EVENING;
		break;
	case WhiteBalanceMode_CLOUDY:
		Formats.WhiteBalanceMode = AvWhiteBalanceMode_CLOUDY;
		break;
	case WhiteBalanceMode_OFFICE:
		Formats.WhiteBalanceMode = AvWhiteBalanceMode_OFFICE;
		break;
	case WhiteBalanceMode_FLUORESCENT:
		Formats.WhiteBalanceMode = AvWhiteBalanceMode_FLUORESCENT;
		break;
	case WhiteBalanceMode_INCANDESCENT:
		Formats.WhiteBalanceMode = AvWhiteBalanceMode_INCANDESCENT;
		break;
	case WhiteBalanceMode_MANUAL:
		Formats.WhiteBalanceMode = AvWhiteBalanceMode_MANUAL;
		break;
	default:
		break;
	}

	if (CaptureProfile.MirrorMaskValue & ProtoMask(MirrorMode_HALFMIRROR)){
		Formats.Reverse90 = av_true;
	}
	else{
		Formats.Reverse90 = av_false;
	}
	if (CaptureProfile.MirrorMaskValue & ProtoMask(MirrorMode_HOR)){
		 Formats.ReverseHor = av_true;
	}
	else{
		Formats.ReverseHor = av_false;
	}
	if (CaptureProfile.MirrorMaskValue & ProtoMask(MirrorMode_VER)){
		Formats.ReverseVer = av_true;
	}
	else{
		Formats.ReverseVer = av_false;
	}

	switch (CaptureProfile.IrCut)
	{
	case IrCutMode_OPEN :
		Formats.IrCutMode = IRCUT_OPEN;
		break;
	case IrCutMode_CLOSE:
		Formats.IrCutMode = IRCUT_CLOSE ;
		break;
	case IrCutMode_AUTO:
		Formats.IrCutMode = IRCUT_AUTO ;
		break;
	case IrCutMode_TIMER:
		Formats.IrCutMode = IRCUT_TIMER ;
		break;
	default:
		break;
	}
	ConfigCapture.SettingUp();
	return 0;
}

int CAvMoon::LocalGetEnCodeCaps(C_EnCodeCaps &EnCodeCaps)
{
	av_msg("%s\n", __FUNCTION__);
	C_EncodeCaps Caps = { 0 };
	CAvDevice::GetCaptureCaps(EnCodeCaps.Channel, Caps);
	if (EnCodeCaps.Slave == 0){
		EnCodeCaps.CapResolutionMask = 0;
		if (1 << CaptureSize_300W & Caps.ImageSizeMask){
			EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res2048_1536);
		}

		if (1 << CaptureSize_500W & Caps.ImageSizeMask){
			EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res2592_1944);
		}

		if (1 << CaptureSize_600W & Caps.ImageSizeMask){
			EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res3072_2048);
		}

		if (1 << CaptureSize_800W & Caps.ImageSizeMask){
			EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res3840_2160);
		}


		if (1 << CaptureSize_1080P & Caps.ImageSizeMask){
			EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res1920_1080);
		}

		if (1 << CaptureSize_720P & Caps.ImageSizeMask){
			EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res1280_720);
		}

		if (1 << CaptureSize_960P & Caps.ImageSizeMask){
			EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res1280_960);
		}

	}
	else {
		if (1 << CaptureSize_1080P & Caps.ImageSizeMask){
			if ((1 << CaptureSize_D1) & Caps.ExtImageSizeMask[CaptureSize_1080P]){
				EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res704_576);
			}
			if ((1 << CaptureSize_VGA) & Caps.ExtImageSizeMask[CaptureSize_1080P]){
				EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res640_480);
			}
			if ((1 << CaptureSize_QVGA) & Caps.ExtImageSizeMask[CaptureSize_1080P]){
				EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res320_240);
			}
			if ((1 << CaptureSize_VGAEX) & Caps.ExtImageSizeMask[CaptureSize_1080P]){
				EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res640_320);
			}
			if ((1 << CaptureSize_QVGAEX) & Caps.ExtImageSizeMask[CaptureSize_1080P]){
				EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res320_160);
			}
		}
		else if (1 << CaptureSize_960P & Caps.ImageSizeMask){
			if ((1 << CaptureSize_D1) & Caps.ExtImageSizeMask[CaptureSize_960P]){
				EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res704_576);
			}
			if ((1 << CaptureSize_VGA) & Caps.ExtImageSizeMask[CaptureSize_960P]){
				EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res640_480);
			}
			if ((1 << CaptureSize_QVGA) & Caps.ExtImageSizeMask[CaptureSize_960P]){
				EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res320_240);
			}
			if ((1 << CaptureSize_VGAEX) & Caps.ExtImageSizeMask[CaptureSize_960P]){
				EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res640_320);
			}
			if ((1 << CaptureSize_QVGAEX) & Caps.ExtImageSizeMask[CaptureSize_960P]){
				EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res320_160);
			}
		}
		else if (1 << CaptureSize_300W & Caps.ImageSizeMask){
			if ((1 << CaptureSize_D1) & Caps.ExtImageSizeMask[CaptureSize_300W]){
				EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res704_576);
			}
			if ((1 << CaptureSize_VGA) & Caps.ExtImageSizeMask[CaptureSize_300W]){
				EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res640_480);
			}
			if ((1 << CaptureSize_QVGA) & Caps.ExtImageSizeMask[CaptureSize_300W]){
				EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res320_240);
			}
			if ((1 << CaptureSize_VGAEX) & Caps.ExtImageSizeMask[CaptureSize_300W]){
				EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res640_320);
			}
			if ((1 << CaptureSize_QVGAEX) & Caps.ExtImageSizeMask[CaptureSize_300W]){
				EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res320_160);
			}
		}
		else if (1 << CaptureSize_500W & Caps.ImageSizeMask){
			if ((1 << CaptureSize_D1) & Caps.ExtImageSizeMask[CaptureSize_500W]){
				EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res704_576);
			}
			if ((1 << CaptureSize_VGA) & Caps.ExtImageSizeMask[CaptureSize_500W]){
				EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res640_480);
			}
			if ((1 << CaptureSize_QVGA) & Caps.ExtImageSizeMask[CaptureSize_500W]){
				EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res320_240);
			}
			if ((1 << CaptureSize_VGAEX) & Caps.ExtImageSizeMask[CaptureSize_500W]){
				EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res640_320);
			}
			if ((1 << CaptureSize_QVGAEX) & Caps.ExtImageSizeMask[CaptureSize_500W]){
				EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res320_160);
			}
		}
		else if (1 << CaptureSize_600W & Caps.ImageSizeMask){
			if ((1 << CaptureSize_D1) & Caps.ExtImageSizeMask[CaptureSize_600W]){
				EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res704_576);
			}
			if ((1 << CaptureSize_VGA) & Caps.ExtImageSizeMask[CaptureSize_600W]){
				EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res640_480);
			}
			if ((1 << CaptureSize_QVGA) & Caps.ExtImageSizeMask[CaptureSize_600W]){
				EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res320_240);
			}
			if ((1 << CaptureSize_VGAEX) & Caps.ExtImageSizeMask[CaptureSize_600W]){
				EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res640_320);
			}
			if ((1 << CaptureSize_QVGAEX) & Caps.ExtImageSizeMask[CaptureSize_600W]){
				EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res320_160);
			}
		}
		else if (1 << CaptureSize_800W & Caps.ImageSizeMask){
			if ((1 << CaptureSize_D1) & Caps.ExtImageSizeMask[CaptureSize_800W]){
				EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res704_576);
			}
			if ((1 << CaptureSize_VGA) & Caps.ExtImageSizeMask[CaptureSize_800W]){
				EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res640_480);
			}
			if ((1 << CaptureSize_QVGA) & Caps.ExtImageSizeMask[CaptureSize_800W]){
				EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res320_240);
			}
			if ((1 << CaptureSize_VGAEX) & Caps.ExtImageSizeMask[CaptureSize_800W]){
				EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res640_320);
			}
			if ((1 << CaptureSize_QVGAEX) & Caps.ExtImageSizeMask[CaptureSize_800W]){
				EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res320_160);
			}
		}
		
		
		
		else{
			if ((1 << CaptureSize_D1) & Caps.ExtImageSizeMask[CaptureSize_720P]){
				EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res704_576);
			}
			if ((1 << CaptureSize_VGA) & Caps.ExtImageSizeMask[CaptureSize_720P]){
				EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res640_480);
			}
			if ((1 << CaptureSize_QVGA) & Caps.ExtImageSizeMask[CaptureSize_720P]){
				EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res320_240);
			}
			if ((1 << CaptureSize_VGAEX) & Caps.ExtImageSizeMask[CaptureSize_720P]){
				EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res640_320);
			}
			if ((1 << CaptureSize_QVGAEX) & Caps.ExtImageSizeMask[CaptureSize_720P]){
				EnCodeCaps.CapResolutionMask |= ProtoMask(Resolution_Res320_160);
			}
		}
	}
	if (EnCodeCaps.Slave == 0){
		EnCodeCaps.CapCbrRate.start = 512;
		EnCodeCaps.CapCbrRate.end = 8192;

	}
	else{

		EnCodeCaps.CapCbrRate.start = 64;
		EnCodeCaps.CapCbrRate.end = 1024;

	}

	EnCodeCaps.CapEncodeProfileMask = ProtoMask(Profile_High) | ProtoMask(Profile_Main) | ProtoMask(Profile_BaseLine);
	EnCodeCaps.CapFrameRate.start = 3;
	EnCodeCaps.CapFrameRate.end = 30;

	EnCodeCaps.CapGop.start = 5;
	EnCodeCaps.CapGop.end = 100;
	if (0x01 << AvBitRate_CBR & Caps.BitRateCtrlMask){
		EnCodeCaps.CapStreamCvbrCtrlMask = ProtoMask(BitRateCtrl_BRC_CBR);
	}
	if (0x01 << AvBitRate_VBR & Caps.BitRateCtrlMask){
		EnCodeCaps.CapStreamCvbrCtrlMask |= ProtoMask(BitRateCtrl_BRC_VBR);
	}


	if (0x01 << AvComp_H265 & Caps.CompMask){
		EnCodeCaps.CapStreamEnCodeTypeMask |= ProtoMask(StreamCompress_H265);
	}
	if (0x01 << AvComp_H264 & Caps.CompMask){
		EnCodeCaps.CapStreamEnCodeTypeMask |= ProtoMask(StreamCompress_H264);
	}
	if (0x01 << AvComp_MJPEG & Caps.CompMask){
		EnCodeCaps.CapStreamEnCodeTypeMask |= ProtoMask(StreamCompress_MJPEG);
	}
	return 0;
}
int CAvMoon::LocalGetEnCodeProfile(C_EnCodeProfile &EnCodeProfile)
{
	av_msg("%s\n", __FUNCTION__);
	CAvConfigEncode ConfigEncode;
	ConfigEncode.Update(EnCodeProfile.Channel);
	ConfigEncodeFormats &Formats = ConfigEncode.GetConfig();
	E_EncodeCHL ch;
	if (EnCodeProfile.Slave == 2){
		ch = CHL_JPEG_T;
	}
	else{
		ch = (E_EncodeCHL)EnCodeProfile.Slave;
	}

	switch (Formats.CHLFormats[ch].Formats.ImageSize)
	{
	case CaptureSize_1080P:
		EnCodeProfile.Size = Resolution_Res1920_1080;
		break;
	case CaptureSize_300W:
		EnCodeProfile.Size = Resolution_Res2048_1536;
		break;
	case CaptureSize_400W:
		EnCodeProfile.Size = Resolution_LAST;
		break;
	case CaptureSize_500W:
		EnCodeProfile.Size = Resolution_Res2592_1944;
		break;
	case CaptureSize_720P:
		EnCodeProfile.Size = Resolution_Res1280_720;
		break;
	case CaptureSize_800W:
		EnCodeProfile.Size = Resolution_Res3840_2160;
		break;
	case CaptureSize_960P:
		EnCodeProfile.Size = Resolution_Res1280_960;
		break;
	case CaptureSize_D1:
		EnCodeProfile.Size = Resolution_Res704_576;
		break;
	case CaptureSize_QVGA:
		EnCodeProfile.Size = Resolution_Res320_240;
		break;
	case CaptureSize_QVGAEX:
		EnCodeProfile.Size = Resolution_Res320_160;
		break;
	case CaptureSize_VGA:
		EnCodeProfile.Size = Resolution_Res640_480;
		break;
	case CaptureSize_VGAEX:
		EnCodeProfile.Size = Resolution_Res640_320;
		break;
	case CaptureSize_600W:
		EnCodeProfile.Size = Resolution_Res3072_2048;
		break;
	case CaptureSize_4K:
		EnCodeProfile.Size = Resolution_Res4096_2160;

			break;
	default:
		break;
	}

	EnCodeProfile.Brc = Formats.CHLFormats[ch].Formats.BitRateCtrl == AvBitRate_CBR ? BitRateCtrl_BRC_CBR : BitRateCtrl_BRC_VBR;
	EnCodeProfile.BitRate = Formats.CHLFormats[ch].Formats.BitRateValue;
	EnCodeProfile.BitLevel = Formats.CHLFormats[ch].Formats.BitRateValue;
	EnCodeProfile.EnCodeProfile = Profile_High;
	

	switch (Formats.CHLFormats[ch].Formats.Comp)
	{
	case AvComp_H264:
		EnCodeProfile.EnCodeType = StreamCompress_H264;
		break;
	case AvComp_H265:
		EnCodeProfile.EnCodeType = StreamCompress_H265;
		break;
	case AvComp_JPEG:
		EnCodeProfile.EnCodeType = StreamCompress_JPEG;
		break;
	case AvComp_MJPEG:
		EnCodeProfile.EnCodeType = StreamCompress_MJPEG;
		break;
		default:
			break;
	}

	EnCodeProfile.FrameRate = Formats.CHLFormats[ch].Formats.FrameRate;
	EnCodeProfile.Gop = Formats.CHLFormats[ch].Formats.Gop;

	return 0;
}
int CAvMoon::LocalSetEnCodeProfile(C_EnCodeProfile &EnCodeProfile)
{
	av_msg("%s\n", __FUNCTION__);
	CAvConfigEncode ConfigEncode;
	ConfigEncode.Update(EnCodeProfile.Channel);
	ConfigEncodeFormats &Formats = ConfigEncode.GetConfig();
	E_EncodeCHL ch;
	if (EnCodeProfile.Slave == 2){
		ch = CHL_JPEG_T;
	}
	else{
		ch = (E_EncodeCHL)EnCodeProfile.Slave;
	}

	switch (EnCodeProfile.Size)
	{
	case Resolution_Res320_160:
		Formats.CHLFormats[ch].Formats.ImageSize = CaptureSize_QVGAEX;
		break;
	case Resolution_Res320_240:
		Formats.CHLFormats[ch].Formats.ImageSize = CaptureSize_QVGA;
		break;
		case Resolution_Res640_320:
			Formats.CHLFormats[ch].Formats.ImageSize = CaptureSize_VGAEX;
			break;
		case Resolution_Res640_480:
			Formats.CHLFormats[ch].Formats.ImageSize = CaptureSize_VGA;
			break;
		case Resolution_Res704_576:
			Formats.CHLFormats[ch].Formats.ImageSize = CaptureSize_D1;
			break;
		case Resolution_Res1280_720:
			Formats.CHLFormats[ch].Formats.ImageSize = CaptureSize_720P;
			break;
		case Resolution_Res1280_960:
			Formats.CHLFormats[ch].Formats.ImageSize = CaptureSize_960P;
			break;
		case Resolution_Res1920_1080:
			Formats.CHLFormats[ch].Formats.ImageSize = CaptureSize_1080P;
			break;
		case Resolution_Res2048_1536:
			Formats.CHLFormats[ch].Formats.ImageSize = CaptureSize_300W;
			break;
		case Resolution_Res2592_1944:
			Formats.CHLFormats[ch].Formats.ImageSize = CaptureSize_500W;
			break;
		case Resolution_Res3072_2048:
			Formats.CHLFormats[ch].Formats.ImageSize = CaptureSize_600W;
			break;
		case Resolution_Res3840_2160:
			Formats.CHLFormats[ch].Formats.ImageSize = CaptureSize_800W;
			break;
		case Resolution_Res4096_2160:
			Formats.CHLFormats[ch].Formats.ImageSize = CaptureSize_4K;
			break;
		case Resolution_Res7680_4320:
			Formats.CHLFormats[ch].Formats.ImageSize = CaptureSize_8K;
			break;
		default:
			break;
	}

	Formats.CHLFormats[ch].Formats.BitRateCtrl = EnCodeProfile.Brc == BitRateCtrl_BRC_CBR ? AvBitRate_CBR : AvBitRate_VBR;
	Formats.CHLFormats[ch].Formats.BitRateValue = EnCodeProfile.BitRate;
	Formats.CHLFormats[ch].Formats.FrameRate = EnCodeProfile.FrameRate > 0 ? EnCodeProfile.FrameRate : Formats.CHLFormats[ch].Formats.FrameRate;
	Formats.CHLFormats[ch].Formats.Gop = EnCodeProfile.Gop;
	Formats.CHLFormats[ch].Formats.Qlevel = EnCodeProfile.BitLevel;

	switch (EnCodeProfile.EnCodeType)
	{
	case 	StreamCompress_JPEG:
		Formats.CHLFormats[ch].Formats.Comp = AvComp_JPEG;
		break;
	case StreamCompress_MJPEG:
		Formats.CHLFormats[ch].Formats.Comp = AvComp_MJPEG;
		break;
	case StreamCompress_H264:
		Formats.CHLFormats[ch].Formats.Comp = AvComp_H264;
		break;
	case StreamCompress_H265:
		Formats.CHLFormats[ch].Formats.Comp = AvComp_H265;
		break;
	default:
		break;
	}

	ConfigEncode.SettingUp();
	return 0;
}

int CAvMoon::LocalGetAlarmCaps()
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::LocalGetAlarmProfile()
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::LocalSetAlarmProfile()
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}

int CAvMoon::LocalGetSysTimeProfile()
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::LocalSetSysTimeProfile()
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}


int CAvMoon::LocalGetNetWorkCaps(C_NetWorkCaps &NetWorkCaps)
{
	av_msg("%s\n", __FUNCTION__);
	C_NetCommCaps NetCommCaps;
	CAvDevice::GetNetCommCaps(NetCommCaps);
	if (NetCommCaps.NetCommMask & AvMask(NetCommT_Lan0)){
		NetWorkCaps.NetDevMask |= ProtoMask(NetWorkDev_LINK);
		NetWorkCaps.NetGetModeMask[NetWorkDev_LINK] |= ProtoMask(NetGetMode_MANUAL) | ProtoMask(NetGetMode_AUTO);
	}
	if (NetCommCaps.NetCommMask & AvMask(NetCommT_Lan1)){
		NetWorkCaps.NetDevMask |= ProtoMask(NetWorkDev_LINK);
		NetWorkCaps.NetGetModeMask[NetWorkDev_LINK] |= ProtoMask(NetGetMode_MANUAL) | ProtoMask(NetGetMode_AUTO);
	}
	if (NetCommCaps.NetCommMask & AvMask(NetCommT_Wireless)){
		NetWorkCaps.NetDevMask |= ProtoMask(NetWorkDev_WIFI);
		NetWorkCaps.NetGetModeMask[NetWorkDev_WIFI] |= ProtoMask(NetGetMode_MANUAL) | ProtoMask(NetGetMode_AUTO);
	}
	if (NetCommCaps.NetCommMask & AvMask(NetCommT_BlueTooth)){
		NetWorkCaps.NetDevMask |= ProtoMask(NetWorkDev_BlueTooth);
		NetWorkCaps.NetGetModeMask[NetWorkDev_BlueTooth] |= ProtoMask(NetGetMode_MANUAL);
	}
	if (NetCommCaps.NetCommMask & AvMask(NetCommT_SIM)){
		NetWorkCaps.NetDevMask |= ProtoMask(NetWorkDev_SIMLINK);
		NetWorkCaps.NetGetModeMask[NetWorkDev_SIMLINK] |= ProtoMask(NetGetMode_AUTO);
	}
	return 0;
}

int CAvMoon::LocalGetNetWorkProfile(C_NetWorkProfile &NetWorkProfile)
{
	av_msg("%s\n", __FUNCTION__);
	CAvConfigNetComm NetComm;
	NetComm.Update();
	ConfigNetComm &ConfigNet = NetComm.GetConfig(NetCommT_Lan0);
	sprintf(NetWorkProfile.Ipv4, "%s", ConfigNet.LanAttr.IpAddr);
	sprintf(NetWorkProfile.Ipv6, "");
	sprintf(NetWorkProfile.GateWay, "%s", ConfigNet.LanAttr.Gateway);
	sprintf(NetWorkProfile.DnsI, "%s", ConfigNet.LanAttr.Dns1);
	sprintf(NetWorkProfile.DnsII, "%s", ConfigNet.LanAttr.Dns2);
	NetWorkProfile.NetDev = NetWorkDev_LINK;
	sprintf(NetWorkProfile.SubMask, "%s", ConfigNet.LanAttr.Submask);
	
	C_DeviceFactoryInfo FacInfo;
	CAvDevice::GetDeviceInfo(FacInfo);
	sprintf(NetWorkProfile.Mac, "%s", FacInfo.ProductMacAddr);

	NetWorkProfile.WebServicePort = 80;
	NetWorkProfile.ServicePort = 5000;

	return 0;
}
int CAvMoon::LocalSetNetWorkProfile(C_NetWorkProfile &NetWorkProfile)
{
	av_msg("%s\n", __FUNCTION__);
	CAvConfigNetComm NetComm;
	NetComm.Update();
	
	E_NetComm_Type NetCom;
	switch (NetWorkProfile.NetDev)
	{
	case NetWorkDev_LINK:
	{
		ConfigNetComm &ConfigNet = NetComm.GetConfig(NetCommT_Lan0);
		sprintf(ConfigNet.LanAttr.IpAddr, "%s",		NetWorkProfile.Ipv4);
		sprintf(ConfigNet.LanAttr.Dns1, "%s",		NetWorkProfile.DnsI);
		sprintf(ConfigNet.LanAttr.Dns2, "%s",		NetWorkProfile.DnsII);
		sprintf(ConfigNet.LanAttr.Gateway, "%s",	NetWorkProfile.GateWay);
		sprintf(ConfigNet.LanAttr.Submask, "%s",	NetWorkProfile.SubMask);
	}
		break;
	case NetWorkDev_WIFI:
	{
		ConfigNetComm &ConfigNet = NetComm.GetConfig(NetCommT_Wireless);
		sprintf(ConfigNet.WirelessAttr.RouterLinkInfo[0].SSID, "%s", NetWorkProfile.wifiSSID);
		sprintf(ConfigNet.WirelessAttr.RouterLinkInfo[0].Passwd, "%s", NetWorkProfile.wifiPasswd);
	}
		break;
	case NetWorkDev_SIMLINK:
	{
		ConfigNetComm &ConfigNet = NetComm.GetConfig(NetCommT_SIM);
		
	}
		break;
	case NetWorkDev_BlueTooth:
	{
		ConfigNetComm &ConfigNet = NetComm.GetConfig(NetCommT_BlueTooth);

	}
		break;
		default:
			return -1;
			break;
	}
	
	NetComm.SettingUp();
	return 0;
}

int CAvMoon::LocalGetManufacturerInfo(C_ManufacturerInfo &FacInfo)
{
	av_msg("%s\n", __FUNCTION__);
	C_DspCaps DspCaps;
	CAvDevice::GetDspCaps(DspCaps);
	C_DeviceFactoryInfo DeviceFacInfo;
	CAvDevice::GetDeviceInfo(DeviceFacInfo);
	FacInfo.ChannelMax = DspCaps.nMaxEncodeChannel;
	FacInfo.FacTime = DeviceFacInfo.FActoryTime;
	sprintf(FacInfo.FacProductionModel, "%s", DeviceFacInfo.ProductModel);
	sprintf(FacInfo.HardWareVersion, "%s", DeviceFacInfo.HardWareVersion);
	sprintf(FacInfo.FacManufacturer, "%s", DeviceFacInfo.FactoryName);
	sprintf(FacInfo.FacProductionSerialNo, "%s", DeviceFacInfo.SerialNumber);

	FacInfo.FacChip = (Chip)0xff;
	FacInfo.FacSenSor = (Sensor)0xff;

	std::string guid;
	CAvDevice::GetStartUpGuid(guid);
	sprintf(FacInfo.ProtocolUniqueCode, "%s", guid.c_str());

	return 0;
}
int CAvMoon::LocalSetManufacturerInfo(C_ManufacturerInfo &FacInfo)
{
	av_msg("%s\n", __FUNCTION__);
	C_DeviceFactoryInfo DeviceFacInfo;
	
	DeviceFacInfo.MaxChannel = FacInfo.ChannelMax;
	DeviceFacInfo.FActoryTime = FacInfo.FacTime;
	sprintf(DeviceFacInfo.ProductModel, "%s", FacInfo.FacProductionModel);
	sprintf(DeviceFacInfo.HardWareVersion, "%s", FacInfo.HardWareVersion);
	sprintf(DeviceFacInfo.FactoryName, "%s", FacInfo.FacManufacturer);
	sprintf(DeviceFacInfo.SerialNumber, "%s", FacInfo.FacProductionSerialNo);
	char buffer[32];
	int bufferlen = 0;
	for (int i = 0; i < strlen(DeviceFacInfo.SerialNumber); i++){
		if (bufferlen == 0 && DeviceFacInfo.SerialNumber[i] != '-') continue;
		if (bufferlen != 0 && DeviceFacInfo.SerialNumber[i] == '-'){
			break;
		}
		else{
			if (bufferlen == 0){
				i++;
			}
		}
		buffer[bufferlen] = toupper(DeviceFacInfo.SerialNumber[i]);
		bufferlen++;
		buffer[bufferlen] = '\0';
	}
	sprintf(DeviceFacInfo.ProductMacAddr, "%s", buffer);

	CAvDevice::SetDeviceInfo(DeviceFacInfo);
	return 0;
}

int CAvMoon::LocalGetDevStatus(C_DevStatusInfo &DevStatus)
{
	av_msg("%s\n", __FUNCTION__);
	sprintf(DevStatus.devname, "IPC");
	DevStatus.uptime = clock() / CLOCKS_PER_SEC;
	return 0;
}
int CAvMoon::LocalGetFirmwareInfo(C_FirmwareInfo &FirmwareInfo)
{
	av_msg("%s\n", __FUNCTION__);


	sprintf(FirmwareInfo.BuildTime, "%s %s", __DATE__, __TIME__);
	FirmwareInfo.ChipMask = Chip_WINDOWS_32;
	FirmwareInfo.CustomMask = 0x00;
	sprintf(FirmwareInfo.FilesystemVerion, "0.0.1");

	return 0;
}

int CAvMoon::LocalCheckOutAuthorization(const char *UsrName, const char *UsrPasswd, unsigned int &UsrAccess)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}


int CAvMoon::LocalGetDiskStatus(C_DiskStatus &DiskStatus)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::LocalGetAlarmCaps(C_AlarmCaps &AlarmCaps)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::LocalGetUserList(std::list <C_UserInfo> &UserList)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::LocalAddUser(C_UserInfo &AddUser)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::LocalDelUser(C_UserInfo &DelUser)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::LocalModifyUser(C_UserInfo &ModifyUser)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}

int CAvMoon::LocalGetRecordFileSearch(C_RecordFileSearch &RecordFileSearch)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::LocalGetRecordFileDownload(C_RecordFileDownload &RecordFileDownload)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::LocalGetFtpProfile(C_FtpProfile &FtpProfile)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::LocalSetFtpProfile(C_FtpProfile &FtpProfile)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::LocalGetEmailProfile(C_EmailProfile &EmailProfile)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::LocalSetEmailProfile(C_EmailProfile &EmailProfile)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::LocalGetUpnpProfile(C_UpnpProfile &UpnpProfile)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::LocalSetUpnpProfile(C_UpnpProfile &UpnpProfile)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::LocalGetNtpProfile(C_NtpProfile &NtpProfile)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::LocalSetNtpProfile(C_NtpProfile &NtpProfile)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::LocalGetPppoeProfile(C_PppoeProfile &PpppoeProfile)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::LocalSetPppoeProfile(C_PppoeProfile &PpppoeProfile)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::LocalGetRtspProfile(C_RtspProfile &RtspProfile)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::LocalSetRtspProfile(C_RtspProfile &RtspProfile)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::LocalGetDdnsProfile(C_DdnsProfile &DdnsProfile)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::LocalSetDdnsProfile(C_DdnsProfile &DdnsProfile)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::LocalGetP2pProfile(C_P2pProfile &P2pProfile)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::LocalSetP2pProfile(C_P2pProfile &P2pProfile)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::LocalGetRtmpProfile(C_RtmpProfile &RtmpProfile)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::LocalSetRtmpProfile(C_RtmpProfile &RtmpProfile)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::LocalGetPtzCaps(C_PtzCaps		&PtzCaps)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::LocalGetPtzProfile(C_PtzProfile	&PtzProfile)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::LocalSetPtzProfile(C_PtzProfile	&PtzProfile)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::LocalPtzCommand(C_PtzCmd		&command)
{
	av_msg("%s\n", __FUNCTION__);
	PtzCmd cmd;
	cmd.Cmd      = command.PtzCmd;
	cmd.HSpeed   = command.PtzSpeed;
	cmd.VSpeed   = command.PtzSpeed;

	g_AvUart.PtzStart(cmd);
	return 0;
}

int CAvMoon::LocalFirmwareUpgrade(C_FirmwareUpgrade &FirmwareUpgrade)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::LocalFirmwareData(unsigned char *data, int datalen, unsigned int status, unsigned int *Progress)
{
	av_msg("%s\n", __FUNCTION__);
	static int firmwarefd = -1;
	static int firmwarelen = 0;
	static std::string FilePatch;
	firmwarelen += datalen;

#if defined(WIN32)
	FilePatch.assign("firmwware.bin");
	if (firmwarefd == -1) firmwarefd = open(FilePatch.c_str(), O_WRONLY | O_CREAT | O_BINARY);
#else
	FilePatch.assign("/tmp/firmware.bin");
	if (firmwarefd == -1) firmwarefd = open(FilePatch.c_str(), O_WRONLY | O_CREAT, 0x666);
#endif

	if (PROTO_STATUS_ING == status){
		write(firmwarefd, data, datalen);
	}
	else if (PROTO_STATUS_END == status){
		write(firmwarefd, data, datalen);
		close(firmwarefd);
		CAvDevice::SystemUpgrade(FilePatch, *Progress);
	}
	else if (PROTO_STATUS_BEGIN == status){
		write(firmwarefd, data, datalen);
	}
	else{

	}

	return 0;
}

int CAvMoon::LocalGetLogCaps(C_LogCaps &LogCaps)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::LocalGetLogProfile(C_LogProfile &LogProfile)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int CAvMoon::LocalSetLogProfile(C_LogProfile &LogProfile)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
int  CAvMoon::LocalGetLogList(std::list <C_LogContent> &LogContent, int start, int count, unsigned int starttm, unsigned int endtime)
{
	av_msg("%s\n", __FUNCTION__);
	return 0;
}
CNetUv *	CAvMoon::OnConnect(const char *remoteAddr, const int remotePort)
{
	av_msg("%s\n", __FUNCTION__);
	CAvMoon *pNewClients;
	pNewClients = new CAvMoon;
	return (CNetUv *)pNewClients;

}
int		CAvMoon::OnInfo(NetUvStatus nvi, const char *message)
{
	av_msg("%s  nvi = %d\n", __FUNCTION__, nvi);
	switch (nvi){

	case NV_SUCCEED:
		break;
	case NV_FAILD:
		break;
	case NV_CONNECT:
		break;
	case NV_REMOTECONNECT:
		break;
	case NV_CLOSEOVER:
		printf("delete this \n");
		av_msleep(40);
		break;
	case NV_DROPCONNECT:


		break;
	case NV_WRITE_ERR:
		break;
	case NV_READ_ERR:
		break;

	default:
		break;

	}
	return 0;
}
