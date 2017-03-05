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
#ifndef _PROTO_H_
#define _PROTO_H_



#include "Apis/AvWareType.h"
#include "uvPlusPlus/libuvCpp.h"
#include "Apis/AvWareStruct.h"
#include "Apis/AvSystemStruct.h"
#include "CAvObject.h"
#include "AvPacket/AvPacket.h"
#include "AvThread/AvThread.h"
#include "AvProtocol/AvNetProto.h"


#define ProtoStreamMut (8*1024)

#if defined (AvProtoShare_EXPORTS)
#define AVWARE_SHARE_EXPORT
#elif defined (AvProtoShare_IMPORTS)
#define AVWARE_SHARE_IMPORT
#else 

#endif
#include "Apis/AvWareLibDef.h"
#include "AvPacket/AvPacket.h"


class AVWARE_API AvProtoSDK :public CAvNetProto, public CNetUv
{
public:
	AvProtoSDK();
	AvProtoSDK(C_ProtoFormats &ProtoFormats);
	~AvProtoSDK();
	static I_RET StartAvProtoService(AvProtoSDK *LinkService, AvProtoSDK *SearchService);
	static I_RET StopAvProtoService();
	static I_RET SearchCmdStart(av_bool bClearCache = av_false);
	static I_RET RemoteNetCommSetProfile(C_NetWorkProfile &NetWorkProfile);
	static I_RET RemoteFactoryInfoSet(C_ManufacturerInfo &ManfacturerInfo);

	static I_RET SearchResult(std::list<C_DevSearch> &SearchResult);

public:
	I_RET Connect(C_ProtoFormats *Formats = NULL);
	I_RET Disconnect();

public:
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


	I_RET RemoteFactoryInfoGet(C_ManufacturerInfo &ManfacturerInfo);
	

	I_RET RemoteNetCommGetCaps(C_NetCommCaps &NetCommCaps);
	I_RET RemoteNetCommGetProfile(C_NetWorkProfile &NetWorkProfile);
	
	I_RET RemoteFirmWareUpgrade(C_FirmWareUpgrade &FirmWareUpgrade);
	I_RET RemoteFirmWareUpgradeGetProgress(int &Progress);


	I_RET RemoteStreamStart(int Channel, int Slave);
	I_RET RemoteStreamStop(int Channel, int Slave);
	I_RET RemoteSendAvPacket(int Channel, int Slave, CAvPacket *pack);
	
	I_RET		RemoteSyncSystemTime(av_timeval &atv);
	I_RET		RemoteRequestIdrFrame(av_int Slave = CHL_MAIN_T);
	CAvPacket * RemoteSnapshot(int Channel, av_bool bRealTime = av_false, av_uint SnapshotInterval = 0, av_uint ContinuousTimes = 0);

public:
	virtual I_RET RemoteOnAvPacket(int Channel, int Slave, CAvPacket *pack) = 0;
	virtual CAvPacket * RemoteStreamGet(int Channel, int Slave) = 0;

public:
	virtual I_RET  LocalDspGetCaps(C_DspCaps &DspCaps) = 0;

	virtual I_RET  LocalCaptureGetCaps(int Channel, C_CaptureCaps &CaptureCaps) = 0;
	virtual I_RET  LocalCaptureGetProfile(int Channel, C_CaptureProfile &CaptureProfile) = 0;
	virtual I_RET  LocalCaptureSetProfile(int Channel, C_CaptureProfile &CaptureProfile) = 0;

	virtual I_RET  LocalEncodeGetCaps(int Channel, C_EncodeCaps &EncodeCaps) = 0;
	virtual I_RET  LocalEncodeGetProfile(int Channel, int Slave, C_EnCodeProfile &EnCodeProfile) = 0;
	virtual I_RET  LocalEncodeSetProfile(int Channel, int Slave, C_EnCodeProfile &EnCodeProfile) = 0;

	virtual I_RET  LocalAudioGetCaps(int Channel, C_AudioCaps &AudioCaps) = 0;
	virtual I_RET  LocalAudioGetProfile(int Channel, C_AudioProfile &AudioProfile) = 0;
	virtual I_RET  LocalAudioSetProfile(int Channel, C_AudioProfile &AudioProfile) = 0;

	virtual I_RET  LocalImageGetCaps(int Channel, C_ImageCaps &ImageCaps) = 0;
	virtual I_RET  LocalImageGetProfile(int Channel, C_ImageProfile &ImageProfile) = 0;
	virtual I_RET  LocalImageSetProfile(int Channel, C_ImageProfile &ImageProfile) = 0;

	virtual I_RET  LocalPtzGetCaps(int Channel, C_PtzCaps &PtzCaps) = 0;
	virtual I_RET  LocalPtzGetProfile(int Channel, C_PtzProfile &PtzProfile) = 0;
	virtual I_RET  LocalPtzSetProfile(int Channel, C_PtzProfile &PtzProfile) = 0;
	virtual I_RET  LocalPtzSetCommand(int Channel, C_PtzCmd &PtzCmd) = 0;

	virtual I_RET  LocalAdvancedSystemGetCaps(int Channel, C_AdvancedSystemCaps &AdvancedSystemCaps) = 0;
	virtual I_RET  LocalAdvancedSystemGetProfile(int Channel, C_AdvancedSystemProfile &AdvancedSystemProfile) = 0;
	virtual I_RET  LocalAdvancedSystemSetProfile(int Channel, C_AdvancedSystemProfile &AdvancedSystemProfile) = 0;

	virtual I_RET  LocalNetCommGetCaps(C_NetCommCaps &NetCommCaps) = 0;
	virtual I_RET  LocalNetCommGetProfile(C_NetWorkProfile &NetWorkProfile) = 0;
	virtual I_RET  LocalNetCommSetProfile(C_NetWorkProfile &NetWorkProfile) = 0;


	virtual I_RET  LocalFirmWareUpgrade(C_FirmwareData &FirmwareData) = 0;
	virtual I_RET  LocalFirmWareUpgradeGetProgress(int &Progress) = 0;

	virtual I_RET  LocalStreamStart(int Channel, int Slave) = 0;
	virtual I_RET  LocalStreamStop(int Channel, int Slave) = 0;

	virtual CAvPacket * LocalSnapshot(int Channel, av_bool bRealTime = av_false, av_uint SnapshotInterval = 0, av_uint ContinuousTimes = 0) = 0;

	virtual I_RET  LocalSyncSystemTime(int Channel, av_timeval &atv) = 0;
	virtual I_RET  LocalRequestIdrFrame(int Channel, int Slave) = 0;


	virtual CNetUv *OnConnect(const char *remoteAddr, const int remotePort);
	virtual I_RET  LocalCheckAuthorization(const char *usrname, const char *passwd, unsigned int &UsrAccess) = 0;




public:
	void *		GetAvProtoSDKInUse();



private:
	int			OnRead(const buf_t *bufbase, int nbufs, struct sockaddr_in *addr = NULL);
	void		OnTimer100Ms();
	int			OnEvent(NetUvStatus nvi, const char *message);
public:
	int			StartTask(unsigned int msg, void *Args);
	int			OnTask(void *Args);
	int			OnTaskAfter(void *Args);

private:
	void		*	m_AvProtoSDKInUse;
};



#endif

