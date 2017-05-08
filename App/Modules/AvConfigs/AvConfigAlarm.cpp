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

template<> void ProcessValue<ConfigAlarmProfile>(CAvConfigBase &ConfBase, AvConfigValue &ConfValue, ConfigAlarmProfile &AlarmProfile, int index, int diff)
{
	ConfBase.Process("AlarmEventMask", ConfValue, AlarmProfile.AlarmEventMask, 0, 0, 0xffffffff);
	ConfBase.Process("AlarmbEnable", ConfValue, AlarmProfile.AlarmbEnable, av_false);
	ConfBase.Process("AlarmbLinkageEmail", ConfValue, AlarmProfile.AlarmbLinkageEmail, av_false);
	ConfBase.Process("AlarmbLinkageFtp", ConfValue, AlarmProfile.AlarmbLinkageFtp, av_false);
	ConfBase.Process("AlarmbLinkageSnapshot", ConfValue, AlarmProfile.AlarmbLinkageSnapshot, av_false);
	ConfBase.Process("AlarmbLinkageAlmOut", ConfValue, AlarmProfile.AlarmbLinkageAlmOut, av_false);
	ConfBase.Process("AlarmbLinkageBeep", ConfValue, AlarmProfile.AlarmbLinkageBeep, av_false);
	ConfBase.Process("AlarmbLinkagePtz", ConfValue, AlarmProfile.AlarmbLinkagePtz, av_false);
	ConfBase.Process("AlarmbLinkageRecord", ConfValue, AlarmProfile.AlarmbLinkageRecord, av_false);
	ConfBase.Process("AlarmMotionLevel", ConfValue, AlarmProfile.AlarmMotionLevel, 3, 1, 5);
	ConfBase.Process("AlarmLinkagePrerecordSec", ConfValue, AlarmProfile.AlarmLinkagePrerecordSec, 0, 0, 30);

	AvConfigValue &MotionDetectionConfTable = ConfValue["MotionDetectionConfTable"];
	for (int i = 0; i < ConfMotionDetectionLine; i++){
		ConfBase.Process("AlarmMotionArea", MotionDetectionConfTable[i], AlarmProfile.AlarmMotionArea[i], 0, 0, (0x01 << 22) -1);
	}

	AvConfigValue &DaySpanTable = ConfValue["DaySpanTable"];
	for (int i = 0; i < AvMaxWeeks; i++){
		AvConfigValue &TimeSpanTable = DaySpanTable[i]["TimeSpanTable"];
		for (int j = 0; j < AvMaxTimeArea; j++){
			ConfBase.Process("WeekIndex", TimeSpanTable[j], AlarmProfile.AlarmTimeArea[i][j].WeekIndex, i, i, i);
			ConfBase.Process("StartSec", TimeSpanTable[j], AlarmProfile.AlarmTimeArea[i][j].StartSec, 0, 0, 24 * 3600 - 1);
			ConfBase.Process("StopSec", TimeSpanTable[j], AlarmProfile.AlarmTimeArea[i][j].StopSec, 0, 0, 24 * 3600 - 1);
		}
	}
}
