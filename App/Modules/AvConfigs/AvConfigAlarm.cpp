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
#include "AvConfigs/AvConfigAlarm.h"
#include "AvDevice/AvDevice.h"

template<> void ProcessValue<ConfigIoAlarmFormats>(CAvConfigBase &ConfBase, AvConfigValue &ConfValue, ConfigIoAlarmFormats &IoAlarmFormats, int index, int diff)
{
	ConfBase.Process("Enable",			ConfValue, (int &)IoAlarmFormats.Enable, (int)av_true, (int)av_false, (int)av_true);
	ConfBase.Process("EnableAlmOut",	ConfValue, (int &)IoAlarmFormats.EnableAlmOut, (int)av_true, (int)av_false, (int)av_true);
	ConfBase.Process("SendEmail",		ConfValue, (int &)IoAlarmFormats.SendEmail, (int)av_true, (int)av_false, (int)av_true);
	ConfBase.Process("PtzAction",		ConfValue, (int &)IoAlarmFormats.PtzAction, (int)av_true, (int)av_false, (int)av_true);
	ConfBase.Process("AlmOutTimes",		ConfValue, (int &)IoAlarmFormats.AlmOutTimes, (int)15, (int)0, (int)3600);
	AvConfigValue &WeekSpanTable = ConfValue["WeekSpan"];
	for (int i = 0; i < ConfMaxWeekDaySpan; i++){
		ConfBase.Process("Enable", WeekSpanTable[i], (int &)IoAlarmFormats.WeekSpan.DaySpan[i].DayEnable, (int)av_true, (int)av_false, (int)av_true);
		AvConfigValue &DaySpanTable = WeekSpanTable[i]["DaySpan"];
		for (int j = 0; j < ConfMaxDayTimeSpan; j++){
			ConfBase.Process("Enable", DaySpanTable[j], (int &)IoAlarmFormats.WeekSpan.DaySpan[i].TimeSpan[j].TimeEnable, (int)av_true, (int)av_false, (int)av_true);
			ConfBase.Process("Sec_S", DaySpanTable[j], IoAlarmFormats.WeekSpan.DaySpan[i].TimeSpan[j].StartSec, 0, 0, 24 * 3600 - 1);
			ConfBase.Process("Sec_E", DaySpanTable[j], IoAlarmFormats.WeekSpan.DaySpan[i].TimeSpan[j].EndSec, 24 * 3600 - 1, 0, 24 * 3600 - 1);
		}
	}

}
template<> void ProcessValue<ConfigMdAlarmFormats>(CAvConfigBase &ConfBase, AvConfigValue &ConfValue, ConfigMdAlarmFormats &MdAlarmFormats, int index, int diff)
{
	ConfBase.Process("Enable", ConfValue, (int &)MdAlarmFormats.Enable, (int)av_true, (int)av_false, (int)av_true);
	ConfBase.Process("EnableAlmOut", ConfValue, (int &)MdAlarmFormats.EnableAlmOut, (int)av_true, (int)av_false, (int)av_true);
	ConfBase.Process("SendEmail", ConfValue, (int &)MdAlarmFormats.SendEmail, (int)av_true, (int)av_false, (int)av_true);
	ConfBase.Process("PtzAction", ConfValue, (int &)MdAlarmFormats.PtzAction, (int)av_true, (int)av_false, (int)av_true);
	ConfBase.Process("AlmOutTimes", ConfValue, (int &)MdAlarmFormats.AlmOutTimes, (int)15, (int)0, (int)3600);
	AvConfigValue &WeekSpanTable = ConfValue["WeekSpan"];



	for (int i = 0; i < ConfMaxWeekDaySpan; i++){
		ConfBase.Process("Enable", WeekSpanTable[i], (int &)MdAlarmFormats.WeekSpan.DaySpan[i].DayEnable, (int)av_true, (int)av_false, (int)av_true);
		AvConfigValue &DaySpanTable = WeekSpanTable[i]["DaySpan"];
		for (int j = 0; j < ConfMaxDayTimeSpan; j++){
			ConfBase.Process("Enable", DaySpanTable[j], (int &)MdAlarmFormats.WeekSpan.DaySpan[i].TimeSpan[j].TimeEnable, (int)av_true, (int)av_false, (int)av_true);
			ConfBase.Process("Sec_S", DaySpanTable[j], MdAlarmFormats.WeekSpan.DaySpan[i].TimeSpan[j].StartSec, 0, 0, 24 * 3600 - 1);
			ConfBase.Process("Sec_E", DaySpanTable[j], MdAlarmFormats.WeekSpan.DaySpan[i].TimeSpan[j].EndSec, 24 * 3600 - 1, 0, 24 * 3600 - 1);
		}
	}

#ifndef ConfMaxMdChunkLine
	AvConfigValue &ChunkTable = ConfValue["MdAreaZone"];
	for (int i = 0; i < ConfMaxMdAreaNum; i++){
		ConfBase.Process("Enable", ChunkTable[i], (int &)MdAlarmFormats.MdAreaZone[i].Enable, (int)av_true, (int)av_false, (int)av_true);
		AvConfigValue &AraeZone = ChunkTable[i]["MdAreaZone"];
		ConfBase.Process("buttom",	AraeZone, MdAlarmFormats.MdAreaZone[i].Zone.buttom, 0x00, 0x00, 10000);
		ConfBase.Process("top",		AraeZone, MdAlarmFormats.MdAreaZone[i].Zone.top,	0x00, 0x00, 10000);
		ConfBase.Process("left",	AraeZone, MdAlarmFormats.MdAreaZone[i].Zone.left,	0x00, 0x00, 10000);
		ConfBase.Process("rigth",	AraeZone, MdAlarmFormats.MdAreaZone[i].Zone.rigth,	0x00, 0x00, 10000);
	}
#else
	AvConfigValue &ChunkTable = ConfValue["MdChunk"];
	for (int i = 0; i < ConfMaxMdChunkLine; i++){
		ConfBase.Process(i, ChunkTable, MdAlarmFormats.Chunk[i], 0x00, 0x00, 0xffffffff);
	}
#endif
}
