#ifndef _LOGAVWAREEXPORT_H_
#define _LOGAVWAREEXPORT_H_

typedef enum {
	IOCTRL_CMD_SET_DEVINFO,
	IOCTRL_CMD_SET_CAPSTAT,
	IOCTRL_CMD_SET_DECODE,
	IOCTRL_CMD_SET_RECORD,
}IOCTRL_CMD;


typedef struct {
	int MaxEncodeChn;
	int MaxDecodeChn;
	char avWareVersion[64];
}avWare_Device_Info;


typedef struct {
	int Channel;
	int Slave;
	
	unsigned int 	CaptureIn;
	unsigned int 	CaptureOut;
	unsigned int 	CaptureError;
	unsigned int 	CaptureDrop;
	unsigned int 	CaptureFps;
	unsigned int 	CaptureWidth;
	unsigned int 	CaptureHeigh;
	unsigned char 	CaptureFormats[10];
}avWare_Capture_Status;


#endif