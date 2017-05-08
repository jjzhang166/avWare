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
#include "AvConfigs/AvConfigCapture.h"
#include "AvDevice/AvDevice.h"
#include "Apis/AvWareCplusplus.h"

template<> void ProcessValue<ConfigEncodeProfile>(CAvConfigBase &ConfBase, AvConfigValue &ConfValue, ConfigEncodeProfile &EncodeProfile, int index, int diff)
{
	C_EncodeCaps EncodeCaps;
	CAvDevice::GetEncodeCaps(index, EncodeCaps);
	AvConfigValue &StreamTable = ConfValue["Formats"];
	CaptureSize ImageSizeMax = CaptureSize_LAST;
	CaptureSize ImageSizeMin = CaptureSize_Self;
	AvComp		CompMax = AvComp_LAST;
	AvComp		CompMin = AvComp_H264;

	av_findMaxMask(CompMax, EncodeCaps.CompMask, AvComp);
	av_findMinMask(CompMin, EncodeCaps.CompMask, AvComp);

	for (int i = CHL_MAIN_T; i < CHL_NR_T; i++){
		ImageSizeMax = CaptureSize_LAST;
		ImageSizeMin = CaptureSize_Self;
		AvConfigValue &VideoTable = StreamTable[i]["Video"];
		/*
		//print for test
		av_warning("i[%d] Enable[%d] ImageSize[%d] FrameRate[%d]\n", i,
			StreamTable[i]["Enable"].asInt(),
			VideoTable["ImageSize"].asInt(),
			VideoTable["FrameRate"].asInt());
		*/
		if (i == CHL_MAIN_T){
			av_findMaxMask(ImageSizeMax, EncodeCaps.ImageSizeMask, CaptureSize);
			av_findMinMask(ImageSizeMin, EncodeCaps.ImageSizeMask, CaptureSize);
			//assert(ImageSizeMin > ImageSizeMax || ImageSizeMax == 0 || ImageSizeMin == 0);
			ConfBase.Process("Enable", StreamTable[i], (int &)EncodeProfile.CHLProfile[i].Enable, (int)av_true, (int)av_false, (int)av_true);
			ConfBase.Process("ImageSize", VideoTable, (int &)EncodeProfile.CHLProfile[i].Profile.ImageSize, (int)ImageSizeMax, (int)ImageSizeMin, (int)ImageSizeMax);
			ConfBase.Process("Comp", VideoTable, (int &)EncodeProfile.CHLProfile[i].Profile.Comp, (int)CompMax, (int)CompMin, (int)CompMax);
			ConfBase.Process("BitRateValue", VideoTable, EncodeProfile.CHLProfile[i].Profile.BitRateValue, 4096, 512, 8192);
			ConfBase.Process("FrameRate", VideoTable, EncodeProfile.CHLProfile[i].Profile.FrameRate, 25, 1, 120);
		}
		else if (AvMask(i) & EncodeCaps.ExtChannelMask){
			CaptureSize ImageSizeMainMax;
			av_findMaxMask(ImageSizeMainMax, EncodeCaps.ImageSizeMask, CaptureSize);
			av_findMaxMask(ImageSizeMax, EncodeCaps.ExtImageSizeMask[ImageSizeMainMax], CaptureSize);
			av_findMinMask(ImageSizeMin, EncodeCaps.ExtImageSizeMask[ImageSizeMainMax], CaptureSize);
			//av_warning("index [%d] Slave [%d] ExtImageSizeMask [%x],,,ImageSizeMainMax [%d] ImageSizeMin[%d] ImageSizeMax [%d]\n", index, i, EncodeCaps.ExtImageSizeMask[ImageSizeMainMax],ImageSizeMainMax, ImageSizeMin, ImageSizeMax);
			//assert(ImageSizeMin < ImageSizeMax || ImageSizeMax == 0 || ImageSizeMin == 0);
			ConfBase.Process("Enable", StreamTable[i], (int &)EncodeProfile.CHLProfile[i].Enable, (int)av_true, (int)av_false, (int)av_true);
			ConfBase.Process("ImageSize", VideoTable, (int &)EncodeProfile.CHLProfile[i].Profile.ImageSize, (int)ImageSizeMax, (int)ImageSizeMin, (int)ImageSizeMax);
			if (av_true == EncodeProfile.CHLProfile[i].Enable) {
				if (i == CHL_JPEG_T){
				ConfBase.Process("Comp", VideoTable, (int &)EncodeProfile.CHLProfile[i].Profile.Comp, (int)AvComp_JPEG, (int)AvComp_JPEG, (int)AvComp_JPEG);
				ConfBase.Process("BitRateValue", VideoTable, EncodeProfile.CHLProfile[i].Profile.BitRateValue, 512, 512, 4096);
				ConfBase.Process("FrameRate", VideoTable, EncodeProfile.CHLProfile[i].Profile.FrameRate, 3, 1, 8);
				}
				else{
					ConfBase.Process("Comp", VideoTable, (int &)EncodeProfile.CHLProfile[i].Profile.Comp, (int)CompMax, (int)CompMin, (int)CompMax);
					ConfBase.Process("BitRateValue", VideoTable, EncodeProfile.CHLProfile[i].Profile.BitRateValue, 768, 64, 2048);
					ConfBase.Process("FrameRate", VideoTable, EncodeProfile.CHLProfile[i].Profile.FrameRate, 25, 1, 120);
				}
			} else {
					ConfBase.Process("Comp", VideoTable, (int &)EncodeProfile.CHLProfile[i].Profile.Comp, (int)AvComp_LAST, (int)AvComp_LAST, (int)AvComp_LAST);
					ConfBase.Process("BitRateValue", VideoTable, EncodeProfile.CHLProfile[i].Profile.BitRateValue, 0, 0, 2048);
					ConfBase.Process("FrameRate", VideoTable, EncodeProfile.CHLProfile[i].Profile.FrameRate, 0, 0, 120);
			}
			
		}
		else{
			ConfBase.Process("Enable", StreamTable[i], (int &)EncodeProfile.CHLProfile[i].Enable, (int)av_false, (int)av_false, (int)av_false);
			ConfBase.Process("Comp", VideoTable, (int &)EncodeProfile.CHLProfile[i].Profile.Comp, (int)AvComp_LAST, (int)AvComp_LAST, (int)AvComp_LAST);
			ConfBase.Process("ImageSize", VideoTable, (int &)EncodeProfile.CHLProfile[i].Profile.ImageSize, (int)CaptureSize_LAST, (int)CaptureSize_LAST, (int)CaptureSize_LAST);
			ConfBase.Process("BitRateValue", VideoTable, EncodeProfile.CHLProfile[i].Profile.BitRateValue, 0, 0, 0);
			ConfBase.Process("FrameRate", VideoTable, EncodeProfile.CHLProfile[i].Profile.FrameRate, 0, 0, 0);
		}

		ConfBase.Process("BitRateCtrl", VideoTable, (int &)EncodeProfile.CHLProfile[i].Profile.BitRateCtl, (int)BitRateCtrl_CBR, (int)BitRateCtrl_CBR, (int)BitRateCtrl_VCBR);
		ConfBase.Process("Gop", VideoTable, EncodeProfile.CHLProfile[i].Profile.Gop, 30, 5, 120);
		ConfBase.Process("ImageSelfHeigh", VideoTable, EncodeProfile.CHLProfile[i].Profile.ImageSelfHeigh, 128, 128, 1080);
		ConfBase.Process("ImageSelfWidth", VideoTable, EncodeProfile.CHLProfile[i].Profile.ImageSelfWidth, 128, 128, 1920);
		ConfBase.Process("Qlevel", VideoTable, EncodeProfile.CHLProfile[i].Profile.Qlevel, 1, 0, 4);
	}

}
template<> void ProcessValue<ConfigCoverProfile>(CAvConfigBase &ConfBase, AvConfigValue &ConfValue, ConfigCoverProfile &CoverFormats, int index, int diff)
{
	AvConfigValue &CoverTable = ConfValue["Formats"];


	ConfBase.Process("CoverBgRGB", ConfValue, CoverFormats.CoverBgRGB, 0, 0, 0xffffffff);
	ConfBase.Process("CoverbEnMask", ConfValue, CoverFormats.CoverbEnMask, 0, 0, AvMask(ConfMaxCover + 1) - 1);

	for (int i = 0; i < ConfMaxCover; i++){
		AvConfigValue &RectTable = CoverTable[i]["Rect"];
		ConfBase.Process("Sx", RectTable, CoverFormats.CoverZone[i].Sx, 0, 0, AV_WARE_COVER_RECT_WIDTH);
		ConfBase.Process("Sy", RectTable, CoverFormats.CoverZone[i].Sy, 0, 0, AV_WARE_COVER_RECT_HEITH);
		ConfBase.Process("Width", RectTable, CoverFormats.CoverZone[i].Width, 0, 0, AV_WARE_COVER_RECT_WIDTH);
		ConfBase.Process("Heigh", RectTable, CoverFormats.CoverZone[i].Heigh, 0, 0, AV_WARE_COVER_RECT_HEITH);
	}



}

template<> void ProcessValue<ConfigOverLayProfile>(CAvConfigBase &ConfBase, AvConfigValue &ConfValue, ConfigOverLayProfile &OverLayProfile, int index, int diff)
{
	AvConfigValue &OverLayTable = ConfValue["Formats"];

	for (int i = 0; i < OverLayType_Last; i++)
	{

		ConfBase.Process("Index", OverLayTable[i], (int &)OverLayProfile.OverlayProfile[i].Index, i, i, i);
		ConfBase.Process("Size", OverLayTable[i], (int &)OverLayProfile.OverlayProfile[i].Fontsize, FontSize__24, FontSize_None/*for title*/, FontSize__128);
		ConfBase.Process("BgRgba", OverLayTable[i], OverLayProfile.OverlayProfile[i].BgRgba, 0, 0, 0xffffffff);
		ConfBase.Process("FgRgba", OverLayTable[i], OverLayProfile.OverlayProfile[i].FgRgba, 0, 0, 0xffffffff);
		ConfBase.Process("Split", OverLayTable[i], (int &)OverLayProfile.OverlayProfile[i].Split, OverLayDateTimeSplit_Split_Symbol, OverLayDateTimeSplit_None/*for title*/, OverLayDateTimeSplit_Last);
		ConfBase.Process("Style", OverLayTable[i], (int &)OverLayProfile.OverlayProfile[i].Style, OverLayDateTimeStyle_Y_M_D_H_M_S, OverLayDateTimeStyle_None/*for title*/, OverLayDateTimeStyle_Last);
		ConfBase.Process("TextChannel", OverLayTable[i], OverLayProfile.OverlayProfile[i].TextChannel, "avWare");
		ConfBase.Process("TextExt1", OverLayTable[i], OverLayProfile.OverlayProfile[i].TextExt1, "");
		ConfBase.Process("TextExt2", OverLayTable[i], OverLayProfile.OverlayProfile[i].TextExt2, "");
		ConfBase.Process("TextExt3", OverLayTable[i], OverLayProfile.OverlayProfile[i].TextExt3, "");

		if (i == OverLayType_Title || i == OverLayType_Time){
			ConfBase.Process("bOpen", OverLayTable[i], (int &)OverLayProfile.OverlayProfile[i].bOpen, av_true, av_false, av_true);
		}
		else{
			ConfBase.Process("bOpen", OverLayTable[i], (int &)OverLayProfile.OverlayProfile[i].bOpen, av_false, av_false, av_true);
		}

		AvConfigValue &OverLayRect = OverLayTable[i]["Rect"];
		ConfBase.Process("Sx", OverLayRect, OverLayProfile.OverlayProfile[i].Rect.Sx, 0, 0, AV_WARE_COVER_RECT_WIDTH);
		ConfBase.Process("Sy", OverLayRect, OverLayProfile.OverlayProfile[i].Rect.Sy, 0, 0, AV_WARE_COVER_RECT_HEITH);
		ConfBase.Process("Width", OverLayRect, OverLayProfile.OverlayProfile[i].Rect.Width, 0, 0, AV_WARE_COVER_RECT_WIDTH);
		ConfBase.Process("Heigh", OverLayRect, OverLayProfile.OverlayProfile[i].Rect.Heigh, 0, 0, AV_WARE_COVER_RECT_HEITH);
	}

}

template<> void ProcessValue<ConfigImageProfile>(CAvConfigBase &ConfBase, AvConfigValue &ConfValue, ConfigImageProfile &ImageProfile, int index, int diff)
{

	C_ImageCaps ImageCaps;

	CAvDevice::GetImageCaps(index, ImageCaps);
	
	ConfBase.Process("Brightness", ConfValue, ImageProfile.Brightness, ImageCaps.BrightnessRang / 2, 0, ImageCaps.BrightnessRang);
	ConfBase.Process("Contrast", ConfValue, ImageProfile.Contrast, ImageCaps.ContrastRang / 2, 0, ImageCaps.ContrastRang);
	ConfBase.Process("Hue", ConfValue, ImageProfile.Hue, ImageCaps.HueRang / 2, 0, ImageCaps.HueRang);
	ConfBase.Process("Saturation", ConfValue, ImageProfile.Saturation, ImageCaps.SaturationRang / 2, 0, ImageCaps.SaturationRang);

}
template<> void ProcessValue<ConfigCaptureProfile>(CAvConfigBase &ConfBase, AvConfigValue &ConfValue, ConfigCaptureProfile &CaptureProfile, int index, int diff)
{

	C_CaptureCaps CaptureCaps;
	CAvDevice::GetCaputreCaps(index, CaptureCaps);


	{
		IrCutMode		IrCutMaxMode;
		IrCutMode		IrCutMinMode;
		AvConfigValue &IRCUTTable = ConfValue["Ircut"];
		av_findMaxMask(IrCutMaxMode, CaptureCaps.IrCutMask, IrCutMode);
		av_findMinMask(IrCutMinMode, CaptureCaps.IrCutMask, IrCutMode);
		ConfBase.Process("Mode",	IRCUTTable, (int&)CaptureProfile.IrCut, (int)IrCutMaxMode, (int)IrCutMinMode, (int)IrCutMaxMode);
		ConfBase.Process("TimerS", IRCUTTable, CaptureProfile.IrCutTimer.Start, 6 * 3600, 0, 24 * 3600 - 1);
		ConfBase.Process("TimerE", IRCUTTable, CaptureProfile.IrCutTimer.End, 18 * 3600, 0, 24 * 3600 - 1);
	}

	{
		AvConfigValue &MrrorRotateTable = ConfValue["MrrorRotate"];
		ConfBase.Process("MrrorRotate", MrrorRotateTable, CaptureProfile.MirrorMaskValue, 0, 0, 0xffffffff);
	}


	{
		AvConfigValue &AntiFlckerTable = ConfValue["AntiFlcker"];
		ConfBase.Process("mode", AntiFlckerTable, (int &)CaptureProfile.AntiFlcker, (int)AntiFlckerMode_AUTO_50HZ, (int)AntiFlckerMode_INDOOR_50HZ, (int)AntiFlckerMode_ANTI_FLICKER_CLOSED);
		ConfBase.Process("Value", AntiFlckerTable, CaptureProfile.AntiFlckerValue, (CaptureCaps.AntiFlckerValueRegion.End
			- CaptureCaps.AntiFlckerValueRegion.Start) / 2, CaptureCaps.AntiFlckerValueRegion.Start, CaptureCaps.AntiFlckerValueRegion.End);
	}
	{
		AvConfigValue &WhiteBalanceTable = ConfValue["WhiteBalance"];
		ConfBase.Process("mode", WhiteBalanceTable, (int &)CaptureProfile.WhiteBalance, (int)WhiteBalanceMode_AUTO, (int)WhiteBalanceMode_OFF, (int)WhiteBalanceMode_MANUAL);
		ConfBase.Process("Value", WhiteBalanceTable, CaptureProfile.WhiteBalanceValue, (CaptureCaps.WhiteBalanceValueRegion.End
			- CaptureCaps.WhiteBalanceValueRegion.Start) / 2, CaptureCaps.WhiteBalanceValueRegion.Start, CaptureCaps.WhiteBalanceValueRegion.End);
	}
	
	{
		AvConfigValue &ExposureTable = ConfValue["Exposure"];
		ConfBase.Process("mode", ExposureTable, (int &)CaptureProfile.Exposure, (int)ExposureMode_AUTO, (int)ExposureMode_AUTO, (int)ExposureMode_TRAFFIC);
		ConfBase.Process("Value", ExposureTable, CaptureProfile.ExposureValue, (CaptureCaps.ExposureValueRegion.End
			- CaptureCaps.ExposureValueRegion.Start) / 2, CaptureCaps.ExposureValueRegion.Start, CaptureCaps.ExposureValueRegion.End);

	}

	{
		AvConfigValue &WdrTable = ConfValue["Wdr"];
		ConfBase.Process("mode", WdrTable, (int &)CaptureProfile.Wdr, (int)WdrMode_Open, (int)WdrMode_None, (int)WdrMode_Last);
		ConfBase.Process("Value", WdrTable, CaptureProfile.WdrValue, 0/*0 is not suport valude only open close*/, CaptureCaps.WdrValueRegion.End);

	}

	{
		AvConfigValue &ShutterTable = ConfValue["Shutter"];
		ConfBase.Process("mode", ShutterTable, (int &)CaptureProfile.Shutter, (int)ShutterMode_Auto, (int)ShutterMode_None, (int)ShutterMode_Last);
		ConfBase.Process("Value", ShutterTable, (int &)CaptureProfile.ShutterMax, CaptureCaps.ShutterMaxRegion.Start, CaptureCaps.ShutterMaxRegion.End);
	}

}


template<> void ProcessValue<ConfigProtoFormats>(CAvConfigBase &ConfBase, AvConfigValue &ConfValue, ConfigProtoFormats &ProtocolFormats, int index, int diff)
{
	AvConfigValue &ProtoTable = ConfValue["Formats"];
	ConfBase.Process("IsEnable", ProtoTable, (int&)ProtocolFormats.IsEnable, (int)av_false, (int)av_false, (int)av_true);
	ConfBase.Process("CheckAliveAddress", ProtoTable, ProtocolFormats.CheckAliveAddress, "");
	ConfBase.Process("ProtoMode", ProtoTable, (int &)ProtocolFormats.ProtoMode, (int)ProtocolMoon, (int)ProtocolOnvif, (int)ProtocolMoon);
	ConfBase.Process("UsrName", ProtoTable, ProtocolFormats.UsrName, "");
	ConfBase.Process("Passwd", ProtoTable, ProtocolFormats.Passwd, "");

	{

		ConfBase.Process("OnvifUrl", ProtoTable, ProtocolFormats.OnvifFormats.Url, "");
		ConfBase.Process("RtmpMainUrl", ProtoTable, ProtocolFormats.RtmpFormats.MainUrl, "");
		ConfBase.Process("RtmpSubUrl", ProtoTable, ProtocolFormats.RtmpFormats.SubUrl, "");
		ConfBase.Process("RtspMainUrl", ProtoTable, ProtocolFormats.RtspFormats.MainUrl, "");
		ConfBase.Process("RtspSubUrl", ProtoTable, ProtocolFormats.RtspFormats.SubUrl, "");
		ConfBase.Process("MoonUrl", ProtoTable, ProtocolFormats.MoonFormats.Url, "");
		ConfBase.Process("MoonPort", ProtoTable, ProtocolFormats.MoonFormats.Port, 5000);
	}
// 	switch (ProtocolFormats.ProtoMode)
// 	{
// 	case ProtocolOnvif:
// 		ConfBase.Process("Url", ProtoTable, ProtocolFormats.OnvifFormats.Url, "");
// 		break;
// 	case ProtocolRtmp:
// 		ConfBase.Process("MainUrl", ProtoTable, ProtocolFormats.RtmpFormats.MainUrl, "");
// 		ConfBase.Process("SubUrl", ProtoTable, ProtocolFormats.RtmpFormats.SubUrl, "");
// 		break;
// 	case ProtocolRtsp:
// 		ConfBase.Process("MainUrl", ProtoTable, ProtocolFormats.RtspFormats.MainUrl, "");
// 		ConfBase.Process("SubUrl", ProtoTable, ProtocolFormats.RtspFormats.SubUrl, "");
// 		break;
// 	case ProtocolMoon:
// 		ConfBase.Process("Url", ProtoTable, ProtocolFormats.MoonFormats.Url, "");
// 		ConfBase.Process("Port", ProtoTable, ProtocolFormats.MoonFormats.Port, 0, 0);
// 		break;
// 	default:
// 		break;
// 	}

}
template<> void ProcessValue<ConfigAudioFormats>(CAvConfigBase &ConfBase, AvConfigValue &ConfValue, ConfigAudioFormats &AudioFormats, int index, int diff)
{
	AvConfigValue &AudioTable = ConfValue["Formats"];
	C_AudioCaps AudioCaps;
	CAvDevice::GetACaptureCaps(CHL_ACAP_T, AudioCaps);
	AvComp		CompMax = AvComp_LAST;
	AvComp		CompMin = AvComp_JPEG;
	av_findMaxMask(CompMax, AudioCaps.CompMask, AvComp);
	av_findMinMask(CompMin, AudioCaps.CompMask, AvComp);

	AvComp DefaultComp = CompMax;
	AudioSampleBits DefaultSampleBits;
	AudioSampleRate DefaultSampleRate;

	av_findMinMask(DefaultSampleBits, AudioCaps.SampleBitsMask[DefaultComp], AudioSampleBits);
	av_findMinMask(DefaultSampleRate, AudioCaps.SampleRateMask[DefaultComp], AudioSampleRate);

	ConfBase.Process("Comp", AudioTable, (int &)AudioFormats.Comp, (int)CompMax, (int)CompMin, (int)CompMax);
	ConfBase.Process("Samplebits", AudioTable, (int &)AudioFormats.SampleBits, (int)DefaultSampleBits, (int)AudioSampleBits_B8, (int)AudioSampleBits_B32);
	ConfBase.Process("SampleRate", AudioTable, (int &)AudioFormats.SampleRate, (int)DefaultSampleRate, (int)AudioSampleRate_R8000, (int)AudioSampleRate_R192000);
	ConfBase.Process("CaptureVolume", AudioTable, AudioFormats.VolumeCapture, AudioCaps.VolumeRang / 2, 0, AudioCaps.VolumeRang);
	ConfBase.Process("PlayVolume", AudioTable, AudioFormats.VolumePlay, AudioCaps.VolumeRang / 2, 0, AudioCaps.VolumeRang);
	ConfBase.Process("SoundMode", AudioTable, (int &)AudioFormats.SoundMode, (int)AudioSoundMode_MONO, (int)AudioSoundMode_MONO, (int)AudioSoundMode_STEREO);
	ConfBase.Process("CaptureDevice", AudioTable, (int &)AudioFormats.CaptureDevice, (int)AudioCaptureDevice_MIC_IN, (int)AudioCaptureDevice_LINE_IN, (int)AudioCaptureDevice_MIC_IN);
	ConfBase.Process("VoiceQualityMask", AudioTable, AudioFormats.VoiceQualityMask, 0, 0, 0xffffffff);
}

