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
template<> void ProcessValue<ConfigCoverFormats>(CAvConfigBase &ConfBase, AvConfigValue &ConfValue, ConfigCoverFormats &CoverFormats, int index, int diff)
{
	AvConfigValue &CoverTable = ConfValue["Formats"];
	
	C_EncodeCaps EncodeCaps;
	CAvDevice::GetEncodeCaps(index, EncodeCaps);
	for (int i = 0; i < ConfMaxCover; i++){
		ConfBase.Process("Index", CoverTable[i], CoverFormats.CHLFormats[i].Index, i, i, i);
		ConfBase.Process("Enable", CoverTable[i], (int &)CoverFormats.CHLFormats[i].Enable, (int)av_false, (int)av_false, (int)av_true);
		ConfBase.Process("Color", CoverTable[i], CoverFormats.CHLFormats[i].Color.U_RGBA, 0xff, 0x00, 0xffffffff);

		AvConfigValue &Rect = CoverTable[i]["Rect"];
		ConfBase.Process("Sx", Rect, CoverFormats.CHLFormats[i].Rect.Sx, 0, 0, 10000);
		ConfBase.Process("Sy", Rect, CoverFormats.CHLFormats[i].Rect.Sy, 0, 0, 10000);
		ConfBase.Process("Width", Rect, CoverFormats.CHLFormats[i].Rect.Width, 0, 0, 10000);
		ConfBase.Process("Heigh", Rect, CoverFormats.CHLFormats[i].Rect.Heigh, 0, 0, 10000);
	}
}

template<> void ProcessValue<ConfigWaterMarkingFormats>(CAvConfigBase &ConfBase, AvConfigValue &ConfValue, ConfigWaterMarkingFormats &WaterMarkingFormats, int index, int diff)
{
	AvConfigValue &WaterMarkingTable = ConfValue["Formats"];
	C_EncodeCaps EncodeCaps;
	CAvDevice::GetEncodeCaps(index, EncodeCaps);
	assert(EncodeCaps.MaxWaterMaring <= ConfMaxWaterMarking);

	for (int i = 0; i < ConfMaxWaterMarking; i++)
	{
		if (i == 0){
			ConfBase.Process("Type", WaterMarkingTable[i], (int &)WaterMarkingFormats.CHLFormats[i].Type, (int)WM_DATE_TIME, (int)WM_DATE_TIME, (int)WM_DATE_TIME);
			ConfBase.Process("Style", WaterMarkingTable[i], (int &)WaterMarkingFormats.CHLFormats[i].Style, (int)WM_YMD_HMS, (int)WM_YMD_HMS, (int)WM_HMS_YMD);
		}else{
			ConfBase.Process("Type", WaterMarkingTable[i], (int &)WaterMarkingFormats.CHLFormats[i].Type, (int)WM_CHANNEL_NAME, (int)WM_CHANNEL_NAME, (int)WM_CHANNEL_NAME);
			ConfBase.Process("Style", WaterMarkingTable[i], (int &)WaterMarkingFormats.CHLFormats[i].Style, (int)VM_STYLE_NR, (int)VM_STYLE_NR, (int)VM_STYLE_NR);

		}
		ConfBase.Process("Enable", WaterMarkingTable[i], (int &)WaterMarkingFormats.CHLFormats[i].Enable, (int)av_true, (int)av_false, (int)av_true);
		ConfBase.Process("Color", WaterMarkingTable[i], WaterMarkingFormats.CHLFormats[i].Color.U_RGBA, 0xff, 0x00, 0xffffffff);
		ConfBase.Process("String", WaterMarkingTable[i], WaterMarkingFormats.CHLFormats[i].WaterMarkString, "avWare");

		AvConfigValue &Rect = WaterMarkingTable[i]["Rect"];
		ConfBase.Process("Sx", Rect, WaterMarkingFormats.CHLFormats[i].Rect.Sx, 0, 0, 10000);
		ConfBase.Process("Sy", Rect, WaterMarkingFormats.CHLFormats[i].Rect.Sy, 0, 0, 10000);
		ConfBase.Process("Width", Rect, WaterMarkingFormats.CHLFormats[i].Rect.Width, 0, 0, 10000);
		ConfBase.Process("Heigh", Rect, WaterMarkingFormats.CHLFormats[i].Rect.Heigh, 0, 0, 10000);
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
}

template<> void ProcessValue<ConfigProtoFormats>(CAvConfigBase &ConfBase, AvConfigValue &ConfValue, ConfigProtoFormats &ProtocolFormats, int index, int diff)
{
	AvConfigValue &ProtoTable = ConfValue["Formats"];
	ConfBase.Process("IsEnable", ProtoTable, (int&)ProtocolFormats.IsEnable, (int)av_false, (int)av_false, (int)av_true);
	ConfBase.Process("DigitalChannel", ProtoTable, ProtocolFormats.DigitalChannel, index, index, index);
	ConfBase.Process("ShowWindowsId", ProtoTable, ProtocolFormats.ShowWindowsId, index, 0);
	ConfBase.Process("ProtoMode", ProtoTable, (int &)ProtocolFormats.ProtoMode, (int)ProtocolMoon, (int)ProtocolOnvif, (int)ProtocolMoon);
	ConfBase.Process("UsrName", ProtoTable, ProtocolFormats.UsrName, "");
	ConfBase.Process("Passwd", ProtoTable, ProtocolFormats.Passwd, "");

	switch (ProtocolFormats.ProtoMode)
	{
	case ProtocolOnvif:
		ConfBase.Process("Url", ProtoTable, ProtocolFormats.OnvifFormats.Url, "");
		break;
	case ProtocolRtmp:
		ConfBase.Process("MainUrl", ProtoTable, ProtocolFormats.RtmpFormats.MainUrl, "");
		ConfBase.Process("SubUrl", ProtoTable, ProtocolFormats.RtmpFormats.SubUrl, "");
		break;
	case ProtocolRtsp:
		ConfBase.Process("MainUrl", ProtoTable, ProtocolFormats.RtspFormats.MainUrl, "");
		ConfBase.Process("SubUrl", ProtoTable, ProtocolFormats.RtspFormats.SubUrl, "");
		break;
	case ProtocolMoon:
		ConfBase.Process("Url", ProtoTable, ProtocolFormats.MoonFormats.Url, "");
		ConfBase.Process("Port", ProtoTable, ProtocolFormats.MoonFormats.Port, 0, 0);
		break;
	default:
		break;
	}

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

	AudioSampleRate	SampleRateMax = AudioSampleRate_LAST;
	AudioSampleRate	SampleRateMin = AudioSampleRate_NONE;
	av_findMaxMask(SampleRateMax, AudioCaps.SampleRateMask[CompMax], AudioSampleRate);
	av_findMinMask(SampleRateMin, AudioCaps.SampleRateMask[CompMax], AudioSampleRate);

	AudioSampleBits	SampleBitsMax = AudioSampleBits_NONE;
	AudioSampleBits	SampleBitsMin = AudioSampleBits_LAST;
	av_findMaxMask(SampleBitsMax, AudioCaps.SampleBitsMask[CompMax], AudioSampleBits);
	av_findMinMask(SampleBitsMin, AudioCaps.SampleBitsMask[CompMax], AudioSampleBits);

	ConfBase.Process("Comp", AudioTable, (int &)AudioFormats.Comp, (int)CompMax, (int)CompMin, (int)CompMax);
	ConfBase.Process("Samplebits", AudioTable, (int &)AudioFormats.SampleBits, (int)SampleBitsMin, (int)SampleBitsMin, (int)SampleBitsMax);
	ConfBase.Process("SampleRate", AudioTable, (int &)AudioFormats.SampleRate, (int)SampleRateMin, (int)SampleRateMin, (int)SampleRateMax);
	ConfBase.Process("CaptureVolume", AudioTable, AudioFormats.VolumeCapture, AudioCaps.VolumeRang / 2, 0, AudioCaps.VolumeRang);
	ConfBase.Process("PlayVolume", AudioTable, AudioFormats.VolumePlay, AudioCaps.VolumeRang / 2, 0, AudioCaps.VolumeRang);
	ConfBase.Process("SoundMode", AudioTable, (int &)AudioFormats.SoundMode, (int)AudioSoundMode_MONO, (int)AudioSoundMode_MONO, (int)AudioSoundMode_STEREO);
	ConfBase.Process("CaptureDevice", AudioTable, (int &)AudioFormats.CaptureDevice, (int)AudioCaptureDevice_MIC_IN, (int)AudioCaptureDevice_LINE_IN, (int)AudioCaptureDevice_MIC_IN);
	ConfBase.Process("VoiceQualityMask", AudioTable, AudioFormats.VoiceQualityMask, 0, 0, 0xffffffff);
}

