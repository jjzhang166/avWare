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
#include "Apis/AvWareStruct.h"
#include "Apis/AvWareCplusplus.h"
#include "AvNetService/AvSmtp.h"
#include "AvNetService/AvUpnp.h"
#include "AvNetService/AvNtp.h"
#include "AvNetService/AvDdns.h"
#include "AvNetService/AvFtp.h"
#include "Apis/LibSystem.h"
#include "AvConfigs/AvConfigTable.h"



typedef C_SmtpProfile			ConfigNetSmtp;
typedef C_FtpProfile			ConfigNetFtp;
typedef C_DdnsProfile			ConfigNetDdns;
typedef C_RtmpNodeProfile		ConfigRtmp;
typedef C_UpnpProfile			ConfigNetUpnp;
typedef C_NtpProfile			ConfigNetNtp;
typedef C_RtspProfile			ConfigNetRtsp;
typedef C_P2pProfile			ConfigNetP2p;






typedef TAvConfig<ConfigNetSmtp>	CAvConfigNetSmtp;
typedef TAvConfig<ConfigNetFtp>		CAvConfigNetFtp;
typedef TAvConfig<ConfigNetDdns>	CAvConfigNetDdns;
typedef TAvConfig<ConfigNetUpnp>	CAvConfigNetUpnp;
typedef TAvConfig<ConfigNetNtp>		CAvConfigNetNtp;
typedef TAvConfig<ConfigNetP2p>		CAvConfigNetP2p;
typedef TAvConfig<ConfigNetRtsp>	CAvConfigNetRtsp;

typedef TAvConfig<ConfigRtmp,		0, SYS_CHN_NUM>		CAvConfigRtmp;






#endif