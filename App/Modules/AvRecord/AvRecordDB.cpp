#include "AvRecord/AvRecordDB.h"


#define RecordDbColumnFilePath		"FilePath"
#define RecordDbColumnFileSize		"FileSize"
#define RecordDbColumnFileRecTmS	"FileRecTmS"
#define RecordDbColumnFileRecTmE	"FileRecTmE"
#define RecordDbColumnFileChannel	"FileChannel"
#define RecordDbColumnFileSlave		"FileSlave"
#define RecordDbColumnFileTypeMask	"FileTypeMask"

#define RecordDbFmtCreate	"CREATE TABLE RecordDb(FilePath VAR CHAR(150), FileSize INTEGER, FileRecTmS INTEGER, FileRecTmE INTEGER, FileChannel INTEGER, FileSlave INTEGER, FileTypeMask INTEGER)"
#define RecordDbFmtInsert	"INSERT INTO RecordDb VALUES('%s', %d, %d, %d, %d, %d, %d)"
#define RecordDbFmtUpdate	"UPDATE RecordDb set FileSize=%d, FileRecTmE=%d, FileTypeMask=%d where FilePath = \"%s\""
#define RecordDbFmtDelete	"DELETE FROM RecordDb WHERE FilePath=\"%s\""


CAvRecordDB::CAvRecordDB()
{
	m_Db = NULL;
	memset(DbTableName, 0x00, sizeof(DbTableName));
}

CAvRecordDB::~CAvRecordDB()
{
	DbClose();
}

av_bool CAvRecordDB::DbOpen(const char *DbPath)
{
	av_bool isDbHave = av_false;
	int ret = 0;
	if (NULL != m_Db) sqlite3_close(m_Db);
	m_Db = NULL;

	if (0 == access(DbPath, 0)){
		isDbHave = av_true;
	}
	ret = sqlite3_open(DbPath, &m_Db);
	if (ret != 0){
		av_error("Can't Open %s Db\n", DbPath);
		m_Db = NULL;
		return av_false;
	}
	//设置安全等级
	const char *sql = "PRAGMA synchronous = 0;PRAGMA journal_mode = OFF;";
	ret = sqlite3_exec(m_Db, sql, NULL, NULL, NULL);
	if (ret != SQLITE_OK){
		av_error("sql exec %s error ret = %d\n", sql, ret);
		return av_false;
	}
	if (isDbHave == av_false){
		//create table
		char* DbErrorMessage = NULL;
		int ret = sqlite3_exec(m_Db, RecordDbFmtCreate, NULL, NULL, &DbErrorMessage);
		if (ret != SQLITE_OK){
			av_error("sqlite3_exec return error %s\n", DbErrorMessage);
			return av_false;
		}
	}
	return av_true;
}
av_bool CAvRecordDB::DbClose()
{
	if (NULL == m_Db) return av_true;
	int ret = sqlite3_close(m_Db);
	m_Db = NULL;
	if (ret != SQLITE_OK){
		av_error("sqlite3_close Error ret(%d)\n", ret);
		return av_false;
	}
	return av_true;
}

av_bool CAvRecordDB::DbInsert(RecordFileInfo &FileInfo)
{
	char sql[512];
	char* DbErrorMessage = NULL;
	int ret = 0;
	sprintf(sql, RecordDbFmtInsert, FileInfo.FilePath,
		FileInfo.FileSize, FileInfo.FileRecTmS, FileInfo.FileRecTmE,
		FileInfo.FileChannel, FileInfo.FileSlave, FileInfo.FileTypeMask );
	ret = sqlite3_exec(m_Db, sql, NULL, NULL, &DbErrorMessage);
	if (ret != SQLITE_OK){
		av_error("sqlite3_exec return error %s\n", DbErrorMessage);
		return av_false;
	}

	return av_true;
}
av_bool CAvRecordDB::DbUpdate(RecordFileInfo &FileInfo)
{
	char sql[512];
	char* DbErrorMessage = NULL;
	int ret = 0;
	sprintf(sql, RecordDbFmtUpdate, FileInfo.FileSize, FileInfo.FileRecTmE,
		FileInfo.FileTypeMask,FileInfo.FilePath);
	ret = sqlite3_exec(m_Db, sql, NULL, NULL, &DbErrorMessage);
	if (ret != SQLITE_OK){
		av_error("sqlite3_exec return error %s\n", DbErrorMessage);
		return av_false;
	}
	return av_true;
}
av_bool CAvRecordDB::DbDelete(RecordFileInfo &FileInfo)
{
	char sql[512];
	char* DbErrorMessage = NULL;
	int ret = 0;
	sprintf(sql, RecordDbFmtDelete, FileInfo.FilePath);
	ret = sqlite3_exec(m_Db, sql, NULL, NULL, &DbErrorMessage);
	if (ret != SQLITE_OK){
		av_error("sqlite3_exec return error %s\n", DbErrorMessage);
		return av_false;
	}

	return av_true;
}

av_bool CAvRecordDB::DbSearch(RecordFileSearch &Search, std::list<RecordFileInfo> &result)
{
	av_char SearchFmt[256] = { 0 };
	strcat(SearchFmt, "select * from RecordDb where ");
	{
		av_bool HasFirst = av_false;
		if (Search.FileSize != -1){
			sprintf(&SearchFmt[strlen(SearchFmt)], RecordDbColumnFileSize" <= %d ", Search.FileSize);
			HasFirst = av_true;
		}
		
		if (Search.FileRecTmS != -1 && Search.FileRecTmE != -1){
			if (HasFirst == av_false){
				sprintf(&SearchFmt[strlen(SearchFmt)], RecordDbColumnFileRecTmS" >= %d ", Search.FileRecTmS);
			}
			else{
				sprintf(&SearchFmt[strlen(SearchFmt)], " and "RecordDbColumnFileRecTmS" >= %d ", Search.FileRecTmS);
			}

			sprintf(&SearchFmt[strlen(SearchFmt)], " and "RecordDbColumnFileRecTmE" <= %d ", Search.FileRecTmE);
		}
	}
	
	


	sqlite3_stmt* stmt = NULL;
	RecordFileInfo FileInfo;
	int ncolumn = 0;
	int vtype = 0;
	const char *column_name = NULL;

	int rc = sqlite3_prepare_v2(m_Db, SearchFmt, -1, &stmt, 0);
	if (rc != SQLITE_OK){
		return av_false;
	}

	ncolumn = sqlite3_column_count(stmt);
	while (sqlite3_step(stmt) == SQLITE_ROW)
	{
		for (int i = 0; i < ncolumn; i++)
		{
			vtype = sqlite3_column_type(stmt, i);
			column_name = sqlite3_column_name(stmt, i);
			switch (vtype)
			{
			case SQLITE_NULL:
				av_error("%s: null\n", column_name);
				break;
			case SQLITE_INTEGER:
			{
				if (0 == strncasecmp(column_name, RecordDbColumnFileSize, strlen(RecordDbColumnFileSize))){
					FileInfo.FileSize = sqlite3_column_int(stmt, i);
				}
				else if (0 == strncasecmp(column_name, RecordDbColumnFileRecTmS, strlen(RecordDbColumnFileRecTmS))){
					FileInfo.FileRecTmS = sqlite3_column_int(stmt, i);
				}
				else if (0 == strncasecmp(column_name, RecordDbColumnFileRecTmE, strlen(RecordDbColumnFileRecTmE))){
					FileInfo.FileRecTmE = sqlite3_column_int(stmt, i);
				}
				else if (0 == strncasecmp(column_name, RecordDbColumnFileChannel, strlen(RecordDbColumnFileChannel))){
					FileInfo.FileChannel = sqlite3_column_int(stmt, i);
				}
				else if (0 == strncasecmp(column_name, RecordDbColumnFileSlave, strlen(RecordDbColumnFileSlave))){
					FileInfo.FileSlave = sqlite3_column_int(stmt, i);
				}
				else if (0 == strncasecmp(column_name, RecordDbColumnFileTypeMask, strlen(RecordDbColumnFileTypeMask))){
					FileInfo.FileTypeMask = sqlite3_column_int(stmt, i);
				}
				else {

				}
			}
				break;
			case SQLITE_FLOAT:
				av_error("%s: %f\n", column_name, sqlite3_column_double(stmt, i));
				break;
			case SQLITE_BLOB:
				av_error("%s: BLOB\n", column_name);
				break;
			case SQLITE_TEXT:
				if (0 == strncasecmp(column_name, RecordDbColumnFilePath, strlen(RecordDbColumnFilePath))){
					sprintf(FileInfo.FilePath, "%s", sqlite3_column_text(stmt, i));
				}
				else{
					assert(0);
				}
				break;

			default:
				av_error("%s: ERROR [%s]\n", column_name, sqlite3_errmsg(m_Db));
				break;
			}
		}
		result.push_back(FileInfo);
	}

	sqlite3_finalize(stmt);
	return av_true;
}
