#ifndef _AVNETPROTO_H_
#define _AVNETPROTO_H_
#include "CAvObject.h"

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
	av_char CheckAliveAddress[64];//程序自动使用ping 其是否在线
	E_ProtoMode ProtoMode;
	struct{
		C_ProtoOnvifFormats OnvifFormats;
		C_ProtoMoonFormats  MoonFormats;
		C_ProtoRtmpFormats  RtmpFormats;
		C_ProtoRtspFormats	RtspFormats;
	};
}C_ProtoFormats;

class CAvNetProto: public CAvObject
{
public:
	CAvNetProto(){}
	CAvNetProto(C_ProtoFormats &ProtoFormats){
		m_ProtoFromats = ProtoFormats;
	}

	virtual ~CAvNetProto(){ av_error("relase CAvNetProto\n"); };
public:
	C_ProtoFormats & ProtoFromats(){
		return m_ProtoFromats;
	}
public:
	virtual I_RET Connect(C_ProtoFormats *Formats = NULL) = 0;
	virtual I_RET Disconnect() = 0;

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
	virtual I_RET RemoteImageGetProfile(int Channel, C_ImageProfile &ImageProfile) = 0;
	virtual I_RET RemoteImageSetProfile(int Channel, C_ImageProfile &ImageProfile) = 0;

	virtual I_RET RemotePtzGetCaps(int Channel, C_PtzCaps &PtzCaps) = 0;
	virtual I_RET RemotePtzGetProfile(int Channel, C_PtzProfile &PtzProfile) = 0;
	virtual I_RET RemotePtzSetProfile(int Channel, C_PtzProfile &PtzProfile) = 0;
	virtual I_RET RemotePtzSetCommand(int Channel, C_PtzCmd &PtzCmd) = 0;

	virtual I_RET RemoteAdvancedSystemGetCaps(int Channel, C_AdvancedSystemCaps &AdvancedSystemCaps) = 0;
	virtual I_RET RemoteAdvancedSystemGetProfile(int Channel, C_AdvancedSystemProfile &AdvancedSystemProfile) = 0;
	virtual I_RET RemoteAdvancedSystemSetProfile(int Channel, C_AdvancedSystemProfile &AdvancedSystemProfile) = 0;

	virtual I_RET RemoteRecordFileSearch(int Channel, C_RecordFileSearch &RecordFileSearch) = 0;
	virtual I_RET RemoteRecordFileDownload(int Channel, C_RecordFileDownload &RecordFileDeownload) = 0;
	virtual I_RET RemoteRecordFileDownloadCtrl(int Channel, C_RecordFileDownloadCtrl &RecordFileDownloadCtrl) = 0;



	virtual I_RET RemoteFactoryInfoGet(C_ManufacturerInfo &ManfacturerInfo) = 0;

	virtual I_RET RemoteNetCommGetCaps(C_NetCommCaps &NetCommCaps) = 0;
	virtual I_RET RemoteNetCommGetProfile(C_NetWorkProfile &NetWorkProfile) = 0;

	virtual I_RET RemoteFirmWareUpgrade(C_FirmWareUpgrade &FirmWareUpgrade) = 0;
	virtual I_RET RemoteFirmWareUpgradeGetProgress(int &Progress) = 0;

	virtual I_RET RemoteStreamStart(int Channel, int Slave) = 0;
	virtual I_RET RemoteStreamStop(int Channel, int Slave) = 0;
	virtual I_RET RemoteSendAvPacket(int Channel, int Slave, CAvPacket *pack) = 0;
	virtual I_RET RemoteSendAlarmMsg(C_AlmMsg &AlmMsg) = 0;
	virtual I_RET RemoteGetAlarmMsg(C_AlmMsg &AlmMsg) = 0;
	virtual I_RET RemoteOnAvPacket(int Channel, int Slave, CAvPacket *pack) = 0;
	virtual CAvPacket * RemoteStreamGet(int Channel, int Slave) = 0;

	virtual CAvPacket * RemoteSnapshot(int Channel, av_bool bRealTime = av_false, av_uint SnapshotInterval = 0, av_uint ContinuousTimes = 0) = 0;
protected:
	C_ProtoFormats	m_ProtoFromats;
};


#endif
