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

/*************************************
Company Name:深圳市恩港科技开发有限公司(GracePort)
Function	:ONVIFCallback
Author		:FootMan
Email		:FootMan@graceport.cn
**************************************/
#include "Apis/AvWareType.h"
#include "AvNetService/AvOnvif.h"
#include "Apis/LibEncode.h"

#include "AvConfigs/AvConfigDevice.h"
#include "AvConfigs/AvConfigNetService.h"
#include "AvConfigs/AvConfigCapture.h"
#include "AvDevice/AvDevice.h"
#include "AvCapture/AvCapture.h"
//	AvComp_G711A,
//	AvComp_G711U,
//	AvComp_AAC,
static VideoEncoding_E AVCompToOnvifEncoding(av_comp_t		Comp)
{
	switch(Comp){
		case AvComp_H264:  return VideoEncoding__H264;
		case AvComp_H265:  return VideoEncoding__H265;
		case AvComp_MJPEG: return VideoEncoding__JPEG;
		case AvComp_NR:
		default:
			return VideoEncoding__NULL;
	}
}

static av_comp_t OnvifEncodingToAV(VideoEncoding_E		encoding)
{
	switch(encoding){
		case VideoEncoding__H264: return AvComp_H264;
		case VideoEncoding__H265: return AvComp_H265;
		case VideoEncoding__JPEG: return AvComp_MJPEG;
		case VideoEncoding__NULL:
		default:
			return AvComp_NR;
	}
}

static av_capture_size GetResEnum(av_32 width, av_32 hight)
{
	if (width == 480 && hight == 240)
		return CaptureSize_QVGAEX;
	else if (width == 480 && hight == 360)
		return CaptureSize_QVGA;
	else if (width == 640 && hight == 360)
		return CaptureSize_VGAEX;
	else if (width == 640 && hight == 480)
		return CaptureSize_VGA;
	else if (width == 720 && hight == 576)
		return CaptureSize_D1;
	else if (width == 1280 && hight == 720)
		return CaptureSize_720P;
	else if (width == 1280 && hight == 960)
		return CaptureSize_960P;
	else if (width == 1920 && hight == 1080)
		return CaptureSize_1080P;
	else if (width == 2048 && hight == 1536)
		return CaptureSize_300W;
	else if (width == 2688 && hight == 1520)
		return CaptureSize_400W;
	else if (width == 2592 && hight == 1944)
		return CaptureSize_500W;
	else if (width == 3072 && hight == 2048)
		return CaptureSize_600W;
	else if (width == 3840 && hight == 2160)
		return CaptureSize_800W;
	else if (width == 4096 && hight == 2160)
		return CaptureSize_4K;
	else if (width == 7680 && hight == 4320)
		return CaptureSize_8K;
	else {
		return CaptureSize_Self;
	}
	return CaptureSize_NR;
}

static int GetResWidthAndHight(av_capture_size ImageSize, int *width, int *height)
{
	switch (ImageSize)
	{
		case CaptureSize_QVGAEX:
			*height = 160;
			*width = 320;
			break;

		case CaptureSize_QVGA://	//480*360
			*height = 360;
			*width = 480;
			break;
		case CaptureSize_VGAEX:	//640*360
			*height = 360;
			*width = 640;
			break;

		case CaptureSize_VGA:	//640*480
			*height = 480;
			*width = 640;
			break;
		case CaptureSize_D1:		//720*576
			*height = 576;
			*width = 720;
			break;
		case CaptureSize_720P:
			*height = 720;
			*width = 1280;
			break;
		case CaptureSize_960P:
			*height = 960;
			*width = 1280;
			break;
		case CaptureSize_1080P:
		case DisplaySize_1920_1080:
			*height = 1080;
			*width = 1920;
			break;
		default:
			*height = 0;
			*width = 0;
			break;
	}
	return 0;
}
static int	GetDeviceInfo(DeviceInfo_S *info)
{
	C_DeviceFactoryInfo FactoryInfo;
	CAvDevice::GetDeviceInfo(FactoryInfo);
	if(NULL == info){
		return -1;
	}
	memset(info,0,sizeof(DeviceInfo_S));
	strcat(info->city,"shenzhen");
	strcat(info->country,"china");
	strcat(info->name,"IPC");
	strcat(info->softwareVer,"Build V1.0");
	info->devType = ENUM_DEVTYPE_IPC;
	snprintf(info->facturer,sizeof(info->facturer),"%s",FactoryInfo.FactoryName);
	snprintf(info->model,sizeof(info->model),"%s",FactoryInfo.ProductModel);
	snprintf(info->SerialNumber,sizeof(info->SerialNumber),"%s",FactoryInfo.SerialNumber);

	return 0;
}

static int	SetDeviceInfo(DeviceInfo_S *info)
{
	
	return 0;
}

static int	GetDeviceCab(DeviceCab_S *info)
{
	printf("GetDeviceCab\r\n");
	info->chnelCount = 1;
	info->streamCount= 2;
	info->AnalyticsEnable = 0;
	info->AudioEnable = 0;
	info->PtzEnable = 0;
	info->VideoEnable = 0;
	return 0;
}

static int	GetNetWorkInfo(NetWorkInfo_S *info)
{
	C_DeviceFactoryInfo FactoryInfo;
	CAvConfigNetComm NetComm;
	NetComm.Update();
	ConfigNetComm &ConfigNet = NetComm.GetConfig(NetCommT_Lan0);
	CAvDevice::GetDeviceInfo(FactoryInfo);
	snprintf(info->ip,sizeof(info->ip),"%s",ConfigNet.LanAttr.IpAddr);
	snprintf(info->dns,sizeof(info->dns),"%s",ConfigNet.LanAttr.Dns1);
	snprintf(info->gateway,sizeof(info->gateway),"%s",ConfigNet.LanAttr.Gateway);
	snprintf(info->hostName,sizeof(info->hostName),"%s",ConfigNet.LanAttr.Host);
	snprintf(info->mac,sizeof(info->mac),"%s",FactoryInfo.ProductMacAddr);
	strcat(info->ethernetName,"eth0");
	info->DhcpEnable = ENUM_ENABLED_FALSE;	
	return 0;
}

static int	SetNetWorkInfo(NetWorkInfo_S *info)
{
	//网卡未配置
	//DHCP未配置
// 	CAvConfigsNetWork ConfigNet;
// 	ConfigNet.Update();
// 	snprintf(ConfigNet.m_network.mac,sizeof(ConfigNet.m_network.mac),"%s",info->mac);
// 	StrAddr2AvAddr(info->ip,ConfigNet.m_network.ipaddr);
// 	StrAddr2AvAddr(info->dns,ConfigNet.m_network.dns1);
// 	StrAddr2AvAddr(info->gateway,ConfigNet.m_network.gateway);
// 	snprintf(ConfigNet.m_network.host,sizeof(ConfigNet.m_network.host),"%s",info->hostName);
// 	ConfigNet.Submit();
	return 0;
}


static int	GetNetProtocolInfo(NetProtocolInfo *info)
{
	CAvConfigNetComm NetComm;
	NetComm.Update();
	ConfigNetComm &ConfigNet = NetComm.GetConfig(NetCommT_Lan0);
 	info->rtspPort = 554;
 	info->onvifPort = 8080; 
 	sprintf(info->rtspUrl[0][0],"rtsp://%s:%d/channel/1",ConfigNet.LanAttr.IpAddr,info->rtspPort);
 	sprintf(info->rtspUrl[0][1],"rtsp://%s:%d/channel/2",ConfigNet.LanAttr.IpAddr,info->rtspPort);
	return 0;
}
static int	SetNetProtocolInfo(NetProtocolInfo *info)
{	
	printf("rtsp port = %d\r\n",info->rtspPort);
	return 0;
}
static int	GetVideoEncode(int chn, int streamId, VedioEncode_S *info)
{
	CAvConfigEncode Encode;
	Encode.Update();
	ConfigEncodeFormats &Formats = Encode.GetConfig(chn);
		
	memset(info,0,sizeof(VedioEncode_S));
	info->bitrate = Formats.CHLFormats[streamId].Formats.BitRateValue;
	info->cvbrMode = Formats.CHLFormats[streamId].Formats.BitRateCtrl;
	info->encoding =  AVCompToOnvifEncoding(Formats.CHLFormats[streamId].Formats.Comp);
	info->frameRate = (int)Formats.CHLFormats[streamId].Formats.FrameRate;
	info->gop = (int)Formats.CHLFormats[streamId].Formats.Gop;
	info->h264Profile = H264Profile__Main;
	info->qulity = (int)Formats.CHLFormats[streamId].Formats.Qlevel;
	if(CaptureSize_Self != Formats.CHLFormats[streamId].Formats.ImageSize){
 		GetResWidthAndHight(Formats.CHLFormats[streamId].Formats.ImageSize,&info->reslution.width,&info->reslution.height);
 	}else{
 		info->reslution.width = Formats.CHLFormats[streamId].Formats.ImageSelfWidth;
 		info->reslution.height =  Formats.CHLFormats[streamId].Formats.ImageSelfHeigh;
 	}
	return 0;
}

static int	SetVideoEncode(int chn, int streamId, VedioEncode_S *info)
{
	//profile qulity
	CAvConfigEncode Encode;
	Encode.Update();
	ConfigEncodeFormats &Formats = Encode.GetConfig(chn);

 	Formats.CHLFormats[streamId].Formats.BitRateValue = (av_u32)info->bitrate;
 	Formats.CHLFormats[streamId].Formats.BitRateCtrl = (av_bitrate_ctrl)info->cvbrMode;
 	Formats.CHLFormats[streamId].Formats.FrameRate = info->frameRate;
 	Formats.CHLFormats[streamId].Formats.Gop = info->gop;
	Formats.CHLFormats[streamId].Formats.Qlevel = info->qulity;
 	Formats.CHLFormats[streamId].Formats.ImageSize = GetResEnum(info->reslution.width,info->reslution.height);
 	if(CaptureSize_Self ==  Formats.CHLFormats[streamId].Formats.ImageSize){
 		Formats.CHLFormats[streamId].Formats.ImageSelfHeigh = info->reslution.height;
 		Formats.CHLFormats[streamId].Formats.ImageSelfWidth =  info->reslution.width;
 	}
 	Formats.CHLFormats[streamId].Formats.Comp = OnvifEncodingToAV(info->encoding);
 	Encode.SettingUp();
	return 0;
}

static int	GetVideoEncodeCab(int chn, int streamId, VedioEncodeCab_S *info)
{
	info->bitrateRange.max = 8192;
	info->bitrateRange.min = 512;
	info->encodingCab = (VideoEncoding_E)(VideoEncoding__JPEG | VideoEncoding__H264 | VideoEncoding__H265);
	info->frameRateRange.max = 30;
	info->frameRateRange.min = 1;
	info->gopRange.max = 50;
	info->gopRange.min = 10;
	info->h264ProfileCab = (H264Profile_E)(H264Profile__Baseline | H264Profile__Main | H264Profile__High);
	info->qulityRange.max = 10;
	info->qulityRange.min = 1;
	
	info->resSupport[0].width = 1920;
	info->resSupport[0].height = 1080;
	
	info->resSupport[1].width = 1920;
	info->resSupport[1].height = 960;

	info->resSupport[2].width = 1280;
	info->resSupport[2].height = 720;
	info->resSupportNum = 3;
	info->maxReslution.width = 1920;
	info->maxReslution.height = 1080;
	info->defaultReslution.height = 1080;
	info->defaultReslution.width = 1920;
	return 0;
}

static int	GetAudioEncode(int chn, int streamId, AudioEncode_S *info)
{
 	info->Bitrate = 8;
 	info->Encoding = AudioEncoding__G711;
 	info->SampleRate = 8000;
	return 0;
}

static int	SetAudioEncode(int chn, int streamId, AudioEncode_S *info)
{
	return 0;
}

static int	GetSystemTime(SystemTime_S *info)
{
	time_t timer;   
	struct tm* t_tm;   
	time(&timer);   
	t_tm = localtime(&timer);   
	info->timeZone = -1;
	info->year =  t_tm->tm_year+1900;
	info->month = t_tm->tm_mon+1;
	info->day = t_tm->tm_mday;
	info->hour = t_tm->tm_hour;
	info->min =  t_tm->tm_min;
	info->sec =  t_tm->tm_sec;
	return 0;   
}

static int	SetSystemTime(SystemTime_S *info)
{
	
	struct tm t;
	t.tm_year = info->year - 1900;
	t.tm_mon = info->month;
	t.tm_mday = info->day;
	t.tm_hour = info->hour;
	t.tm_min = info->min;
	t.tm_sec = info->sec;


	av_timeval timev;
	timev.tv_sec = (av_int)mktime(&t);
	timev.tv_usec = 0;
	CAvDevice::SetSysTime(timev);
	return 0;
}

static int	GetUsrInfo(int *usrNum, UsrInfo_S *info)//最多50个
{
	*usrNum = 3;
	info[0].userLevel = ENUM_USRLEVEL_Operator;
	sprintf(info[0].usrname,"test1");
	info[1].userLevel = ENUM_USRLEVEL_Operator;
	sprintf(info[1].usrname,"test2");
	info[2].userLevel = ENUM_USRLEVEL_Administrator;
	sprintf(info[2].usrname,"admin");
	return 0;
}

static int	GetImagingParam(ImagingParam_S *info)
{
	CAvConfigImage ConfigImage;
	ConfigImage.Update();
	ConfigImageFormats ImageFomats = ConfigImage.GetConfig();

	info->Brightness = ImageFomats.Brightness;
	info->ColorSaturation = ImageFomats.Saturation;
	info->Contrast = ImageFomats.Contrast;
	info->Sharpness = ImageFomats.Hue;
	return 0;
}

static int	SetImagingParam(ImagingParam_S *info)
{
	CAvConfigImage ConfigImage;
	ConfigImage.Update();
	ConfigImageFormats ImageFomats = ConfigImage.GetConfig();
	ImageFomats.Brightness = info->Brightness;
	ImageFomats.Saturation = info->ColorSaturation;
	ImageFomats.Contrast = info->Contrast;
	ImageFomats.Hue = info->Sharpness;

	ConfigImage.SettingUp();
	return 0;
}

static int	GetImagingCab(ImagingCab_S *info)
{
	C_ImageQualityCaps ImageQualityCaps;
	CAvDevice::GetImageCaps(0, ImageQualityCaps);
	/*Brightness[0], Contrast[1], Saturation[2], Hue[3]*/
	if (ImageQualityCaps.SupportMask & (0x01 << 0)){
		info->Brightness.max = ImageQualityCaps.MaxRange;
		info->Brightness.min = 0;
	}
	else{
		info->Brightness.max = 0;
		info->Brightness.min = 0;
	}

	if (ImageQualityCaps.SupportMask & (0x01 << 1)){
		info->ColorSaturation.max = ImageQualityCaps.MaxRange;
		info->ColorSaturation.min = 0;
	}
	else{
		info->ColorSaturation.max = 0;
		info->ColorSaturation.min = 0;
	}

	if (ImageQualityCaps.SupportMask & (0x01 << 2)){
		info->Contrast.max = ImageQualityCaps.MaxRange;
		info->Contrast.min = 0;
	}
	else{
		info->Contrast.max = 0;
		info->Contrast.min = 0;
	}

	if (ImageQualityCaps.SupportMask & (0x01 << 3)){
		info->Sharpness.max = ImageQualityCaps.MaxRange;
		info->Sharpness.min = 0;
	}
	else{
		info->Sharpness.max = 0;
		info->Sharpness.min = 0;
	}
	return 0;
}

SINGLETON_IMPLEMENT(CAvOnvifSer)

av_bool CAvOnvifSer::InitCbfun()
{
	memset(&m_OnvifSerHandle,0,sizeof(OnvifSerHandle_S));
	m_OnvifSerHandle.pGetDeviceInfo			= GetDeviceInfo;
	m_OnvifSerHandle.pSetDeviceInfo			= SetDeviceInfo;
	m_OnvifSerHandle.pGetDeviceCab			= GetDeviceCab;
	m_OnvifSerHandle.pGetNetWorkInfo		= GetNetWorkInfo;
	m_OnvifSerHandle.pSetNetWorkInfo		= SetNetWorkInfo;
	m_OnvifSerHandle.pGetNetProtocolInfo	= GetNetProtocolInfo;
	m_OnvifSerHandle.pSetNetProtocolInfo	= SetNetProtocolInfo;
	m_OnvifSerHandle.pGetVideoEncode		= GetVideoEncode;
	m_OnvifSerHandle.pSetVideoEncode		= SetVideoEncode;
	m_OnvifSerHandle.pGetVideoEncodeCab		= GetVideoEncodeCab;
	m_OnvifSerHandle.pGetAudioEncode		= GetAudioEncode;
	m_OnvifSerHandle.pSetAudioEncode		= SetAudioEncode;
	m_OnvifSerHandle.pGetSystemTime			= GetSystemTime;
	m_OnvifSerHandle.pSetSystemTime			= SetSystemTime;
	m_OnvifSerHandle.pGetUsrInfo			= GetUsrInfo;
	m_OnvifSerHandle.pSetImagingParam		= SetImagingParam;
	m_OnvifSerHandle.pGetImagingParam		= GetImagingParam;
	m_OnvifSerHandle.pGetImagingCab			= GetImagingCab;
	return av_true;
}

CAvOnvifSer::CAvOnvifSer()
{

}

CAvOnvifSer::~CAvOnvifSer()
{

}

av_bool CAvOnvifSer::Start()
{
	int ret = 0;
	InitCbfun();
	ONVIFAPI_Init(&m_OnvifSerHandle);
	ret = ONVIFAPI_SerStart(8080);
	return ret == 0 ? av_true : av_false;
}

av_bool CAvOnvifSer::ReStart()
{
	int ret = 0;
	ret = Stop();
	ret = Start();
	return ret == 0 ? av_true : av_false;
}

av_bool CAvOnvifSer::Stop()
{
	int ret = 0;
	ret = ONVIFAPI_ServerStop();
	return ret == 0 ? av_true : av_false;
}
