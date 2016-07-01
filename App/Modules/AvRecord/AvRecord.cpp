#include "AvRecord/AvRecord.h"





CAvRecord::CAvRecord()
{

}
CAvRecord::~CAvRecord()
{

}

SINGLETON_IMPLEMENT(CAvRecordManager)


CAvRecordManager::CAvRecordManager()
{

}

CAvRecordManager::~CAvRecordManager()
{

}

av_bool CAvRecordManager::Initialize()
{
	return av_true;
}