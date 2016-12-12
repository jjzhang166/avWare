/*******************************************************************
|  Copyright(c) 2015-2016 Graceport Technology Development Co.,Ltd
|  All rights reserved.
|
|  版本: 1.0
|  作者: Knight [knight@graceport.cn]
|  日期: 2016年6月6日
|  说明:
|
|  版本: 1.1
|  作者:
|  日期:
|  说明:
******************************************************************/
#ifndef _RBUFFER_H_
#define _RBUFFER_H_


#define DEFAULT_RBUFFER_LEN 6*1024

class CRBuffer
{
public:
	CRBuffer(int size = DEFAULT_RBUFFER_LEN);
	virtual ~CRBuffer();
	int Append(const char *buf, int len);
	char *GetBuffer();
	int   GetBufferLen();
	int	  GetbufferSize();
	int	  Reset();
	int	  Pop(int popLen);
private:
	char *m_Buffer;
	int	  m_BufSize;
	int	  m_BufLen;
};







#endif