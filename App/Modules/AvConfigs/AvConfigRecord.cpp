#include "Apis/AvWareCplusplus.h"
#include "AvConfigs/AvConfigRecord.h"
#include "Apis/LibEncode.h"



template<> void ProcessValue<ConfigRecordProfile>(CAvConfigBase &ConfBase, AvConfigValue &ConfValue, ConfigRecordProfile &Formats, int index, int diff)
{
	AvConfigValue &RecordTable = ConfValue;

	ConfBase.Process("RecChnMask", RecordTable, Formats.RecordCHLMask, (int)(AvMask(CHL_MAIN_T)|AvMask(CHL_SUB1_T)), 1, 8);
	ConfBase.Process("RecTypeMask", RecordTable, Formats.RecordTypeMask, 0, 0, 0xffffffff);
	ConfBase.Process("RecordPerSecond", RecordTable, Formats.RecordPerSecond, 0, 0, 15);


	AvConfigValue &WeekTable = RecordTable["WeekRange"];
	for (int i = 0; i < AvMaxWeeks; i++){
		AvConfigValue &TimeTable = WeekTable[i];
		for (int j = 0; j < AvMaxTimeArea; j++){
			
			ConfBase.Process("WeekIndex", TimeTable[j], Formats.RecordTm[i][j].WeekIndex, i, i, i);
			ConfBase.Process("StartSec", TimeTable[j], Formats.RecordTm[i][j].StartSec, 0, 0, 24 * 3600 - 1);
			ConfBase.Process("StopSec", TimeTable[j], Formats.RecordTm[i][j].StopSec, 0, 0, 24 * 3600 - 1);
		}
	}
}

template<> void ProcessValue<ConfigRecordCtrlProfile>(CAvConfigBase &ConfBase, AvConfigValue &ConfValue, ConfigRecordCtrlProfile &Formats, int index, int diff)
{
	AvConfigValue &RecordCtrlTable = ConfValue;
	ConfBase.Process("LimitType", RecordCtrlTable, (int &)Formats.LimitType, EAvRecord_LimitSize, EAvRecord_LimitTime, EAvRecord_LimitSize);
	ConfBase.Process("LimitArgs", RecordCtrlTable, Formats.LimitArgs, EAvRecordMaxRecordFileSize / 2, EAvRecordMaxRecordFileSize / 8, EAvRecordMaxRecordFileSize);
	ConfBase.Process("LimitPrerecordSec", RecordCtrlTable, Formats.LimitPrerecordSec, 0, 0, EAvRecordMaxPrerecordSec);
}