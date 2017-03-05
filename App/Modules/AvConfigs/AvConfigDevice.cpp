/*******************************************************************
|  Copyright(c) 2015-2016 Graceport Technology Development Co.,Ltd
|  All rights reserved.
|
|  版本: 1.0
|  作者: Xman [xman@graceport.cn]
|  日期: 2016年6月6日
|  说明:
|
|  版本: 1.1
|  作者:
|  日期:
|  说明:
******************************************************************/
#include "AvConfigs/AvConfigDevice.h"
#include "AvDevice/AvDevice.h"
template<> void ProcessValue<ConfigUart>(CAvConfigBase &av_conf, AvConfigValue &val, ConfigUart &conf, int index, int diff)
{
	C_SerialCaps SerialCaps;
	CAvDevice::GetSerialCaps(SerialCaps);
	if (index < SerialCaps.TotalSerials){
		av_conf.Process("Enable",	val, (int &)conf.Enable, (int)av_true, (int)av_false, (int)av_true);
		av_conf.Process("Type", val, (int &)conf.Type, (int)SerialCaps.Com_t[index], (int)COM_PTZ, (int)COM_SERIAL);
	}
	else{
		av_conf.Process("Enable", val, (int &)conf.Enable, (int)av_false, (int)av_false, (int)av_true);
		av_conf.Process("Type", val, (int &)conf.Type, (int)COM_PTZ, (int)COM_PTZ, (int)COM_SERIAL);
	}

	AvConfigValue &Attribute = val["Attribute"];
	av_conf.Process("Baudrate", Attribute, conf.Attribute.baudrate, 9600, 0, 230400);
	av_conf.Process("DataBits", Attribute, conf.Attribute.databits, 8, 2, 8);
	av_conf.Process("Parity", Attribute, conf.Attribute.parity, SerialParity_NOPARITY, SerialParity_NOPARITY, SerialParity_SPACE);
	av_conf.Process("StopBits", Attribute, conf.Attribute.stopbits, 1, 1, 2);
	av_conf.Process("Reserved", Attribute, conf.Attribute.reserved, 0, 0, 10);
}


template<> void ProcessValue<ConfigPtzCameraLensProfile>(CAvConfigBase &AvConfig, AvConfigValue &ConfigValue, ConfigPtzCameraLensProfile &PtzCameraLensProfile, int index, int diff)
{
	C_PtzAdvancedCameraLensCaps PtzCameraLensCaps;
	PtzCameraLensFocusCtrlMode FocusCtrlModeMax =  PtzCameraLensFocusCtrlMode_Last;
	PtzCameraLensFocusCtrlMode FocusCtrlModeMin = PtzCameraLensFocusCtrlMode_None;
	PtzCameraLensFocusRegion FocusRegionMax = PtzCameraLensFocusRegion_Last;
	PtzCameraLensFocusRegion FocusRegionMin = PtzCameraLensFocusRegion_None;

	PtzCameraLensFocusSearchMode FocusSearchModeMax = PtzCameraLensFocusSearchMode_Last;
	PtzCameraLensFocusSearchMode FocusSearchModeMin = PtzCameraLensFocusSearchMode_None;
	PtzCameraLensIrisCtrlMode IrisCtrlModeMax = PtzCameraLensIrisCtrlMode_Last;
	PtzCameraLensIrisCtrlMode IrisCtrlModeMin = PtzCameraLensIrisCtrlMode_None;


	AvPtzAdvancedCameraLensCaps(&PtzCameraLensCaps);

	av_findMaxMask(FocusCtrlModeMax, PtzCameraLensCaps.FocusCtrlModeMask, PtzCameraLensFocusCtrlMode);
	av_findMinMask(FocusCtrlModeMin, PtzCameraLensCaps.FocusCtrlModeMask, PtzCameraLensFocusCtrlMode);

	av_findMaxMask(FocusRegionMax, PtzCameraLensCaps.FocusRegionMask, PtzCameraLensFocusRegion);
	av_findMinMask(FocusRegionMin, PtzCameraLensCaps.FocusRegionMask, PtzCameraLensFocusRegion);

	av_findMaxMask(FocusSearchModeMax, PtzCameraLensCaps.FocusSearchModeMask, PtzCameraLensFocusSearchMode);
	av_findMinMask(FocusSearchModeMin, PtzCameraLensCaps.FocusSearchModeMask, PtzCameraLensFocusSearchMode);

	av_findMaxMask(IrisCtrlModeMax, PtzCameraLensCaps.IrisCtrlModeMask, PtzCameraLensIrisCtrlMode);
	av_findMinMask(IrisCtrlModeMin, PtzCameraLensCaps.IrisCtrlModeMask, PtzCameraLensIrisCtrlMode);

	AvConfig.Process("FocusCtrlMode", ConfigValue, (int &)PtzCameraLensProfile.FocusCtrlMode, (int )FocusCtrlModeMin, (int )FocusCtrlModeMin, (int )FocusCtrlModeMax);
	AvConfig.Process("FocusRegion", ConfigValue, (int &)PtzCameraLensProfile.FocusRegion, (int )FocusRegionMin, (int )FocusRegionMin, (int )FocusRegionMax);
	AvConfig.Process("FocusSearchMode", ConfigValue, (int &)PtzCameraLensProfile.FocusSearchMode, (int )FocusSearchModeMin, (int )FocusSearchModeMin, (int )FocusSearchModeMax);
	AvConfig.Process("IrisCtrlMode", ConfigValue, (int &)PtzCameraLensProfile.IrisCtrlMode, (int )IrisCtrlModeMin, (int )IrisCtrlModeMin, (int )IrisCtrlModeMax);
	AvConfig.Process("FocusSensitivity", ConfigValue, PtzCameraLensProfile.FocusSensitivity, PtzCameraLensCaps.FocusSensitivityMax/2, 1, PtzCameraLensCaps.FocusSensitivityMax);
	AvConfig.Process("ZoomSpeed", ConfigValue, PtzCameraLensProfile.ZoomSpeed, PtzCameraLensCaps.ZoomSpeedMax / 2, 1, PtzCameraLensCaps.ZoomSpeedMax);

	if (PtzCameraLensCaps.bSupportFigureZoom == av_true){
		AvConfig.Process("bFigureZoom", ConfigValue, (int &)PtzCameraLensProfile.bFigureZoom, (int)av_false, (int)av_false, (int)av_true);
	}
	else{
		AvConfig.Process("bFigureZoom", ConfigValue, (int &)PtzCameraLensProfile.bFigureZoom, (int)av_false, (int)av_false, (int)av_false);
	}
	
}