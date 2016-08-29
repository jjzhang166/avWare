#include "AvNetService/AvWebProcess.h"
#include "Apis/AvWareType.h"
#include "AvConfigs/AvConfigCapture.h"
#include "AvDevice/AvDevice.h"
#include "AvUart/AvUart.h"

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

}// namespace av_web


template<> 
const char* av_web::EnumNameStr<av_comp_t>::List[] = 
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
const char* av_web::EnumNameStr<av_capture_size>::List[] =
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
const char* av_web::EnumNameStr<av_bitrate_ctrl>::List[] =
{
	"CBR",		//AvBitRate_CBR
	"VBR",		//AvBitRate_VBR
};

template<>
const char* av_web::EnumNameStr<E_IrCutMode>::List[] =
{
	"Open",			//IRCUT_OPEN,
	"Close",		//IRCUT_CLOSE,
	"Auto",			//IRCUT_AUTO,
	"Schelude",		//IRCUT_TIMER,
};

template<>
const char* av_web::EnumNameStr<E_AntiFlckerMode>::List[] =
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

int av_web::AvWebLogIn(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param)
{
	av_msg("User: %s  PW: %s\n", web_req["Username"].asCString(), web_req["Password"].asCString());
	have_resp_param = av_true;
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
	enccap_comp_array[(unsigned int)0] = "H264";
	enccap_comp_array[(unsigned int)1] = "H265";
	enccap_comp_array[(unsigned int)2] = "MJPEG";
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
	g_AvDevice.GetCaptureCaps(channel, enc_cap);
	web_resp["Channel"] = channel;
	web_resp["Slave"] = slave;
	//TODO:暂时写死 后面改活
	CWebMsg &enccap_res_array = web_resp["Resolution"];
	if (CHL_MAIN_T == (slave)) {
		enccap_res_array[(unsigned int)0] = ConvertEnumToString<av_capture_size>(CaptureSize_1080P);
		enccap_res_array[(unsigned int)1] = ConvertEnumToString<av_capture_size>(CaptureSize_960P);
		enccap_res_array[(unsigned int)2] = ConvertEnumToString<av_capture_size>(CaptureSize_720P);
	} else {
		enccap_res_array[(unsigned int)0] = ConvertEnumToString<av_capture_size>(CaptureSize_D1);
		enccap_res_array[(unsigned int)1] = ConvertEnumToString<av_capture_size>(CaptureSize_VGA);
		enccap_res_array[(unsigned int)2] = ConvertEnumToString<av_capture_size>(CaptureSize_QVGA);
	}


	CWebMsg &enccap_comp_array = web_resp["Compress"];
	enccap_comp_array[(unsigned int)0] = ConvertEnumToString<av_comp_t>(AvComp_H264);
	enccap_comp_array[(unsigned int)1] = ConvertEnumToString<av_comp_t>(AvComp_H265);
	enccap_comp_array[(unsigned int)2] = ConvertEnumToString<av_comp_t>(AvComp_MJPEG);

	CWebMsg &enccap_bitctrl_array = web_resp["Bitctrl"];
	enccap_bitctrl_array[(unsigned int)0] = ConvertEnumToString<av_bitrate_ctrl>(AvBitRate_CBR);
	enccap_bitctrl_array[(unsigned int)1] = ConvertEnumToString<av_bitrate_ctrl>(AvBitRate_VBR);

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
	ConfigEncodeFormats &tag_encode = conf_encode.GetConfig(channel);
	if (tag_encode.CHLFormats[slave].Enable) {

		//C_EncodeFormats &format = tag_encode.CHLFormats[slave].Formats;
		web_resp["Channel"] = channel;
		web_resp["Slave"] = slave;
		string str_tmp;

		web_resp["Compress"] = ConvertEnumToString<av_comp_t>(tag_encode.CHLFormats[slave].Formats.Comp);

		if (nullptr != ConvertEnumToString<av_capture_size>(tag_encode.CHLFormats[slave].Formats.ImageSize)) {
			web_resp["Resolution"] = ConvertEnumToString<av_capture_size>(tag_encode.CHLFormats[slave].Formats.ImageSize);
		} else {
			char tmp[36] = { 0 };
			sprintf(tmp, "%d*%d",
				tag_encode.CHLFormats[slave].Formats.ImageSelfWidth,
				tag_encode.CHLFormats[slave].Formats.ImageSelfHeigh);
			web_resp["Resolution"] = tmp;
		}
		web_resp["FrameRate"] = tag_encode.CHLFormats[slave].Formats.FrameRate;
		web_resp["Bitctrl"] = ConvertEnumToString<av_bitrate_ctrl>(tag_encode.CHLFormats[slave].Formats.BitRateCtrl);
		web_resp["BitctrlCbrValue"] = tag_encode.CHLFormats[slave].Formats.BitRateValue;
		web_resp["BitctrlVbrValue"] = tag_encode.CHLFormats[slave].Formats.BitRateValue;
		web_resp["GOP"] = tag_encode.CHLFormats[slave].Formats.Gop;
		web_resp["QLevel"] = tag_encode.CHLFormats[slave].Formats.Qlevel;
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
	ConfigEncodeFormats &tag_encode = pConfEncode->GetConfig(channel);
	C_EncodeFormats &tmp_enc_fmt = tag_encode.CHLFormats[slave].Formats;

	tmp_enc_fmt.Comp = ConvertStringToEnum<av_comp_t>(web_req["Compress"].asCString());
	tmp_enc_fmt.ImageSize = ConvertStringToEnum<av_capture_size>(web_req["Resolution"].asCString());
	tmp_enc_fmt.FrameRate = web_req["FrameRate"].isInt() ? web_req["FrameRate"].asInt() : -1;

	tmp_enc_fmt.BitRateCtrl = ConvertStringToEnum<av_bitrate_ctrl>(web_req["Bitctrl"].asCString());
	if (AvBitRate_CBR == tmp_enc_fmt.BitRateCtrl) {
		tmp_enc_fmt.BitRateValue = web_req["BitctrlCbrValue"].isInt() ? web_req["BitctrlCbrValue"].asInt() : -1;
	} else if (AvBitRate_VBR == tmp_enc_fmt.BitRateCtrl) {
		//tmp_enc_fmt.BitRateValue = web_req["BitctrlVbrValue"].isInt() ? web_req["BitctrlVbrValue"].asInt() : -1;
		tmp_enc_fmt.Qlevel = web_req["QLevel"].isInt() ? web_req["QLevel"].asInt() : -1;
	}

	tmp_enc_fmt.Gop = web_req["GOP"].isInt() ? web_req["GOP"].asInt() : -1;
	int ret = 1;
	ret = pConfEncode->SettingUp();
	delete pConfEncode;
	if (0 == ret) {
		return WEB_STATUS_OK;
	} else {
		return WEB_STATUS_PARAM_ERROR;
	}
	
#if 0
	CAvConfigEncode	conf_encode;
	conf_encode.Update(channel);
	ConfigEncodeFormats &tag_encode = conf_encode.GetConfig(channel);
	C_EncodeFormats &tmp_enc_fmt = tag_encode.CHLFormats[slave].Formats;

	tmp_enc_fmt.Comp = ConvertStringToEnum<av_comp_t>(web_req["Compress"].asCString());
	tmp_enc_fmt.ImageSize = ConvertStringToEnum<av_capture_size>(web_req["Resolution"].asCString());
	tmp_enc_fmt.FrameRate = web_req["FrameRate"].isInt() ? web_req["FrameRate"].asInt() : -1;

	tmp_enc_fmt.BitRateCtrl = ConvertStringToEnum<av_bitrate_ctrl>(web_req["Bitctrl"].asCString());
	if (AvBitRate_CBR == tmp_enc_fmt.BitRateCtrl) {
		tmp_enc_fmt.BitRateValue = web_req["BitctrlCbrValue"].isInt() ? web_req["BitctrlCbrValue"].asInt() : -1;
	} else if (AvBitRate_VBR == tmp_enc_fmt.BitRateCtrl) {
		//tmp_enc_fmt.BitRateValue = web_req["BitctrlVbrValue"].isInt() ? web_req["BitctrlVbrValue"].asInt() : -1;
		tmp_enc_fmt.Qlevel = web_req["QLevel"].isInt() ? web_req["QLevel"].asInt() : -1;
	}

	tmp_enc_fmt.Gop = web_req["GOP"].isInt() ? web_req["GOP"].asInt() : -1;

	if (1 == conf_encode.SettingUp(channel)) {
		return WEB_STATUS_OK;
	} else {
		return WEB_STATUS_PARAM_ERROR;
	}
#endif
	//return (1 == conf_encode.SettingUp(channel)) ? WEB_STATUS_OK : WEB_STATUS_PARAM_ERROR;
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
	ConfigImageFormats &image_fmt = conf_image.GetConfig(channel);
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
	ConfigImageFormats &image_fmt = conf_image.GetConfig(channel);
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
	PtzCmd ptzcmd = { 0 };
	ptzcmd.Cmd = web_req["Cmd"].asInt();
	ptzcmd.HSpeed = web_req["HSpeed"].isInt() ? web_req["HSpeed"].asInt() : 1;
	ptzcmd.VSpeed = web_req["VSpeed"].isInt() ? web_req["VSpeed"].asInt() : 1;
	return (av_true == g_AvUart.PtzStart(ptzcmd)) ? WEB_STATUS_OK : WEB_STATUS_PARAM_ERROR;
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
	PtzCmd ptzcmd = { 0 };
	ptzcmd.Cmd = web_req["Cmd"].asInt();
	ptzcmd.HSpeed = web_req["HSpeed"].isInt() ? web_req["HSpeed"].asInt() : 1;
	ptzcmd.VSpeed = web_req["VSpeed"].isInt() ? web_req["VSpeed"].asInt() : 1;
	av_msg("TODO: Call Ptz Focus cmd %d HSpeed %d\n", ptzcmd.Cmd, ptzcmd.HSpeed);
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
	PtzCmd ptzcmd = { 0 };
	ptzcmd.Cmd = web_req["Cmd"].asInt();
	ptzcmd.HSpeed = web_req["HSpeed"].isInt() ? web_req["HSpeed"].asInt() : 1;
	ptzcmd.VSpeed = web_req["VSpeed"].isInt() ? web_req["VSpeed"].asInt() : 1;
	av_msg("TODO: Call Ptz Iris cmd %d HSpeed %d\n", ptzcmd.Cmd, ptzcmd.HSpeed);
	return WEB_STATUS_OK;
}

int av_web::AvWebGetVideoCaptureCaps(CWebMsg &web_req, CWebMsg &web_resp, av_bool &have_resp_param)
{
	have_resp_param = av_false;
	int channel = 0;
	int slave = 0;
	int ret = AvWebChannelSalveCheck(web_req, channel, slave);
	if (WEB_STATUS_OK != ret) { return ret; }

	CWebMsg &tmpAf = web_resp["Antiflicker"];
	tmpAf[(unsigned int)0] = ConvertEnumToString<E_AntiFlckerMode>(AvAntiFlckerMode_AUTO_50HZ);
	tmpAf[(unsigned int)1] = ConvertEnumToString<E_AntiFlckerMode>(AvAntiFlckerMode_AUTO_60HZ);

	CWebMsg &tmpIr = web_resp["IrCut"];
	tmpIr[(unsigned int)0] = ConvertEnumToString<E_IrCutMode>(IRCUT_AUTO);
	tmpIr[(unsigned int)1] = ConvertEnumToString<E_IrCutMode>(IRCUT_CLOSE);
	tmpIr[(unsigned int)2] = ConvertEnumToString<E_IrCutMode>(IRCUT_OPEN);

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
	ConfigCaptureFormats &cap_fmat = conf_cap.GetConfig(channel);

	web_resp["Antiflicker"] = ConvertEnumToString<E_AntiFlckerMode>(cap_fmat.AntiFlckerAttr.mode);
	web_resp["IrCut"] = ConvertEnumToString<E_IrCutMode>(cap_fmat.IrCutAttr.mode);
	web_resp["Horreverse"] = cap_fmat.ReverseAttr.bFilp == av_true ? 1 : 0;
	web_resp["Verreverse"] = cap_fmat.ReverseAttr.bMirror == av_true ? 1 : 0;
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
	ConfigCaptureFormats &cap_fmat = conf_cap.GetConfig(channel);
	cap_fmat.AntiFlckerAttr.mode = ConvertStringToEnum<E_AntiFlckerMode>(web_req["Antiflicker"].asCString());
	cap_fmat.IrCutAttr.mode = ConvertStringToEnum<E_IrCutMode>(web_req["IrCut"].asCString());
	cap_fmat.ReverseAttr.bFilp = web_req["Horreverse"].asInt() == 1 ? av_true : av_false;
	cap_fmat.ReverseAttr.bMirror = web_req["Verreverse"].asInt() == 1 ? av_true : av_false;
	have_resp_param = av_true;
	return (0 == conf_cap.SettingUp(channel)) ? WEB_STATUS_OK : WEB_STATUS_PARAM_ERROR;
}
