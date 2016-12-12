#ifndef _AVRECORDDB_H_
#define _AVRECORDDB_H_

#include "Apis/AvWareCplusplus.h"
#include "Sqlite/sqlite3.h"




typedef struct {
	av_char		FilePath[128];
	av_uint		FileSize;
	av_uint		FileRecTmS;
	av_uint		FileRecTmE;
	av_uint		FileChannel;
	av_uint		FileSlave;
	av_uint		FileTypeMask;
}RecordFileInfo;

/*
	av_int  类型的变量 -1 为不关心这个属性
	av_uint 类型的变量  0 为不关心这个属性
*/
typedef struct {
	av_int	FileSize;
	av_int  FileRecTmS;
	av_int  FileRecTmE;

	av_uint SearchDay;
	av_uint FileChannelMask;
	av_uint FileSlaveMask;
	av_uint FileTypeMask;
}RecordFileSearch;

class CAvRecordDB
{
public:
	CAvRecordDB();
	~CAvRecordDB();


public:
	av_bool DbOpen(const char *DbPath);
	av_bool DbClose();
	av_bool DbInsert(RecordFileInfo &FileInfo);
	av_bool DbUpdate(RecordFileInfo &FileInfo);
	av_bool DbDelete(RecordFileInfo &FileInfo);

	av_bool DbSearch(RecordFileSearch &Search, std::list<RecordFileInfo> &result);
private:
	sqlite3 *m_Db;
	av_char  DbTableName[128];
};




#endif

