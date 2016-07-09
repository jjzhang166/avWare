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

#ifndef PROTOSTRUCT_H_
#define PROTOSTRUCT_H_
#include <time.h>
#include <list>
#include <vector>
#include <string>
#include "ProtoEnum.h"


#define PROTO_MAX_CHANNEL 32
#define Proto_MAX_SLAVE   2

typedef struct {
	int start;
	int end;
}C_Region;

typedef struct {
	char *base;
	int	  len;
	int	  size;
}Cache_t;

typedef struct{
	NetGetMode NetGetType;
	char ipv4[32];
	char ipv6[32];
	char submask[32];
	char gateway[32];
	char dnsi[32];
	char dnsii[32];
	char macaddr[32];
	char PPpoeName[32];
	char PPpoePasswd[32];
	int  ServicePort;
} C_NetInfo;

typedef struct {
	unsigned int NetDevMask;
	unsigned int NetGetModeMask[NetWorkDev_LAST];
}C_NetWorkCaps;

typedef struct {
	NetWorkDev NetDev;
	NetGetMode NetGetType;
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
	Chip FacChip;
	Sensor FacSenSor;
	unsigned int FacTime;
	char FacProductionModel[64];
	char FacProductionSerialNo[64];
	char FacManufacturer[64];
	char HardWareVersion[32];
	char ProtocolUniqueCode[128];
} C_ManufacturerInfo;

typedef struct {
	unsigned int Platform;
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
	int Channel;
	int Slave;
	unsigned int CapResolutionMask;/*分辨率 掩码*/
	unsigned int CapEncodeProfileMask;/*编码等级 掩码*/
	unsigned int CapStreamCvbrCtrlMask;/*码率控制方式 掩码*/
	unsigned int CapStreamEnCodeTypeMask;/*编码类型 掩码*/
	C_Region	CapFrameRate;
	C_Region  CapGop;
	C_Region  CapCbrRate;
}C_EnCodeCaps;


typedef struct {
	int Channel;
	int Slave;
	Resolution		Size;
	Profile			EnCodeProfile;
	BitRateCtrl		Brc;
	StreamCompress	EnCodeType;
	int				FrameRate;
	unsigned int	BitRate;
	unsigned int	BitLevel;
	unsigned int	Gop;
}C_EnCodeProfile;



typedef struct {
	int Channel;
	int Slave;
	C_Region Brightness;
	C_Region Contrast;
	C_Region Chroma;
	C_Region Saturation;
}C_ImageCaps;
typedef struct {
	int Channel;
	int Slave;
	unsigned char Bright;
	unsigned char Contrast;
	unsigned char Chroma;
	unsigned char Saturation;
}C_ImageProfile;

typedef struct {
	int MaxChannel;
	int MaxSlave;
	bool EnEnCode;
	bool EnDeCode;
	unsigned int EncodeType;
	unsigned int DecodeType;
}C_DeviceCaps;

typedef struct {
	int Channel;
	int Slave;
	unsigned int StreamContentMask;
	TransportMode Tmode;
}C_StreamStart;

typedef struct {
	int Channel;
	int Slave;
	int FrameRate;
	Resolution Size;
	StreamCompress Compress;
	int FrameLen;
	int PacketNum;
	int PacketCnt;
}C_StreamData;

typedef struct {
	int Channel;
	int Slave;
	ProtoSwitch RegisterStatus;
	unsigned int StreamContentMask;
}C_StreamStop;

typedef struct {
	int Channel; 
	int Slave;
	unsigned int IrCutMask;
	unsigned int MirrorMask;
	unsigned int ExposureMask;
	C_Region ExposureValueRegion;
	unsigned int WhiteBalanceMask;
	C_Region WhiteBalanceValueRegion;
	unsigned int AntiFlckerMask;
	C_Region AntiFlckerValueRegion;
	unsigned int CvbsMask;

}C_CaptureCaps;

typedef struct {
	int Channel;
	int Slave;
	IrCutMode IrCut;
	C_Region IrCutTimer;
	unsigned int MirrorMaskValue;
	ExposureMode Exposure;
	int ExposureValue;
	WhiteBalanceMode WhiteBalance;
	int WhiteBalanceValue;
	AntiFlckerMode AntiFlcker;
	int AntiFlckerValue;
	ProtoSwitch Cvbs;
}C_CaptureProfile;
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
	Platform PlatForm;
	char	 UsrName[32];
	char	 UsrPasswd[32];
	unsigned int UserAccessMask;
}C_LogIn;

typedef struct {
	int start_x;
	int start_y;
	int end_x;
	int end_y;
}C_Zone;

typedef struct {
	int Channel;
	int Slave;
	unsigned int AlarmEventMask;
	int MaxAlmIn;
	int MaxAlmOut;
}C_AlarmCaps;

typedef struct {
	int Channel;
	int Slave;
	ProtoSwitch RegisterStatus;
	unsigned int AlarmEventMask;
}C_AlarmRegister;

typedef struct {
	int Channel;
	int Slave;
	unsigned int AlarmEventMask;
	ProtoSwitch AlarmStatus;
	unsigned int AlarmTime;
	unsigned int AlarmPortInNuMask;
	C_Zone MotionDetection;
	C_Zone MotionTrace;
}C_Alarm;

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
	int Channel;
	int Slave;
	unsigned int StartTime;
	unsigned int EndTime;
	unsigned int FileTypeMask;
	std::list <C_RecordFileAttr> FileList;
}C_RecordFileSearch;//这个结构在使用过程中，一定要使用new  不能用malloc替代

typedef struct {
	int Channel;
	int Slave;
	std::list <C_RecordFileAttr> FileList;
}C_RecordFileDownload;//这个结构在使用过程中，一定要使用new  不能用malloc替代


typedef struct {
	int Channel;
	int Slave;
	RecordFileDownloadCtrl DownloadCtrl;
}C_RecordFileDownloadCtrl;


typedef struct {
	ProtoSwitch FtpEnable;
	char FtpServerAddr[64];
	unsigned int FtpServerPort;
	char FtpUserName[32];
	char FtpUserPasswd[32];
}C_FtpProfile;

typedef struct {
	char EmailSmtpServer[32];
	unsigned int EmailPort;
	ProtoSwitch EmailEnable;
	ProtoSwitch EmailSSl;
	char EmailSender[32];
	char EmailUserName[32];
	char EmailUserPasswd[32];
	char EmailTitle[32];
	char EmailRecver1[32];
	char EmailRecver2[32];
	char EmailRecver3[32];
}C_EmailProfile;

typedef struct {
	ProtoSwitch UpnpEnable;
	unsigned int UpnpHttpPort;
	unsigned int UpnpTcpPort;
	unsigned int UpnpRtspPort;
}C_UpnpProfile;

typedef struct {
	ProtoSwitch NtpEnable;
	char NtpServer[64];
	unsigned int NtpPort;
	int NtpTimeZone;
	unsigned int  NtpUpdateTime;
}C_NtpProfile;

typedef struct {
	ProtoSwitch PppoeEnable;
	char PppoeUsrname[32];
	char PppoePasswd[32];
}C_PppoeProfile;

typedef struct {
	ProtoSwitch RtspEnable;
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
	ProtoSwitch DdnsEnable;
	std::list<C_DdnsInfo> DdnsList;
}C_DdnsProfile;//这个结构在使用过程中，一定要使用new  不能用malloc替代

typedef struct
{
	ProtoSwitch P2pEnable;
	ProtoSwitch P2pUseLinkId;
	char P2pLinkId[32];
	char P2pServerIP[32];
	unsigned int P2pPort;
	char P2pUsername[32];
	char P2pUserPasswd[32];
}C_P2pProfile;

typedef struct {
	ProtoSwitch RtmpEnable;
	char RtmpPushServer[64];
	char RtmpPushID[64];
	ProtoSwitch RtmpEnableAudio;
	ProtoSwitch RtmpPushMainStream;
	ProtoSwitch RtmpPushStreamAdaptiveNetSpeed;
}C_RtmpProfile;

typedef struct {
	int			 Channel;
	int			 Slave;
	unsigned int PtzProtocolMask;	//支持协议的掩码
	unsigned int PtzBaudrateMask;	//支持波特率掩码
	unsigned int PtzAddressMax;		//地址范围
	unsigned int PtzDataBitMask;		//数据位范围
	unsigned int PtzStopBitMask;		//停止位范围
	unsigned int PtzVerifyMask;		//检验位掩码
}C_PtzCaps;

typedef struct {
	int			 Channel;
	int			 Slave;
	PtzProtocol  PtzProto;
	unsigned int PtzAddress;
	PtzBaudrate  PtzBaudRate;
	unsigned int PtzDataBit;
	unsigned int PtzStopBit;
	PtzVeriFy	 PtzVerify;
}C_PtzProfile;

typedef struct {
	int			 Channel;
	int			 Slave;
	PtzCommand PtzCmd;
	PtzCommand PtzCmdExt;
	unsigned int PtzSpeed;
	unsigned int PtzPresetPointNu;
	ProtoBool PtzEnableAck;
}C_PtzCmd;


typedef struct {
	ProgressStatus ProgressCmd;
	unsigned int ProgressValue;
	char	DevMac[32];
}C_FirmwareUpgradeProgress;

typedef struct {
	C_FirmwareInfo FirmwareInfo;
	unsigned int FirmwareSize;
	char FirmwareName[128];
	char DevMac[32];
	char UpgradeServer[32];
	unsigned int UpgradePort;
	char LoginUserName[32];
	char LoginPasswd[32];
}C_FirmwareUpgrade;

typedef struct {
	int Channel;
	int Slave;
	unsigned int		VideoCoverMax;
	ProtoSwitch			VideoCoverEnable;
	std::list<C_Zone>		VideoCoverZone;
}C_VideoCoverProfile;

typedef struct {
	ProtoSwitch TimeAreaUseWeek;
	ProtoSwitch TimeAreaUseMon;
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
	std::list <C_Zone> AlarmVideoArea;
	ProtoSwitch AlarmEnable;
	ProtoSwitch AlarmLinkageEmail;
	ProtoSwitch AlarmLinkageFtp;
	ProtoSwitch AlarmLinkageRecord;
	unsigned int AlarmLinkagePrerecordSec;
	ProtoSwitch AlarmLinkageSnapshot;
	ProtoSwitch AlarmLinkageAlmOut;
	ProtoSwitch AlarmLinkageBeep;
	ProtoSwitch AlarmLinkagePtz;
	///第一个uint 0-31通道 第二个uint 32-64通道，以此类推,Channel 标识需按位与
	std::list <unsigned int > AlarmLinkageRecordChannel;
	///第一个uint 0-31通道 第二个uint 32-64通道，以此类推,Channel 标识需按位与
	std::list <unsigned int >AlarmLinkageSnapshotChannel;
}C_AlarmProfileInfo;

typedef struct{
	int Channel;
	int Slave;
	std::list <C_AlarmProfileInfo> AlarmProfileInfo;
}C_AlarmProfile;

typedef struct {
	int Channel;
	int Slave;
	char Server[64];
	unsigned int Port;
	LinkProtocol LinkProto;
	char username[32];
	char userpasswd[32];
}C_ConnectDeviceInfo;

typedef struct {
	unsigned int LogTypeMask;
	ProtoSwitch CyclicCover;
}C_LogCaps;

typedef struct {
	unsigned int LogTypeMask;
	ProtoSwitch CyclicCover;
	ProtoSwitch ClearLog;
}C_LogProfile;

typedef struct {
	LogType LogTypes;
	unsigned int LogTime;
	char LogText[120];
}C_LogContent;
#endif
