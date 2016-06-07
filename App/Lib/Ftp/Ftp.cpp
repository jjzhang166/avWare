#include "Ftp.h"

int set_nonblock(int bNb, int s)
{
#ifdef _WIN32
	unsigned long l = bNb ? 1 : 0;
	int n = ioctlsocket(s, FIONBIO, &l);
	if (n != 0)
	{
		int errcode;
		errcode = WSAGetLastError();
		printf("ioctlsocket(FIONBIO) (%d) ", errcode);
		return FALSE;
	}
#else
	int flags = 0;
	if ((flags = fcntl(s, F_GETFL, 0)) == -1)
	{
		printf("fcntl(F_GETFL, O_NONBLOCK)");
		return FALSE;
	}

	if (bNb)
	{
		if (fcntl(s, F_SETFL, flags | O_NONBLOCK) == -1)
		{
			printf("fcntl(F_SETFL, O_NONBLOCK)");
			return FALSE;
		}
	}
	else
	{
		if (fcntl(s, F_SETFL, 0) == -1)
		{
			printf("fcntl(F_SETFL, 0)");
			return FALSE;
		}
	}
#endif

	return TRUE;
}

int FTP_close(int fd)
{
#if (defined (WINCE) || defined (WIN32))
	return closesocket(fd);
#else

	return close(fd);
#endif
}



CFtp::CFtp()
{

}

CFtp::~CFtp()
{

}
int CFtp::FtpLogin()
{
	int ret  = FTP_ERROR;
    char recv_buf[1500];
	int state = 0;
	char tmpBuf[256];

    ret = FtpConnect();
    if(ret < 0)
    {
        printf("ftp>: Connect Ftp Server Failed-->!\n");
        return FTP_CONNECT_FAILED;
    }

	ret = FtpSendAndGet("USER ", m_Username.c_str(), recv_buf, state);
    FTP_ASSERT_RETURN( (ret >= FTP_OK), ret);
    FTP_ASSERT_RETURN( (state == 331), FTP_AUTH_FAILED);

	memset(recv_buf, 0, sizeof(recv_buf));
    state = 0;
    ret = FtpSendAndGet("PASS ", m_Passwd.c_str(), recv_buf, state);
    FTP_ASSERT_RETURN( (ret >= FTP_OK), ret);
    FTP_ASSERT_RETURN( (state == 230), FTP_AUTH_FAILED);

	/************************************************************************/
    /* SYST:查出服务类型, REST:从文件一偏移位置下载                         */
    /************************************************************************/
    login_success = true;
    
    ret = FtpPwd( tmpBuf );
    FTP_ASSERT_RETURN( (ret >= FTP_OK), ret);

    Initial_dir = tmpBuf;
    return FTP_OK;
}

int CFtp::FtpQuit()
{
	char recv_buf[1500];
    int state;
    int ret = FTP_ERROR;

    state = 0;

	if(login_success)
    {
        ret = FtpSendAndGet("QUIT", "", recv_buf, state);
        if( state != 221 ||  ret < FTP_OK )
        {
            printf("CFtpProtocol>: FtpQuit failed, socket[%d]\n", m_Ctrlsocket);
            ret = FTP_ERROR;
        }
    }

	if(m_Ctrlsocket > 0)
    {
        FTP_close(m_Ctrlsocket);//关闭控制通道
        m_Ctrlsocket = -1;
    }
    if(m_Datasocket > 0)
    {
        FTP_close(m_Datasocket);//关闭数据通道
        m_Datasocket = -1;
    }
	
    return ret;
}

int CFtp::FtpSetConf(std::string Server, int Port, std::string Usrname, std::string Passwd)
{
	if (0 == Server.size() || 0 == Usrname.size() || 0 == Passwd.size() || Port < 0)
	{
		printf("input param is error...\n");
		return FTP_ERROR;
	}

	m_ServerIP = Server;
	m_ServerPort = Port;
	m_Username = Usrname;
	m_Passwd = Passwd;

	return FTP_OK;
}

int CFtp::FtpCloseData()
{
	int ret = FTP_ERROR;
    int state;
    char recv_buf[1500];

    if( m_Datasocket <= 0)
    {
        printf("CFtpProtocol: ftp data channel is not open\n");
        return FTP_ERROR;
    }
    else
    {
        FTP_close(m_Datasocket);
        m_Datasocket = -1;
    }

    state = -1;
    ret = FtpSendAndGet("", "", recv_buf, state);
    FTP_ASSERT_RETURN( (ret >= FTP_OK), ret);
    FTP_ASSERT_RETURN( (state == 226), FTP_ERROR);    //验证返回码

    return FTP_OK;
}

int CFtp::FtpSendData(const char * buf, int& buflen)
{
	struct timeval timetmp;
    int ret;
    char *pSend = (char*)buf;
    int sendlen = 0;
    int errCount = 0;
	//printf("buf");
	if( m_Datasocket <= 0)
    {
        return FTP_CONNECT_FAILED;
    }

	while(buflen > sendlen)
	{
		timetmp.tv_sec = FTP_SEND_TIME/1000;
        timetmp.tv_usec = 0;

        fd_set writefd;

        FD_ZERO(&writefd);
        if(m_Datasocket <= 0)
        {
            return FTP_ERROR;
        }
        FD_SET(m_Datasocket, &writefd);

        ret = select(m_Datasocket+1, NULL, &writefd, NULL, &timetmp);

		if(ret > 0)
        {
            int iWantSendLen = buflen-sendlen;
            if (iWantSendLen >  4*1024)
            {
                iWantSendLen = 4*1024;
            }

            ret=send(m_Datasocket, pSend, iWantSendLen, 0);
            FTP_ASSERT_RETURN( (ret > 0), FTP_ERROR);
            sendlen += ret;
            pSend += ret;
        }
		else
		{
#ifdef __linux__
			if (errno == EINTR)
#else
			if (WSAGetLastError() == WSAEINTR)
#endif
            {
                printf("FtpSendData %s %d\n",__FILE__, __LINE__);
                if(errCount++ < 10)
                {
                	continue;
                }
            }
			
			return FTP_SEND_TIMEOUT;
		}
	}

	return FTP_OK;
}

int CFtp::FtpRecvData(char * buf, int& buflen)
{
	FtpRecv(m_Datasocket, buf, buflen, FTP_RECV_TIME);

	return 0;
}


int CFtp::FtpConnect()
{
	struct sockaddr_in sin;    
    int on = 1;
    int iRetCode = -1;
    const int optval = 1;
	struct hostent* host;
	char recv_buf[1500];
    int state = -1;

	m_Ctrlsocket = socket(AF_INET, SOCK_STREAM, 0) ;
	FTP_ASSERT_RETURN( (m_Ctrlsocket > 0), FTP_ERROR);

	iRetCode = setsockopt(m_Ctrlsocket,SOL_SOCKET,SO_REUSEADDR,(const char *)&on,sizeof(on));
    FTP_ASSERT_RETURN( (iRetCode >= 0), FTP_ERROR);

	if (setsockopt(m_Ctrlsocket, IPPROTO_TCP, TCP_NODELAY,(char *)&optval, sizeof(optval)) < 0)
    {
        printf("error setsockopt nodelay");
        FTP_ASSERT_RETURN( 0, FTP_ERROR );
    }

	memset(&sin,0,sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(m_ServerPort);

	host = gethostbyname( (char*)m_ServerIP.c_str() );
	if(host == NULL)
	{
		return -1;
	}
	sin.sin_addr.s_addr = *(unsigned long *)host->h_addr_list[0];

	iRetCode = connect(m_Ctrlsocket,(struct sockaddr *)(&sin),sizeof(sin));
	if (iRetCode < 0)
	{
		printf("FTP:Connet[%s][%d] Fail\n",m_ServerIP.c_str(),m_ServerPort);
		FTP_close(m_Ctrlsocket);
		return FTP_CONNECT_FAILED;
	}

	set_nonblock(TRUE, m_Ctrlsocket);
	iRetCode = FtpSendAndGet("", "", recv_buf, state);
    FTP_ASSERT_RETURN( (iRetCode >= FTP_OK), iRetCode);
    FTP_ASSERT_RETURN( (state == 220), FTP_ERROR);
	
    return FTP_OK;
}

int CFtp::FtpSend(char * buf, int buflen)
{
	int ret = -1;
    int sendlen = 0;

    struct timeval timetmp;

    fd_set writefd;

	while( sendlen < buflen )
    {
        FD_ZERO(&writefd);
        if(m_Ctrlsocket<= 0)
        {
            return FTP_ERROR;
        }
        FD_SET(m_Ctrlsocket, &writefd);

        timetmp.tv_sec = FTP_SEND_TIME/1000;
        timetmp.tv_usec = 0;

        ret = select(m_Ctrlsocket+1, NULL, &writefd, NULL, &timetmp);
        if(ret > 0)
        {

    #ifdef FTP_DBG_PROTOCOL
            FTP_Printf("ftp>:send: %s\n", buf);
    #endif
            ret=send(m_Ctrlsocket,buf,buflen,0);
            FTP_ASSERT_RETURN( (ret > 0), FTP_ERROR);
            sendlen += ret;
        }
        else
        {
            return FTP_SEND_TIMEOUT;
        }

    }

	return FTP_OK;
}

int CFtp::FtpRecv(char * buf, int &buflen, int timeout)
{
	struct timeval timetmp;
    int ret;

    timetmp.tv_sec = timeout/1000;
    timetmp.tv_usec = (timeout%1000)*1000;

    fd_set readfd;

    FD_ZERO(&readfd);
    if(m_Ctrlsocket<= 0)
    {
    	return FTP_ERROR;
    }
    FD_SET(m_Ctrlsocket, &readfd);

	ret = select(m_Ctrlsocket+1, &readfd, NULL, NULL, &timetmp);
	//printf("===ret = %d\n",ret);
    if(ret > 0)
    {
    	
        ret = recv(m_Ctrlsocket, buf, FTP_MAX_LEN-1, 0);
		if(0 == ret)
		{
			if(m_Ctrlsocket > 0)
		    {
		        FTP_close(m_Ctrlsocket);//关闭控制通道
		        m_Ctrlsocket = -1;
		    }
		    if(m_Datasocket > 0)
		    {
		        FTP_close(m_Datasocket);//关闭数据通道
		        m_Datasocket = -1;
		    }
		}
        buf[ret] = 0;
		//printf("bbbbbbbbbbbb ret = %d\n",ret);
        FTP_ASSERT_RETURN( (ret > 0), FTP_ERROR);
        buflen = ret;
//#ifdef FTP_DBG_PROTOCOL
        printf("ftp>:recv: %s\n", buf);
//#endif
        return FTP_OK;
    }
    else if(ret == 0)
    {
        return FTP_RECV_TIMEOUT;
    }

    return FTP_ERROR;
}

int CFtp::FtpRecv(int ftp_sock, char * buf, int &buflen, int timeout)
{
	struct timeval timetmp;
    int ret;

    timetmp.tv_sec = timeout/1000;
    timetmp.tv_usec = (timeout%1000)*1000;

    fd_set readfd;

    FD_ZERO(&readfd);
    if(ftp_sock <= 0)
    {
            return FTP_ERROR;
    }
    FD_SET(ftp_sock, &readfd);

	ret = select(ftp_sock+1, &readfd, NULL, NULL, &timetmp);
    if(ret > 0)
    {
        buflen = recv(ftp_sock, buf, FTP_MAX_LEN-1, 0);
        FTP_ASSERT_RETURN( (buflen >= 0), FTP_ERROR);
#ifdef _DEBUG
        //FTP_Printf("ftp>:recv: %s\n", buf);
#endif
        return FTP_OK;
    }
    else if(ret == 0)
    {
        return FTP_RECV_TIMEOUT;
    }

    return FTP_ERROR;
}

int CFtp::FtpSendAndGet( const char * strCommand, const char * param, char * Response, int &ReponseState )
{
	int ret;
	int buflen = 0;
    char send_buf[1500];
	printf("strCommand %s\n",strCommand);
    if(ReponseState != -1)
    {
        char send_buf[1500];
        sprintf(send_buf, "%s%s\r\n", strCommand, param);
        ret = FtpSend( (char*)send_buf, (int)strlen(send_buf) );
        if(ret < 0)
        {
            printf("ftp>: ftp send failed, error[%d], content[%s]\n", ret, send_buf);
            //return FTP_ERROR;
            //在这不直接返回，后面要用recv的返回值判断该如何处理
        }
    }

	ret = FtpRecv(Response, buflen, FTP_RECV_TIME);
    if(ret < 0)
    {
        printf("ftp>: ftp recv failed: ");
        if(ReponseState != -1)
        {
            
            sprintf(send_buf, "%s%s\r\n", strCommand, param);
            printf("send[%s]\n", send_buf);
        }
        else
        {
            printf("\n");
        }
        return ret;
    }

    char tmpbuf[5];
    memcpy(tmpbuf, Response, 4);
    tmpbuf[4]=0;
    ReponseState = atoi(tmpbuf);

	return FTP_OK;
}

int CFtp::FtpChangePath(char * szPath)
{
	int state;
    int ret = FTP_ERROR;
    char recv_buf[1500];

    if (NULL == szPath)
    {
        return FTP_ERROR;
    }

    state = 0;
    ret = FtpSendAndGet("CWD ", szPath, recv_buf, state);
    FTP_ASSERT_RETURN( (ret >= FTP_OK), ret);
    FTP_ASSERT_RETURN( (state == 250), FTP_ERROR);
    return FTP_OK;
}

int CFtp::FtpMkDir(char * szDir)
{
	char recv_buf[1500];
    int state;
    int ret = FTP_ERROR;

    state = 0;
    ret = FtpSendAndGet("MKD ", szDir, recv_buf, state);
    FTP_ASSERT_RETURN( (ret >= FTP_OK), ret);
    FTP_ASSERT_RETURN( (state == 257 || state == 550), FTP_ERROR); //550状态:示执行请求操作,说明目录已存在

    return FTP_OK;
}

int CFtp::FtpMkDirRecursion(char * szDir)
{
	int ret = FTP_ERROR;
    int count = 0; //记录目录的级数

    char tmpDir[260];
    memset(tmpDir, 0, sizeof(tmpDir));
    char * p1, *p2;

    strcpy(tmpDir, szDir);
    printf("CFtpProtocol:> tmpDir[%s]\n", tmpDir);
    p1 = tmpDir;
    
    //移除字符串首的可能的"."或"/"字符
    while(1)
    {
        if(*p1 == '.' || *p1 == '/')
        {
            *p1 = 0;
            p1++;
            continue;
        }
        else
        {
            p2 = p1;
            break;
        }
    }

	while (1)
    {
        p1++;
        if( *p1 == '/' || *p1 == 0 )
        {
            *p1 = 0;
            p1 ++;
			int changePath_flag = 0;	// 1表示已经进入该路径
			int i =0;
		    do		 
            {
				ret = FtpChangePath(p2);
				if((!ret)&& (!changePath_flag))
				{
					printf(" ftp changed path [%s]\n",p2);
					i++;
					changePath_flag = 1;
					break;
				}
	    		ret = FtpMkDir(p2);
	  			printf(" ftp maked path [%s]\n",p2);
				i++;		
            }while( ret != FTP_OK  && i < 4 );
	        FTP_ASSERT_RETURN( (ret >= FTP_OK), ret);
            if(*p1 == 0)
            {
                break; //字符串分析完了, 目录也建完了
            }
			if(!changePath_flag)
			{
            	ret = FtpChangePath(p2);
	            FTP_ASSERT_RETURN( (ret >= FTP_OK), ret);
			}
            count++;
            p2 = p1;
        }
    }

	//退回到初始建立目录的地方
    while (count--)
    {
        ret = FtpChangePath((char *)"..");
        FTP_ASSERT_RETURN( (ret >= FTP_OK), ret);
    }

	return FTP_OK;
}

int CFtp::FtpPassive()
{
	char recv_buf[1500];
    int state;
    int ret = FTP_ERROR;
    const int optval = 1;
	char * buf_ptr;
    int port_num;
    
    //进入passive模式前,先关闭其它的数据连接, 本ftp协议目前只支持一条数据连接
    if(m_Datasocket != -1)
    {
        FTP_close(m_Datasocket);
        m_Datasocket = -1;
    }

    state = 0;
    ret = FtpSendAndGet("PASV", "", recv_buf, state);
    FTP_ASSERT_RETURN( (ret >= FTP_OK), ret);
    FTP_ASSERT_RETURN( (state == 227), FTP_ERROR);    //验证返回码

	buf_ptr = strrchr(recv_buf, ',');
    *buf_ptr = '\0';
    port_num = atoi(buf_ptr + 1);

    buf_ptr = strrchr(recv_buf, ',');
    *buf_ptr = '\0';
    port_num += atoi(buf_ptr + 1) * 256;

	//开始建立数据连接
    struct sockaddr_in szTmpAddr;
    int on = 1;
    m_Datasocket = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if (m_Datasocket == -1)
    {
        printf("FTP:DataSocket Fail\n");
        FTP_close(m_Datasocket);
        return -1;
    }
	
	if (m_Ctrlsocket == -1)
    {
        printf("FTP:m_Ctrlsocket Fail\n");
        FTP_close(m_Ctrlsocket);
        return -1;
    }

	if (setsockopt(m_Ctrlsocket, IPPROTO_TCP, TCP_NODELAY,
                   (char *)&optval, sizeof(optval)) < 0)
    {
        printf("error setsockopt nodelay");
        FTP_ASSERT_RETURN( 0, FTP_ERROR );
    }

    ret = setsockopt(m_Datasocket,SOL_SOCKET,SO_REUSEADDR,(char *)&on,sizeof(on));
    if (ret == -1)
    {
        printf("FTP:Data Setsockopt Fail\n");
        FTP_close(m_Datasocket);
        return -1;
    }

	/*数据端口地址*/
    memset(&szTmpAddr, 0,sizeof(struct sockaddr_in));
    szTmpAddr.sin_port   = htons(port_num);
    szTmpAddr.sin_family = AF_INET;
    szTmpAddr.sin_addr.s_addr = inet_addr((char*)m_ServerIP.c_str()); 

    ret = connect(m_Datasocket, (struct sockaddr *)&szTmpAddr, sizeof(szTmpAddr));
    if ( ret < 0)
    {
        printf("FTP:DataSocket Connect[%s][%d] Fail\n",m_ServerIP.c_str(),port_num);
        FTP_close(m_Datasocket);
        return FTP_ERROR;
    }

    int buflen = 16*1024;
    int optlen = 4;
#ifndef WIN32
    setsockopt(m_Datasocket, SOL_SOCKET, SO_SNDBUF, &buflen, optlen);
#endif
    set_nonblock(TRUE, m_Datasocket);

    return FTP_OK;
}

int CFtp::FtpList()
{
	char recv_buf[1500];
    int state;
    int ret = FTP_ERROR;

    FtpPassive();

    state = 0;
    ret = FtpSendAndGet("list", "", recv_buf, state);
    FTP_ASSERT_RETURN( (ret >= FTP_OK), ret);
    FTP_ASSERT_RETURN( (state == 150), FTP_ERROR);    //验证返回码
    
    char buf[FTP_MAX_LEN];
    int buf_len = 0;
    ret = FtpRecv(m_Datasocket, buf, buf_len, FTP_RECV_TIME);
    FTP_ASSERT_RETURN( (ret >= FTP_OK), ret);
    
    state = -1; //标识不发送,只接收
    ret = FtpSendAndGet("", "", recv_buf, state);
    FTP_ASSERT_RETURN( (ret >= FTP_OK), ret);
    FTP_ASSERT_RETURN( (state == 226), FTP_ERROR);    //验证返回码

    return FTP_OK;
}

int CFtp::FtpReturnRoot()
{
	int ret = FTP_ERROR;

    ret = FtpChangePath( (char*)Initial_dir.c_str() );
    FTP_ASSERT_RETURN( (ret >= FTP_OK), ret);

    return FTP_OK;
}

int CFtp::FtpSize(const char *filename)
{
	char recv_buf[1500];
    int state;
    int ret = FTP_ERROR;
    int file_size = 0;

    state = 0;
    ret = FtpSendAndGet("size ", filename, recv_buf, state);
    FTP_ASSERT_RETURN( (ret >= FTP_OK), ret);

	if(550 == state)//文件不存在
	{
	     return 0;
	}
	
    if(state != 213)
    {
        return FTP_ERROR;
    }
    recv_buf[3]=0;
    file_size = atoi( recv_buf+4 );
	
    return file_size;
}

int CFtp::FtpRename(const char * sourc, const char * dst)
{
	char recv_buf[1500];
    int state;
    int ret = FTP_ERROR;

    //文件正常传送完毕， 把文件名改回原来的真实文件名
    state = 0;
    ret = FtpSendAndGet("RNFR ", sourc, recv_buf, state);
    FTP_ASSERT_RETURN( (ret >= FTP_OK), ret);
    FTP_ASSERT_RETURN( (state == 350), FTP_ERROR);

    state = 0;
    ret = FtpSendAndGet("RNTO ", dst, recv_buf, state);
    FTP_ASSERT_RETURN( (ret >= FTP_OK), ret);
    FTP_ASSERT_RETURN( (state == 250), FTP_ERROR);
    
    return FTP_OK;
}

int CFtp::FtpRest(int offset)
{
	char recv_buf[1500];
    int state;
    int ret = FTP_ERROR;

    char buf[20];
    sprintf(buf, "%d", offset);

    state = 0;
    ret = FtpSendAndGet("rest ", buf, recv_buf, state);
    FTP_ASSERT_RETURN( (ret >= FTP_OK), ret);
    FTP_ASSERT_RETURN( (state == 350), FTP_ERROR);    //验证返回码
    
    return FTP_OK;
}

int CFtp::FtpAppend(const char *filename)
{
	char recv_buf[1500];
    int state;
    int ret = FTP_ERROR;

    state = 0;
    ret = FtpSendAndGet("appe ", filename, recv_buf, state);
    FTP_ASSERT_RETURN( (ret >= FTP_OK), ret);
    if( (state < 100) &&(state >= 200) )
    {
        return FTP_ERROR;
    }

    return FTP_OK;
}

int CFtp::FtpStor(const char *filename)
{
	char recv_buf[1500];
    int state;
    int ret = FTP_ERROR;

    state = 0;
    ret = FtpSendAndGet("stor ", filename, recv_buf, state);
    FTP_ASSERT_RETURN( (ret >= FTP_OK), ret);
    if( (state < 100) &&(state >= 200) )
    {
        return FTP_ERROR;
    }

    return FTP_OK;
}

int CFtp::FtpRetr( const char *filename )
{
    char recv_buf[1500];
    int state;
    int ret = FTP_ERROR;

    state = 0;
    ret = FtpSendAndGet("retr ", filename, recv_buf, state);
    FTP_ASSERT_RETURN( (ret >= FTP_OK), ret);
    if( (state < 100) &&(state >= 200) )
    {
        return FTP_ERROR;
    }

    return FTP_OK;
}

int CFtp::FtpType( const FTP_TRAN_TYPE_E type_in )
{
    char recv_buf[1500];
    int state;
    int ret = FTP_ERROR;

    if(type_in == FTT_BINEARY)
    {
        state = 0;
        ret = FtpSendAndGet("TYPE ", "I", recv_buf, state);
        FTP_ASSERT_RETURN( (ret >= FTP_OK), ret);
        FTP_ASSERT_RETURN( (state == 200), FTP_ERROR);    //验证返回码
    }
    else
    {
        printf("CFtpProtocol: no do type\n");
        return FTP_ERROR;
    }

    return FTP_OK;
}

int CFtp::FtpPwd( char * cur_dir )
{
	char recv_buf[1500];
    int state;
    int ret = FTP_ERROR;

    ret = FtpSendAndGet("PWD", "", recv_buf, state);
    FTP_ASSERT_RETURN( (ret >= FTP_OK), ret);
    FTP_ASSERT_RETURN( (state == 257), FTP_ERROR);

    char *pstart, *pend;

    pstart =  strchr( recv_buf, 0x22);//0x22 是字符"
    FTP_ASSERT_RETURN( (pstart != NULL), FTP_ERROR);
    *pstart = 0;
    pstart ++;
    pend = strchr( pstart, 0x22);
    FTP_ASSERT_RETURN( (pend != NULL), FTP_ERROR);
    *pend = 0;

    strcpy(cur_dir, pstart);

    return FTP_OK;
}


