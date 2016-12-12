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
} C_ManufacturerInfo;


typedef struct {
//	unsigned int Platform;
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
	time_t	uptime;
	char	devname[64];
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
	unsigned int AlarmEventMask;
	int MaxAlmIn;
	int MaxAlmOut;
}C_AlarmCaps;

typedef struct {
	av_bool RegisterStatus;
	unsigned int AlarmEventMask;
}C_AlarmRegister;
#if 0
typedef struct {
	unsigned int AlarmEventMask;
	av_bool AlarmStatus;
	unsigned int AlarmTime;
	unsigned int AlarmPortInNuMask;
	unsigned int  MotionDetection;
	C_RECT MotionTrace;
}C_Alarm;
#endif

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
	av_bool FtpbEnable;
	char FtpServerAddr[64];
	unsigned int FtpServerPort;
	char FtpUserName[32];
	char FtpUserPasswd[32];
}C_FtpProfile;

typedef struct {
	char EmailSmtpServer[32];
	unsigned int EmailPort;
	av_bool EmailEnable;
	av_bool EmailSSl;
	char EmailSender[32];
	char EmailUserName[32];
	char EmailUserPasswd[32];
	char EmailTitle[32];
	char EmailRecver1[32];
	char EmailRecver2[32];
	char EmailRecver3[32];
}C_EmailProfile;

typedef struct {
	av_bool UpnpEnable;
	unsigned int UpnpHttpPort;
	unsigned int UpnpTcpPort;
	unsigned int UpnpRtspPort;
}C_UpnpProfile;



typedef struct {
	av_bool NtpbEnable;
	char NtpServer[64];
	unsigned int NtpPort;
	int NtpTimeZone;
	unsigned int  NtpUpdateTime;
}C_NtpProfile;

typedef struct {
	av_bool PppoebEnable;
	char PppoeUsrname[32];
	char PppoePasswd[32];
}C_PppoeProfile;


typedef struct {
	av_bool RtspbEnable;
	unsigned int RtspPort;
}C_RtspProfile;

typedef struct {
	char DdnsServerName[64];
	char DdnsServerAddr[64];
	char DdnsUserName[32];
	char DdnsUserPasswd[32];
	int  DdnsPort;
}C_DdnsInfo;

typedef struct {
	unsigned int DdnsIndex;
	av_bool DdnsbEnable;
	std::list<C_DdnsInfo> DdnsList;
}C_DdnsProfile;//这个结构在使用过程中，一定要使用new  不能用malloc替代


typedef struct
{
	av_bool P2pbEnable;
	av_bool P2pbUseLinkId;
	char P2pLinkId[32];
	char P2pServerIP[32];
	unsigned int P2pPort;
	char P2pUsername[32];
	char P2pUserPasswd[32];
}C_P2pProfile;

typedef struct {
	av_bool RtmpbEnable;
	char RtmpPushServer[64];
	char RtmpPushID[64];
	av_bool RtmpbEnableAudio;
	av_bool RtmpbPushMainStream;
	av_bool RtmpbPushStreamAdaptiveNetSpeed;
}C_RtmpProfile;

typedef struct {
	unsigned int PtzProtocolMask;	//支持协议的掩码
	unsigned int PtzBaudrateMask;	//支持波特率掩码
	unsigned int PtzAddressMax;		//地址范围
	unsigned int PtzDataBitMask;		//数据位范围
	unsigned int PtzStopBitMask;		//停止位范围
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
	unsigned int		VideoCoverMax;
	av_bool				VideoCoverEnable;
	std::list<C_RECT>	VideoCoverZone;
}C_VideoCoverProfile;


typedef struct {
	av_bool TimeAreaUseWeek;
	av_bool TimeAreaUseMon;
	///若使用week 则表示每周几，0表示周一至周日
	///若使用mon 则表示每月TimeAreaIndex号 若0表示每天
	int TimeAreaIndex;
	unsigned int TimeAreaStartSec;
	unsigned int TimeAreaStopSec;
}C_TimeArea;

typedef struct {
	AlarmEvent AlarmEventName;
	std::list <C_TimeArea> AlarmTimeArea;
	unsigned int AlarmVideoAreaMax;
	std::list <C_RECT> AlarmVideoArea;
	av_bool AlarmEnable;
	av_bool AlarmLinkageEmail;
	av_bool AlarmLinkageFtp;
	av_bool AlarmLinkageRecord;
	unsigned int AlarmLinkagePrerecordSec;
	av_bool AlarmLinkageSnapshot;
	av_bool AlarmLinkageAlmOut;
	av_bool AlarmLinkageBeep;
	av_bool AlarmLinkagePtz;
	///第一个uint 0-31通道 第二个uint 32-64通道，以此类推,Channel 标识需按位与
	std::list <unsigned int > AlarmLinkageRecordChannel;
	///第一个uint 0-31通道 第二个uint 32-64通道，以此类推,Channel 标识需按位与
	std::list <unsigned int >AlarmLinkageSnapshotChannel;
}C_AlarmProfileInfo;

typedef struct{
	std::list <C_AlarmProfileInfo> AlarmProfileInfo;
}C_AlarmProfile;

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
	unsigned int		_msg;
	unsigned int		_stat;
	union
	{
		C_DspCaps			DspCaps;
		C_CaptureCaps		CaptureCaps;
		C_CaptureProfile	CaptureProfile;
		C_EncodeCaps		EncodeCaps;
		C_EnCodeProfile		EnCodeProfile;
		C_ImageCaps			ImageCaps;
		C_ImageProfile		ImageProfile;
		C_PtzCaps			PtzCaps;
		C_PtzProfile		PtzProfile;
		C_PtzCmd			PtzCmd;
		C_ManufacturerInfo	ManfacturerInfo;
		C_NetCommCaps		NetCommCaps;
		C_NetWorkProfile	NetWorkProfile;
		C_FirmwareInfo		FirmwareInfo;
		C_AudioCaps			AudioCaps;
		C_AudioProfile		AudioProfile;
	};
}NetProtocolParam;


#endif

