/*******************************************************************
|  Copyright(c) 2015-2016 Graceport Technology Development Co.,Ltd
|  All rights reserved.
|
|  版本: 1.0
|  作者: FootMan [FootMan@graceport.cn]
|  日期: 2016年6月6日
|  说明:
|
|  版本: 1.1
|  作者:
|  日期:
|  说明:
******************************************************************/
#ifndef __ONVIF_INIT_H__
#define __ONVIF_INIT_H__

#include "onvifAPI.h"
#include "network.h"


OnvifSerHandle_S	*GetOnvifSerHandle();
NetWorkSerHandle_S	*GeonvifSerNetHandle();
NetWorkSerHandle_S	*GetonvifMulicatstserHandle();
NetWorkSerHandle_S	*GetonvifSerNetHandle();


int		OnvifInitAllValue();
int		OnvifCreateInitProfile();
int		initUuid();
char *	GetAddressUuid();
char *	GetMsgUuid(char *MsgID,int len);

#endif


