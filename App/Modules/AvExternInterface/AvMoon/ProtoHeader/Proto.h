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
#include "ProtoStream.h"
#include "libuvCpp.h"
#include "ProtoStruct.h"
#include "ProtoEnum.h"



#ifdef PROTOSHARE_EXPORTS

#if defined (WIN32)

#if defined (USE_STATIC_LIB)
#define PROTOSHARE_API
#else
#define PROTOSHARE_API __declspec(dllexport)
#endif

#else //else define win32
#define PROTOSHARE_API __attribute__((visibility("default")))
#endif //end define win32

#else //else PROTOSHARE_EXPORTS

#if defined(WIN32)
#if defined (USE_STATIC_LIB)
#define PROTOSHARE_API
#else
#define PROTOSHARE_API __declspec(dllimport)
#endif

#else
#define PROTOSHARE_API
#endif

#endif //end PROTOSHARE_EXPORTS

#ifndef ProtoMask
#define ProtoMask(code) (0x01 << (code))
#endif

typedef enum {
	LINK_STATUS_LINKED,		//网络层已经连接
	LINK_STATUS_LOGINED,	//已经登陆
}LINK_STATUS;

typedef enum {
	PROTO_STATUS_SUCCEED,
	PROTO_STATUS_FAILED,
	PROTO_STATUS_NONSUPPORT,
	PROTO_STATUS_TIMEOUT,
	PROTO_STATUS_OVER,

	PROTO_STATUS_BEGIN,		//开始数据
	PROTO_STATUS_ING,		//中间数据
	PROTO_STATUS_END,		//数据结束

	PROTO_STATUS_INVALID = 0xffffffff
}STATUS_CODE;
typedef enum{
	COMMAND_ACK_REQ		= 0x80000000,
	COMMAND_ACK_RESP	= 0x40000000,
	COMMAND_ACK_SET		= 0X20000000,
	COMMAND_ACK_SETRESP = 0X10000000,
	
	COMMAND_ACK_ADD		= 0x08000000,
	COMMAND_ACK_ADDRESP	= 0x04000000,
	COMMAND_ACK_DEL		= 0x02000000,
	COMMAND_ACK_DELRESP	= 0x01000000,

	COMMAND_ACK_MODIFY		= 0x00800000,
	COMMAND_ACK_MODIFYRESP	= 0x00400000,

	COMMAND_ACK_DELCMD		= 0x0000ffff,
	COMMAND_ACK_DELMSG		= 0xffff0000,
}COMMAND_E;


class PROTOSHARE_API CProtoMoon :public CAvObject, public CNetUv
{
public:
	CProtoMoon();
	~CProtoMoon();
public:
	unsigned int MoonGetVer			();
	const char * MoonGetVerStr		();
public:
	int MoonService					(const char *address, const int port);
	int MoonConnect					(const char *address, const int port, const char *usrname, const char *passwd);
	int MoonConnect					(const char *linkid);
	int MoonConnectUpgrade			(C_FirmwareUpgrade &FirmwareUpgrade, void *FirwareData, int DateLen);
public:
	int MoonSearchService			(char *BindAddr = NULL);
	int MoonSearchStart				(int sec, std::list<C_DevSearch > &DevList);
	int MoonSearchUpgrade			(std::list <C_FirmwareUpgrade> &FirmwareUpgradeList);
	int MoonSearchUpgrade			(C_FirmwareUpgrade &FirmwareUpgrade);

public:
	virtual int OnMoonUpgradeProgress(C_FirmwareUpgradeProgress FirmwareUpgradeProgress);

public:
	int RemoteGetDeviceCaps			();
	int RemoteGetDeviceStatus		();
	int RemoteGetFirmwareVersion	();

	int RemoteGetManufacturerInfo	();
	int RemoteSetManufacturerInfo	(C_ManufacturerInfo &FacInfo);

	int RemoteGetNetWorkCaps		();
	int RemoteGetNetWorkProfile		();
	int RemoteSetNetWorkProfile		(C_NetWorkProfile &NetWorkProfile);

	int RemoteGetDeCodeCaps			(int Channel = -1, int Slave = -1);
	int RemoteGetDeCodeProfile		(int Channel = -1, int Slave = -1);
	int RemoteSetDeCodeProfile		(int Channel = -1, int Slave = -1);


	int RemoteGetEnCodeCaps			(int Channel = -1, int Slave = -1);
	int RemoteGetEnCodeProfile		(int Channel = -1, int Slave = -1);
	int RemoteSetEnCodeProfile		(C_EnCodeProfile &EnCodeProfile);

	int RemoteGetImageCaps			(int Channel = -1, int Slave = -1);
	int RemoteGetImageProfile		(int Channel = -1, int Slave = -1);
	int RemoteSetImageProfile		(C_ImageProfile &Image);

	int RemoteGetVideoCoverProfile	(int Channel = -1, int Slave = -1);
	int RemoteSetVideoCoverProfile(C_VideoCoverProfile &VideoCoverProfile);

	int RemoteGetCaptureCaps		(int Channel = -1, int Slave = -1);
	int RemoteGetCaptureProfile		(int Channel = -1, int Slave = -1);
	int RemoteSetCaptureProfile		(C_CaptureProfile &CaptureProfile);

	int RemoteGetPtzCaps			(int Channel = -1, int Slave = -1);
	int RemoteGetPtzProfile			(int Channel = -1, int Slave = -1);
	int RemoteSetPtzProfile			(C_PtzProfile &PtzProfile);
	int RemotePtzCommand			(C_PtzCmd &PtzCmd);


	int RemoteGetSysTimeProfile		();

	int RemoteGetStream				(C_StreamStart &StreamStart);
	int RemoteStopStream			(C_StreamStop &StreamStop);
	void StreamData					(CStreamPacket *Packet);

	int RemoteGetDiskStatus			();
	int RemoteGetAlarmCaps			(int Channel = -1, int Slave = -1);
	int RemoteGetAlarmPorfile		(int Channel = -1, int Slave = -1);
	int RemoteSetAlarmPorfile		(C_AlarmProfile &AlarmProfile);
	int RemoteSetAlarmRegister		(C_AlarmRegister &AlarmRegister);


	int RemoteUserGetList			();
	int RemoteUserAdd				(C_UserInfo &UserInfo);
	int RemoteUserDel				(C_UserInfo &UserInfo);
	int RemoteUserModify			(C_UserInfo &UserInfo);
	
	int RemoteGetRecordFileSearch	(C_RecordFileSearch &RecordFileSearch);
	int RemoteGetRecordFileDownload	(C_RecordFileDownload &RecordFileDownload);
	int RemoteSetRecordFileDownloadCtrl(C_RecordFileDownloadCtrl &RecordFileDownloadCtrl);

	int RemoteGetFtpProfile();
	int RemoteSetFtpProfile(C_FtpProfile &FtpProfile);
	int RemoteGetEmailProfile();
	int RemoteSetEmailProfile(C_EmailProfile &EmailProfile);
	int RemoteGetUpnpProfile();
	int RemoteSetUpnpProfile(C_UpnpProfile &UpnpProfile);
	int RemoteGetNtpProfile();
	int RemoteSetNtpProfile(C_NtpProfile &NtpProfile);
	int RemoteGetPppoeProfile();
	int RemoteSetPppoeProfile(C_PppoeProfile &PpppoeProfile);
	int RemoteGetRtspProfile();
	int RemoteSetRtspProfile(C_RtspProfile &RtspProfile);
	int RemoteGetDdnsProfile();
	int RemoteSetDdnsProfile(C_DdnsProfile &DdnsProfile);
	int RemoteGetP2pProfile();
	int RemoteSetP2pProfile(C_P2pProfile &P2pProfile);
	int RemoteGetRtmpProfile();
	int RemoteSetRtmpProfile(C_RtmpProfile &RtmpProfile);

	int RemoteConnectDeviceGetList();
	int RemoteConnectDeviceAdd(std::list<C_ConnectDeviceInfo> &AddDeviceList);
	int RemoteConnectDeviceDel(std::list<C_ConnectDeviceInfo> &DelDeviceList);
	int RemoteConnectDeviceModify(std::list<C_ConnectDeviceInfo> &ModifyDeviceList);


	int RemoteGetLogCaps();
	int RemoteGetLogProfile();
	int RemoteSetLogProfile(C_LogProfile &LogProfile);
	int RemoteGetLogList(unsigned int SearchStartTime, unsigned int SearchEndTime);

private:
	//next two func only for "LocalGetRecordFileDownload" callback
	int RemoteSendRecordFileData(CStreamPacket *Packet);
	int RemoteSendRecordFileDataStatus(int Channel, int Slave);

public:
	virtual int OnRemoteGetDeviceCaps		(C_DeviceCaps		&DeviceCaps,	int status);
	virtual int OnRemoteGetDeviceStatus		(C_DevStatusInfo	&DevStatusInfo, int status);
	virtual int OnRemoteGetDeCodeProfile	(									int status);
	virtual int OnRemoteSetDeCodeProfile	(									int status);


	virtual int OnRemoteGetEnCodeCaps		(C_EnCodeCaps		&Encodecaps,	int status);
	virtual int OnRemoteGetImageCaps		(C_ImageCaps		&ImageCaps,		int status);
	virtual int OnRemoteGetCaptureCaps		(C_CaptureCaps		&CaptureCaps,	int status);

	virtual int OnRemoteGetImageProfile		(C_ImageProfile		&ImageProfile,	int status);
	virtual int OnRemoteGetCaptureProfile	(C_CaptureProfile	&CaptureProfile,int status);
	virtual int OnRemoteGetEnCodeProfile	(C_EnCodeProfile	&EnCodeProfile, int status);

	virtual int OnRemoteSetImageProfile		(C_ImageProfile		&ImageProfile,	int status);
	virtual int OnRemoteSetCaptureProfile	(C_CaptureProfile	&CaptureProfile,int status);
	virtual int OnRemoteSetEnCodeProfile	(C_EnCodeProfile	&EnCodeProfile, int status);

	virtual int OnRemoteGetNetWorkCaps		(C_NetWorkCaps		&NetWorkCaps,	int status);
	virtual int OnRemoteGetNetWorkProfile	(C_NetWorkProfile	&NetWorkProfile,int status);
	virtual int OnRemoteSetNetWorkProfile	(C_NetWorkProfile	&NetWorkProfile,int status);

	virtual int OnRemoteGetVideoCoverProfile	(C_VideoCoverProfile &VideoCoverProfile, int status);
	virtual int OnRemoteSetVideoCoverProfile(C_VideoCoverProfile &VideoCoverProfile, int status);


	virtual int OnRemoteGetFirmwareVersion	(C_FirmwareInfo		&FirmwareInfo,	int status);
	virtual int OnRemoteGetManufacturerInfo	(C_ManufacturerInfo &FacInfo,		int status);
	virtual int OnRemoteSetManufacturerInfo	(C_ManufacturerInfo &FacInfo,		int status);

	
	virtual int OnRemoteGetSysTimeProfile	(int status);
	virtual int OnRemoteStreamData			(CStreamPacket *pStream);
	virtual int OnRemoteStreamStart			(C_StreamStart &StreamStart, COMMAND_E ReqOrResp);
	virtual int OnRemoteStreamStop			(C_StreamStop &StreamStop, COMMAND_E ReqOrResp);
	virtual int OnRemoteMoonConnect			(STATUS_CODE StatusCode);

	virtual int OnRemoteGetDiskStatus(C_DiskStatus &DiskStatus, int status);
	virtual int OnRemoteGetAlarmCaps(C_AlarmCaps &AlarmCaps, int status);
	virtual int OnRemoteSetRegister(C_AlarmRegister &AlarmRegister, int status);
	virtual int OnRemoteAlarm(C_Alarm &Alarm);
	virtual int OnRemoteGetAlarmProfile(C_AlarmProfile &AlarmProfile, int status);
	virtual int OnRemoteSetAlarmProfile(C_AlarmProfile &AlarmProfile, int status);

	virtual int OnRemoteUserGetList(std::list <C_UserInfo> &UserInfoList);
	virtual int OnRemoteUserAdd(C_UserInfo &UserInfo);
	virtual int OnRemoteUserDel(C_UserInfo &UserInfo);
	virtual int OnRemoteUserModify(C_UserInfo &UserInfo);

	virtual int OnRemoteGetRecordFileSearch(C_RecordFileSearch &RecordFileSearch, int status);
	virtual int OnRemoteGetRecordFileDownload(C_RecordFileDownload &RecordFileDownload, int status);
	virtual int OnRemoteRecordFileData(CStreamPacket *pStream);
	virtual int OnRemoteSetRecordFileDownloadCtrl(C_RecordFileDownloadCtrl &RecordFileDownloadCtrl, int status);


	virtual int OnRemoteGetFtpProfile	(C_FtpProfile	&FtpProfile,	int status);
	virtual int OnRemoteSetFtpProfile	(C_FtpProfile	&FtpProfile,	int status);
	virtual int OnRemoteGetEmailProfile	(C_EmailProfile &EmailProfile,	int status);
	virtual int OnRemoteSetEmailProfile	(C_EmailProfile &EmailProfile,	int status);
	virtual int OnRemoteGetUpnpProfile	(C_UpnpProfile	&UpnpProfile,	int status);
	virtual int OnRemoteSetUpnpProfile	(C_UpnpProfile	&UpnpProfile,	int status);
	virtual int OnRemoteGetNtpProfile	(C_NtpProfile	&NtpProfile,	int status);
	virtual int OnRemoteSetNtpProfile	(C_NtpProfile	&NtpProfile,	int status);
	virtual int OnRemoteGetPppoeProfile	(C_PppoeProfile &PpppoeProfile, int status);
	virtual int OnRemoteSetPppoeProfile	(C_PppoeProfile &PpppoeProfile, int status);
	virtual int OnRemoteGetRtspProfile	(C_RtspProfile	&RtspProfile,	int status);
	virtual int OnRemoteSetRtspProfile	(C_RtspProfile	&RtspProfile,	int status);
	virtual int OnRemoteGetDdnsProfile	(C_DdnsProfile	&DdnsProfile,	int status);
	virtual int OnRemoteSetDdnsProfile	(C_DdnsProfile	&DdnsProfile,	int status);
	virtual int OnRemoteGetP2pProfile	(C_P2pProfile	&P2pProfile,	int status);
	virtual int OnRemoteSetP2pProfile	(C_P2pProfile	&P2pProfile,	int status);
	virtual int OnRemoteGetRtmpProfile	(C_RtmpProfile	&RtmpProfile,	int status);
	virtual int OnRemoteSetRtmpProfile	(C_RtmpProfile	&RtmpProfile,	int status);
	virtual int OnRemoteGetPtzCaps		(C_PtzCaps		&PtzCaps,		int status);
	virtual int OnRemoteGetPtzProfile	(C_PtzProfile	&PtzProfile,	int status);
	virtual int OnRemoteSetPtzProfile	(C_PtzProfile	&PtzProfile,	int status);
	virtual int OnRemotePtzCommand		(C_PtzCmd		&PtzCmd,		int status);

	virtual int OnRemoteConnectDeviceGetList(std::list<C_ConnectDeviceInfo> &ConnectDeviceList, int status);
	virtual int OnRemoteConnectDeviceAdd	(std::list<C_ConnectDeviceInfo> &AddDeviceList, int status);
	virtual int OnRemoteConnectDeviceDel	(std::list<C_ConnectDeviceInfo> &DelDeviceList, int status);
	virtual int OnRemoteConnectDeviceModify	(std::list<C_ConnectDeviceInfo> &ModifyDeviceList, int status);

	virtual int OnRemoteFirmwareUpgradeProgress(C_FirmwareUpgradeProgress &FirmwareUpgradeProgress, int status);

	virtual int OnRemoteGetLogCaps(C_LogCaps &LogCaps, int status);
	virtual int OnRemoteGetLogProfile(C_LogProfile &LogProfile, int status);
	virtual int OnRemoteSetLogProfile(C_LogProfile &LogProfile, int status);
	virtual int OnRemoteGetLogList(std::list <C_LogContent> &LogContent, int status);

public:
	virtual int LocalGetDeviceCaps(C_DeviceCaps &DeviceCaps);
	virtual int LocalGetDeCodeCaps();
	virtual int LocalGetDeCodeProfile();
	
	virtual int LocalGetImageCaps(C_ImageCaps &ImageCaps);
	virtual int LocalGetImageProfile(C_ImageProfile &ImageProfile);
	virtual int LocalSetImageProfile(C_ImageProfile &ImageProfile);

	virtual int LocalGetVideoCoverProfile(C_VideoCoverProfile &VideoCoverProfile);
	virtual int LocalSetVideoCoverProfile(C_VideoCoverProfile &VideoCoverProfile);


	virtual int LocalGetCaptureCaps(C_CaptureCaps &CaptureCaps);
	virtual int LocalGetCaptureProfile(C_CaptureProfile &CaptureProfile);
	virtual int LocalSetCaptureProfile(C_CaptureProfile &CaptureProfile);

	virtual int LocalGetEnCodeCaps(C_EnCodeCaps &EnCodeCaps);
	virtual int LocalGetEnCodeProfile(C_EnCodeProfile &EnCodeProfile);
	virtual int LocalSetEnCodeProfile(C_EnCodeProfile &EnCodeProfile);

	virtual int LocalGetAlarmCaps();
	virtual int LocalGetAlarmProfile(C_AlarmProfile &AlarmProfile);
	virtual int LocalSetAlarmProfile(C_AlarmProfile &AlarmProfile);

	virtual int LocalGetSysTimeProfile();
	virtual int LocalSetSysTimeProfile();

	virtual int LocalGetNetWorkCaps			(C_NetWorkCaps &NetWorkCaps);
	virtual int LocalGetNetWorkProfile		(C_NetWorkProfile &NetWorkProfile);
	virtual int LocalSetNetWorkProfile		(C_NetWorkProfile &NetWorkProfile);

	virtual int LocalGetManufacturerInfo	(C_ManufacturerInfo &FacInfo);
	virtual int LocalSetManufacturerInfo	(C_ManufacturerInfo &FacInfo);

	virtual int LocalGetDevStatus			(C_DevStatusInfo &DevStatus);
	virtual int LocalGetFirmwareInfo		(C_FirmwareInfo &FirmwareInfo);
	virtual int LocalCheckOutAuthorization	(const char *UsrName, const char *UsrPasswd, unsigned int &UsrAccess);

	virtual int LocalGetDiskStatus			(C_DiskStatus &DiskStatus);
	virtual int LocalGetAlarmCaps			(C_AlarmCaps &AlarmCaps);
	virtual int LocalGetUserList			(std::list <C_UserInfo> &UserList);
	virtual int LocalAddUser				(C_UserInfo &AddUser);
	virtual int LocalDelUser				(C_UserInfo &DelUser);
	virtual int LocalModifyUser				(C_UserInfo &ModifyUser);
	
	virtual int LocalGetRecordFileSearch	(C_RecordFileSearch &RecordFileSearch);
	virtual int LocalGetRecordFileDownload	(C_RecordFileDownload &RecordFileDownload);

	virtual int LocalGetFtpProfile			(C_FtpProfile	&FtpProfile);
	virtual int LocalSetFtpProfile			(C_FtpProfile	&FtpProfile);
	virtual int LocalGetEmailProfile		(C_EmailProfile &EmailProfile);
	virtual int LocalSetEmailProfile		(C_EmailProfile &EmailProfile);
	virtual int LocalGetUpnpProfile			(C_UpnpProfile	&UpnpProfile);
	virtual int LocalSetUpnpProfile			(C_UpnpProfile	&UpnpProfile);
	virtual int LocalGetNtpProfile			(C_NtpProfile	&NtpProfile);
	virtual int LocalSetNtpProfile			(C_NtpProfile	&NtpProfile);
	virtual int LocalGetPppoeProfile		(C_PppoeProfile &PpppoeProfile);
	virtual int LocalSetPppoeProfile		(C_PppoeProfile &PpppoeProfile);
	virtual int LocalGetRtspProfile			(C_RtspProfile	&RtspProfile);
	virtual int LocalSetRtspProfile			(C_RtspProfile	&RtspProfile);
	virtual int LocalGetDdnsProfile			(C_DdnsProfile	&DdnsProfile);
	virtual int LocalSetDdnsProfile			(C_DdnsProfile	&DdnsProfile);
	virtual int LocalGetP2pProfile			(C_P2pProfile	&P2pProfile);
	virtual int LocalSetP2pProfile			(C_P2pProfile	&P2pProfile);
	virtual int LocalGetRtmpProfile			(C_RtmpProfile	&RtmpProfile);
	virtual int LocalSetRtmpProfile			(C_RtmpProfile	&RtmpProfile);

	virtual int LocalGetPtzCaps				(C_PtzCaps		&PtzCaps);
	virtual int LocalGetPtzProfile			(C_PtzProfile	&PtzProfile);
	virtual int LocalSetPtzProfile			(C_PtzProfile	&PtzProfile);
	virtual int LocalPtzCommand				(C_PtzCmd		&PtzCmd);


	virtual int LocalConnectDeviceGetList(std::list<C_ConnectDeviceInfo> &ConnectDeviceList);
	virtual int LocalConnectDeviceAdd(std::list<C_ConnectDeviceInfo> &AddDeviceList);
	virtual int LocalConnectDeviceDel(std::list<C_ConnectDeviceInfo> &DelDeviceList);
	virtual int LocalConnectDeviceModify(std::list<C_ConnectDeviceInfo> &ModifyDeviceList);


	virtual int LocalFirmwareUpgrade(C_FirmwareUpgrade &FirmwareUpgrade);
	virtual int LocalFirmwareData(unsigned char *data, int datalen, unsigned int status/*PROTO_STATUS_BEGIN //开始数据|  PROTO_STATUS_ING,//中间数据| PROTO_STATUS_END,	//数据结束*/, unsigned int *Progress);
	virtual int LocalGetLogCaps(C_LogCaps &LogCaps);
	virtual int LocalGetLogProfile(C_LogProfile &LogProfile);
	virtual int LocalSetLogProfile(C_LogProfile &LogProfile);
	virtual int LocalGetLogList(std::list <C_LogContent> &LogContent, int start, int count, unsigned int starttm, unsigned int endtime);
public:
	virtual CNetUv *	OnConnect(const char *remoteAddr, const int remotePort);
	virtual int			OnInfo(NetUvStatus nvi, const char *message);

private:
	int		OnEvent(NetUvStatus nvi, const char *message);
	int		OnRead(const buf_t *bufs, int nbufs, struct sockaddr_in *addr = NULL);
	void	OnTimer100Ms();
	void	OnAlarmSLOT(C_Alarm &Alarm);

public:
	int		StartTask(unsigned int msg, void *Args);
	int		OnTask(void *Args);
	int		OnTaskAfter(void *Args);
	int		StopTask();



private:
	int OnDeviceCaps			(char *ProtoHead, char *StrPacket);
	int OnEncodeCaps			(char *ProtoHead, char *StrPacket);
	int OnEncodeProfile			(char *ProtoHead, char *StrPacket);
	int OnStreamStart			(char *ProtoHead, char *StrPacket);
	int OnStreamStop			(char *ProtoHead, char *StrPacket);
	int OnStreamStopAll			();
	int OnStreamData			(char *ProtoHead, char *StrPacket);

	int OnCaptureCaps			(char *ProtoHead, char *StrPacket);
	int OnCaptureProfile		(char *ProtoHead, char *StrPacket);
	int OnImageCaps				(char *ProtoHead, char *StrPacket);
	int OnImageProfile			(char *ProtoHead, char *StrPacket);
	int OnNetWorkProfile		(char *ProtoHead, char *StrPacket);
	int OnNetWorkCaps			(char *ProtoHead, char *StrPacket);
	int OnFirmwareVersion		(char *ProtoHead, char *StrPacket);
	int OnManufacturerInfo		(char *ProtoHead, char *StrPacket);
	int OnDevSearch				(char *ProtoHead, char *StrPacket);
	int OnDeviceStatus			(char *ProtoHead, char *StrPacket);
	int OnLogIn					(char *ProtoHead, char *StrPacket);
	int OnDiskStatus			(char *ProtoHead, char *StrPacket);
	int OnAlarmCaps				(char *ProtoHead, char *StrPacket);
	int OnAlarm					(char *ProtoHead, char *StrPacket);
	int OnAlarmRegister			(char *ProtoHead, char *StrPacket);
	int OnAlarmProfile			(char *ProtoHead, char *StrPacket);
	int OnUserList				(char *ProtoHead, char *StrPacket);
	int OnUserAdd				(char *ProtoHead, char *StrPacket);
	int OnUserDel				(char *ProtoHead, char *StrPacket);
	int OnUserModify			(char *ProtoHead, char *StrPacket);
	int OnRecordFileSearch		(char *ProtoHead, char *StrPacket);
	int OnRecordFileDownload	(char *ProtoHead, char *StrPacket);
	int OnRecordFileDownloadCtrl(char *ProtoHead, char *StrPacket);
	int OnRecordFileDownloadData(char *ProtoHead, char *StrPacket);

	int OnFtpProfile			(char *ProtoHead, char *StrPacket);
	int OnEmailProfile			(char *ProtoHead, char *StrPacket);
	int OnUpnpProfile			(char *ProtoHead, char *StrPacket);
	int OnNtpProfile			(char *ProtoHead, char *StrPacket);
	int OnPppoeProfile			(char *ProtoHead, char *StrPacket);
	int OnRtspProfile			(char *ProtoHead, char *StrPacket);
	int OnDdnsProfile			(char *ProtoHead, char *StrPacket);
	int OnP2pProfile			(char *ProtoHead, char *StrPacket);
	int OnRtmpProfile			(char *ProtoHead, char *StrPacket);
	int OnPtzCaps				(char *ProtoHead, char *StrPacket);
	int OnPtzProfile			(char *ProtoHead, char *StrPacket);
	int OnPtzCmd				(char *ProtoHead, char *StrPacket);
	int OnVideoCoverProfile		(char *ProtoHead, char *StrPacket);
	int OnConnectDevice			(char *ProtoHead, char *StrPacket);
	int OnFirmwareUpgrade		(char *ProtoHead, char *StrPacket);
	int OnFirmwareData			(char *ProtoHead, char *StrPacket);
	int OnFirmwareUpgradeProgress(char *ProtoHead, char *StrPacket);
	int OnLogCaps				(char *ProtoHead, char *StrPacket);
	int OnLogProfile			(char *ProtoHead, char *StrPacket);
	int OnLogList				(char *ProtoHead, char *StrPacket);
public:
	int MutexThreadPoolLock();
	int MutexThreadPoolUnlock();
private:
	std::list <C_DevSearch> *mP_DevList; // only for search;
	Cache_t					m_Cache;
	CNetUvMutex_t			m_ThreadPool;
	CStreamPacket			m_StreamPacket[PROTO_MAX_CHANNEL][Proto_MAX_SLAVE];
	int						m_StreamCount[PROTO_MAX_CHANNEL][Proto_MAX_SLAVE];
	void					*mP_StreamCtrl;

	unsigned int			m_LinkStatus;
	char					m_ProtoUsrName[32];
	char					m_ProtoUsrPasswd[32];
	unsigned int			m_ProtoVisitMask;
	unsigned int			m_ProtoTaskRef;
	unsigned int			m_ProtoTaskCtrl;
	unsigned int		    m_RecordDownloadCtrl[PROTO_MAX_CHANNEL][Proto_MAX_SLAVE];
	CStreamPacket			m_RecordDownloadPacket[PROTO_MAX_CHANNEL][Proto_MAX_SLAVE];
	void					*mP_Args;

	void					*m_Args;
};

PROTOSHARE_API void ProtoSleepMs(int msec);
PROTOSHARE_API int  ProtoCreateThread(ThreadTask_t Func, void *args);



#endif

