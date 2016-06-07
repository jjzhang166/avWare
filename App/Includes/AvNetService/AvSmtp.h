/*******************************************************************
|  Copyright(c) 2015-2016 Graceport Technology Development Co.,Ltd
|  All rights reserved.
|
|  版本: 1.0
|  作者: ironman [ironman@graceport.cn]
|  日期: 2016年6月6日
|  说明:
|
|  版本: 1.1
|  作者:
|  日期:
|  说明:
******************************************************************/
#ifndef _AV_SMTP_H_
#define _AV_SMTP_H_

#include "Apis/AvWareType.h"


class CAvSmtp
{
public:
	typedef enum {
		MAIL_ENTLS,
		MAIL_ENSSL,
		MAIL_ENNULL,
		MAIL_ENNr,
	}E_SMTP_EN_MODE;
	inline static std::string GetCAvSmtpEnModeString(E_SMTP_EN_MODE e)
	{
		static std::string sCAvSmtpEnModeString[MAIL_ENNr + 1] =
		{ "TLS", "SSL", "NULL" , "NR"};
		return sCAvSmtpEnModeString[e];
	};
private:
	class CRawAttach
	{
	public:
		CRawAttach();
		~CRawAttach();
		av_bool FillInData(std::string FileName, av_uchar *data, av_32 datalen);
	public:
		std::string m_FileName;
		av_uchar	*m_data;
		av_32		m_datalen;
	};

public:
	CAvSmtp();
	~CAvSmtp();


	av_bool Start();
	av_bool Stop();

	av_bool SetServer(std::string Server, std::string Port, E_SMTP_EN_MODE mode);
	av_bool SetAuthorization(std::string usrname, std::string passwd);
	av_bool SetSender(std::string sender);
	av_bool SetAddRecver(std::string recver);
	av_bool SetTitle(std::string Title);
	av_bool SetContent(std::string Content);

	av_bool SetAddAttachment(std::string localFile);
	av_bool SetAddAttachment(std::string RawFileName, av_uchar *data, av_32 datalen);

private:
	std::string				m_Server;
	std::string				m_Port;
	E_SMTP_EN_MODE			m_EnMode;
	std::string				m_Usrname;
	std::string				m_Passwd;
	std::string				m_Sender;
	std::list<std::string>	m_Recver;
	std::string				m_Title;
	std::string				m_Text;
	std::list<std::string>	m_LocalFile;
	std::list<CRawAttach>  m_RawFile;
};





#endif
