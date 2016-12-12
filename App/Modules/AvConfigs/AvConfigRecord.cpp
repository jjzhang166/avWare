#include "Apis/AvWareCplusplus.h"
#include "AvConfigs/AvConfigRecord.h"
#include "Apis/LibEncode.h"



template<> void ProcessValue<ConfigRecordFormats>(CAvConfigBase &ConfBase, AvConfigValue &ConfValue, ConfigRecordFormats &Formats, int index, int diff)
{
	AvConfigValue &RecordTable = ConfValue;

	ConfBase.Process("RecChnMask", RecordTable, Formats.RecordChlMask, (int)(AvMask(CHL_MAIN_T)|AvMask(CHL_SUB1_T)), 1, 8);
	ConfBase.Process("RecTypeMask", RecordTable, Formats.RecordTypeMask, EAvRecord_Manul, EAvRecord_NONE, EAvRecord_Alarm);
	
	AvConfigValue &TimeTable = RecordTable["TimeRange"];
	for (int i = 0; i < RECORD_MAX_TIME_RANGE; i++){
		ConfBase.Process("bEnable", TimeTable[i], (int &)Formats.RecordTm[i].bEnable, (int)av_false, (int)av_false, (int)av_true);
		ConfBase.Process("tStart", TimeTable[i], Formats.RecordTm[i].tStart, 0, 0, 24 * 3600 - 1);
		ConfBase.Process("tEnd", TimeTable[i], Formats.RecordTm[i].tEnd, 24 * 3600 - 1, 0, 24 * 3600 - 1);
	}

}

template<> void ProcessValue<ConfigRecordCtrlFormats>(CAvConfigBase &ConfBase, AvConfigValue &ConfValue, ConfigRecordCtrlFormats &Formats, int index, int diff)
{
	AvConfigValue &RecordCtrlTable = ConfValue;
	ConfBase.Process("LimitType", RecordCtrlTable, (int &)Formats.LimitType, EAvRecord_LimitSize, EAvRecord_LimitTime, EAvRecord_LimitSize);
	ConfBase.Process("LimitArgs", RecordCtrlTable, Formats.LimitArgs, EAvRecordMaxRecordFileSize / 2, EAvRecordMaxRecordFileSize / 8, EAvRecordMaxRecordFileSize);
	ConfBase.Process("LimitPrerecordSec", RecordCtrlTable, Formats.LimitPrerecordSec, 0, 0, EAvRecordMaxPrerecordSec);
}