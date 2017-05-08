#include "AvGuiStatusMachine.h"
#include "Apis/AvSystemStruct.h"
#include "AvDevice/AvDevice.h"
#include "Apis/AvEnuminline.h"

CAvGuiStatus::CAvGuiStatus()
{
	m_SplitNum = 0;
	m_PreviewMode = PreviewMode_None;
	m_StartChannel = -1;
	m_EndChannel = -1;
	m_AvChip = AvChip_LAST;

	m_LastSplitNum		= m_PreviewMode;
	m_LastStartChannel	= m_StartChannel;
	m_LastEndChannel	= m_EndChannel;
	m_bIsMaximized = false;
}

CAvGuiStatus::~CAvGuiStatus()
{
}

CAvGuiStatus *CAvGuiStatus::instance()
{
	static CAvGuiStatus *_instance = NULL;
	if (_instance == NULL){
		_instance = new CAvGuiStatus;
	}
	return _instance;
}

void CAvGuiStatus::SplitSetNum(int num)
{
	m_LastSplitNum = m_SplitNum;
	m_SplitNum = num;
}
int  CAvGuiStatus::SplitGetNum()
{
	return m_SplitNum;
}
int CAvGuiStatus::SplitGetLastNum()
{
	return m_LastSplitNum;
}
void CAvGuiStatus::PreviewSetMode(PreviewMode e)
{
	m_PreviewMode = e;
}
CAvGuiStatus::PreviewMode CAvGuiStatus::PreviewGetMode()
{
	return m_PreviewMode;
}

void CAvGuiStatus::SplitSetStartChannel(int StartChannel, int EndChannel)
{
	m_LastStartChannel = m_StartChannel;
	m_LastEndChannel = m_EndChannel;

	m_StartChannel = StartChannel;
	m_EndChannel = EndChannel;
}
void CAvGuiStatus::SplitGetStartChannel(int &StartChannel, int &EndChannel)
{
	StartChannel = m_StartChannel;
	EndChannel = m_EndChannel;
}
void CAvGuiStatus::SplitGetLastStartChannel(int &StartChannel, int &EndChannel)
{
	StartChannel = m_LastStartChannel;
	EndChannel = m_LastEndChannel;
}

bool CAvGuiStatus::IsEmbeddedSystem()
{
	return CAvDevice::IsEmbeddedSystem() == av_true ? true : false;
}

bool CAvGuiStatus::IsMaximizedWindows()
{
	return m_bIsMaximized;
}
void CAvGuiStatus::SetMaximizedWindows(bool bIsMaximized)
{
	m_bIsMaximized = bIsMaximized;
}