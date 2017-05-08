#include "AvNetService/AvWebProcess.h"
#include "Apis/AvWareCplusplus.h"
#include "AvConfigs/AvConfigCapture.h"
#include "AvDevice/AvDevice.h"
#include "AvUart/AvUart.h"
#include "Apis/AvWareStruct.h"
#include "Apis/AvEnuminline.h"
#include "AvAlarm/AvAlarm.h"
#include "AvConfigs/AvConfigNetService.h"


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


int AvWebLogIn(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param)
{
	av_msg("User: %s  PW: %s\n", web_req["Username"].asCString(), web_req["Password"].asCString());
	have_resp_param = av_true;
	//TODO: session还没处理
	web_resp["Session"] = "24678";
	return WEB_STATUS_OK;
}

int AvWebLogOut(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param)
{
	have_resp_param = av_false;
	return WEB_STATUS_OK;
}

int AvWebGetDeviceCaps(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param)
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


int AvWebGetVideoEncodeCaps(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param)
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

int AvWebGetVideoEncodeProfile(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param)
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

int AvWebSetVideoEncodeProfile(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param)
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

int AvWebGetImageProfile(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param)
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

int AvWebSetImageProfile(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param)
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

//web ptz cmd define:
enum{
		PtzCommand_LEFT_START = 2,
		PtzCommand_LEFT_STOP = 3,
		PtzCommand_RIGHT_START = 4,
		PtzCommand_RIGHT_STOP = 5,
		PtzCommand_UP_START = 6,
		PtzCommand_UP_STOP = 7,
		PtzCommand_DOWN_START = 8,
		PtzCommand_DOWN_STOP = 9,

		PtzCommand_LEFT_UP_START = 10,
		PtzCommand_LEFT_UP_STOP = 11,
		PtzCommand_RIGHT_UP_START = 12,
		PtzCommand_RIGHT_UP_STOP = 13,
		PtzCommand_LEFT_DOWN_START = 14,
		PtzCommand_LEFT_DOWN_STOP = 15,
		PtzCommand_RIGHT_DOWN_START = 16,
		PtzCommand_RIGHT_DOWN_STOP = 17,

		
		//// 图象变小开始
		PtzCommand_ZOOM_WIDE_START = 108,
		//// 图象变小停止
		PtzCommand_ZOOM_WIDE_STOP = 109,
		//// 图象变大开始
		PtzCommand_ZOOM_TELE_START = 110,
		//// 图象变大停止
		PtzCommand_ZOOM_TELE_STOP = 111,

		
		//// 焦距近开始
		PtzCommand_FOCUS_NEAR_START = 104,
		//// 焦距近停止
		PtzCommand_FOCUS_NEAR_STOP = 105,
		//// 焦距远开始
		PtzCommand_FOCUS_FAR_START = 106,
		//// 焦距远停止
		PtzCommand_FOCUS_FAR_STOP = 107,

		
		//// 开光圈开始
		PtzCommand_IRIS_OPEN_START = 100,
		//// 开光圈停止
		PtzCommand_IRIS_OPEN_STOP = 101,
		//// 关光圈开始
		PtzCommand_IRIS_CLOSE_START = 102,
		//// 关光圈停止
		PtzCommand_IRIS_CLOSE_STOP = 103,
};

static inline PtzCommand WebMsgConversionToPtzCommand(int web_cmd)
{
	switch (web_cmd) {
	case PtzCommand_LEFT_STOP:
	case PtzCommand_RIGHT_STOP:
	case PtzCommand_UP_STOP:
	case PtzCommand_DOWN_STOP:
	case PtzCommand_LEFT_UP_STOP:
	case PtzCommand_RIGHT_UP_STOP:
	case PtzCommand_LEFT_DOWN_STOP:
	case PtzCommand_RIGHT_DOWN_STOP:
	case PtzCommand_ZOOM_WIDE_STOP:
	case PtzCommand_ZOOM_TELE_STOP:
	case PtzCommand_FOCUS_NEAR_STOP:
	case PtzCommand_FOCUS_FAR_STOP:
	case PtzCommand_IRIS_OPEN_STOP:
	case PtzCommand_IRIS_CLOSE_STOP:
		return PtzCommand_STOP;
		
	case PtzCommand_LEFT_START:
		return PtzCommand_LEFT;
	case PtzCommand_RIGHT_START:
		return PtzCommand_RIGHT;

		
	case PtzCommand_UP_START:
		return PtzCommand_UP;
	case PtzCommand_DOWN_START:
		return PtzCommand_DOWN;


	case PtzCommand_ZOOM_WIDE_START:
		return PtzCommand_ZOOM_WIDE;
	case PtzCommand_ZOOM_TELE_START:
		return PtzCommand_ZOOM_TELE;


	case PtzCommand_FOCUS_NEAR_START:
		return PtzCommand_FOCUS_NEAR;
	case PtzCommand_FOCUS_FAR_START:
		return PtzCommand_FOCUS_FAR;


	case PtzCommand_IRIS_OPEN_START:
		return PtzCommand_IRIS_LARGE;
	case PtzCommand_IRIS_CLOSE_START:
		return PtzCommand_IRIS_SMALL;

	default:
		return PtzCommand_STOP;
	}
}

int AvWebCallPt(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param)
{
	have_resp_param = av_false;
	int channel = 0;
	int slave = 0;
	int ret = AvWebChannelSalveCheck(web_req, channel, slave);
	if (WEB_STATUS_OK != ret) { return ret; }

	if (!web_req["Cmd"].isInt()) {
		return WEB_STATUS_PARAM_ERROR;
	}
	
 	C_PtzCmd command;
	memset(&command, 0, sizeof(command));
 	command.PtzCmd = WebMsgConversionToPtzCommand(web_req["Cmd"].asInt());
	//cradle head speed area 1-63,web speed area 0-10
	command.PtzSpeed = 6 * (web_req["HSpeed"].isInt() ? web_req["HSpeed"].asInt() : 5);
 	return (av_true == g_AvUart.PtzSetCommand(command)) ? WEB_STATUS_OK : WEB_STATUS_PARAM_ERROR;
}

int AvWebCallZoom_Focus_Iris(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param)
{
	have_resp_param = av_false;
	int channel = 0;
	int slave = 0;
	int ret = AvWebChannelSalveCheck(web_req, channel, slave);
	if (WEB_STATUS_OK != ret) { return ret; }

	if (!web_req["Cmd"].isInt()) {
		return WEB_STATUS_PARAM_ERROR;
	}
	
 	C_PtzCmd command;
	memset(&command, 0, sizeof(command));
 	command.PtzCmd = WebMsgConversionToPtzCommand(web_req["Cmd"].asInt());
	//zoom speed area 1-16, web speed area 0-10
	command.PtzSpeed = web_req["HSpeed"].isInt() ? web_req["HSpeed"].asInt() : 5;

 	return (av_true == g_AvUart.PtzSetCommand(command)) ? WEB_STATUS_OK : WEB_STATUS_PARAM_ERROR;
}


int AvWebGetVideoCaptureCaps(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param)
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

int AvWebGetVideoCaptureProfile(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param)
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

int AvWebSetVideoCaptureProfile(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param)
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

int AvWebGetRtmpProfile(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param)
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

int AvWebSetRtmpProfile(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param)
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

int AvWebGetFtp(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param)
{
	CAvConfigNetFtp ftp;
	ftp.Update();
	ConfigNetFtp &conf_ftp = ftp.GetConfig();

	web_resp["FtpEnable"] = conf_ftp.bEnable == av_true ? 1 : 0;
	web_resp["FtpSeverAddr"] = conf_ftp.ServerAddress;
	web_resp["FtpServerPort"] = conf_ftp.ServicePort;
	web_resp["UsrName"] = conf_ftp.UserName;
	web_resp["PassWord"] = conf_ftp.PassWord;
	web_resp["FtpDir"] = conf_ftp.RemotePath;

	have_resp_param = av_true;
	return WEB_STATUS_OK;
}

int AvWebSetFtp(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param)
{
	have_resp_param = av_false;
	CAvConfigNetFtp ftp;
	ftp.Update();
	ConfigNetFtp &conf_ftp = ftp.GetConfig();

	conf_ftp.bEnable = 1 == web_req["FtpEnable"].asInt() ? av_true : av_false;
	conf_ftp.ServicePort = web_req["FtpServerPort"].asInt();

	strcpy(conf_ftp.ServerAddress, web_req["FtpSeverAddr"].asCString());
	strcpy(conf_ftp.UserName, web_req["UsrName"].asCString());
	strcpy(conf_ftp.PassWord, web_req["PassWord"].asCString());
	strcpy(conf_ftp.RemotePath, web_req["FtpDir"].asCString());

	return (0 == ftp.SettingUp()) ?WEB_STATUS_OK : WEB_STATUS_PARAM_ERROR;
}


int AvWebTestFtp(CWebMsg &web_req, CWebMsg &web_resp, av_bool &hava_resp_param)
{
	hava_resp_param = av_false;
	
	int channel = web_req["Channel"].isInt() ? web_req["Channel"].asInt() : -1;
	int slave = web_req["Slave"].isInt() ? web_req["Slave"].asInt() : -1;
	if ((channel < 0) || (slave < 0) || (slave >= CHL_NR_T)) {
		av_msg("Channel: %d Slave: %d\n", channel, slave);
		return WEB_STATUS_PARAM_ERROR;
	}

	C_AlmMsg msg;
	msg.AlarmTime = (av_u32)time(NULL);
	msg.AlarmEventName = AlarmEvent_TestFtpUpload;
	msg.Channel = -1;
	msg.Slave = CHL_NR_T;
	CAvQmsg AlarmMsgQueue(ALARM_QUEUE_MSG_NAME);
	av_u32 msglen = sizeof(C_AlmMsg);
	AlarmMsgQueue.QmSnd((av_char *)&msg, msglen);
	return WEB_STATUS_OK;
}

#if 0
int AvWebGetWifiConf(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param)
{
	CAvConfigNetComm conf_wifi;
	conf_wifi.Update(NetCommT_Wireless);
	ConfigNetComm &wifi_fmt = conf_wifi.GetConfig(NetCommT_Wireless);

	web_resp["SSID"] = wifi_fmt.WirelessAttr.RouterLinkInfo[0].SSID;
	web_resp["Passwd"] = wifi_fmt.WirelessAttr.RouterLinkInfo[0].Passwd;
	web_resp["WifiMode"] = "Station";
	av_msg("AvWebGetWifiConf SSID:[%s] Passwd[%s] Mode[%s]",
		wifi_fmt.WirelessAttr.RouterLinkInfo[0].SSID,
		wifi_fmt.WirelessAttr.RouterLinkInfo[0].Passwd,
		"Station");

	have_resp_param = av_true;
	return WEB_STATUS_OK;
}

int AvWebSetWifiConf(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param)
{
	have_resp_param = av_false;
	CAvConfigNetComm conf_wifi;
	conf_wifi.Update(NetCommT_Wireless);
	ConfigNetComm &wifi_fmt = conf_wifi.GetConfig(NetCommT_Wireless);
	
	av_msg("AvWebSetWifiConf SSID:[%s] Passwd[%s] Mode[%s]",
		web_req["SSID"].asCString(),
		web_req["Passwd"].asCString(),
		web_req["WifiMode"].asCString());
	
	strcpy(wifi_fmt.WirelessAttr.RouterLinkInfo[0].SSID, web_req["SSID"].asCString());
	strcpy(wifi_fmt.WirelessAttr.RouterLinkInfo[0].Passwd, web_req["Passwd"].asCString());
	return (0 == conf_wifi.SettingUp(NetCommT_Wireless)) ?WEB_STATUS_OK : WEB_STATUS_PARAM_ERROR;
}
#endif


#if 0
{"Header":{"Action":"Request","Method":"SetWifiConfig","Session":""},
"Param":{"WifiEnable":1,"WIFIState":"1","WIFISSID":"ssid123131",
"WIFIPW":"32131dsa","WifiDhcp":false,"WifiIPAddr":"192.168.2.36",
"WifiGetWay":"192.168.2.1","WifiMask":"255.255.255.255"}}

#endif

int AvWebGetWifiConf(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param)
{
	CAvConfigNetComm ConfigWifi;
	ConfigWifi.Update(NetCommT_Wireless);
	ConfigNetComm &WifiConfig = ConfigWifi.GetConfig(NetCommT_Wireless);

	int index = WifiConfig.WirelessAttr.WirelessPreferred;
	index--;
	if (index < 0) index = MAX_CONF_ROUTER_LINK - 1;

	web_resp["WifiEnable"]	= WifiConfig.bEnable;
	web_resp["WIFIState"]	= WifiConfig.WirelessAttr.Mode - 1;
	web_resp["WIFISSID"]	= WifiConfig.WirelessAttr.WirelessInfo[index].SSID;
	web_resp["WIFIPW"]		= WifiConfig.WirelessAttr.WirelessInfo[index].Passwd;
	web_resp["WifiDhcp"]	= av_true;
	web_resp["APSSID"]		= WifiConfig.WirelessAttr.WirelessApConf.SSID;
	web_resp["APPW"]		= WifiConfig.WirelessAttr.WirelessApConf.Passwd;
		
	have_resp_param = av_true;
	return WEB_STATUS_OK;
}

int AvWebSetWifiConf(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param)
{
	have_resp_param = av_false;
	CAvConfigNetComm ConfigWifi;
	ConfigWifi.Update();
	ConfigNetComm &WifiProfile = ConfigWifi.GetConfig(NetCommT_Wireless);

	av_msg("AvWebSetWifiConf WifiEnable[%d], WIFIState(0:AP, 1:station)[%s], SSID:[%s] Passwd[%s]\n",
			web_req["WifiEnable"].asInt(),
			web_req["WIFIState"].asCString(),
			web_req["WIFISSID"].asCString(),
			web_req["WIFIPW"].asCString());
	
	if (web_req["WifiEnable"].asInt()){
	    /* 0:AP Mode   1:station Mode*/
		if (0 == atoi(web_req["WIFIState"].asCString())){
			sprintf(WifiProfile.WirelessAttr.WirelessApConf.SSID, "%s", web_req["APSSID"].asCString());
			sprintf(WifiProfile.WirelessAttr.WirelessApConf.Passwd, "%s", web_req["APPW"].asCString());
			WifiProfile.WirelessAttr.Mode = WirelessMode_AP;

		}else if (1 == atoi(web_req["WIFIState"].asCString())){
			int index = WifiProfile.WirelessAttr.WirelessPreferred;
			sprintf(WifiProfile.WirelessAttr.WirelessInfo[index].SSID, web_req["WIFISSID"].asCString());
			sprintf(WifiProfile.WirelessAttr.WirelessInfo[index].Passwd, web_req["WIFIPW"].asCString());
			WifiProfile.WirelessAttr.Mode = WirelessMode_STATION;
			WifiProfile.WirelessAttr.WirelessPreferred = ++WifiProfile.WirelessAttr.WirelessPreferred%MAX_CONF_ROUTER_LINK;
			if (true == web_req["WifiDhcp"].asBool()){
				WifiProfile.mGetMode = NetCommGetMode_AUTO;
			}
			else{
				WifiProfile.mGetMode = NetCommGetMode_MANUAL;
				sprintf(WifiProfile.WirelessAttr.WirelessConf.IpAddr, web_req["WifiIPAddr"].asCString());
				sprintf(WifiProfile.WirelessAttr.WirelessConf.Gateway, web_req["WifiGateWay"].asCString());
				sprintf(WifiProfile.WirelessAttr.WirelessConf.Submask, web_req["WifiMask"].asCString());
				sprintf(WifiProfile.WirelessAttr.WirelessConf.Dns1, "8.8.8.8");
				sprintf(WifiProfile.WirelessAttr.WirelessConf.Dns2, "8.8.4.4");

				//sprintf(WifiProfile.WirelessAttr.WirelessConf.Dns1, web_req["WIFISSID"].asCString());
				//sprintf(WifiProfile.WirelessAttr.WirelessConf.Dns2, web_req["WIFISSID"].asCString());
			}
		}
	}else{
		WifiProfile.bEnable = av_false;
	}
	ConfigWifi.SettingUp();

	return WEB_STATUS_OK;
}

}// namespace av_web

