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
#ifndef __XML_H__
#define __XML_H__
#include "common.h"

#define DF_NAMESPACE_MAX 		100
#define DF_NAMESPACETAG_MAX 	24
#define DF_ROOTBUFF_SIZE		1024*20	
#define DF_NAMESPACESTR			1024*4

typedef enum _HTTP_METHOD_E{
	HTTP_POST = 0X01,
	HTTP_GET,
}HTTP_METHOD_E;

typedef enum _HTTP_CONNECT_E{
	HTTP_CONNECT_CLOSE = 0X01,
	HTTP_CONNECT_KEEPALIVE,
}HTTP_CONNECT_E;

typedef struct _HttpReqHead_S{
	HTTP_METHOD_E	method;
	HTTP_CONNECT_E	connect;
	int 			contentLength;
	char 			host[64];
	char			url[128];
}HttpReqHead_S;

typedef struct _Root_S
{
	char	namespaceTag[DF_NAMESPACE_MAX][DF_NAMESPACETAG_MAX];
	char 	namespacestr[DF_NAMESPACESTR]; 
	char	buff[DF_ROOTBUFF_SIZE];
}Root_S;

typedef struct _XmlElementObj_S{
	Root_S *root;
	char *namespacestr;
	char *tag;
	char *attribute;
	char *value;
	struct _XmlElementObj_S	*containObj;
	struct _XmlElementObj_S	*parallelObj;
}XmlElementObj_S;


typedef struct _ClientReqObj_S{
	char *msg;
	XmlElementObj_S *obj;//soap消息
	HttpReqHead_S head;//头消息
	char buf[DF_ROOTBUFF_SIZE];
}ClientReqObj_S;
XmlElementObj_S* CreateXmlElementObj(Root_S* root,char *namespacestr,char *tag,char *attribute,char *value,XmlElementObj_S *fatherObj);
XmlElementObj_S* GetXmlElementObj(XmlElementObj_S* obj,char *tag);
int DelAllXmlElementObj(XmlElementObj_S *obj);//慎用 用完记得把上一个指针指向的内容赋值为NULL
int NamespaceRecord(Root_S *root,char *namespacestr);
int ParaOnvifMsg(ClientReqObj_S *obj);
int MakeSrcCode(ClientReqObj_S *obj);

#endif

