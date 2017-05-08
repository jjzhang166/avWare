#ifndef _AVPROTOSTRUCT_H_
#define _AVPROTOSTRUCT_H_

#include "Apis/AvWareCplusplus.h"
#include "AvEnum.h"
#include "AvSystemStruct.h"





typedef struct {
	NetCommT NetDevice;
	NetCommGetMode NetGetType;
	char Ipv4[32];
	char Ipv6[64];
	char SubMask[32];
	char Mac[32];
	char GateWay[32];
	char DnsI[32];
	char DnsII[32];
	char PppoeName[64];
	char PppoePasswd[64];
	char wifiSSID[128];
	char wifiPasswd[128];
	int	 ServicePort;
	int  WebServicePort;
}C_NetWorkProfile;

typedef struct {
	unsigned int ChannelMax;
	AvChip FacChip;
	AvSensor FacSenSor;
	unsigned int FacTime;
	char FacProductionModel[64];
	char FacProductionSerialNo[64];
	char FacManufacturer[64];
	char HardWareVersion[32];
	char ProtocolUniqueCode[128];
	unsigned int HardWareInterfaceMask;
} C_ManufacturerInfo;


typedef struct {
	unsigned int Version;
	unsigned int ChipMask;
	unsigned int SensorMask;
	unsigned int CustomMask;
	char		 Descriptor[128];
	char		 BuildTime[64];
	char		 ProtoVersion[64];
	char		 KernelVersion[64];
	char		 FilesystemVerion[64];
}C_FirmwareInfo;

typedef struct {
	C_FirmwareInfo	FirmwareInfo;

	char			FirmwareName[128]; 
	unsigned char *	FirmwareAddr;
	int				FirmwareSize;
	int				FirmwareSent;

	char DeviceName[64];
	char IpAddr[64];
	char DeviceMac[64];
	int  Port;
	char Usrname[64];
	char Passwd[64];

	ProgressStatus	_status;
	int				_value;

}C_FirmWareUpgrade;

typedef struct {
	ProgressStatus ProgressCmd;
	unsigned int ProgressValue;
	char	DevMac[32];
}C_FirmwareUpgradeProgress;


typedef struct {
	unsigned char *Data;
	unsigned int   DataLen;
	unsigned int   PackNum;
	unsigned int   PackCnt;
}C_FirmwareData;



typedef struct {
	unsigned int  StreamContentMask;
	TransportMode TMode;
}C_StreamStart;

typedef struct {
	unsigned int  StreamContentMask;
}C_StreamStop;

typedef struct {
	StreamContent Content;
	int FrameRate;
	CaptureSize Size;
	AvComp Comp;
	avFrameT FrameT;
	unsigned int FrameLen;
	unsigned int PacketNum;
	unsigned int PacketCnt;
	unsigned int PacketSec;
	unsigned int PacketUsec;
	unsigned int PacketSeqNum;
}C_StreamData;




typedef struct {
	time_t				UpTime;
	char				DeviceName[64];
	C_NetStatusInfo		NetStatus[NetCommT_LAST];
	C_CpuMemStatusInfo	CpuMemStatus[32];
}C_DevStatusInfo;


typedef struct {
	C_NetWorkProfile	NetWorkProfile;
	C_FirmwareInfo		FirmwareInfo;
	C_ManufacturerInfo	ManufacturerInfo;
	C_DevStatusInfo		DevStatusInfo;
}C_DevSearch;


typedef struct {
	char UserName[32];
	char UserPasswd[32];
	unsigned int UserAccessMask;
	UserCategory UserCateGory;
	char UserCreatBy[32];
}C_UserInfo;

typedef struct {
	char	 UsrName[32];
	char	 UsrPasswd[32];
	unsigned int UserAccessMask;
}C_LogIn;


typedef struct {
	int WeekIndex;
	unsigned int StartSec;
	unsigned int StopSec;
}C_TimeArea;


typedef struct {
	AlarmEvent				AlarmEventName;
	AlarmStat				AlarmStatus;
	av_u32					AlarmTime;
	av_u32					Channel;
	av_u32					Slave;
	union
	{
		av_u32	MdResult[ConfMotionDetectionLine];
		av_u32	IoResult;
	};

}C_AlmMsg;

typedef struct {
	av_bool RegisterStatus;
	unsigned int AlarmEventMask;
}C_AlarmRegister;

typedef struct{
	av_u32		AlarmEventMask;
	av_bool		AlarmbEnable;
	C_TimeArea  AlarmTimeArea[AvMaxWeeks][AvMaxTimeArea];
	av_u32		AlarmMotionArea[ConfMotionDetectionLine];
	av_u32		AlarmMotionLevel;
	av_bool		AlarmbLinkageEmail;
	av_bool		AlarmbLinkageFtp;
	av_u32		AlarmLinkagePrerecordSec;
	av_bool		AlarmbLinkageSnapshot;
	av_bool		AlarmbLinkageAlmOut;
	av_bool		AlarmbLinkageBeep;
	av_bool		AlarmbLinkagePtz;
	av_bool		AlarmbLinkageRecord;
}C_AlarmProfile;

// 
// typedef struct {
// 	AlarmEvent	AlarmEventName;
// 	av_bool		AlarmbStat;
// 	av_u32		AlarmTime;
// 	av_u32		AlarmPortInNuMask;
// 	av_u32		AlarmMotionArea[ConfMotionDetectionLine];
// }C_Alarm;


typedef struct {
	///单位M
	unsigned int DiskTotalCapacity;
	unsigned int DiskSurplusCapacity;
	unsigned int DiskNumber;
	unsigned int DiskStatusMask[32];//下标表示第几块硬盘状态
}C_DiskStatus;

typedef struct {
	char FileName[64];
	unsigned int FileSize;
	unsigned int FileStartTime;
	unsigned int FileEndTime;
	unsigned int FileTypeMask;
}C_RecordFileAttr;

typedef struct {
	unsigned int StartTime;
	unsigned int EndTime;
	unsigned int FileTypeMask;
	std::list <C_RecordFileAttr> FileList;
}C_RecordFileSearch;//这个结构在使用过程中，一定要使用new  不能用malloc替代


typedef struct {
	std::list <C_RecordFileAttr> FileList;
}C_RecordFileDownload;//这个结构在使用过程中，一定要使用new  不能用malloc替代


typedef struct {
	RecordFileDownloadCtrl DownloadCtrl;
}C_RecordFileDownloadCtrl;


typedef struct {
	av_bool bEnable;
	EmailEncodeType EncodeType;
	int ServicePort;
	char ServerAddress[64];
	char UserName[64];
	char PassWord[64];
	char Sender[64];
	char Receiver1[64];
	char Receiver2[64];
	char Title[64];
}C_SmtpProfile;

typedef struct{
	av_bool bEnable;
	int		ServicePort;
	char	ServerAddress[64];
	char	UserName[64];
	char	PassWord[64];
	char	RemotePath[64];
}C_FtpProfile;

typedef struct{
	av_bool bEnable;
	DdnsType Type;
	char ServerAddress[64];
	char UserName[64];
	char PassWord[64];
	int  UpdateInterval;
}C_DdnsProfile;

typedef struct {
	av_bool bEnableHttpPort;
	av_bool bEnableMoonProtoPort;
	av_bool bEnableRtspPort;
}C_UpnpProfile;


typedef struct
{
	av_bool bEnable;
	av_bool bUseLinkId;
	char LinkId[64];
	char ServerAddress[64];
	unsigned int ServicePort;
	char UserName[32];
	char PassWord[32];
}C_P2pProfile;

typedef struct {
	av_bool bEnable;
	char ServerAddress[64];
	TimeZone Timezone;
	unsigned int  UpdateInterval;
}C_NtpProfile;

typedef struct {
	av_bool PppoebEnable;
	char PppoeUsrname[32];
	char PppoePasswd[32];
}C_PppoeProfile;


typedef struct {
	av_bool bEnable;
	unsigned int ServicePort;
}C_RtspProfile;

typedef struct {
	av_bool bEnable;
	char PushServer[64];
	char PushStrings[64];
	av_bool bEnableAudio;
	unsigned int PushStream;
	unsigned int PushChannel;
	av_bool RtmpbPushStreamAdaptiveNetSpeed;
}C_RtmpNodeProfile;

typedef struct {
	std::list <C_RtmpNodeProfile> RtmpNodeProfileList;
}C_RtmpProfile;

typedef struct {
	unsigned int PtzProtocolMask;	//支持协议的掩码
	unsigned int PtzBaudrateMask;	//支持波特率掩码
	unsigned int PtzAddressMax;		//地址范围
	unsigned int PtzDataBitMask;	//数据位范围
	unsigned int PtzStopBitMask;	//停止位范围
	unsigned int PtzVerifyMask;		//检验位掩码
}C_PtzCaps;

typedef struct {
	PtzProtocol  PtzProto;
	unsigned int PtzAddress;
	PtzBaudrate  PtzBaudRate;
	unsigned int PtzDataBit;
	unsigned int PtzStopBit;
	PtzVeriFy	 PtzVerify;
}C_PtzProfile;

typedef struct {
	PtzCommand		PtzCmd;
	unsigned int	PtzSpeed;
	unsigned int	Value;
	int				ValueExt1;
	int				ValueExt2;
	int				ValueExt3;
	int				ValueExt4;
	av_bool			bEnAck;
}C_PtzCmd;







typedef struct {
	char Server[64];
	unsigned int Port;
	LinkProtocol LinkProto;
	char username[32];
	char userpasswd[32];
}C_ConnectDeviceInfo;


typedef struct {
	unsigned int LogTypeMask;
	av_bool CyclicCover;
}C_LogCaps;

typedef struct {
	unsigned int LogTypeMask;
	av_bool CyclicCover;
	av_bool ClearLog;
}C_LogProfile;

typedef struct {
	LogType LogTypes;
	unsigned int LogTime;
	char LogText[120];
}C_LogContent;


typedef struct {
	av_uint ContinuousTimes;
	av_uint SnapshotInterval;
	av_bool bRealTime;
}C_Snapshot;

typedef struct{
	av_uint Second;
}C_SyncSystemTime;

typedef struct {
	unsigned int		_msg;
	unsigned int		_stat;
	union
	{
		C_DspCaps						DspCaps;
		C_CaptureCaps					CaptureCaps;
		C_CaptureProfile				CaptureProfile;
		C_EncodeCaps					EncodeCaps;
		C_EnCodeProfile					EnCodeProfile;
		C_ImageCaps						ImageCaps;
		C_ImageProfile					ImageProfile;
		C_PtzCaps						PtzCaps;
		C_PtzProfile					PtzProfile;
		C_PtzCmd						PtzCmd;
		C_ManufacturerInfo				ManfacturerInfo;
		C_NetCommCaps					NetCommCaps;
		C_NetWorkProfile				NetWorkProfile;
		C_FirmwareInfo					FirmwareInfo;
		C_AudioCaps						AudioCaps;
		C_AudioProfile					AudioProfile;
		C_PtzAdvancedCameraLensCaps		PtzCameralensCaps;
		C_PtzAdvancedCameraLensProfile	PtzAdvancedCameraLensProfile;

		C_SmtpProfile					SmtpProfile;
		C_RtspProfile					RtspProfile;
		C_FtpProfile					FtpProfile;
		C_CoverProfile					CoverProfile;
		C_OverLayCaps					OverLayCaps;
		C_OverLayProfile				OverLayProfile;
		C_UpnpProfile					UpnpProfile;
		C_NtpProfile					NtpProfile;
		C_P2pProfile					P2pProfile;
		C_DdnsProfile					DdnsProfile;
		C_AlarmCaps						AlarmCaps;
		C_AlarmProfile					AlarmProfile;
		C_SyncSystemTime				SyncSystemTime;
	};
	C_RecordFileSearch					RecordFileSearch;
	C_RtmpProfile						RtmpProfile;
}NetProtocolParam;



typedef struct {
	__Msg _msg;
	union{
		C_LogCaps							LogCaps;
		C_PtzAdvancedCameraLensCaps			PtzCameralensCaps;
		C_OverLayCaps						OverLayCaps;
		C_AlarmCaps							AlarmCaps;
		C_DspCaps							DspCaps;
	};
}C_AdvancedSystemCaps;


typedef struct {
	__Msg _msg;
	union {
		C_DevStatusInfo						DevStatusInfo;
		C_SmtpProfile						SmtpProfile;
		C_FtpProfile						FtpProfile;
		C_UpnpProfile						UpnpProfile;
		C_RtspProfile						RtspProfile;
		C_LogProfile						LogProfile;
		C_FirmwareInfo						FirmwareInfo;
		C_ManufacturerInfo					ManufacturerInfo;
		C_PtzAdvancedCameraLensProfile		PtzAdvancedCameraLensProfile;
		C_CoverProfile						CoverProfile;
		C_OverLayProfile					OverLayProfile;
		C_NtpProfile						NtpProfile;
		C_DdnsProfile						DdnsProfile;
		C_NetWorkProfile					NetWorkProfile;
		C_P2pProfile						P2pProfile;
		C_AlarmProfile						AlarmProfile;
		C_SyncSystemTime					SyncSystemTime;
	};
	C_RtmpProfile							RtmpProfile;
}C_AdvancedSystemProfile;

#endif

