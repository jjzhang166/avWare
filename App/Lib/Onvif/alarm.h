#ifndef __ONVIF_ALARM_H__
#define __ONVIF_ALARM_H__

typedef enum _ONVIF_ALARMTYPE_E{
	ALARMTYPE_NULL = 0x00,
	ALARMTYPE_MOTION = 0x01,
	ALARMTYPE_IO1 = 0x02,
	ALARMTYPE_IO2 = 0X03,
}ONVIF_ALARMTYPE_E;

typedef enum _ONVIF_ALARMSTATE_E{
	ALARMSTATE_NULL = 0x00,
	ALARMSTATE_START = 0x01,
	ALARMSTATE_END = 0x02,
}ONVIF_ALARMSTATE_E;

typedef struct _PullMsgAlarmState_S{
	ONVIF_ALARMSTATE_E stat;
}PullMsgAlarmState_S; 
typedef struct _PullMsg_S{
	int idx;
	PullMsgAlarmState_S motionAlarm;
	PullMsgAlarmState_S io1Alarm;
	PullMsgAlarmState_S io2Alarm;
	long int terminationTime;
	long int createTime;
	char address[128];
	char eventService[128];
	char topic[256];
}PullMsg_S;

typedef struct _PushMsg_S{
	int idx;
	long int terminationTime;
	int	 pushAlarmPort;	
	char pushAlarmIp[16];	
	char notifyServer[128];
	char address[128];
	char eventService[128];
}PushMsg_S;

char* AlarmTimeToStr(long int timer,char *buf,int len);
char* GetNowTimeStr(char *buf,int len);
int InitAlarmManage();
//PULLMSG
int AddPullMsg();
int DelPullMsg(int idx);
int GetPullMsgObjByIdx(int idx,PullMsg_S *info);
int RenewPullMsg(int addSec,char *address);
int SetPullMsgAlarmStat(int idx,ONVIF_ALARMTYPE_E type,ONVIF_ALARMSTATE_E stat);
int SetPullMsgTopicAttr(int idx,char *topicAttr,int len);
//Push
int AddPushMsg(int addTime,char *notifyServer);//·µ»ØË÷ÒýºÅ
int DelPushMsgByAddress(char *addr);
int GetPushMsgObjByIdx(int idx,PushMsg_S *info);
int GetPullMsgIdxByAddress(char *addr);
#endif



