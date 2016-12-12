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
#include "Apis/AvWareCplusplus.h"
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
static VideoEncoding_E AVCompToOnvifEncoding(AvComp		Comp)
{
	switch(Comp){
		case AvComp_H264:  return VideoEncoding__H264;
		case AvComp_H265:  return VideoEncoding__H265;
		case AvComp_MJPEG: return VideoEncoding__JPEG;
		case AvComp_LAST:
		default:
			return VideoEncoding__NULL;
	}
}

static AvComp OnvifEncodingToAV(VideoEncoding_E		encoding)
{
	switch(encoding){
		case VideoEncoding__H264: return AvComp_H264;
		case VideoEncoding__H265: return AvComp_H265;
		case VideoEncoding__JPEG: return AvComp_MJPEG;
		case VideoEncoding__NULL:
		default:
			return AvComp_LAST;
	}
}

static CaptureSize GetResEnum(av_32 width, av_32 hight)
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
		return CaptureSize_HD720P;
	else if (width == 1280 && hight == 960)
		return CaptureSize_HD960P;
	else if (width == 1920 && hight == 1080)
		return CaptureSize_HD1080P;
	else if (width == 2048 && hight == 1536)
		return CaptureSize_HC300W;
	else if (width == 2688 && hight == 1520)
		return CaptureSize_HC400W;
	else if (width == 2592 && hight == 1944)
		return CaptureSize_HC500W;
// 	else if (width == 3072 && hight == 2048)
// 		return CaptureSize_600W;
	else if (width == 3840 && hight == 2160)
		return CaptureSize_HC800W;
	else if (width == 4096 && hight == 2160)
		return CaptureSize_HC4K;
	else if (width == 7680 && hight == 4320)
		return CaptureSize_HC8K;
	else {
		return CaptureSize_Self;
	}
	return CaptureSize_LAST;
}

static int GetResWidthAndHight(CaptureSize ImageSize, int *width, int *height)
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
		case CaptureSize_HD720P:
			*height = 720;
			*width = 1280;
			break;
		case CaptureSize_HD960P:
			*height = 960;
			*width = 1280;
			break;
		case CaptureSize_HD1080P:
		case CaptureSize_S1920_1080:
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
	strcat(info->country,"China");
	strcat(info->name,"IPC");
	strcat(info->softwareVer,"Build V1.0");
	sprintf(info->softwareVer, "%s", CAvDevice::GetSoftVersionString().c_str());
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
	ConfigNetComm &ConfigNet = NetComm.GetConfig(NetCommT_LAN0);
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
	ConfigNetComm &ConfigNet = NetComm.GetConfig(NetCommT_LAN0);
 	info->rtspPort = 554;
 	info->onvifPort = 8080; 
 	sprintf(info->rtspUrl[0][0],"rtsp://%s:%d/c=0&amp;s=0",ConfigNet.LanAttr.IpAddr,info->rtspPort);
	sprintf(info->rtspUrl[0][1],"rtsp://%s:%d/c=0&amp;s=1",ConfigNet.LanAttr.IpAddr,info->rtspPort);
	sprintf(info->snapUrl[0][0], "http://%s/%d.snap", ConfigNet.LanAttr.IpAddr, 0);
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
	ConfigEncodeProfile &Profile = Encode.GetConfig(chn);
		
	memset(info,0,sizeof(VedioEncode_S));
	info->bitrate = Profile.CHLProfile[streamId].Profile.BitRateValue;
	info->cvbrMode = Profile.CHLProfile[streamId].Profile.BitRateCtl;
	info->encoding = AVCompToOnvifEncoding(Profile.CHLProfile[streamId].Profile.Comp);
	info->frameRate = (int)Profile.CHLProfile[streamId].Profile.FrameRate;
	info->gop = (int)Profile.CHLProfile[streamId].Profile.Gop;
	info->h264Profile = H264Profile__Main;
	info->qulity = (int)Profile.CHLProfile[streamId].Profile.Qlevel;
	if (CaptureSize_Self != Profile.CHLProfile[streamId].Profile.ImageSize){
		GetResWidthAndHight(Profile.CHLProfile[streamId].Profile.ImageSize, &info->reslution.width, &info->reslution.height);
 	}else{
		info->reslution.width = Profile.CHLProfile[streamId].Profile.ImageSelfWidth;
		info->reslution.height = Profile.CHLProfile[streamId].Profile.ImageSelfHeigh;
 	}
	return 0;
}

static int	SetVideoEncode(int chn, int streamId, VedioEncode_S *info)
{
	//profile qulity
	CAvConfigEncode Encode;
	Encode.Update();
	ConfigEncodeProfile &Profile = Encode.GetConfig(chn);

	Profile.CHLProfile[streamId].Profile.BitRateValue = (av_u32)info->bitrate;
	Profile.CHLProfile[streamId].Profile.BitRateCtl = (BitRateCtrl)info->cvbrMode;
	Profile.CHLProfile[streamId].Profile.FrameRate = info->frameRate;
	Profile.CHLProfile[streamId].Profile.Gop = info->gop;
	Profile.CHLProfile[streamId].Profile.Qlevel = info->qulity;
	Profile.CHLProfile[streamId].Profile.ImageSize = GetResEnum(info->reslution.width, info->reslution.height);
	if (CaptureSize_Self == Profile.CHLProfile[streamId].Profile.ImageSize){
		Profile.CHLProfile[streamId].Profile.ImageSelfHeigh = info->reslution.height;
		Profile.CHLProfile[streamId].Profile.ImageSelfWidth = info->reslution.width;
 	}
	Profile.CHLProfile[streamId].Profile.Comp = OnvifEncodingToAV(info->encoding);
 	Encode.SettingUp();
	return 0;
}

static int	GetVideoEncodeCab(int chn, int streamId, VedioEncodeCab_S *info)
{
	C_DspCaps DspCaps;
	
	CAvDevice::GetDspCaps(DspCaps);
	if (DspCaps.nMaxDecodeChannel <= chn && DspCaps.nMaxEncodeChannel <= chn){
		av_error("This Device Have no %d Channel \n", chn);
		return -1;
	}
	C_EncodeCaps EncodeCaps;
	CAvDevice::GetEncodeCaps(chn, EncodeCaps);
	if (streamId == 0){
		info->bitrateRange.max = 8192;
		info->bitrateRange.min = 512;
	}
	else{
		info->bitrateRange.max = 1024;
		info->bitrateRange.min = 64;
	}
	info->encodingCab = VideoEncoding__NULL;

	CAvConfigEncode ConfigEncode;
	ConfigEncodeProfile &Profile = ConfigEncode.GetConfig(chn);

	for (int i = 0; i < AvComp_LAST; i++){
		if (streamId == CHL_MAIN_T){
			if (!(AvMask(i) & EncodeCaps.CompMask)) continue;
		}
		else if (streamId == CHL_SUB1_T){
			if (!(AvMask(i) & EncodeCaps.ExtImageSizeMask[Profile.CHLProfile[CHL_MAIN_T].Profile.ImageSize])) continue;
		}
		else{
			av_error("Onvif Stream id = %d  bad number\n", streamId);
			break;
		}
		
		switch (i)
		{
		case AvComp_MJPEG:
			info->encodingCab = (VideoEncoding_E)(info->encodingCab | VideoEncoding__JPEG);
			break;
		case AvComp_H264:
			info->encodingCab = (VideoEncoding_E)(info->encodingCab | VideoEncoding__H264);
			break;
		case AvComp_H265:
			info->encodingCab = (VideoEncoding_E)(info->encodingCab | VideoEncoding__H265);
			break;
		case AvComp_JPEG:
			info->encodingCab = (VideoEncoding_E)(info->encodingCab | VideoEncoding__JPEG);
			break;
		default:
			break;
		}
	}
	info->frameRateRange.max = 30;
	info->frameRateRange.min = 5;
	info->gopRange.max = 50;
	info->gopRange.min = 10;

	info->h264ProfileCab = (H264Profile_E)(H264Profile__Baseline | H264Profile__Main | H264Profile__High);
	info->qulityRange.max = 10;
	info->qulityRange.min = 1;
	int pos = 0;
	for (int i = CaptureSize_HC8K; i > CaptureSize_Self; i--){
		if (!(AvMask(i) & EncodeCaps.ImageSizeMask)) continue;
		switch (i)
		{
		case CaptureSize_QVGAEX:	//320*180
			info->resSupport[pos].width = 320;
			info->resSupport[pos++].height = 180;
			break;
		case CaptureSize_QVGA:	//320*240
			info->resSupport[pos].width = 320;
			info->resSupport[pos++].height = 240;
			break;
		case CaptureSize_VGAEX:	//640*360
			info->resSupport[pos].width = 640;
			info->resSupport[pos++].height = 360;
			break;
		case CaptureSize_VGA:	//640*480
			info->resSupport[pos].width = 640;
			info->resSupport[pos++].height = 480;
			break;
		case CaptureSize_D1:		//720*576
			info->resSupport[pos].width = 720;
			info->resSupport[pos++].height = 576;
			break;
		case CaptureSize_HD720P:
			info->resSupport[pos].width = 1280;
			info->resSupport[pos++].height = 720;
			break;
		case CaptureSize_HD960P:
			info->resSupport[pos].width = 1280;
			info->resSupport[pos++].height = 960;
			break;
		case CaptureSize_HD1080P:
			info->resSupport[pos].width = 1920;
			info->resSupport[pos++].height = 1080;
			break;

		case CaptureSize_HC300W:	//2048*1536
			info->resSupport[pos].width = 2048;
			info->resSupport[pos++].height = 1536;
			break;
		case CaptureSize_HC400W:	//2560*1440
			info->resSupport[pos].width = 2560;
			info->resSupport[pos++].height = 1440;
			break;
		case CaptureSize_HC500W:	//2592*1944
			info->resSupport[pos].width = 2592;
			info->resSupport[pos++].height = 1944;
			break;
// 		case CaptureSize_600W:	//3072*2048
// 			info->resSupport[pos].width = 3072;
// 			info->resSupport[pos++].height = 2048;
// 			break;
		case CaptureSize_HC800W:	//3840*2160
			info->resSupport[pos].width = 2048;
			info->resSupport[pos++].height = 2160;
			break;
		case CaptureSize_HC4K:	//4096*2160
			info->resSupport[pos].width = 4096;
			info->resSupport[pos++].height = 2160;
			break;
		case CaptureSize_HC8K:	//7680*4320
			info->resSupport[pos].width = 7680;
			info->resSupport[pos++].height = 4320;
			break;
		default:
			assert(0);
			break;
		}
	}
	info->resSupportNum = pos;
	assert(pos);
	info->maxReslution.width = info->resSupport[0].width ;
	info->maxReslution.height = info->resSupport[0].height;
	info->defaultReslution.height = info->resSupport[0].width;;
	info->defaultReslution.width = info->resSupport[0].height;;
	return 0;
}

static int	GetAudioEncode(int chn, int streamId, AudioEncode_S *info)
{

 	info->Bitrate = 16;
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
	ConfigImageProfile ImageProfile = ConfigImage.GetConfig();

	info->Brightness = ImageProfile.Brightness;
	info->ColorSaturation = ImageProfile.Saturation;
	info->Contrast = ImageProfile.Contrast;
	info->Sharpness = ImageProfile.Hue;
	return 0;
}

static int	SetImagingParam(ImagingParam_S *info)
{
	CAvConfigImage ConfigImage;
	ConfigImage.Update();
	ConfigImageProfile ImageProfile = ConfigImage.GetConfig();
	ImageProfile.Brightness = info->Brightness;
	ImageProfile.Saturation = info->ColorSaturation;
	ImageProfile.Contrast = info->Contrast;
	ImageProfile.Hue = info->Sharpness;

	ConfigImage.SettingUp();
	return 0;
}

static int	GetImagingCab(ImagingCab_S *info)
{
	C_ImageCaps ImageCaps;
	CAvDevice::GetImageCaps(0, ImageCaps);
	/*Brightness[0], Contrast[1], Saturation[2], Hue[3]*/
	info->Brightness.max = ImageCaps.BrightnessRang;
	info->Brightness.min = 0;
	info->ColorSaturation.max = ImageCaps.SaturationRang;
	info->ColorSaturation.min = 0;

	info->Contrast.max = ImageCaps.ContrastRang;
	info->Contrast.min = 0;

	info->Sharpness.max = ImageCaps.HueRang;
	info->Sharpness.min = 0;
	return 0;
}

static	int	GetPtzCab(PtzCab_S *info)
{
	info->absolutePos.x.max = 1;
	info->absolutePos.x.min = -1;
	info->absolutePos.y.max = 1;
	info->absolutePos.y.min = -1;
	info->absoluteZoom.max = 1;
	info->absoluteZoom.min = 0;

	info->relationPos.x.max = 1;
	info->relationPos.x.min = -1;
	info->relationPos.y.max = 1;
	info->relationPos.y.min = -1;
	info->relationZoom.max = 1;
	info->relationZoom.min = 0;

	info->continuePos.x.max = 1;
	info->continuePos.x.min = -1;
	info->continuePos.y.max = 1;
	info->continuePos.y.min = -1;
	info->continueZoom.max = 1;
	info->continueZoom.min = 0;

	info->homeSupported = KEY_TRUE;
	info->maxPreset = 80;
	info->tourSupported = KEY_TRUE;
	info->ptzTimeOut.max = 200;
	info->ptzTimeOut.min = 1;
	return 0;
}

static	int	GetPtzConfig(PtzConfig_S *info)
{
	info->timeOut = 100;//此处过ODT不能写死
	return 0;
}

static	int	SetPtzConfig(PtzConfig_S *info)
{
	
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
	m_OnvifSerHandle.pGetPtzCab				= GetPtzCab;
	m_OnvifSerHandle.pGetPtzConfig			= GetPtzConfig;
	m_OnvifSerHandle.pSetPtzConfig			= SetPtzConfig;
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
