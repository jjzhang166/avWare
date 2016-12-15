#ifndef _PROTOFIRWAREMANAGE_H_
#define _PROTOFIRWAREMANAGE_H_
#include <list>
#include "Apis/AvWareType.h"
#include "Apis/AvWareStruct.h"
#include "CAvObject.h"
#include "AvThread/AvThread.h"
#include "Apis/AvEnum.h"
#include "Proto.h"


class CProtoUpgrade:public CThread
{
public:
	CProtoUpgrade();
	~CProtoUpgrade();
	SINGLETON_DECLARE(CProtoUpgrade);
public:
	av_bool ProtoUpgrade(std::list <C_FirmWareUpgrade> *FirmWareUpgrade);
private:
	void ThreadProc();

private:
	std::list <C_FirmWareUpgrade> *m_FirmWareUpgrade;
	std::list <AvProtoSDK *>	m_UpgradeDevices;

};
#define g_ProtoUpgrade (*CProtoUpgrade::instance())




#endif