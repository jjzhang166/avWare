/*******************************************************************
|  Copyright(c) 2015-2016 Graceport Technology Development Co.,Ltd
|  All rights reserved.
|
|  版本: 1.0
|  作者: FootMan [FootMan@graceport.cn]
|  日期: 2016年6月6日
|  说明:
|
|  版本: 1.1
|  作者:
|  日期:
|  说明:
******************************************************************/
#ifndef  __ONVIF_PROFILE_C_
#define  __ONVIF_PROFILE_C_
#include "onvifAPI.h"
#define  DF_MAX_PROFILE 	DF_MAX_CHN*2 

typedef struct _ProfileToken_S{
	char	ProfilesToken[32];
	char	ProfilesTokenName[32];
	char	VideoSourceToken[32];
	char	VideoSourceTokenName[32];
	char	AudioSourceToken[32];
	char	AudioSourceTokenName[32];
	char	VideoEncoderToken[32];
	char	VideoEncoderTokenName[32];
	char	AudioEncoderToken[32];
	char	AudioEncoderTokenName[32];
	char	AudioOutputToken[32];
	char	AudioOutputTokenName[32];
	char	VideoAnalyticsToken[32];
	char	VideoAnalyticsTokenName[32];
	char	PTZConfigurationToken[32];
	char	PTZConfigurationTokenName[32];
	char	NodeToken[32];
	int		VideoEnable;
	int		AudioEnable;
	int		AnalyticsEnable;
	int		PtzEnable;
	int		chn;
	int		streamID; 
	int 	Enable;
	int		audioEncodeUsrCount;
}ProfileToken_S;

int GetNewProfileId();
int InitProfileConfig();
int InitNewProfile(ProfileToken_S *profile);
int GetProfileByIdx(int idx,ProfileToken_S *profile);
int	GetIdxByChnAndStreamID(int chn,int streamID);
int	GetIdxByProfileToken(char *token);
int	GetIdxByVideoSourceToken(char *token);
int	GetIdxByVideoEncodeToken(char *token);
int	GetIdxByVideoEncodeTokenName(char *name);
int SetProfile(int idx,ProfileToken_S *profile);
int AddProfile(ProfileToken_S *profile);
int DelProfile(int idx);
int CheckVideoSourceToken(char *token);
int CheckVideoEncoderToken(char *token);
int GetIdxByAudioSourceToken();
int	GetIdxByAudioOutputTokenName(char *name);
int	GetIdxByAudioOutputToken(char *token);
int GetProfileCount();
int	GetAudioSourceUsrCount(char *token);
int	GetAudioEncoderUsrCount(char *token);
int	GetVideoEncoderUsrCount(char *token);
int	GetVideoSourceUsrCount(char *token);

#endif
