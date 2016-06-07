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
#ifndef _AVNETCAPTURE_H_
#define _AVNETCAPTURE_H_

#include "Apis/AvWareType.h"
#include "CObject.h"
#include "AvAlarm/AvAlarm.h"
#include "AvPacket/AvPacket.h"
#include "AvThread/AvThread.h"

#define MAX_DEQUE_SIZE  8
class CAvNetCapture
{
public:
	typedef enum {
		Moon,
		Onvif,
		Rtsp,
		ProtoNr,
	}E_NetProtocolType;
	inline static std::string GetCAvNetCaptureProtocolTypeString(E_NetProtocolType e)
	{
		static std::string sCAvNetCaptureProtocolTypeString[ProtoNr + 1] =
		{ "Moon", "Onvif", "Rtsp", "ProtoNr" };
		return sCAvNetCaptureProtocolTypeString[e];
	};

	typedef enum {
		GetDeviceCaps,
		GetEncodeCaps,
		GetEncodeProfile,
		SetEncodeProfile,
		Reboot,
		SetSystemTime,
		NetProtoMsgCmd_Nr,
	}E_NetProtoMsgCmd;

	typedef enum {
		Succeed,
		Failed,
		TimeOut,

	}E_NetProtoMsgStatus;
	inline static std::string GetNetProtoMsgCmdString(E_NetProtocolType e)
	{
		static std::string sNetProtoMsgCmdString[NetProtoMsgCmd_Nr + 1] =
		{ "GetDeviceCaps", "GetEncodeCaps", "GetEncodeProfile", "SetEncodeProfile" 
		"Reboot", "SetSystemTime", "NetProtoMsgCmd_Nr"};
		return sNetProtoMsgCmdString[e];
	};

	typedef struct{
		std::string address;
		std::string port;
		std::string username;
		std::string passwd;
	}C_ConnectArgs;


	typedef struct {
		int temp;
	}C_NetProtoDeviceCaps;

	typedef struct {
		int temp;
	}C_NetProtoEncodeCaps;

	typedef struct {
		int temp;
	}C_NetProtoEncodeProfile;

	typedef struct{
		av_u32 systemTime;
	}C_NetProtoSystemTime;
	

	typedef struct {
		av_u16 Channel;
		av_u16 Slave;
		E_NetProtoMsgCmd Command;
		union {
			C_NetProtoDeviceCaps DeviceCaps;
			C_NetProtoEncodeCaps EncodeCaps;
			C_NetProtoEncodeProfile EncodeProfile;
			C_NetProtoSystemTime  SystemTime;
		};
	}C_DeviceInfo;

	typedef struct {
		std::string DeviceName;
		std::string Address;
		std::string ServicePort;
		std::string GateWay;
		std::string NetMask;
		std::string Dns1;
		std::string Dns2;
		std::string MaxChannel;
	}C_DeviceList;

	typedef struct {
		std::string DeviceMac;
		std::string NewAddress;
		std::string ServicePort;
		std::string GateWay;
		std::string NetMask;
		std::string Dns1;
		std::string Dns2;
	}C_NetParam;

public:
	CAvNetCapture();
	virtual ~CAvNetCapture();
	av_bool StreamData(av_u16 Channel, av_uchar Slave, CPacket &Packet);
	av_bool AlarmMsg(CAvAlarm::AlmMsg &Msg);
	av_bool OnDeviceInfoAck(C_DeviceInfo &DeviceInfo, E_NetProtoMsgStatus StatusCode);

	//以下用于码流分发管理
public:
	typedef TSignal3<av_u16, av_uchar, CPacket &>::SigProc OnStreamSigNalFunc;
	av_bool StreamStart(av_uchar Slave, CObject *obj, OnStreamSigNalFunc proc);
	av_bool StreamStop(av_uchar Slave, CObject *obj, OnStreamSigNalFunc proc);
private:
	TSignal3<av_u16, av_uchar, CPacket &> m_StreamSignal[CHL_NR_T];



public:
	virtual av_bool Connect(C_ConnectArgs &ConArgs) = 0;
	virtual av_bool DisConnect() = 0;
	virtual av_bool StartRemoteStream(av_ushort Channel, av_uchar Slave) = 0;
	virtual av_bool StopRemoteStream() = 0;
	virtual av_bool GetDeviceInfo(C_DeviceInfo &DeviceInfo) = 0;
	virtual av_bool SetDeviceInfo(C_DeviceInfo &DeviceInfo) = 0;

	//以下两个函数需要走搜索 相关接口。
	virtual av_bool SearchDevice(std::list<C_DeviceList> &DeviceList) = 0;
	virtual av_bool SetNetParam(C_NetParam &NetParam) = 0;

private:
	std::deque <CPacket *> m_MediaDeque[CHL_NR_T];
	CMutex				   m_MediaMutex;
};


class CMNetCapture
{
public:
	PATTERN_SINGLETON_DECLARE(CMNetCapture);
private:
	CMNetCapture();
	~CMNetCapture();

public:
	av_bool Initialize();
	CAvNetCapture *GetAvNetCaptureInstance(av_u16 iLocalChannel);

private:
	std::map <av_u16, CAvNetCapture *>  m_MNetCaptureMap;
private:

};











#endif
