#ifndef _AVONVIFCLIENT_H_
#define _AVONVIFCLIENT_H_



#include "AvProtocol/AvNetProto.h"



class CAvOnvifClient :public CAvNetProto
{
public:
	CAvOnvifClient();
	~CAvOnvifClient();
public:
	static I_RET SearchCmdStart(av_bool bClearCache = av_false);
	static I_RET SearchResult(std::list<C_DevSearch> &SearchResults);

	I_RET Connect(C_ProtoFormats *Formats = NULL);
	I_RET Disconnect();

	I_RET RemoteDspGetCaps(C_DspCaps &DspCaps);
	I_RET RemoteCaptureGetCaps(int Channel, C_CaptureCaps &CaptureCaps);
	I_RET RemoteCaptureGetProfile(int Channel, C_CaptureProfile &CaptureProfile);
	I_RET RemoteCaptureSetProfile(int Channel, C_CaptureProfile &CaptureProfile);

	I_RET RemoteEncodeGetCaps(int Channel, C_EncodeCaps &EncodeCaps);
	I_RET RemoteEncodeGetProfile(int Channel, int Slave, C_EnCodeProfile &EnCodeProfile);
	I_RET RemoteEncodeSetProfile(int Channel, int Slave, C_EnCodeProfile &EnCodeProfile);

	I_RET RemoteAudioGetCaps(int Channel, C_AudioCaps &AudioCaps);
	I_RET RemoteAudioGetProfile(int Channel, C_AudioProfile &AudioProfile);
	I_RET RemoteAudioSetProfile(int Channel, C_AudioProfile &AudioProfile);

	I_RET RemoteImageGetCaps(int Channel, C_ImageCaps &ImageCaps);
	I_RET RemoteImageGetProfile(int Channel, C_ImageProfile &ImageProfile);
	I_RET RemoteImageSetProfile(int Channel, C_ImageProfile &ImageProfile);

	I_RET RemotePtzGetCaps(int Channel, C_PtzCaps &PtzCaps);
	I_RET RemotePtzGetProfile(int Channel, C_PtzProfile &PtzProfile);
	I_RET RemotePtzSetProfile(int Channel, C_PtzProfile &PtzProfile);
	I_RET RemotePtzSetCommand(int Channel, C_PtzCmd &PtzCmd);

	I_RET RemoteAdvancedSystemGetCaps(int Channel, C_AdvancedSystemCaps &AdvancedSystemCaps);
	I_RET RemoteAdvancedSystemGetProfile(int Channel, C_AdvancedSystemProfile &AdvancedSystemProfile);
	I_RET RemoteAdvancedSystemSetProfile(int Channel, C_AdvancedSystemProfile &AdvancedSystemProfile);

	I_RET RemoteRecordFileSearch(int Channel, C_RecordFileSearch &RecordFileSearch);
	I_RET RemoteRecordFileDownload(int Channel, C_RecordFileDownload &RecordFileDeownload);
	I_RET RemoteRecordFileDownloadCtrl(int Channel, C_RecordFileDownloadCtrl &RecordFileDownloadCtrl);

	I_RET RemoteFactoryInfoGet(C_ManufacturerInfo &ManfacturerInfo);

	I_RET RemoteNetCommGetCaps(C_NetCommCaps &NetCommCaps);
	I_RET RemoteNetCommGetProfile(C_NetWorkProfile &NetWorkProfile);

	I_RET RemoteFirmWareUpgrade(C_FirmWareUpgrade &FirmWareUpgrade);
	I_RET RemoteFirmWareUpgradeGetProgress(int &Progress);

	I_RET RemoteStreamStart(int Channel, int Slave);
	I_RET RemoteStreamStop(int Channel, int Slave);
	I_RET RemoteSendAvPacket(int Channel, int Slave, CAvPacket *pack);
	I_RET RemoteSendAlarmMsg(C_AlmMsg &AlmMsg);
	I_RET RemoteGetAlarmMsg(C_AlmMsg &AlmMsg);
	I_RET RemoteOnAvPacket(int Channel, int Slave, CAvPacket *pack);
	CAvPacket * RemoteStreamGet(int Channel, int Slave);

	CAvPacket * RemoteSnapshot(int Channel, av_bool bRealTime = av_false, av_uint SnapshotInterval = 0, av_uint ContinuousTimes = 0);


private:

};



#endif

