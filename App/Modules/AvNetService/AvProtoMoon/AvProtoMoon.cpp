#include "AvNetService/AvProtoMoon.h"
#include "AvDevice/AvDevice.h"
#include "AvCapture/AvManCapture.h"
#include "AvProto/Proto.h"



SINGLETON_IMPLEMENT(CAvProtoMoon);

CAvProtoMoon::CAvProtoMoon()
{

}
CAvProtoMoon::CAvProtoMoon(C_ProtoFormats &ProtoFormats) :AvProtoSDK(ProtoFormats)
{

}
CAvProtoMoon::~CAvProtoMoon()
{
	CAvPacket *Packet = NULL;
	for (int i = CHL_MAIN_T; i < CHL_NR_T; ){
		Packet = RemoteStreamGet(0, i);
		if (NULL != Packet){
			Packet->Release();
		}
		else{
			i++;
		}

	}
}
av_bool CAvProtoMoon::Start()
{
	AvProtoSDK::StartAvProtoService(new CAvProtoMoon, new CAvProtoMoon);
	return av_true;
}
av_bool CAvProtoMoon::Stop()
{
	AvProtoSDK::StopAvProtoService();
	return av_true;
}


I_RET CAvProtoMoon::RemoteOnAvPacket(int Channel, int Slave, CAvPacket *pack)
{
	if (m_Avpacket[Slave].size() > 100){
		av_error("%s Cache too mach frame\n", __FUNCTION__);
		return IRet_faild;
	}
	pack->AddRefer();
	m_Mutex[Slave].Enter();
	m_Avpacket[Slave].push(pack);
	m_Mutex[Slave].Leave();

	return IRet_succeed;
}

CAvPacket * CAvProtoMoon::RemoteStreamGet(int Channel, int Slave)
{
	CAvPacket *pRet = NULL;
	m_Mutex[Slave].Enter();
	if (m_Avpacket[Slave].empty() == true){
		m_Mutex[Slave].Leave();
		return NULL;
	}
	else{
		pRet = m_Avpacket[Slave].front();
		m_Avpacket[Slave].pop();
		m_Mutex[Slave].Leave();
		return pRet;
	}
}

I_RET  CAvProtoMoon::LocalDspGetCaps(C_DspCaps &DspCaps)
{
	av_msg("%s\n", __FUNCTION__);
	return CAvDevice::GetDspCaps(DspCaps) == av_true ? IRet_succeed:IRet_faild;
}

I_RET  CAvProtoMoon::LocalCaptureGetCaps(int Channel, C_CaptureCaps &CaptureCaps)
{
	av_msg("%s\n", __FUNCTION__);
	Capture *pCapture = g_AvManCapture.GetAvCaptureInstance(Channel);
	return pCapture->CaptureGetCaps(CaptureCaps) == av_true ? IRet_succeed : IRet_faild;
}

I_RET  CAvProtoMoon::LocalCaptureGetProfile(int Channel, C_CaptureProfile &CaptureProfile)
{
	av_msg("%s\n", __FUNCTION__);
	Capture *pCapture = g_AvManCapture.GetAvCaptureInstance(Channel);
	return pCapture->CaptureGetProfile(CaptureProfile) == av_true ? IRet_succeed : IRet_faild;
}

I_RET  CAvProtoMoon::LocalCaptureSetProfile(int Channel, C_CaptureProfile &CaptureProfile)
{
	av_msg("%s\n", __FUNCTION__);
	Capture *pCapture = g_AvManCapture.GetAvCaptureInstance(Channel);
	return pCapture->CaptureSetProfile(CaptureProfile) == av_true ? IRet_succeed : IRet_faild;
}


I_RET  CAvProtoMoon::LocalEncodeGetCaps(int Channel, C_EncodeCaps &EncodeCaps)
{
	av_msg("%s\n", __FUNCTION__);
	Capture *pCapture = g_AvManCapture.GetAvCaptureInstance(Channel);
	return pCapture->EncodeGetCaps(EncodeCaps) == av_true ? IRet_succeed : IRet_faild;
}

I_RET  CAvProtoMoon::LocalEncodeGetProfile(int Channel, int Slave, C_EnCodeProfile &EnCodeProfile)
{
	av_msg("%s\n", __FUNCTION__);
	Capture *pCapture = g_AvManCapture.GetAvCaptureInstance(Channel);
	return pCapture->EncodeGetProfile(Slave, EnCodeProfile) == av_true ? IRet_succeed : IRet_faild;
}

I_RET  CAvProtoMoon::LocalEncodeSetProfile(int Channel, int Slave, C_EnCodeProfile &EnCodeProfile)
{
	av_msg("%s\n", __FUNCTION__);
	Capture *pCapture = g_AvManCapture.GetAvCaptureInstance(Channel);
	return pCapture->EncodeSetProfile(Slave, EnCodeProfile) == av_true ? IRet_succeed : IRet_faild;
}

I_RET  CAvProtoMoon::LocalAudioGetCaps(int Channel, C_AudioCaps &AudioCaps)
{
	av_msg("%s\n", __FUNCTION__);
	Capture *pCapture = g_AvManCapture.GetAvCaptureInstance(Channel);
	return pCapture->AudioGetCaps(AudioCaps) == av_true ? IRet_succeed : IRet_faild;
}
I_RET  CAvProtoMoon::LocalAudioGetProfile(int Channel, C_AudioProfile &AudioProfile)
{
	av_msg("%s\n", __FUNCTION__);
	Capture *pCapture = g_AvManCapture.GetAvCaptureInstance(Channel);
	return pCapture->AudioGetProfile(AudioProfile) == av_true ? IRet_succeed : IRet_faild;
}
I_RET  CAvProtoMoon::LocalAudioSetProfile(int Channel, C_AudioProfile &AudioProfile)
{
	av_msg("%s\n", __FUNCTION__);
	Capture *pCapture = g_AvManCapture.GetAvCaptureInstance(Channel);
	return pCapture->AudioSetProfile(AudioProfile) == av_true ? IRet_succeed : IRet_faild;
}


I_RET  CAvProtoMoon::LocalImageGetCaps(int Channel, C_ImageCaps &ImageCaps)
{
	av_msg("%s\n", __FUNCTION__);
	Capture *pCapture = g_AvManCapture.GetAvCaptureInstance(Channel);
	return pCapture->ImageGetCaps(ImageCaps) == av_true ? IRet_succeed : IRet_faild;
}

I_RET  CAvProtoMoon::LocalImageGetProfile(int Channel, C_ImageProfile &ImageProfile)
{
	av_msg("%s\n", __FUNCTION__);
	Capture *pCapture = g_AvManCapture.GetAvCaptureInstance(Channel);
	return pCapture->ImageGetProfile(ImageProfile) == av_true ? IRet_succeed : IRet_faild;
}

I_RET  CAvProtoMoon::LocalImageSetProfile(int Channel, C_ImageProfile &ImageProfile)
{
	av_msg("%s\n", __FUNCTION__);
	Capture *pCapture = g_AvManCapture.GetAvCaptureInstance(Channel);
	return pCapture->ImageSetProfile(ImageProfile) == av_true ? IRet_succeed : IRet_faild;
}


I_RET  CAvProtoMoon::LocalPtzGetCaps(int Channel, C_PtzCaps &PtzCaps)
{
	av_msg("%s\n", __FUNCTION__);
	Capture *pCapture = g_AvManCapture.GetAvCaptureInstance(Channel);
	return pCapture->PtzGetCaps(PtzCaps) == av_true ? IRet_succeed : IRet_faild;
}

I_RET  CAvProtoMoon::LocalPtzGetProfile(int Channel, C_PtzProfile &PtzProfile)
{
	av_msg("%s\n", __FUNCTION__);
	Capture *pCapture = g_AvManCapture.GetAvCaptureInstance(Channel);
	return pCapture->PtzGetProfile(PtzProfile) == av_true ? IRet_succeed : IRet_faild;
}

I_RET  CAvProtoMoon::LocalPtzSetProfile(int Channel, C_PtzProfile &PtzProfile)
{
	av_msg("%s\n", __FUNCTION__);
	Capture *pCapture = g_AvManCapture.GetAvCaptureInstance(Channel);
	return pCapture->PtzSetProfile(PtzProfile) == av_true ? IRet_succeed : IRet_faild;
}

I_RET  CAvProtoMoon::LocalPtzSetCommand(int Channel, C_PtzCmd &PtzCmd)
{

	av_msg("%s\n", __FUNCTION__);
	Capture *pCapture = g_AvManCapture.GetAvCaptureInstance(Channel);
	return pCapture->PtzSetCommand(PtzCmd) == av_true ? IRet_succeed : IRet_faild;
}


I_RET  CAvProtoMoon::LocalFactoryInfoGet(C_ManufacturerInfo &ManfacturerInfo)
{
	av_msg("%s\n", __FUNCTION__);
	C_DeviceFactoryInfo DeviceFactoryInfo;
	CAvDevice::GetDeviceInfo(DeviceFactoryInfo);

	ManfacturerInfo.ChannelMax = DeviceFactoryInfo.MaxChannel;
// 	ManfacturerInfo.FacChip = (AvChip)DeviceFactoryInfo.ChipType;
// 	ManfacturerInfo.FacSenSor = (AvSensor)DeviceFactoryInfo.SensorType;
	ManfacturerInfo.FacChip = (AvChip)0xff;
	ManfacturerInfo.FacSenSor = (AvSensor)0xff;
	sprintf(ManfacturerInfo.FacManufacturer, "%s", DeviceFactoryInfo.FactoryName);
	sprintf(ManfacturerInfo.FacProductionModel, "%s", DeviceFactoryInfo.ProductModel);
	sprintf(ManfacturerInfo.FacProductionSerialNo, "%s", DeviceFactoryInfo.SerialNumber);
	
	sprintf(ManfacturerInfo.HardWareVersion, DeviceFactoryInfo.HardWareVersion);
	ManfacturerInfo.FacTime = DeviceFactoryInfo.FActoryTime;
	std::string guid;
	CAvDevice::GetStartUpGuid(guid);
	sprintf(ManfacturerInfo.ProtocolUniqueCode, "%s", guid.c_str());
	return IRet_succeed;
}

I_RET  CAvProtoMoon::LocalFactoryInfoSet(C_ManufacturerInfo &ManfacturerInfo)
{
	av_msg("%s\n", __FUNCTION__);
	C_DeviceFactoryInfo DeviceFacInfo;

	sprintf(DeviceFacInfo.SerialNumber, "%s", ManfacturerInfo.FacProductionSerialNo);
	sprintf(DeviceFacInfo.FactoryName, "%s", ManfacturerInfo.FacManufacturer);
	sprintf(DeviceFacInfo.HardWareVersion, "%s", ManfacturerInfo.HardWareVersion);
	sprintf(DeviceFacInfo.ProductModel, "%s", ManfacturerInfo.FacProductionModel);
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
	DeviceFacInfo.ChipType = ManfacturerInfo.FacChip;
	DeviceFacInfo.SensorType = ManfacturerInfo.FacSenSor;
	DeviceFacInfo.FActoryTime = ManfacturerInfo.FacTime;
	DeviceFacInfo.MaxChannel = ManfacturerInfo.ChannelMax;
	DeviceFacInfo.Res = 0x00;
	
	CAvDevice::SetDeviceInfo(DeviceFacInfo);
	return IRet_succeed;
}


I_RET  CAvProtoMoon::LocalNetCommGetCaps(C_NetCommCaps &NetCommCaps)
{
	av_msg("%s\n", __FUNCTION__);
	return IRet_succeed;
}

I_RET  CAvProtoMoon::LocalNetCommGetProfile(C_NetWorkProfile &NetWorkProfile)
{
	av_msg("%s\n", __FUNCTION__);
	C_NetCommAttribute NetCommAttribute;
	C_DeviceFactoryInfo DeviceFactoryInfo;
	CAvDevice::GetDeviceInfo(DeviceFactoryInfo);
	CAvDevice::GetNetCommAttribute(NetCommT_LAN0, NetCommAttribute);

	sprintf(NetWorkProfile.DnsI, "%s", NetCommAttribute.LanAttr.Dns1);
	sprintf(NetWorkProfile.DnsII, "%s", NetCommAttribute.LanAttr.Dns2);
	sprintf(NetWorkProfile.GateWay, "%s", NetCommAttribute.LanAttr.Gateway);
	sprintf(NetWorkProfile.Ipv4, "%s", NetCommAttribute.LanAttr.IpAddr);
	sprintf(NetWorkProfile.Mac, "%s", DeviceFactoryInfo.ProductMacAddr);
	sprintf(NetWorkProfile.SubMask, "%s", NetCommAttribute.LanAttr.Submask);
	NetWorkProfile.WebServicePort = 80;
	NetWorkProfile.ServicePort = 5000;
	
	return IRet_succeed;
}

I_RET  CAvProtoMoon::LocalNetCommSetProfile(C_NetWorkProfile &NetWorkProfile)
{
	av_msg("%s\n", __FUNCTION__);
	C_NetCommAttribute NetCommAttribute;
	NetCommAttribute.Enable = av_true;
	switch (NetWorkProfile.NetDevice)
	{
	case NetCommT_LAN0:
		sprintf(NetCommAttribute.LanAttr.IpAddr, NetWorkProfile.Ipv4);
		sprintf(NetCommAttribute.LanAttr.Dns1, NetWorkProfile.DnsI);
		sprintf(NetCommAttribute.LanAttr.Dns2, NetWorkProfile.DnsII);
		sprintf(NetCommAttribute.LanAttr.Host, "avWare");
		sprintf(NetCommAttribute.LanAttr.Gateway, NetWorkProfile.GateWay);
		sprintf(NetCommAttribute.LanAttr.Submask, NetWorkProfile.SubMask);
		break;
	case NetCommT_LAN1:
		sprintf(NetCommAttribute.LanAttr.IpAddr, NetWorkProfile.Ipv4);
		sprintf(NetCommAttribute.LanAttr.Dns1, NetWorkProfile.DnsI);
		sprintf(NetCommAttribute.LanAttr.Dns2, NetWorkProfile.DnsII);
		sprintf(NetCommAttribute.LanAttr.Host, "avWare");
		sprintf(NetCommAttribute.LanAttr.Gateway, NetWorkProfile.GateWay);
		sprintf(NetCommAttribute.LanAttr.Submask, NetWorkProfile.SubMask);
		break;
	case NetCommT_Wireless:
		sprintf(NetCommAttribute.WirelessAttr.SSID, NetWorkProfile.wifiSSID);
		sprintf(NetCommAttribute.WirelessAttr.Passwd, NetWorkProfile.wifiPasswd);
		break;
	case NetCommT_SIM:
		break;
	case NetCommT_BlueTooth:
		break;
	default:
		break;
	}
// 	av_msg("[%s][%s][%s]\n%s %s\n",
// 		NetCommAttribute.LanAttr.IpAddr, NetCommAttribute.LanAttr.Submask,
// 		NetCommAttribute.LanAttr.Gateway, NetCommAttribute.LanAttr.Dns1,
// 		NetCommAttribute.LanAttr.Dns2);
	CAvDevice::SetNetCommAttribute(NetWorkProfile.NetDevice, NetCommAttribute);
	return IRet_succeed;
}

I_RET  CAvProtoMoon::LocalFirmWareGetInfo(C_FirmwareInfo &FirmwareInfo)
{
	av_msg("%s\n", __FUNCTION__);
	sprintf(FirmwareInfo.BuildTime, "%s %s", __DATE__, __TIME__);
	FirmwareInfo.ChipMask = 0xff;
	FirmwareInfo.CustomMask = 0x00;
	sprintf(FirmwareInfo.Descriptor, "");
	sprintf(FirmwareInfo.FilesystemVerion, "File 0.0.1");
	sprintf(FirmwareInfo.KernelVersion, "Kernel 0.0.1");
	sprintf(FirmwareInfo.ProtoVersion, "proto 0.0.1");
	FirmwareInfo.SensorMask = 0xff;	
	FirmwareInfo.Version = 0x010101;
	sprintf(FirmwareInfo.FilesystemVerion, "%s", CAvDevice::GetSoftVersionString().c_str());

	return IRet_succeed;
}

I_RET  CAvProtoMoon::LocalFirmWareUpgrade(C_FirmwareData &FirmwareData)
{
	av_msg("%s\n", __FUNCTION__);
	av_msg("%s\n", __FUNCTION__);
	static int firmwarefd = -1;
	static int firmwarelen = 0;
	static std::string FilePatch;
	firmwarelen += FirmwareData.DataLen;

#if defined(WIN32)

	FilePatch.assign("firmwware.bin");
	if (firmwarefd == -1) firmwarefd = open(FilePatch.c_str(), O_WRONLY | O_CREAT | O_BINARY);
#else

	FilePatch.assign("/tmp/firmware.bin");
	if (firmwarefd == -1) firmwarefd = open(FilePatch.c_str(), O_WRONLY | O_CREAT, 0x666);
#endif

	write(firmwarefd, FirmwareData.Data, FirmwareData.DataLen);

	if (FirmwareData.PackCnt == FirmwareData.PackNum){
		close(firmwarefd);
		CAvDevice::SystemUpgrade(FilePatch);
	}

	return IRet_succeed;
}

I_RET  CAvProtoMoon::LocalFirmWareUpgradeGetProgress(int &Progress)
{
	//av_msg("%s\n", __FUNCTION__);
	Progress = CAvDevice::SystemUpgradeProgress();
	return IRet_succeed;
}


I_RET  CAvProtoMoon::LocalStreamStart(int Channel, int Slave)
{
	av_msg("%s\n", __FUNCTION__);
	Capture *pCapture = g_AvManCapture.GetAvCaptureInstance(Channel);
	pCapture->Start(Slave, this, (Capture::SIG_PROC_ONDATA)&CAvProtoMoon::OnLocalPacket);
	return IRet_succeed;
}

I_RET  CAvProtoMoon::LocalStreamStop(int Channel, int Slave)
{
	av_msg("%s\n", __FUNCTION__);
	Capture *pCapture = g_AvManCapture.GetAvCaptureInstance(Channel);
	pCapture->Stop(Slave, this, (Capture::SIG_PROC_ONDATA)&CAvProtoMoon::OnLocalPacket);
	return IRet_succeed;
}
I_RET  CAvProtoMoon::LocalDeviceStatus(C_DevStatusInfo &DeviceStat)
{
	av_msg("%s\n", __FUNCTION__);
	DeviceStat.uptime = CAvDevice::GetDeviceStartUp();
	sprintf(DeviceStat.devname, "IPC");
	return IRet_succeed;
}
I_RET  CAvProtoMoon::LocalCheckAuthorization(const char *usrname, const char *passwd, unsigned int &UsrAccess)
{
	av_msg("%s\n", __FUNCTION__);
	av_msg("LocalCheckAuthorization [%s][%s]\n", usrname, passwd);
	UsrAccess = 0xff;
	return IRet_succeed;
}
void CAvProtoMoon::OnLocalPacket(int Channel, int Slave, CAvPacket *pack)
{
	RemoteSendAvPacket(Channel, Slave, pack);
}

CNetUv *CAvProtoMoon::OnConnect(const char *remoteAddr, const int remotePort)
{
	av_msg("AvProtoMoon recv a connect [%s][%d]\n", remoteAddr, remotePort);
	return new CAvProtoMoon;
}