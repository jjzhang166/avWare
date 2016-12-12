#ifndef _AVNETPROTO_H_
#define _AVNETPROTO_H_
#include "CAvObject.h"
#include "Signals.h"
#include "Apis/AvWareCplusplus.h"
#include "AvPacket/AvPacket.h"
#include "Apis/AvWareStruct.h"
#include "Apis/AvWareLibDef.h"

typedef enum{
	ProtocolOnvif,
	ProtocolRtmp,
	ProtocolRtsp,
	ProtocolMoon,
}E_ProtoMode;

typedef struct{
	av_char MainUrl[128];
	av_char SubUrl[128];
}C_ProtoRtspFormats;

typedef struct {
	av_char MainUrl[128];
	av_char SubUrl[128];
}C_ProtoRtmpFormats;

typedef struct {
	av_char Url[128];
	av_int	Port;
}C_ProtoMoonFormats;

typedef struct {
	av_char Url[128];
}C_ProtoOnvifFormats;

typedef struct{
	av_bool IsEnable;
	av_char UsrName[64];
	av_char Passwd[64];
	av_int  DigitalChannel;
	av_int  ShowWindowsId;
	E_ProtoMode ProtoMode;
	union{
		C_ProtoOnvifFormats OnvifFormats;
		C_ProtoMoonFormats  MoonFormats;
		C_ProtoRtmpFormats  RtmpFormats;
		C_ProtoRtspFormats	RtspFormats;
	};
}C_ProtoFormats;

class AVWARE_API CAvNetProto: public CAvObject
{
public:
	CAvNetProto(){}
	CAvNetProto(C_ProtoFormats &ProtoFormats){
		m_ProtoFromats = ProtoFormats;
	}

	virtual ~CAvNetProto(){};
public:
	C_ProtoFormats & ProtoFromats(){
		return m_ProtoFromats;
	}
public:
	virtual I_RET Connect(C_ProtoFormats *Formats = NULL) = 0;
	virtual I_RET Disconnect() = 0;


	virtual I_RET RemoteDspGetCaps(C_DspCaps &DspCaps) = 0;
	virtual I_RET RemoteCaptureGetCaps(int Channel, C_CaptureCaps &CaptureCaps) = 0;
	virtual I_RET RemoteCaptureGetProfile(int Channel, C_CaptureProfile &CaptureProfile) = 0;
	virtual I_RET RemoteCaptureSetProfile(int Channel, C_CaptureProfile &CaptureProfile) = 0;

	virtual I_RET RemoteEncodeGetCaps(int Channel, C_EncodeCaps &EncodeCaps) = 0;
	virtual I_RET RemoteEncodeGetProfile(int Channel, int Slave, C_EnCodeProfile &EnCodeProfile) = 0;
	virtual I_RET RemoteEncodeSetProfile(int Channel, int Slave, C_EnCodeProfile &EnCodeProfile) = 0;

	virtual I_RET RemoteAudioGetCaps(int Channel, C_AudioCaps &AudioCaps) = 0;
	virtual I_RET RemoteAudioGetProfile(int Channel, C_AudioProfile &AudioProfile) = 0;
	virtual I_RET RemoteAudioSetProfile(int Channel, C_AudioProfile &AudioProfile) = 0;

	virtual I_RET RemoteImageGetCaps(int Channel, C_ImageCaps &ImageCaps) = 0;
	virtual I_RET RemoteImageGetrofile(int Channel, C_ImageProfile &ImageProfile) = 0;
	virtual I_RET RemoteImageSetProfile(int Channel, C_ImageProfile &ImageProfile) = 0;

	virtual I_RET RemotePtzGetCaps(int Channel, C_PtzCaps &PtzCaps) = 0;
	virtual I_RET RemotePtzGetProfile(int Channel, C_PtzProfile &PtzProfile) = 0;
	virtual I_RET RemotePtzSetProfile(int Channel, C_PtzProfile &PtzProfile) = 0;
	virtual I_RET RemotePtzSetCommand(int Channel, C_PtzCmd &PtzCmd) = 0;

	virtual I_RET RemoteFactoryInfoGet(C_ManufacturerInfo &ManfacturerInfo) = 0;
//	virtual I_RET RemoteFactoryInfoSet(C_ManufacturerInfo &ManfacturerInfo) = 0;

	virtual I_RET RemoteNetCommGetCaps(C_NetCommCaps &NetCommCaps) = 0;
	virtual I_RET RemoteNetCommGetProfile(C_NetWorkProfile &NetWorkProfile) = 0;
//	virtual I_RET RemoteNetCommSetProfile(C_NetWorkProfile &NetWorkProfile) = 0;

	virtual I_RET RemoteFirmWareGetInfo(C_FirmwareInfo &FirmwareInfo) = 0;
	virtual I_RET RemoteFirmWareUpgrade(C_FirmWareUpgrade &FirmWareUpgrade) = 0;
	virtual I_RET RemoteFirmWareUpgradeGetProgress(int &Progress) = 0;
	virtual I_RET RemoteStreamStart(int Channel, int Slave) = 0;
	virtual I_RET RemoteStreamStop(int Channel, int Slave) = 0;
	virtual I_RET RemoteSendAvPacket(int Channel, int Slave, CAvPacket *pack) = 0;
	virtual I_RET RemoteOnAvPacket(int Channel, int Slave, CAvPacket *pack) = 0;
	virtual CAvPacket * RemoteStreamGet(int Channel, int Slave) = 0;


protected:
	C_ProtoFormats	m_ProtoFromats;
};


#endif
