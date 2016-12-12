#ifndef _AVPROTOMOON_H_
#define _AVPROTOMOON_H_
#include "CAvObject.h"
//#include "AvProto/ProtoLocal.h"
#include "AvProto/Proto.h"
#include "AvProtocol/AvNetProto.h"



class CAvProtoMoon :public AvProtoSDK
{
public:
	CAvProtoMoon();
	CAvProtoMoon(C_ProtoFormats &ProtoFormats);
	~CAvProtoMoon();
	SINGLETON_DECLARE(CAvProtoMoon);

	av_bool Start();
	av_bool Stop();

public:
	I_RET  RemoteOnAvPacket(int Channel, int Slave, CAvPacket *pack);
	CAvPacket * RemoteStreamGet(int Channel, int Slave);
	I_RET  LocalDspGetCaps(C_DspCaps &DspCaps);

	I_RET  LocalCaptureGetCaps(int Channel, C_CaptureCaps &CaptureCaps);
	I_RET  LocalCaptureGetProfile(int Channel, C_CaptureProfile &CaptureProfile);
	I_RET  LocalCaptureSetProfile(int Channel, C_CaptureProfile &CaptureProfile);

	I_RET  LocalEncodeGetCaps(int Channel, C_EncodeCaps &EncodeCaps);
	I_RET  LocalEncodeGetProfile(int Channel, int Slave, C_EnCodeProfile &EnCodeProfile);
	I_RET  LocalEncodeSetProfile(int Channel, int Slave, C_EnCodeProfile &EnCodeProfile);
	I_RET  LocalAudioGetCaps(int Channel, C_AudioCaps &AudioCaps);
	I_RET  LocalAudioGetProfile(int Channel, C_AudioProfile &AudioProfile);
	I_RET  LocalAudioSetProfile(int Channel, C_AudioProfile &AudioProfile);

	I_RET  LocalImageGetCaps(int Channel, C_ImageCaps &ImageCaps);
	I_RET  LocalImageGetProfile(int Channel, C_ImageProfile &ImageProfile);
	I_RET  LocalImageSetProfile(int Channel, C_ImageProfile &ImageProfile);

	I_RET  LocalPtzGetCaps(int Channel, C_PtzCaps &PtzCaps);
	I_RET  LocalPtzGetProfile(int Channel, C_PtzProfile &PtzProfile);
	I_RET  LocalPtzSetProfile(int Channel, C_PtzProfile &PtzProfile);
	I_RET  LocalPtzSetCommand(int Channel, C_PtzCmd &PtzCmd);

	I_RET  LocalFactoryInfoGet(C_ManufacturerInfo &ManfacturerInfo);
	I_RET  LocalFactoryInfoSet(C_ManufacturerInfo &ManfacturerInfo);

	I_RET  LocalNetCommGetCaps(C_NetCommCaps &NetCommCaps);
	I_RET  LocalNetCommGetProfile(C_NetWorkProfile &NetWorkProfile);
	I_RET  LocalNetCommSetProfile(C_NetWorkProfile &NetWorkProfile);
	I_RET  LocalFirmWareGetInfo(C_FirmwareInfo &FirmwareInfo);
	I_RET  LocalFirmWareUpgrade(C_FirmwareData &FirmwareData);
	I_RET  LocalFirmWareUpgradeGetProgress(int &Progress);

	I_RET  LocalStreamStart(int Channel, int Slave);
	I_RET  LocalStreamStop(int Channel, int Slave);
	I_RET  LocalDeviceStatus(C_DevStatusInfo &DeviceStat);

	I_RET  LocalCheckAuthorization(const char *usrname, const char *passwd, unsigned int &UsrAccess);
private:
	void OnLocalPacket(int Channel, int Slave, CAvPacket *pack);
private:
	CNetUv *OnConnect(const char *remoteAddr, const int remotePort);



private:
	std::queue<CAvPacket *> m_Avpacket[CHL_NR_T];
	CMutex	m_Mutex[CHL_NR_T];

};

#define g_AvProtoMoon (*CAvProtoMoon::instance())















#endif

