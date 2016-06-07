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
#include "AvNetService/AvFtp.h"

#define BUFFER_SIZE 1024
#define BLOCK_NUM 512

CAvFtp::CAvFtp()
{
	m_login = av_false;
}

CAvFtp::~CAvFtp()
{
	
}
av_bool CAvFtp::Start()
{
	return av_true;
}
av_bool CAvFtp::Stop()
{
	if (m_login == av_true)
	{
		m_Ftp.FtpQuit();
		m_login = av_false;
	}
	
	return av_true;
}

av_bool CAvFtp::SetConf(std::string Server, std::string Port, std::string Usrname, std::string Passwd)
{
	if (0 == Server.size() || 0 == Port.size() || 0 == Usrname.size() || 0 == Passwd.size())
	{
		av_msg("input param is error...\n");
		return av_false;
	}

	m_ServerIP = Server;
	m_Port = Port;
	m_Usrname = Usrname;
	m_Passwd = Passwd;

	m_Ftp.FtpSetConf(m_ServerIP, atoi(m_Port.c_str()), m_Usrname, m_Passwd);
	
	return av_true;

}
av_bool CAvFtp::Upload(std::string SendFile)
{
	int ret = 0;
	FILE* fp = NULL;
	int filelen = 0;
	int readlen = 0;
	int len = 0;
	char buffer[BUFFER_SIZE] = {0};
	std::string::size_type pos;
	std::string filename;
	
	if (m_login == av_false)
	{
		ret = m_Ftp.FtpLogin();
		if (ret < 0)
		{
			av_msg("ftp login error...\n");
			return av_false;
		}

		m_login = av_true;
	}

	fp = fopen(SendFile.c_str(), "r");
	if (NULL == fp)
	{
		av_error("fopen %s error...\n",SendFile.c_str());
		return av_false;
	}

	ret = fseek(fp,0L,SEEK_END);
	if (-1 == ret)
	{
		av_error("file fseek is error...\n");
		return av_false;
	}
	
	filelen = ftell(fp);
	ret = fseek(fp,0L,SEEK_SET);
	if (-1 == ret)
	{
		av_error("file fseek is error...\n");
		return av_false;
	}

	ret = m_Ftp.FtpType(FTT_BINEARY);
	if (ret < FTP_OK)
	{
		av_error("FtpType error...\n");
		return av_false;
	}
	
	ret = m_Ftp.FtpPassive();
	if (ret < FTP_OK)
	{
		av_error("FtpPassive error...\n");
		return av_false;
	}
#ifndef __linux__
	pos = SendFile.find_last_of("\\");
#else
	pos = SendFile.find_last_of("/");
#endif

	if(pos == std::string::npos) return av_false;
	filename = SendFile.substr(pos+1);
	av_msg("filename = %s\n", filename.c_str());

	ret = m_Ftp.FtpAppend(filename.c_str());
	if (ret < FTP_OK)
	{
		av_error("FtpAppend error...\n");
		return av_false;
	}

	ret = m_Ftp.FtpStor(SendFile.c_str());
	if (ret < FTP_OK)
	{
		av_error("FtpStor error...\n");
		//return av_false;
	}

	while(filelen > len)
	{
		readlen = fread(buffer, sizeof(char),BLOCK_NUM,fp);
		if (readlen < 0)
		{
			av_error("fread error...\n");
			return av_false;
		}
		ret = m_Ftp.FtpSendData(buffer, readlen);
		if (ret < FTP_OK)
		{
			av_error("ftp send data error...\n");
			m_Ftp.FtpCloseData();
			return av_false;
		}

		len += readlen;
	}

	m_Ftp.FtpCloseData();

	fclose(fp);
	
	return av_true;
}
av_bool CAvFtp::Upload(std::string FileName, av_uchar *data, av_u32 datalen)
{
	int ret = 0;

	if (m_login == av_false)
	{
		ret = m_Ftp.FtpLogin();
		if (ret < 0)
		{
			av_error("ftp login error...\n");
			return av_false;
		}

		m_login = av_true;
	}

	ret = m_Ftp.FtpType(FTT_BINEARY);
	if (ret < FTP_OK)
	{
		av_error("FtpType error...\n");
		return av_false;
	}
	
	ret = m_Ftp.FtpPassive();
	if (ret < FTP_OK)
	{
		av_error("FtpPassive error...\n");
		return av_false;
	}

	ret = m_Ftp.FtpAppend(FileName.c_str());
	if (ret < FTP_OK)
	{
		av_error("FtpAppend error...\n");
		return av_false;
	}

	ret = m_Ftp.FtpStor(FileName.c_str());
	if (ret < FTP_OK)
	{
		av_error("FtpStor error...\n");
		return av_false;
	}

	ret = m_Ftp.FtpSendData((const char *)data, (int &)datalen);
	if (ret < FTP_OK)
	{
		av_error("ftp send data error...\n");
		m_Ftp.FtpCloseData();
		return av_false;
	}

	m_Ftp.FtpCloseData();

	return av_true;
}

av_bool CAvFtp::Mkdir(std::string dirName)
{
	int ret = 0;

	if (m_login == av_false)
	{
		ret = m_Ftp.FtpLogin();
		if (ret < 0)
		{
			av_msg("ftp login error...\n");
			return av_false;
		}

		m_login = av_true;
	}
	
	ret = m_Ftp.FtpMkDirRecursion((char *)dirName.c_str());
	if (ret < FTP_OK)
	{
		av_msg("FtpMkDirRecursion error...\n");
		return av_false;
	}
	
	return av_true;
}
av_bool CAvFtp::CdDir(std::string dirName)
{
	int ret = 0;

	if (m_login == av_false)
	{
		ret = m_Ftp.FtpLogin();
		if (ret < 0)
		{
			av_msg("ftp login error...\n");
			return av_false;
		}

		m_login = av_true;
	}

	ret = m_Ftp.FtpReturnRoot();
	if (ret < FTP_OK)
	{
		av_msg("FtpReturnRoot error...\n");
		return av_false;
	}

	ret = m_Ftp.FtpChangePath((char *)dirName.c_str());
	if (ret < FTP_OK)
	{
		ret = m_Ftp.FtpMkDirRecursion((char *)dirName.c_str());
		if (ret < FTP_OK)
		{
			av_msg("FtpMkDirRecursion error...\n");
			return av_false;
		}

		ret = m_Ftp.FtpChangePath((char *)dirName.c_str());
		if (ret < FTP_OK)
		{
			av_msg("FtpChangePath error...\n");
			return av_false;
		}
	}
	
	return av_true;
}