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
#include "AvNetService/AvSmtp.h"
#include "Smtp/CSmtp.h"


CAvSmtp::CRawAttach::CRawAttach()
{
	m_FileName.clear();
	m_data = NULL;
	m_datalen = 0;
}
CAvSmtp::CRawAttach::~CRawAttach()
{
	m_FileName.clear();
	if (NULL != m_data) free(m_data);
	m_datalen = 0;
}
av_bool CAvSmtp::CRawAttach::FillInData(std::string FileName, av_uchar *data, av_32 datalen)
{
	m_FileName = FileName;
	m_data = (av_uchar *)malloc(sizeof(av_uchar)*datalen);
	assert(m_data != NULL);
	memcpy(m_data, data, datalen);
	m_datalen = datalen;
	return av_true;
}
CAvSmtp::CAvSmtp()
{
}

CAvSmtp::~CAvSmtp()
{
}


av_bool CAvSmtp::Start()
{
	bool bError = false;
	try
	{
		CSmtp Smtp;
		switch (m_EnMode)
		{
		case MAIL_ENNULL:
			Smtp.SetSecurityType(NO_SECURITY);
			break;
		case MAIL_ENSSL:
			Smtp.SetSecurityType(USE_SSL);
			break;
		case MAIL_ENTLS:
			Smtp.SetSecurityType(USE_TLS);
			break;
		default:
			Smtp.SetSecurityType(NO_SECURITY);
			break;
		}
		Smtp.SetSMTPServer(m_Server.c_str(), atoi(m_Port.c_str()));
		Smtp.SetLogin(m_Usrname.c_str());
		Smtp.SetPassword(m_Passwd.c_str());
		Smtp.SetSenderMail(m_Sender.c_str());
		Smtp.SetSenderName(m_Sender.c_str());
		Smtp.SetReplyTo(m_Sender.c_str());
		Smtp.SetSubject(m_Title.c_str());
		std::list<std::string>::iterator iList;
		for (iList = m_Recver.begin(); iList != m_Recver.end(); iList++){
			Smtp.AddRecipient(iList->c_str());
		}
		Smtp.SetXPriority(XPRIORITY_NORMAL);
		Smtp.SetXMailer("avWare Professional");
		if (m_Text.size() > 0){
			Smtp.AddMsgLine(m_Text.c_str());
		}
		for (iList = m_LocalFile.begin(); iList != m_LocalFile.end(); iList++){
			Smtp.AddAttachment(iList->c_str());
		}
		std::list <CRawAttach>::iterator iRawList;
		CRawAttach TempAttach;
		for (iRawList = m_RawFile.begin(); iRawList != m_RawFile.end(); iRawList++)
		{
			TempAttach = *iRawList;
			Smtp.AttachmentInfo((char *)TempAttach.m_FileName.c_str(), (char *)TempAttach.m_data, TempAttach.m_datalen);
		}

		Smtp.Send();
	}
	catch (ECSmtp e)
	{
		bError = true;
	}

	return bError == true ? av_true:av_false;
}
av_bool CAvSmtp::Stop()
{
	return av_true;
}

av_bool CAvSmtp::SetServer(std::string Server, std::string Port, E_SMTP_EN_MODE mode)
{
	if (0 == Server.size() || 0 == Port.size())
	{
		av_msg("input param is error...\n");
		return av_false;
	}

	m_Server = Server;
	m_Port = Port;
	m_EnMode = mode;

	return av_true;
}
av_bool CAvSmtp::SetAuthorization(std::string usrname, std::string passwd)
{
	if (0 == usrname.size())
	{
		av_msg("input param is error...\n");
		return av_false;
	}

	m_Usrname = usrname;
	m_Passwd = passwd;

	return av_true;
}
av_bool CAvSmtp::SetSender(std::string sender)
{
	if (0 == sender.size())
	{
		av_msg("input param is error...\n");
		return av_false;
	}

	m_Sender = sender;

	return av_true;
}
av_bool CAvSmtp::SetAddRecver(std::string recver)
{
	if (0 == recver.size())
	{
		av_msg("input param is error...\n");
		return av_false;
	}

	m_Recver.push_back(recver);

	return av_true;
}
av_bool CAvSmtp::SetTitle(std::string Title)
{
	if (0 == Title.size())
	{
		av_msg("input param is error...\n");
		return av_false;
	}

	m_Title = Title;

	return av_true;
}

av_bool CAvSmtp::SetContent(std::string Content)
{
	if (0 == Content.size())
	{
		av_msg("input param is error...\n");
		return av_false;
	}

	m_Title = Content;

	return av_true;
}

av_bool CAvSmtp::SetAddAttachment(std::string localFile)
{
	if (0 == localFile.size())
	{
		av_msg("input param is error...\n");
		return av_false;
	}

	m_LocalFile.push_back(localFile);

	return av_true;
}
av_bool CAvSmtp::SetAddAttachment(std::string RawFileName, av_uchar *data, av_32 datalen)
{
	if (0 == RawFileName.size() || NULL == data || 0 >= datalen)
	{
		av_msg("input param is error...\n");
		return av_false;
	}

	CRawAttach RawAttach;
	RawAttach.FillInData(RawFileName, data, datalen);

	return av_true;
}