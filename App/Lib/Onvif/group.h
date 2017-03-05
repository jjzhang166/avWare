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
#ifndef __GROUP_H__
#define __GROUP_H__

#include "common.h"
#include "xml.h"
int CmdDistribution(XmlElementObj_S *reqObj,XmlElementObj_S *respObj);

#define DF_SOCPEITEM_VIDEOENCODER	"onvif://www.onvif.org/type/video_encoder"
#define DF_SOCPEITEM_PTZ			"onvif://www.onvif.org/type/ptz"
#define DF_SOCPEITEM_AUDIOENCODER	"onvif://www.onvif.org/type/audio_encoder"
#define DF_SOCPEITEM_CITY			"onvif://www.onvif.org/location/city/%s"
#define DF_SOCPEITEM_HARDWARE		"onvif://www.onvif.org/location/hardware/%s"
#define DF_SOCPEITEM_NAME			"onvif://www.onvif.org/location/name/%s"
#define DF_SOCPNUM	6


#define	DF_ANALYTICS_XADDR 			"http://%s:%d/onvif/analytics_service"
#define	DF_DEVICE_XADDR 			"http://%s:%d/onvif/device_service"
#define DF_EVENT_XADDR				"http://%s:%d/onvif/event_service"
#define DF_IMAGING_XADDR			"http://%s:%d/onvif/imaging_service"
#define DF_MEDIA_XADDR				"http://%s:%d/onvif/media_service"
#define DF_PTZ_XADDR				"http://%s:%d/onvif/ptz_service"
#define DF_DEVICEIO_XADDR			"http://%s:%d/onvif/deviceIO_service"
#define DF_TELECOM_XADDR			"http://%s:%d/onvif/telecom_service"

#define DF_SUBSCRIPTION_STR	 		"http://%s:%d/onvif/Subscription?Idx=%d"
#endif
