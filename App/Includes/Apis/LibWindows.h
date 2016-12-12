#ifndef _LIBWINDOWS_H_
#define _LIBWINDOWS_H_



typedef struct {
	av_u32  left;
	av_u32 	top;
	av_u32 	right;
	av_u32 	bottom;
}C_WinRect;


typedef enum{
	SpliteMode1,
	SpliteMode4,
	SpliteMode6,
	SpliteMode8,
	SpliteMode9,
	SpliteMode25,
	SpliteMode36,
	SpliteMode64,
	SpliteMode128,
}E_WinSpliteMode;

av_bool AvWinCreate();

av_bool AvWinDestory();


av_bool AvWinSetMode(E_WinSpliteMode Mode);

av_bool AvWinGetRect(av_int Channel, C_WinRect *Rect);

av_bool AvWinSplit(E_WinSpliteMode Mode, av_int StartChannel);

av_bool AvWinSwitch(av_int Channel1, av_int Channel2);







#endif


