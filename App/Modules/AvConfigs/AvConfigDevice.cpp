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