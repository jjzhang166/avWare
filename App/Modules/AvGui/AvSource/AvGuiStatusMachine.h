

#ifndef _AVGUI_STATUSMACHINE_H_
#define _AVGUI_STATUSMACHINE_H_
#include "Apis/AvEnum.h"



class CAvGuiStatus
{
public:
	typedef enum{
		PreviewMode_None,
		PreviewMode_RealTime,
		PreviewMode_PlayBack,
	}PreviewMode;
public:
	CAvGuiStatus();
	~CAvGuiStatus();

	static CAvGuiStatus *instance();

public:
	void SplitSetNum(int num);
	int  SplitGetNum();
	int  SplitGetLastNum();

	void PreviewSetMode(PreviewMode e);
	PreviewMode PreviewGetMode();

	void SplitSetStartChannel(int StartChannel, int EndChannel);
	void SplitGetStartChannel(int &StartChannel, int &EndChannel);
	void SplitGetLastStartChannel(int &StartChannel, int &EndChannel);
	bool IsEmbeddedSystem();

	bool IsMaximizedWindows();
	void SetMaximizedWindows(bool bIsMaximized);


private:
	
	PreviewMode m_PreviewMode;
	int		m_SplitNum;
	int		m_StartChannel;
	int		m_EndChannel;

	int		m_LastSplitNum;
	int		m_LastStartChannel;
	int		m_LastEndChannel;

	bool	m_bIsMaximized;
	AvChip m_AvChip;
};







#endif