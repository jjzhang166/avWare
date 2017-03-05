#include "AvNetService/AvOnvifClient.h"
#include "AvProtocol/AvNetProto.h"




CAvOnvifClient::CAvOnvifClient()
{

}
CAvOnvifClient::~CAvOnvifClient()
{

}
I_RET CAvOnvifClient::SearchCmdStart(av_bool bClearCache )
{
	return IRet_succeed;
}
I_RET CAvOnvifClient::SearchResult(std::list<C_DevSearch> &SearchResults)
{
	return IRet_succeed;
}

I_RET CAvOnvifClient::Connect(C_ProtoFormats *Formats)
{
	return IRet_faild;
}
I_RET CAvOnvifClient::Disconnect()
{
	return IRet_faild;
}

I_RET CAvOnvifClient::RemoteDspGetCaps(C_DspCaps &DspCaps)
{
	return IRet_faild;
}
I_RET CAvOnvifClient::RemoteCaptureGetCaps(int Channel, C_CaptureCaps &CaptureCaps)
{
	return IRet_faild;
}
I_RET CAvOnvifClient::RemoteCaptureGetProfile(int Channel, C_CaptureProfile &CaptureProfile)
{
	return IRet_faild;
}
I_RET CAvOnvifClient::RemoteCaptureSetProfile(int Channel, C_CaptureProfile &CaptureProfile)
{
	return IRet_faild;
}

I_RET CAvOnvifClient::RemoteEncodeGetCaps(int Channel, C_EncodeCaps &EncodeCaps)
{
	return IRet_faild;
}
I_RET CAvOnvifClient::RemoteEncodeGetProfile(int Channel, int Slave, C_EnCodeProfile &EnCodeProfile)
{
	return IRet_faild;
}
I_RET CAvOnvifClient::RemoteEncodeSetProfile(int Channel, int Slave, C_EnCodeProfile &EnCodeProfile)
{
	return IRet_faild;
}

I_RET CAvOnvifClient::RemoteAudioGetCaps(int Channel, C_AudioCaps &AudioCaps)
{
	return IRet_faild;
}
I_RET CAvOnvifClient::RemoteAudioGetProfile(int Channel, C_AudioProfile &AudioProfile)
{
	return IRet_faild;
}
I_RET CAvOnvifClient::RemoteAudioSetProfile(int Channel, C_AudioProfile &AudioProfile)
{
	return IRet_faild;
}

I_RET CAvOnvifClient::RemoteImageGetCaps(int Channel, C_ImageCaps &ImageCaps)
{
	return IRet_faild;
}
I_RET CAvOnvifClient::RemoteImageGetProfile(int Channel, C_ImageProfile &ImageProfile)
{
	return IRet_faild;
}
I_RET CAvOnvifClient::RemoteImageSetProfile(int Channel, C_ImageProfile &ImageProfile)
{
	return IRet_faild;
}

I_RET CAvOnvifClient::RemotePtzGetCaps(int Channel, C_PtzCaps &PtzCaps)
{
	return IRet_faild;
}
I_RET CAvOnvifClient::RemotePtzGetProfile(int Channel, C_PtzProfile &PtzProfile)
{
	return IRet_faild;
}
I_RET CAvOnvifClient::RemotePtzSetProfile(int Channel, C_PtzProfile &PtzProfile)
{
	return IRet_faild;
}
I_RET CAvOnvifClient::RemotePtzSetCommand(int Channel, C_PtzCmd &PtzCmd)
{
	return IRet_faild;
}

I_RET CAvOnvifClient::RemoteAdvancedSystemGetCaps(int Channel, C_AdvancedSystemCaps &AdvancedSystemCaps)
{
	return IRet_faild;
}
I_RET CAvOnvifClient::RemoteAdvancedSystemGetProfile(int Channel, C_AdvancedSystemProfile &AdvancedSystemProfile)
{
	return IRet_faild;
}
I_RET CAvOnvifClient::RemoteAdvancedSystemSetProfile(int Channel, C_AdvancedSystemProfile &AdvancedSystemProfile)
{
	return IRet_faild;
}


I_RET CAvOnvifClient::RemoteFactoryInfoGet(C_ManufacturerInfo &ManfacturerInfo)
{
	return IRet_faild;
}

I_RET CAvOnvifClient::RemoteNetCommGetCaps(C_NetCommCaps &NetCommCaps)
{
	return IRet_faild;
}
I_RET CAvOnvifClient::RemoteNetCommGetProfile(C_NetWorkProfile &NetWorkProfile)
{
	return IRet_faild;
}

I_RET CAvOnvifClient::RemoteFirmWareUpgrade(C_FirmWareUpgrade &FirmWareUpgrade)
{
	return IRet_faild;
}
I_RET CAvOnvifClient::RemoteFirmWareUpgradeGetProgress(int &Progress)
{
	return IRet_faild;
}

I_RET CAvOnvifClient::RemoteStreamStart(int Channel, int Slave)
{
	return IRet_faild;
}
I_RET CAvOnvifClient::RemoteStreamStop(int Channel, int Slave)
{
	return IRet_faild;
}
I_RET CAvOnvifClient::RemoteSendAvPacket(int Channel, int Slave, CAvPacket *pack)
{
	return IRet_faild;
}
I_RET CAvOnvifClient::RemoteOnAvPacket(int Channel, int Slave, CAvPacket *pack)
{
	return IRet_faild;
}
CAvPacket * CAvOnvifClient::RemoteStreamGet(int Channel, int Slave)
{
	return NULL;
}

CAvPacket * CAvOnvifClient::RemoteSnapshot(int Channel, av_bool bRealTime, av_uint SnapshotInterval , av_uint ContinuousTimes)
{
	return NULL;
}


