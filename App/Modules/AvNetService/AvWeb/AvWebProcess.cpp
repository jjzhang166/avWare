#include "AvNetService/AvWebProcess.h"
#include "Apis/AvWareCplusplus.h"
#include "AvConfigs/AvConfigCapture.h"
#include "AvDevice/AvDevice.h"
#include "AvUart/AvUart.h"
#include "Apis/AvEnuminline.h"

using std::string;
namespace av_web{

inline int AvWebChannelSalveCheck(CWebMsg& web_req, int& channel, int& slave)
{
	channel = web_req["Channel"].isInt() ? web_req["Channel"].asInt() : -1;
	slave = web_req["Slave"].isInt() ? web_req["Slave"].asInt() : -1;

	if ((channel < 0) || (slave < 0) || (slave >= CHL_NR_T)) {
		av_msg("Channel: %d Slave: %d\n", channel, slave);
		return WEB_STATUS_PARAM_ERROR;
	}
	return WEB_STATUS_OK;
}

inline void GetSupportEncImageSize(CWebMsg &msg, const av_u32 ImageSizeMask)
{
	int array_index = 0;
	for (int i = CaptureSize_Self; i < CaptureSize_LAST; i++) {
		if (AvMask(i) & ImageSizeMask) {
			msg[array_index] = EnumNameCaptureSize(static_cast<CaptureSize>(i));
			av_msg("support Encode Image Size %s\n",msg[array_index].asCString());
			++array_index;
		}
	}
}


inline void GetSupportEncComp(CWebMsg &msg, const av_u32 EncCompMask)
{
	int array_index = 0;
	for (int i = AvComp_H264; i <= AvComp_LAST; i++) {
		if (AvMask(i) & EncCompMask) {
			msg[array_index] = EnumNameAvComp(static_cast<AvComp>(i));
			av_msg("support Encode Comp %s\n",msg[array_index].asCString());
			++array_index;
		}
	}
}

inline void GetSupportEncBitContrul(CWebMsg &msg, av_uchar EncBitControl)
{
	int array_index = 0;
	av_warning("__%s EncBitControl = %x\n", __FUNCTION__, EncBitControl);
	for (int i = BitRateCtrl_CBR; i < BitRateCtrl_LAST; i++) {
		if (AvMask(i) & EncBitControl) {
			msg[array_index] = EnumNameBitRateCtrl(static_cast<BitRateCtrl>(i));
			av_msg("support Encode Comp %s\n",msg[array_index].asCString());
			++array_index;
		}
	}
}

}// namespace av_web

#if 0
template<> 
const char* av_web::EnumNameStr<AvComp>::List[] =
{
	"MJPEG",	//AvComp_MJPEG,
	"H264",		//AvComp_H264,
	"H265",		//AvComp_H265,
	"JPEG",		//AvComp_JPEG,
	"PCM",		//AvComp_PCM,
	"G711A",	//AvComp_G711A,
	"G711U",	//AvComp_G711U,
	"AAC",		//AvComp_AAC,
	"AMR_NB",	//AvComp_AMR_NB,
	"AMR_WB",	//AvComp_AMR_WB,
	"MP3",		//AvComp_MP3,
	"NR",		//AvComp_NR,
};

template<> 
const char* av_web::EnumNameStr<CaptureSize>::List[] =
{
	"Self",			//CaptureSize_Self,
	"480*270",		//CaptureSize_QVGAEX,	//480*270
	"320*240",		//CaptureSize_QVGA,		//480*360
	"640*360",		//CaptureSize_VGAEX,	//640*360
	"640*480",		//CaptureSize_VGA,		//640*480
	"720*480",		//CaptureSize_D1,		//720*576
	"1280*720",		//CaptureSize_720P,
	"1280*960",		//CaptureSize_960P,
	"1920*1080",	//CaptureSize_1080P,
	"2048*1536",	//CaptureSize_300W,		//2048*1536
	"2560*1440",	//CaptureSize_400W,		//2560*1440
	"2592*1944",	//CaptureSize_500W,		//2592*1944
	"3072*2048",	//CaptureSize_600W,		//3072*2048
	"3840*2160",	//CaptureSize_800W,		//3840*2160
	"4096*2160",	//CaptureSize_4K,		//4096*2160
	"7680*4320",	//CaptureSize_8K,		//7680*4320
	"NR",			//CaptureSize_NR,
};

template<> 
const char* av_web::EnumNameStr<BitRateCtrl>::List[] =
{
	"CBR",		//AvBitRate_CBR
	"VBR",		//AvBitRate_VBR
};

template<>
const char* av_web::EnumNameStr<IrCutMode>::List[] =
{
	"Open",			//IRCUT_OPEN,
	"Close",		//IRCUT_CLOSE,
	"Auto",			//IRCUT_AUTO,
	"Schelude",		//IRCUT_TIMER,
};

template<>
const char* av_web::EnumNameStr<AntiFlckerMode>::List[] =
{
	"None",			//AvAntiFlckerMode_NONE = 0,
	"InDoor50Hz",			//AvAntiFlckerMode_INDOOR_50HZ = 1,
	"OutDoor50Hz",				//AvAntiFlckerMode_OUTDOOR_50HZ = 2,
	"50Hz",				//AvAntiFlckerMode_AUTO_50HZ = 3,
	"InDoor60Hz",				//AvAntiFlckerMode_INDOOR_60HZ = 4,
	"OutDoor60Hz",				//AvAntiFlckerMode_OUTDOOR_60HZ = 5,
	"60Hz",						//AvAntiFlckerMode_AUTO_60HZ = 6,
	"Theater50Hz",				//AvAntiFlckerMode_THEATER_50HZ = 7,
	"Fast50Hz",				//AvAntiFlckerMode_FAST_50HZ = 8,
	"Theater60Hz",				//AvAntiFlckerMode_THEATER_60HZ = 9,
	"Fast60Hz",					//AvAntiFlckerMode_FAST_60HZ = 10,
	"Close",				//AvAntiFlckerMode_ANTI_FLICKER_CLOSED = 11,
	"Nr",				//AvAntiFlckerMode_LAST = 12
};
#endif

int av_web::AvWebLogIn(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param)
{
	av_msg("User: %s  PW: %s\n", web_req["Username"].asCString(), web_req["Password"].asCString());
	have_resp_param = av_true;
	//TODO: session还没处理
	web_resp["Session"] = "24678";
	return WEB_STATUS_OK;
}

int av_web::AvWebLogOut(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param)
{
	have_resp_param = av_false;
	return WEB_STATUS_OK;
}

int av_web::AvWebGetDeviceCaps(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param)
{
	av_msg("AvWebGetDeviceCaps\n");
	C_DspCaps dspcap = { 0 };
	g_AvDevice.GetDspCaps(dspcap);
	web_resp["MaxEncodeChannel"] = dspcap.nMaxEncodeChannel;
	web_resp["MaxDecodeChannel"] = dspcap.nMaxDecodeChannel;
	CWebMsg &enccap_comp_array = web_resp["Compress"];
	//TODO:暂时写死
	enccap_comp_array[static_cast<unsigned int>(0)] = "H264";
	enccap_comp_array[static_cast<int>(1)] = "H265";
	enccap_comp_array[static_cast<int>(2)] = "MJPEG";
	web_resp["MaxResolution"] = "1920*1080";
	have_resp_param = av_true;
	return WEB_STATUS_OK;
}


int av_web::AvWebGetVideoEncodeCaps(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param)
{
	av_msg("AvWebGetVideoEncodeCaps\n");
	have_resp_param = av_false;
	int channel = web_req["Channel"].isInt() ? web_req["Channel"].asInt() : -1;
	int slave = web_req["Slave"].isInt() ? web_req["Slave"].asInt() : -1;
	if ((channel < 0) || (slave < 0) || (slave >= CHL_NR_T)) {
		av_msg("Channel: %d Slave: %d\n", channel, slave);
		return WEB_STATUS_PARAM_ERROR;
	}
	C_EncodeCaps enc_cap = { 0 };
	g_AvDevice.GetEncodeCaps(channel, enc_cap);
	web_resp["Channel"] = channel;
	web_resp["Slave"] = slave;


	CWebMsg &enccap_res_array = web_resp["Resolution"];
	if (CHL_MAIN_T == slave) {
		GetSupportEncImageSize(enccap_res_array, enc_cap.ImageSizeMask);
	}
	else if ((CHL_SUB1_T == slave) && (enc_cap.ExtChannelMask & AvMask(CHL_SUB1_T))) {
	
		CAvConfigEncode	conf_encode;
		conf_encode.Update(channel);
		ConfigEncodeProfile &profile = conf_encode.GetConfig(channel);

		int tmp = profile.CHLProfile[CHL_MAIN_T].Profile.ImageSize;
		GetSupportEncImageSize(enccap_res_array, enc_cap.ExtImageSizeMask[tmp]);
	}


	CWebMsg &enccap_comp_array = web_resp["Compress"];
	GetSupportEncComp(enccap_comp_array, enc_cap.CompMask);

	CWebMsg &enccap_bitctrl_array = web_resp["Bitctrl"];
	GetSupportEncBitContrul(enccap_bitctrl_array, enc_cap.BitRateCtrlMask);

	web_resp["Maxframerate"] = 30;
	web_resp["Minframerate"] = 1;
	web_resp["MaxGop"] = 100;
	web_resp["MinGop"] = 10;
	web_resp["MaxBitRate"] = 1024 * 8;
	web_resp["MinBitRate"] = 512;

	web_resp["MaxQLevel"] = 4;
	web_resp["MinQLevel"] = 0;
	have_resp_param = av_true;
	return 0;
}

int av_web::AvWebGetVideoEncodeProfile(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param)
{
	have_resp_param = av_false;

	//av_msg("%d %d\n", web_req["Channel"].asInt(), web_req["Slave"].asInt());
	int channel = web_req["Channel"].isInt() ? web_req["Channel"].asInt() : -1;
	int slave = web_req["Slave"].isInt() ? web_req["Slave"].asInt() : -1;
	
	if ((channel < 0) || (slave < 0) || (slave >= CHL_NR_T)) {
		av_msg("Channel: %d Slave: %d\n", channel, slave);
		return WEB_STATUS_PARAM_ERROR;
	}

	//TODO: 请求参数范围检测
	CAvConfigEncode	conf_encode;
	conf_encode.Update(channel);
	ConfigEncodeProfile &Profile = conf_encode.GetConfig(channel);
	if (Profile.CHLProfile[slave].Enable) {

		//C_EncodeFormats &format = tag_encode.CHLFormats[slave].Formats;
		web_resp["Channel"] = channel;
		web_resp["Slave"] = slave;
		string str_tmp;

		web_resp["Compress"] = EnumNameAvComp(Profile.CHLProfile[slave].Profile.Comp);
		

		if (nullptr != EnumNameCaptureSize(Profile.CHLProfile[slave].Profile.ImageSize)) {
			web_resp["Resolution"] = EnumNameCaptureSize(Profile.CHLProfile[slave].Profile.ImageSize);
		} else {
			char tmp[36] = { 0 };
			sprintf(tmp, "%d*%d",
				Profile.CHLProfile[slave].Profile.ImageSelfWidth,
				Profile.CHLProfile[slave].Profile.ImageSelfHeigh);
			web_resp["Resolution"] = tmp;
		}
		web_resp["FrameRate"] = Profile.CHLProfile[slave].Profile.FrameRate;
		web_resp["Bitctrl"] = EnumNameBitRateCtrl(Profile.CHLProfile[slave].Profile.BitRateCtl);
		web_resp["BitctrlCbrValue"] = Profile.CHLProfile[slave].Profile.BitRateValue;
		web_resp["BitctrlVbrValue"] = Profile.CHLProfile[slave].Profile.BitRateValue;
		web_resp["GOP"] = Profile.CHLProfile[slave].Profile.Gop;
		web_resp["QLevel"] = Profile.CHLProfile[slave].Profile.Qlevel;
	} else {
		return WEB_STATUS_PARAM_ERROR;
	}

	have_resp_param = av_true;
	return WEB_STATUS_OK;
}

int av_web::AvWebSetVideoEncodeProfile(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param)
{
	have_resp_param = av_false;

	int channel = web_req["Channel"].isInt() ? web_req["Channel"].asInt() : -1;
	int slave = web_req["Slave"].isInt() ? web_req["Slave"].asInt() : -1;
	if ((channel < 0) || (slave < 0) || (slave >= CHL_NR_T)) {
		av_msg("Channel: %d Slave: %d\n", channel, slave);
		return WEB_STATUS_PARAM_ERROR;
	}


	CAvConfigEncode *pConfEncode = new CAvConfigEncode();
	pConfEncode->Update();
	ConfigEncodeProfile &Profile = pConfEncode->GetConfig(channel);
	C_EnCodeProfile &EnCodeProfile = Profile.CHLProfile[slave].Profile;

	
	EnCodeProfile.Comp = AvCompStr2EnumValue(web_req["Compress"].asCString());
	EnCodeProfile.ImageSize = CaptureSizeStr2EnumValue(web_req["Resolution"].asCString());
	EnCodeProfile.FrameRate = web_req["FrameRate"].isInt() ? web_req["FrameRate"].asInt() : -1;

	EnCodeProfile.BitRateCtl = BitRateCtrlStr2EnumValue(web_req["Bitctrl"].asCString());
	if (BitRateCtrl_CBR == EnCodeProfile.BitRateCtl) {
		EnCodeProfile.BitRateValue = web_req["BitctrlCbrValue"].isInt() ? web_req["BitctrlCbrValue"].asInt() : -1;
	}
	else if (BitRateCtrl_VBR == EnCodeProfile.BitRateCtl) {
		EnCodeProfile.Qlevel = web_req["QLevel"].isInt() ? web_req["QLevel"].asInt() : -1;
	}

	EnCodeProfile.Gop = web_req["GOP"].isInt() ? web_req["GOP"].asInt() : -1;
	int ret = 1;
	ret = pConfEncode->SettingUp();
	delete pConfEncode;
	if (0 == ret) {
		return WEB_STATUS_OK;
	} else {
		return WEB_STATUS_PARAM_ERROR;
	}
	
}

int av_web::AvWebGetImageProfile(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param)
{
	have_resp_param = av_false;
	int channel = 0;
	int slave = 0;
	int ret = AvWebChannelSalveCheck(web_req, channel, slave);
	if (WEB_STATUS_OK != ret) { return ret; }

	CAvConfigImage conf_image;
	conf_image.Update();
	ConfigImageProfile &image_fmt = conf_image.GetConfig(channel);
	web_resp["Brightness"] = image_fmt.Brightness;
	web_resp["Contrast"] = image_fmt.Contrast;
	web_resp["Saturation"] = image_fmt.Saturation;
	web_resp["Hue"] = image_fmt.Hue;

	have_resp_param = av_true;
	return WEB_STATUS_OK;
}

int av_web::AvWebSetImageProfile(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param)
{
	have_resp_param = av_false;
	int channel = 0;
	int slave = 0;
	int ret = AvWebChannelSalveCheck(web_req, channel, slave);
	if (WEB_STATUS_OK != ret) { return ret; }

	CAvConfigImage conf_image;
	conf_image.Update();
	ConfigImageProfile &image_fmt = conf_image.GetConfig(channel);
	image_fmt.Brightness = web_req["Brightness"].isInt() ? web_req["Brightness"].asInt() : -1;
	image_fmt.Contrast = web_req["Contrast"].isInt() ? web_req["Contrast"].asInt() : -1;
	image_fmt.Saturation = web_req["Saturation"].isInt() ? web_req["Saturation"].asInt() : -1;
	image_fmt.Hue = web_req["Hue"].isInt() ? web_req["Hue"].asInt() : -1;
	return (0 == conf_image.SettingUp(channel)) ? WEB_STATUS_OK : WEB_STATUS_PARAM_ERROR;
}

int av_web::AvWebCallPtz(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param)
{
	have_resp_param = av_false;
	int channel = 0;
	int slave = 0;
	int ret = AvWebChannelSalveCheck(web_req, channel, slave);
	if (WEB_STATUS_OK != ret) { return ret; }

	if (!web_req["Cmd"].isInt()) {
		return WEB_STATUS_PARAM_ERROR;
	}
// 	C_PtzCmd PtzCmd;
// 
// 	PtzCmd.PtzCmd = web_req["Cmd"].asInt();
// 	PtzCmd.HSpeed = web_req["HSpeed"].isInt() ? web_req["HSpeed"].asInt() : 1;
// 	PtzCmd.VSpeed = web_req["VSpeed"].isInt() ? web_req["VSpeed"].asInt() : 1;
// 	return (av_true == g_AvUart.PtzStart(ptzcmd)) ? WEB_STATUS_OK : WEB_STATUS_PARAM_ERROR;
}

int av_web::AvWebCallPtzFocus(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param)
{
	have_resp_param = av_false;
	int channel = 0;
	int slave = 0;
	int ret = AvWebChannelSalveCheck(web_req, channel, slave);
	if (WEB_STATUS_OK != ret) { return ret; }

	if (!web_req["Cmd"].isInt()) {
		return WEB_STATUS_PARAM_ERROR;
	}
// 	PtzCmd ptzcmd = { 0 };
// 	ptzcmd.Cmd = web_req["Cmd"].asInt();
// 	ptzcmd.HSpeed = web_req["HSpeed"].isInt() ? web_req["HSpeed"].asInt() : 1;
// 	ptzcmd.VSpeed = web_req["VSpeed"].isInt() ? web_req["VSpeed"].asInt() : 1;
// 	av_msg("TODO: Call Ptz Focus cmd %d HSpeed %d\n", ptzcmd.Cmd, ptzcmd.HSpeed);
	return WEB_STATUS_OK;
}

int av_web::AvWebCallPtzIris(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param)
{
	have_resp_param = av_false;
	int channel = 0;
	int slave = 0;
	int ret = AvWebChannelSalveCheck(web_req, channel, slave);
	if (WEB_STATUS_OK != ret) { return ret; }

	if (!web_req["Cmd"].isInt()) {
		return WEB_STATUS_PARAM_ERROR;
	}
// 	PtzCmd ptzcmd = { 0 };
// 	ptzcmd.Cmd = web_req["Cmd"].asInt();
// 	ptzcmd.HSpeed = web_req["HSpeed"].isInt() ? web_req["HSpeed"].asInt() : 1;
// 	ptzcmd.VSpeed = web_req["VSpeed"].isInt() ? web_req["VSpeed"].asInt() : 1;
// 	av_msg("TODO: Call Ptz Iris cmd %d HSpeed %d\n", ptzcmd.Cmd, ptzcmd.HSpeed);
	return WEB_STATUS_OK;
}

int av_web::AvWebGetVideoCaptureCaps(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param)
{
	have_resp_param = av_false;
	int channel = 0;
	int slave = 0;
	int ret = AvWebChannelSalveCheck(web_req, channel, slave);
	if (WEB_STATUS_OK != ret) { return ret; }

	//TODO:暂时写死
	CWebMsg &tmpAf = web_resp["Antiflicker"];
	tmpAf[0] = EnumNameAntiFlckerMode(AntiFlckerMode_AUTO_50HZ);
	tmpAf[1] = EnumNameAntiFlckerMode(AntiFlckerMode_AUTO_60HZ);

	CWebMsg &tmpIr = web_resp["IrCut"];
	tmpIr[(unsigned int)0] = EnumNameIrCutMode(IrCutMode_AUTO);
	tmpIr[(unsigned int)1] = EnumNameIrCutMode(IrCutMode_CLOSE);
	tmpIr[(unsigned int)2] = EnumNameIrCutMode(IrCutMode_OPEN);

	have_resp_param = av_true;
	return WEB_STATUS_OK;
}

int av_web::AvWebGetVideoCaptureProfile(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param)
{
	have_resp_param = av_false;
	int channel = 0;
	int slave = 0;
	int ret = AvWebChannelSalveCheck(web_req, channel, slave);
	if (WEB_STATUS_OK != ret) { return ret; }

	CAvConfigCapture conf_cap;
	conf_cap.Update();
	ConfigCaptureProfile &cap_fmat = conf_cap.GetConfig(channel);

	web_resp["Antiflicker"] = EnumNameAntiFlckerMode(cap_fmat.AntiFlcker);
	web_resp["IrCut"] = EnumNameIrCutMode(cap_fmat.IrCut);
	web_resp["Horreverse"] = cap_fmat.MirrorMaskValue & AvMask(MirrorMode_HOR) ? 1 : 0;
	web_resp["Verreverse"] = cap_fmat.MirrorMaskValue & AvMask(MirrorMode_VER)  ? 1 : 0;
	have_resp_param = av_true;
	return WEB_STATUS_OK;
}

int av_web::AvWebSetVideoCaptureProfile(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param)
{
	have_resp_param = av_false;
	int channel = 0;
	int slave = 0;
	int ret = AvWebChannelSalveCheck(web_req, channel, slave);
	if (WEB_STATUS_OK != ret) { return ret; }

	CAvConfigCapture conf_cap;
	conf_cap.Update();
	ConfigCaptureProfile &CapProfile = conf_cap.GetConfig(channel);
	CapProfile.AntiFlcker = AntiFlckerModeStr2EnumValue(web_req["Antiflicker"].asCString());
	CapProfile.IrCut = IrCutModeStr2EnumValue(web_req["IrCut"].asCString());
	if (web_req["Horreverse"].asInt() == 1){
		CapProfile.MirrorMaskValue |= AvMask(MirrorMode_HOR);
	}
	else{
		CapProfile.MirrorMaskValue &= ~AvMask(MirrorMode_HOR);
	}
	if (web_req["Verreverse"].asInt() == 1){
		CapProfile.MirrorMaskValue |= AvMask(MirrorMode_VER);
	}
	else{
		CapProfile.MirrorMaskValue &= ~AvMask(MirrorMode_VER);
	}
	have_resp_param = av_true;
	return (0 == conf_cap.SettingUp(channel)) ? WEB_STATUS_OK : WEB_STATUS_PARAM_ERROR;
}

int av_web::AvWebGetRtmpProfile(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param)
{
	have_resp_param = av_false;
	int channel = 0;
	int slave = 0;
	int ret = AvWebChannelSalveCheck(web_req, channel, slave);
	if (WEB_STATUS_OK != ret) { return ret; }

	//if (slave > K_RTMP_CONFIG_MAX) { return WEB_STATUS_PARAM_ERROR; }

	CAvConfigRtmp rtmp;
	rtmp.Update();
	ConfigRtmp &conf_rtmp = rtmp.GetConfig(channel);
	
	web_resp["RtmpAddr"] = conf_rtmp.PushServer;
	web_resp["RtmpString"] = conf_rtmp.PushStrings;
	web_resp["Enable"] = conf_rtmp.bEnable;
	web_resp["RtmpAudio"] = conf_rtmp.bEnableAudio;
	
	have_resp_param = av_true;
	return WEB_STATUS_OK;
}

int av_web::AvWebSetRtmpProfile(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param)
{
	have_resp_param = av_false;
	int channel = 0;
	int slave = 0;
	int ret = AvWebChannelSalveCheck(web_req, channel, slave);
	if (WEB_STATUS_OK != ret) { return ret; }

	CAvConfigRtmp rtmp;
	rtmp.Update();
	ConfigRtmp &conf_rtmp = rtmp.GetConfig(channel);
	
	strcpy(conf_rtmp.PushServer, web_req["RtmpAddr"].asCString());
	strcpy(conf_rtmp.PushStrings, web_req["RtmpString"].asCString());
	conf_rtmp.bEnable = web_req["Enable"].asInt() == 1 ? av_true:av_false;
	conf_rtmp.bEnableAudio = web_req["RtmpAudio"].asInt() == 1 ? av_true : av_false;
	conf_rtmp.PushStream = slave;


	return (0 == rtmp.SettingUp(channel)) ? WEB_STATUS_OK : WEB_STATUS_PARAM_ERROR;
}