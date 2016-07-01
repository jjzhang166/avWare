/*******************************************************************
|  Copyright(c) 2015-2016 Graceport Technology Development Co.,Ltd
|  All rights reserved.
|
|  版本: 1.0
|  作者: ironman [ironman@graceport.cn]
|  日期: 2016年6月6日
|  说明:
|
|  版本: 1.1
|  作者:
|  日期:
|  说明:
******************************************************************/
#ifndef _AVCONFIGS_NETSERVICE_H_
#define _AVCONFIGS_NETSERVICE_H_

#include "Apis/AvWareType.h"
#include "AvNetService/AvSmtp.h"
#include "AvNetService/AvUpnp.h"
#include "AvNetService/AvNtp.h"
#include "AvNetService/AvDdns.h"
#include "AvNetService/AvFtp.h"

#include "AvConfigs/AvConfigTable.h"

////////////////////////////////////
//Smtp

#define MAX_EMAIL_STRING_LEN 64
#define MAX_EMAIL_RECEIVER 5
#define MAX_PASSDW_LEN 64
#define MAX_SERVERIP_LEN	16
#define PATH_LEN	240
#define PORT_MAP_NUM 3

#define MAX_CONF_ROUTER_LINK 10

typedef enum {
	Mapping_RTSP,
	Mapping_Web,
	Mapping_SelfProtol,
	Mapping_NR,
}MappedType;

typedef struct {
	av_bool enable;
	int port;
	int save_mode;
	char smtp_server[MAX_EMAIL_STRING_LEN];
	char account[MAX_EMAIL_STRING_LEN];
	char password[MAX_EMAIL_STRING_LEN];
	char sender[MAX_EMAIL_STRING_LEN];
	char receiver[MAX_EMAIL_RECEIVER][MAX_EMAIL_STRING_LEN];
	char title[MAX_EMAIL_STRING_LEN];
}ConfigNetSerSmtp;

typedef struct{
	av_bool Enable;
	int Port;
	char FtpServer[MAX_SERVERIP_LEN];
	char UsrName[MAX_PASSDW_LEN];
	char Passwd[MAX_PASSDW_LEN];
	char RemotePathName[PATH_LEN];
	av_bool Anonymity;
}ConfigNetFtp;

typedef struct{
	av_bool Enable;
	char Server[MAX_PASSDW_LEN];
	char UsrName[MAX_PASSDW_LEN];
	char Passwd[MAX_PASSDW_LEN];
}ConfigNetDdns;

typedef struct{
	av_bool Enable;
	char Name[MAX_PASSDW_LEN];
	int TranMode;
	int LocalPort[Mapping_NR];
	int RouterPort[Mapping_NR];
}ConfigNetUpnp;

typedef struct{
	av_bool Enable;
	char Server[MAX_PASSDW_LEN];
	int Zone;
}ConfigNetNtp;




typedef C_LanAttribute ConfigLanAttribute;
typedef C_BlueToothAttribute ConfigBlueTooth;
typedef C_SimAttribute	ConfigSimAttribute;

typedef struct {
	C_WirelessAttribute RouterLinkInfo[MAX_CONF_ROUTER_LINK];
}ConfigWirelessAttribute;



typedef struct {
	av_bool Enable;//打开或者关闭
	av_bool Support;//硬件上是否支持
	E_NetComm_Type type;
	E_NetComm_Mode GetMode;
	char Ifrname[32];
	union 
	{
		ConfigLanAttribute		LanAttr;
		ConfigBlueTooth			BlueToothAttr;
		ConfigWirelessAttribute	WirelessAttr;
		ConfigSimAttribute		SimAttr;
	};
}ConfigNetComm;

typedef TAvConfig<ConfigNetSerSmtp>	CAvConfigNetSerSmtp;
typedef TAvConfig<ConfigNetFtp>		CAvConfigNetFtp;
typedef TAvConfig<ConfigNetDdns>	CAvConfigNetDdns;
typedef TAvConfig<ConfigNetUpnp>	CAvConfigNetUpnp;
typedef TAvConfig<ConfigNetNtp>		CAvConfigNetNtp;
typedef TAvConfig<ConfigNetComm, 0, ConfMaxNetComm> CAvConfigNetComm;


#endif