#ifndef _ICMP_H_
#define _ICMP_H_



class CICMPPing
{
public:
	CICMPPing();
	~CICMPPing();

public:
	bool SetRemoteHost(const char *Host);
	bool SetTimeOut(int us);
	int	 Ping(int count = 1);

private:
	int		SendICMP();
	int		RecvICMP();
private:
	int		m_nID;
	int		m_nSeq;
private:
	char	m_HostIpAddr[32];
	int		m_TimeOutUs;
	int		m_RawSocket;
};



#endif