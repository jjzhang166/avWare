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
#include "CAvObject.h"
#include "AvAlarm/AvAlarm.h"
#include "AvPacket/AvPacket.h"
#include "AvThread/AvThread.h"
#include "AvCapture/AvCapture.h"

#define MAX_DEQUE_SIZE  8

class CAvNetProto:public CAvObject
{
public:
	CAvNetProto();
	~CAvNetProto();
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
		"Reboot", "SetSystemTime", "NetProtoMsgCmd_Nr" };
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
	virtual av_bool Connect()		= 0;
	virtual av_bool Disconnect()	= 0;
	virtual av_bool GetStream()		= 0;
	virtual av_bool ForceIFrame()	= 0;
private:

};



class CAvNetCapture:public Capture
{
public:
	CAvNetCapture();
	virtual ~CAvNetCapture();

public:
	av_bool Initialize(av_int Channel);
	av_bool Start(av_int Slave);
	av_bool Stop(av_int Slave);

	av_bool Start(av_int Slave, CAvObject *obj, SIG_PROC_ONDATA pOnData);
	av_bool Stop(av_int Slave, CAvObject *obj, SIG_PROC_ONDATA pOnData);

	av_bool SetProfile(av_int Slave, C_EncodeFormats &Formats);
	av_bool GetProfile(av_int Slave, C_EncodeFormats &Fromats);
	av_bool GetCaps(C_EncodeCaps &Caps);

	av_bool SetTime(av_timeval &atv);
	av_bool SetIFrame(av_int Slave = CHL_MAIN_T);

	CAvPacket *GetSnap(av_int Slave = CHL_SUB1_T);
public:
	av_bool LoadConfigs();
private:
	TSignal3<av_int, av_int, CAvPacket *> m_StreamSignal[CHL_NR_T];


private:
	void ThreadProc();


private:
	av_int		m_Channel; 
	CAvNetProto *m_ProtoHandle;

};




#endif
