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
#ifndef _AVMOON_H_
#define _AVMOON_H_



#include "Apis/AvWareType.h"
#include "CAvObject.h"
#include "ProtoHeader/Proto.h"
#include "AvPacket/AvPacket.h"
#include "AvCapture/AvNetCapture.h"

class CStream :public CAvObject
{
public:
	SINGLETON_DECLARE(CStream);
	CStream();
	~CStream();
	int OnVideoData(av_uchar Channel, av_uchar Slave, CAvPacket *packet);

private:
	int m_VideoCount[PROTO_MAX_CHANNEL][Proto_MAX_SLAVE];
};
#define g_LocalStream (*CStream::instance())


class CAvMoon :public CProtoMoon
{
public:
	SINGLETON_DECLARE(CAvMoon);

public:
	int StartMoon();
	int StopMoon();

	//next for client
public:
#if 0
	av_bool Connect(C_ConnectArgs &ConArgs);
	av_bool DisConnect();
	av_bool StartRemoteStream(av_ushort Channel, av_uchar Slave);
	av_bool StopRemoteStream();
	av_bool GetDeviceInfo(C_DeviceInfo &DeviceInfo);
	av_bool SetDeviceInfo(C_DeviceInfo &DeviceInfo);

	//以下两个函数需要走搜索 相关接口。
	av_bool SearchDevice(std::list<C_DeviceList> &DeviceList);
	av_bool SetNetParam(C_NetParam &NetParam);
#endif
private:
	int OnRemoteGetDeviceCaps(C_DeviceCaps &DeviceCaps, int status);
	int OnRemoteGetDeviceStatus(C_DevStatusInfo &DevStatusInfo, int status);
	int OnRemoteGetDeCodeProfile(int status);
	int OnRemoteSetDeCodeProfile(int status);


	int OnRemoteGetEnCodeCaps(C_EnCodeCaps &Encodecaps, int status);
	int OnRemoteGetImageCaps(C_ImageCaps &ImageCaps, int status);
	int OnRemoteGetCaptureCaps(C_CaptureCaps &CaptureCaps, int status);

	int OnRemoteGetImageProfile(C_ImageProfile &ImageProfile, int status);
	int OnRemoteGetCaptureProfile(C_CaptureProfile &CaptureProfile, int status);
	int OnRemoteGetEnCodeProfile(C_EnCodeProfile &EnCodeProfile, int status);

	int OnRemoteSetImageProfile(C_ImageProfile ImageProfile, int status);
	int OnRemoteSetCaptureProfile(C_CaptureProfile &CaptureProfile, int status);
	int OnRemoteSetEnCodeProfile(C_EnCodeProfile &EnCodeProfile, int status);


	int OnRemoteGetAlarmCaps(int status);
	int OnRemoteGetAlarmPorfile(int status);
	int OnRemoteGetSysTimeProfile(int status);
	int OnRemoteStreamData(CStreamPacket *pStream);
	int OnRemoteStreamStart(C_StreamStart &StreamStart, COMMAND_E ReqOrResp);
	int OnRemoteStreamStop(C_StreamStop &StreamStop, COMMAND_E ReqOrResp);

	int OnRemoteFirmwareUpgradeProgress(C_FirmwareUpgradeProgress &FirmwareUpgradeProgress, int status);



public:
	int LocalGetDeviceCaps(C_DeviceCaps &DeviceCaps);
	int LocalGetDeCodeCaps();
	int LocalGetDeCodeProfile();



	int LocalGetImageCaps(C_ImageCaps &ImageCaps);
	int LocalGetImageProfile(C_ImageProfile &ImageProfile);
	int LocalSetImageProfile(C_ImageProfile &ImageProfile);


	int LocalGetVideoCoverProfile(C_VideoCoverProfile &VideoCoverProfile);
	int LocalSetVideoCoverProfile(C_VideoCoverProfile &VideoCoverProfile);


	int LocalGetCaptureCaps(C_CaptureCaps &CaptureCaps);
	int LocalGetCaptureProfile(C_CaptureProfile &CaptureProfile);
	int LocalSetCaptureProfile(C_CaptureProfile &CaptureProfile);

	int LocalGetEnCodeCaps(C_EnCodeCaps &EnCodeCaps);
	int LocalGetEnCodeProfile(C_EnCodeProfile &EnCodeProfile);
	int LocalSetEnCodeProfile(C_EnCodeProfile &EnCodeProfile);

	int LocalGetAlarmCaps();
	int LocalGetAlarmProfile();
	int LocalSetAlarmProfile();

	int LocalGetSysTimeProfile();
	int LocalSetSysTimeProfile();


	int LocalGetNetWorkCaps(C_NetWorkCaps &NetWorkCaps);
	int LocalGetNetWorkProfile(C_NetWorkProfile &NetWorkProfile);
	int LocalSetNetWorkProfile(C_NetWorkProfile &NetWorkProfile);

	int LocalGetManufacturerInfo(C_ManufacturerInfo &FacInfo);
	int LocalSetManufacturerInfo(C_ManufacturerInfo &FacInfo);

	int LocalGetDevStatus(C_DevStatusInfo &DevStatus);
	int LocalGetFirmwareInfo(C_FirmwareInfo &FirmwareInfo);

	int LocalCheckOutAuthorization(const char *UsrName, const char *UsrPasswd, unsigned int &UsrAccess);


	int LocalGetDiskStatus(C_DiskStatus &DiskStatus);
	int LocalGetAlarmCaps(C_AlarmCaps &AlarmCaps);
	int LocalGetUserList(std::list <C_UserInfo> &UserList);
	int LocalAddUser(C_UserInfo &AddUser);
	int LocalDelUser(C_UserInfo &DelUser);
	int LocalModifyUser(C_UserInfo &ModifyUser);

	int LocalGetRecordFileSearch(C_RecordFileSearch &RecordFileSearch);
	int LocalGetRecordFileDownload(C_RecordFileDownload &RecordFileDownload);
	int LocalGetFtpProfile(C_FtpProfile &FtpProfile);
	int LocalSetFtpProfile(C_FtpProfile &FtpProfile);
	int LocalGetEmailProfile(C_EmailProfile &EmailProfile);
	int LocalSetEmailProfile(C_EmailProfile &EmailProfile);
	int LocalGetUpnpProfile(C_UpnpProfile &UpnpProfile);
	int LocalSetUpnpProfile(C_UpnpProfile &UpnpProfile);
	int LocalGetNtpProfile(C_NtpProfile &NtpProfile);
	int LocalSetNtpProfile(C_NtpProfile &NtpProfile);
	int LocalGetPppoeProfile(C_PppoeProfile &PpppoeProfile);
	int LocalSetPppoeProfile(C_PppoeProfile &PpppoeProfile);
	int LocalGetRtspProfile(C_RtspProfile &RtspProfile);
	int LocalSetRtspProfile(C_RtspProfile &RtspProfile);
	int LocalGetDdnsProfile(C_DdnsProfile &DdnsProfile);
	int LocalSetDdnsProfile(C_DdnsProfile &DdnsProfile);
	int LocalGetP2pProfile(C_P2pProfile &P2pProfile);
	int LocalSetP2pProfile(C_P2pProfile &P2pProfile);
	int LocalGetRtmpProfile(C_RtmpProfile &RtmpProfile);
	int LocalSetRtmpProfile(C_RtmpProfile &RtmpProfile);
	int LocalGetPtzCaps(C_PtzCaps		&PtzCaps);
	int LocalGetPtzProfile(C_PtzProfile	&PtzProfile);
	int LocalSetPtzProfile(C_PtzProfile	&PtzProfile);
	int LocalPtzCommand(C_PtzCmd		&PtzCmd);

	int LocalFirmwareUpgrade(C_FirmwareUpgrade &FirmwareUpgrade);
	int LocalFirmwareData(unsigned char *data, int datalen, unsigned int status);
	int LocalFirmwareProgress();

	int LocalGetLogCaps(C_LogCaps &LogCaps);
	int LocalGetLogProfile(C_LogProfile &LogProfile);
	int LocalSetLogProfile(C_LogProfile &LogProfile);
	int  LocalGetLogList(std::list <C_LogContent> &LogContent, int start, int count, unsigned int starttm, unsigned int endtime);
public:
		CNetUv *	OnConnect(const char *remoteAddr, const int remotePort);
		int		OnInfo(NetUvStatus nvi, const char *message);
public:
	CAvMoon();
	~CAvMoon();

private:
	static CAvMoon  *m_SearchService;
};

#define g_CAvMoon (*CAvMoon::instance())







#endif



