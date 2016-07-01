#ifndef _AV_RECORD_H_
#define _AV_RECORD_H_
#include "Apis/AvWareType.h"
#include "CAvObject.h"

class CAvRecord
{
public:
	CAvRecord();
	~CAvRecord();

private:

};


class CAvRecordManager
{
public:
	SINGLETON_DECLARE(CAvRecordManager);
	av_bool Initialize();
private:
	CAvRecordManager();
	~CAvRecordManager();

private:

};


#define g_AvRecord (*CAvRecordManager::instance())








#endif

