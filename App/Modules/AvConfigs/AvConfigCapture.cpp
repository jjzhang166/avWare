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
#include "Apis/AvWareType.h"

template<> void ProcessValue<ConfigEncodeFormats>(CAvConfigBase &ConfBase, AvConfigValue &ConfValue, ConfigEncodeFormats &EncodeFormats, int index, int diff)
{
	C_EncodeCaps EncodeCaps;
	CAvDevice::GetCaptureCaps(index, EncodeCaps);
	AvConfigValue &StreamTable = ConfValue["Formats"];
	av_capture_size ImageSizeMax = CaptureSize_NR;
	av_capture_size ImageSizeMin = CaptureSize_Self;
	av_comp_t		CompMax = AvComp_NR;
	av_comp_t		CompMin = AvComp_H264;

	av_findMaxMask(CompMax, EncodeCaps.CompMask, av_comp_t);
	av_findMinMask(CompMin, EncodeCaps.CompMask, av_comp_t);

	for (int i = CHL_MAIN_T; i < CHL_NR_T; i++){
		ImageSizeMax = CaptureSize_NR;
		ImageSizeMin = CaptureSize_Self;
		AvConfigValue &VideoTable = StreamTable[i]["Video"];
		if (i == CHL_MAIN_T){
			av_findMaxMask(ImageSizeMax, EncodeCaps.ImageSizeMask, av_capture_size);
			av_findMinMask(ImageSizeMin, EncodeCaps.ImageSizeMask, av_capture_size);
			//assert(ImageSizeMin > ImageSizeMax || ImageSizeMax == 0 || ImageSizeMin == 0);
			ConfBase.Process("Enable", StreamTable[i], (int &)EncodeFormats.CHLFormats[i].Enable, (int)av_true, (int)av_false, (int)av_true);
			ConfBase.Process("ImageSize", VideoTable, (int &)EncodeFormats.CHLFormats[i].Formats.ImageSize, (int)ImageSizeMax, (int)ImageSizeMin, (int)ImageSizeMax);
			ConfBase.Process("Comp", VideoTable, (int &)EncodeFormats.CHLFormats[i].Formats.Comp, (int)CompMax, (int)CompMin, (int)CompMax);
			ConfBase.Process("BitRateValue", VideoTable, EncodeFormats.CHLFormats[i].Formats.BitRateValue, 4096, 512, 8192);
			ConfBase.Process("FrameRate", VideoTable, EncodeFormats.CHLFormats[i].Formats.FrameRate, 25, 5, EncodeCaps.InputFrameRate == 0 ? 30 : EncodeCaps.InputFrameRate);
		}
		else if (AvMask(i) & EncodeCaps.ExtChannel){
			av_capture_size ImageSizeMainMax;
			av_findMaxMask(ImageSizeMainMax, EncodeCaps.ImageSizeMask, av_capture_size);
			av_findMaxMask(ImageSizeMax, EncodeCaps.ExtImageSizeMask[ImageSizeMainMax], av_capture_size);
			av_findMinMask(ImageSizeMin, EncodeCaps.ExtImageSizeMask[ImageSizeMainMax], av_capture_size);
			//av_warning("index [%d] Slave [%d] ExtImageSizeMask [%x],,,ImageSizeMainMax [%d] ImageSizeMin[%d] ImageSizeMax [%d]\n", index, i, EncodeCaps.ExtImageSizeMask[ImageSizeMainMax],ImageSizeMainMax, ImageSizeMin, ImageSizeMax);
			//assert(ImageSizeMin < ImageSizeMax || ImageSizeMax == 0 || ImageSizeMin == 0);
			ConfBase.Process("Enable", StreamTable[i], (int &)EncodeFormats.CHLFormats[i].Enable, (int)av_true, (int)av_false, (int)av_true);
			ConfBase.Process("ImageSize", VideoTable, (int &)EncodeFormats.CHLFormats[i].Formats.ImageSize, (int)ImageSizeMax, (int)ImageSizeMin, (int)ImageSizeMax);

			if (i == CHL_JPEG_T){
				ConfBase.Process("Comp", VideoTable, (int &)EncodeFormats.CHLFormats[i].Formats.Comp, (int)AvComp_JPEG, (int)AvComp_JPEG, (int)AvComp_JPEG);
				ConfBase.Process("BitRateValue", VideoTable, EncodeFormats.CHLFormats[i].Formats.BitRateValue, 512, 512, 4096);
				ConfBase.Process("FrameRate", VideoTable, EncodeFormats.CHLFormats[i].Formats.FrameRate, 3, 1, 8);
			}
			else{
				ConfBase.Process("Comp", VideoTable, (int &)EncodeFormats.CHLFormats[i].Formats.Comp, (int)CompMax, (int)CompMin, (int)CompMax);
				ConfBase.Process("BitRateValue", VideoTable, EncodeFormats.CHLFormats[i].Formats.BitRateValue, 768, 64, 2048);
				ConfBase.Process("FrameRate", VideoTable, EncodeFormats.CHLFormats[i].Formats.FrameRate, 25, 5, EncodeCaps.InputFrameRate == 0 ? 30 : EncodeCaps.InputFrameRate);
			}
		}
		else{
			ConfBase.Process("Enable", StreamTable[i], (int &)EncodeFormats.CHLFormats[i].Enable, (int)av_false, (int)av_false, (int)av_false);
			ConfBase.Process("Comp", VideoTable, (int &)EncodeFormats.CHLFormats[i].Formats.Comp, (int)AvComp_NR, (int)AvComp_NR, (int)AvComp_NR);
			ConfBase.Process("ImageSize", VideoTable, (int &)EncodeFormats.CHLFormats[i].Formats.ImageSize, (int)DisplaySize_NR, (int)DisplaySize_NR, (int)DisplaySize_NR);
			ConfBase.Process("BitRateValue", VideoTable, EncodeFormats.CHLFormats[i].Formats.BitRateValue, 0, 0, 0);
			ConfBase.Process("FrameRate", VideoTable, EncodeFormats.CHLFormats[i].Formats.FrameRate, 0, 0, 0);
		}

		ConfBase.Process("BitRateCtrl", VideoTable, (int &)EncodeFormats.CHLFormats[i].Formats.BitRateCtrl, (int)AvBitRate_CBR, (int)AvBitRate_CBR, (int)AvBitRate_VBR);
		ConfBase.Process("Gop", VideoTable, EncodeFormats.CHLFormats[i].Formats.Gop, 30, 5, 120);
		ConfBase.Process("ImageSelfHeigh", VideoTable, EncodeFormats.CHLFormats[i].Formats.ImageSelfHeigh, 128, 128, 1080);
		ConfBase.Process("ImageSelfWidth", VideoTable, EncodeFormats.CHLFormats[i].Formats.ImageSelfWidth, 128, 128, 1920);
		ConfBase.Process("Qlevel", VideoTable, EncodeFormats.CHLFormats[i].Formats.Qlevel, 1, 0, 4);
	}

}
template<> void ProcessValue<ConfigCoverFormats>(CAvConfigBase &ConfBase, AvConfigValue &ConfValue, ConfigCoverFormats &CoverFormats, int index, int diff)
{
	AvConfigValue &CoverTable = ConfValue["Formats"];
	
	C_EncodeCaps EncodeCaps;
	CAvDevice::GetCaptureCaps(index, EncodeCaps);
	//assert(EncodeCaps.MaxCover <= ConfMaxCover);
	for (int i = 0; i < ConfMaxCover; i++){
		ConfBase.Process("Index", CoverTable[i], CoverFormats.CHLFormats[i].index, i, i, i);
		ConfBase.Process("Enable", CoverTable[i], (int &)CoverFormats.CHLFormats[i].enable, (int)av_false, (int)av_false, (int)av_true);
		ConfBase.Process("Color", CoverTable[i], CoverFormats.CHLFormats[i].color, 0xff, 0x00, 0xff);

		AvConfigValue &Rect = CoverTable[i]["Rect"];
		ConfBase.Process("Botton", Rect, CoverFormats.CHLFormats[i].rect.bottom, 0, 0, 10000);
		ConfBase.Process("Left", Rect, CoverFormats.CHLFormats[i].rect.left, 0, 0, 10000);
		ConfBase.Process("Right", Rect, CoverFormats.CHLFormats[i].rect.right, 0, 0, 10000);
		ConfBase.Process("Top", Rect, CoverFormats.CHLFormats[i].rect.top, 0, 0, 10000);
	}
}

template<> void ProcessValue<ConfigWaterMarkingFormats>(CAvConfigBase &ConfBase, AvConfigValue &ConfValue, ConfigWaterMarkingFormats &WaterMarkingFormats, int index, int diff)
{
	AvConfigValue &WaterMarkingTable = ConfValue["Formats"];
	C_EncodeCaps EncodeCaps;
	CAvDevice::GetCaptureCaps(index, EncodeCaps);
	assert(EncodeCaps.MaxWaterMaring <= ConfMaxWaterMarking);

	for (int i = 0; i < ConfMaxWaterMarking; i++)
	{
		ConfBase.Process("Type", WaterMarkingTable[i], (int &)WaterMarkingFormats.CHLFormats[i].type, (int)WM_YMD_HMS, (int)WM_YMD_HMS, (int)WM_HMS_YMD);
		ConfBase.Process("Enable", WaterMarkingTable[i], (int &)WaterMarkingFormats.CHLFormats[i].enable, (int)av_false, (int)av_false, (int)av_true);
		ConfBase.Process("Index", WaterMarkingTable[i], WaterMarkingFormats.CHLFormats[i].index, i, i, i);
		ConfBase.Process("Color", WaterMarkingTable[i], WaterMarkingFormats.CHLFormats[i].color, 0xff, 0x00, 0xff);
		ConfBase.Process("String", WaterMarkingTable[i], WaterMarkingFormats.CHLFormats[i].WaterMarkString, "");

		AvConfigValue &Rect = WaterMarkingTable[i]["Rect"];
		ConfBase.Process("Botton", Rect, WaterMarkingFormats.CHLFormats[i].rect.bottom, 0, 0, 10000);
		ConfBase.Process("Left", Rect, WaterMarkingFormats.CHLFormats[i].rect.left, 0, 0, 10000);
		ConfBase.Process("Right", Rect, WaterMarkingFormats.CHLFormats[i].rect.right, 0, 0, 10000);
		ConfBase.Process("Top", Rect, WaterMarkingFormats.CHLFormats[i].rect.top, 0, 0, 10000);
	}

}

template<> void ProcessValue<ConfigImageFormats>(CAvConfigBase &ConfBase, AvConfigValue &ConfValue, ConfigImageFormats &ImageFormats, int index, int diff)
{

	C_ImageQualityCaps ImageCaps;

	CAvDevice::GetImageCaps(index, ImageCaps);
	
	ConfBase.Process("Brightness", ConfValue, ImageFormats.Brightness, ImageCaps.MaxRange / 2, 0, ImageCaps.MaxRange);
	ConfBase.Process("Contrast", ConfValue, ImageFormats.Contrast, ImageCaps.MaxRange / 2, 0, ImageCaps.MaxRange);
	ConfBase.Process("Hue", ConfValue, ImageFormats.Hue, ImageCaps.MaxRange / 2, 0, ImageCaps.MaxRange);
	ConfBase.Process("Saturation", ConfValue, ImageFormats.Saturation, ImageCaps.MaxRange / 2, 0, ImageCaps.MaxRange);

}
template<> void ProcessValue<ConfigCaptureFormats>(CAvConfigBase &ConfBase, AvConfigValue &ConfValue, ConfigCaptureFormats &CaptureFormats, int index, int diff)
{
	C_CaptureInCaps CaptureInCaps;
	CAvDevice::GetCaputreInCaps(index, CaptureInCaps);


	{
	E_IrCutMode		IrCutMaxMode;
	E_IrCutMode		IrCutMinMode;
		AvConfigValue &IRCUTTable = ConfValue["Ircut"];
	av_findMaxMask(IrCutMaxMode, CaptureInCaps.IrCutModeMask, E_IrCutMode);
	av_findMinMask(IrCutMinMode, CaptureInCaps.IrCutModeMask, E_IrCutMode);
		ConfBase.Process("Mode",	IRCUTTable, (int&)CaptureFormats.IrCutAttr.mode, (int)IrCutMaxMode, (int)IrCutMinMode, (int)IrCutMaxMode);
		ConfBase.Process("TimerS", IRCUTTable, CaptureFormats.IrCutAttr.tOpen, 6 * 3600, 0, 24 * 3600 - 1);
		ConfBase.Process("TimerE", IRCUTTable, CaptureFormats.IrCutAttr.tClose, 18 * 3600, 0, 24 * 3600 - 1);
	}

	{
		AvConfigValue &ReverseTable = ConfValue["Reverse"];
	if (AvMask(VIDEOREVERSE_HOR) & CaptureInCaps.VideoReverseMask){
			ConfBase.Process("ReverseHor", ReverseTable, (int &)CaptureFormats.ReverseAttr.bMirror, (int)av_false, (int)av_false, (int)av_true);
	}
	else{
			ConfBase.Process("ReverseHor", ReverseTable, (int &)CaptureFormats.ReverseAttr.bMirror, (int)av_false, (int)av_false, (int)av_false);
	}
	if (AvMask(VIDEOREVERSE_VER) & CaptureInCaps.VideoReverseMask){
			ConfBase.Process("ReverseVer", ReverseTable, (int &)CaptureFormats.ReverseAttr.bFilp, (int)av_false, (int)av_false, (int)av_true);
	}
	else{
			ConfBase.Process("ReverseVer", ReverseTable, (int &)CaptureFormats.ReverseAttr.bFilp, (int)av_false, (int)av_false, (int)av_false);
	}
	}

	{
		E_VideoRotate		RotateMaxMode;
		E_VideoRotate		RotateMinMode;
		AvConfigValue &RotateTable = ConfValue["Rotate"];
		av_findMaxMask(RotateMaxMode, CaptureInCaps.IrCutModeMask, E_VideoRotate);
		av_findMinMask(RotateMinMode, CaptureInCaps.IrCutModeMask, E_VideoRotate);

		ConfBase.Process("mode", RotateTable, (int &)CaptureFormats.RotateAttr.mode, (int)VIDEOROTATE_0, (int)RotateMinMode, (int)RotateMaxMode);
	}

	{
		AvConfigValue &AntiFlckerTable = ConfValue["AntiFlcker"];
		ConfBase.Process("mode", AntiFlckerTable, (int &)CaptureFormats.AntiFlckerAttr.mode, (int)AvAntiFlckerMode_AUTO_50HZ, (int)AvAntiFlckerMode_INDOOR_50HZ, (int)AvAntiFlckerMode_ANTI_FLICKER_CLOSED);
	}
	{
		AvConfigValue &WhiteBalanceTable = ConfValue["WhiteBalance"];
		ConfBase.Process("mode", WhiteBalanceTable, (int &)CaptureFormats.WhiteBalanceAttr.mode, (int)AvWhiteBalanceMode_AUTO, (int)AvWhiteBalanceMode_OFF, (int)AvWhiteBalanceMode_MANUAL);
		ConfBase.Process("Value", WhiteBalanceTable, (int &)CaptureFormats.WhiteBalanceAttr.value, 4, 0, 20000);
	}

	{
		AvConfigValue &ExposureTable = ConfValue["Exposure"];
		ConfBase.Process("mode", ExposureTable, (int &)CaptureFormats.ExposureAttr.mode, (int)AvExposureMode_AUTO, (int)AvExposureMode_AUTO, (int)AvExposureMode_TRAFFIC);
		ConfBase.Process("Max", ExposureTable, (int &)CaptureFormats.ExposureAttr.max, 1, 0, 100000);
		ConfBase.Process("Min", ExposureTable, (int &)CaptureFormats.ExposureAttr.min, 1000, 0, 10000);
	}
	
	


}
#if 0

union{
	C_ProtoOnvifFormats OnvifFormats;
	C_ProtoMoonFormats  MoonFormats;
	C_ProtoRtmpFormats  RtmpFormats;
	C_ProtoRtspFormats	RtspFormats;
};
#endif
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
	CAvDevice::GetACaptureCaps((E_AUDIO_CHL)index, AudioCaps);
	av_comp_t		CompMax = AvComp_NR;
	av_comp_t		CompMin = AvComp_MJPEG;
	av_findMaxMask(CompMax, AudioCaps.CompMask, av_comp_t);
	av_findMinMask(CompMin, AudioCaps.CompMask, av_comp_t);

	E_SampleRate	SampleRateMax = ASampleRate_2822400;
	E_SampleRate	SampleRateMin = ASampleRate_8000;
	av_findMaxMask(SampleRateMax, AudioCaps.SampleRateMask[CompMax], E_SampleRate);
	av_findMinMask(SampleRateMin, AudioCaps.SampleRateMask[CompMax], E_SampleRate);

	E_SampleBits	SampleBitsMax = ASampleBits_24;
	E_SampleBits	SampleBitsMin = ASampleBits_8;
	av_findMaxMask(SampleBitsMax, AudioCaps.SampleBitsMask[CompMax], E_SampleBits);
	av_findMinMask(SampleBitsMin, AudioCaps.SampleBitsMask[CompMax], E_SampleBits);

	ConfBase.Process("nMaxChannels", AudioTable, AudioFormats.Channels, 0 == AudioCaps.nMaxChannels ? 0 : 1, 0 == AudioCaps.nMaxChannels ? 0 : 1, AudioCaps.nMaxChannels);
	ConfBase.Process("Comp", AudioTable, (int &)AudioFormats.Comp, (int)CompMax, (int)CompMin, (int)CompMax);
	ConfBase.Process("Samplebits", AudioTable, (int &)AudioFormats.Samplebits, (int)SampleBitsMin, (int)SampleBitsMin, (int)SampleBitsMax);
	ConfBase.Process("SampleRate", AudioTable, (int &)AudioFormats.SampleRate, (int)SampleRateMin, (int)SampleRateMin, (int)SampleRateMax);
	ConfBase.Process("Volume", AudioTable, AudioFormats.Volume, 50, 0, 99);
}

