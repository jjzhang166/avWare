#ifndef _ARP_H_
#define _ARP_H_




class CArp
{
public:
	typedef struct {
		char ConflictIpAddr[32];
		char ConflictMac[32];
	}C_ConflictInfo;

public:
	CArp();
	~CArp();

	bool BroadCaseArp(const char *LocalIpAddr, const char *LocalMac);
	bool WaitArpAck(int TmOutMs, C_ConflictInfo ConflitInfo[], int &ConflitCount);

private:
	int m_RawSock;
};











#endif

