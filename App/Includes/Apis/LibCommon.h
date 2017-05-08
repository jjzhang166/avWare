#ifndef _LIBCOMMON_H_
#define _LIBCOMMON_H_
#include "Apis/AvEnum.h"





//////////////////////////////////////////////////////////////////////////
//FirmWare Head 
#define _AV_FirmWarePayLoadHead_Version_V1  1
#define _AV_FirmWarePayLoadHead_Version_V2  2

#define _AV_FirmWareHead_Sync_Code__ 0XBF78AAFF

typedef struct{
	unsigned int _Sync;
	unsigned int _Version;
	//这个位置需要添加一个版本头的长度，偏移的时候好计算，分两部走，1 在V2上添加，C_FirmwarePayloadHeadV2(第一个位置），length,然后再v3上将这个length移到此处
}C_FirmwareCommonHead;

//////////////////////////////////////////////////////////////////////////
//FirmWare HeadV1
typedef struct{
	unsigned int FirmwareVersion;
	unsigned int FirmwarePayloadMask;
	unsigned int FirmwareChipMask;/*这个位置要加入硬件版本支持，因为目前可以升级内核文件系统了，内核与硬件直接相关，与芯片相关*/
	unsigned int FirmwareCustom;
	unsigned int FirmwareMkTime;
	unsigned int PacketLength;
}C_FirmwarePayloadHeadV1;


typedef struct{
	unsigned int		PayLoadCrc32;
	FirmWareFileFormat	PayLoadFilef;
	FirmWarePayload		Payload;
	unsigned int		offset;
	unsigned int		length;
}C_FirmwarePayloadInfoV1;


typedef struct{
	C_FirmwareCommonHead		CommonHead;
	C_FirmwarePayloadHeadV1		PayloadHead;
	C_FirmwarePayloadInfoV1		PayLoadInfo[4];/*这个位置应该写成FirmWarePayload_Last*/
}C_FirmWareHeadV1;

//////////////////////////////////////////////////////////////////////////
//FirmWare HeadV2
typedef struct {
	unsigned int padding;
}C_FirmwarePayloadHeadV2;

typedef struct{
	unsigned int padding;
}C_FirmwarePayloadInfoV2;

typedef struct{
	unsigned int padding;
}C_FirmwareHeadV2;
//////////////////////////////////////////////////////////////////////////



typedef C_FirmWareHeadV1 C_FirmWareHead;

typedef struct{
	union{
		C_FirmwareCommonHead	FirmWareCommHead;
		C_FirmWareHeadV1		FirmWareHeadV1;
		C_FirmwareHeadV2		FirmwareHeadV2;
	};
	unsigned char *base;
	unsigned int   length;/*non-zero avWareCheckoutFirmWare Can read firmware data from file to base mem; you mast be free base*/
}C_FirmWareCheckoutHand;


typedef struct{
	char FilePath[256];
	FirmWareFileFormat	PayLoadFilef;
	FirmWarePayload		Payload;
}C_FirmWareAddAttribe;

#if defined(__cplusplus)
extern "C"{
#endif

	//////////////////////////////////////////////////////////////////////////
	unsigned int	avWarecrc32(const unsigned char *buf, unsigned int size);
	void			avWarecrc32Splite(unsigned int *crc, const unsigned char *buf, unsigned int size);
	unsigned int	avWarecrc32SpliteOver(unsigned int crc);
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////


#define avFirmWareHandle void*

	int avFirmWareCheckout(C_FirmWareCheckoutHand *pFimWareHead, const char *SrcFirmWarePath);
	avFirmWareHandle avFirmWareCreate(const char *ConfigFilePath);
	int		avFirmWareOutPut(avFirmWareHandle Handle, const char *OutPutDirPath);
	int		avFirmWareAdd(avFirmWareHandle Handle, C_FirmWareAddAttribe *Attribe);
	int		avFirmWareSync(avFirmWareHandle Handle);
	int		avFirmWareGetExportFilePath(avFirmWareHandle Handle, char *FilePath);
	int		avFirmWareClose(avFirmWareHandle Handle);
	//////////////////////////////////////////////////////////////////////////

#if defined(__cplusplus)
}
#endif


#endif