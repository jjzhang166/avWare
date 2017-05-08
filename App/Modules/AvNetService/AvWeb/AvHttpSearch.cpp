#include "AvNetService/AvHttpSearch.h"
#include "AvConfigs/AvConfigManagerNetWork.h"
#include "jsoncpp/json.h"
#include "AvThread/AvThread.h"

SINGLETON_IMPLEMENT(CHttpSearchServer);

av_char* CHttpSearchServer::SearchType[TYPE_NR] = { "ALL", "IPC", "NVR", "PLATFORM" };
CHttpSearchServer::CHttpSearchServer()
	:serverSocketFd_(-1),
	thread_run_(av_false)
{
}


CHttpSearchServer::~CHttpSearchServer()
{

}

av_int CHttpSearchServer::Start()
{
	if (thread_run_) return -1;

	thread_run_ = av_true;
	if (0 != pthread_create(&pid_, NULL, CHttpSearchServer::ThreadProcProxy, (void*)this)) {
		av_error("0 != pthread_create(&pid_, NULL, CHttpSearchServer::ThreadProcProxy, (void*)this)\n");
		assert(0);
	}

	return 0;
}

void CHttpSearchServer::Stop()
{
	if (!thread_run_) return;

	thread_run_ = av_false;
}

int CHttpSearchServer::NetMulicastServerSocketInit(const char *mulicastip, const av_u16 mulicastPort)
{
	struct sockaddr_in peeraddr;
	int sockfd;
	unsigned int socklen;
	struct ip_mreq mreq;
	char  one = 1; 
    const int sock_opt = 1; 
	
	/* 创建 socket 用于UDP通讯 */
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd < 0) {
		av_error("socket creating err in udptalk\n");
		return -1;
	}

	/* 设置要加入组播的地址 */
	memset(&mreq, 0,sizeof(struct ip_mreq));
	mreq.imr_multiaddr.s_addr = inet_addr(mulicastip);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);

	 // 设置 允许重用本地地址和端口
 	if((setsockopt(sockfd, SOL_SOCKET,SO_REUSEADDR, (char*)&sock_opt,  sizeof  (sock_opt)))== -1) { 
		av_error("setsockopt %s\n", strerror(errno));
		return -1;
     }
	 
    if  ((setsockopt(sockfd, IPPROTO_IP,IP_MULTICAST_LOOP, &one,  sizeof  (unsigned  char ))) == -1){ 
		av_error("setsockopt IP_MULTICAST_LOOP %s\n", strerror(errno));
		return -1;
     }

	/* 把本机加入组播地址，即本机网卡作为组播成员，只有加入组才能收到组播消息 */
	if (setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (const char *)&mreq,sizeof(struct ip_mreq)) < 0) {
		av_error("setsockopt Add Membership %s\n", strerror(errno));
		return -1;
	}

	socklen = sizeof(struct sockaddr_in);
	memset(&peeraddr, 0, socklen);
	peeraddr.sin_family = AF_INET;
	peeraddr.sin_port = htons(mulicastPort);
	peeraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	/* 绑定自己的端口和IP信息到socket上 */
	if (bind(sockfd, (struct sockaddr *) &peeraddr,sizeof(struct sockaddr_in)) != 0 ){
		av_error("bind %s\n", strerror(errno));
		return -1;
	}
	
	return sockfd;
}

void* CHttpSearchServer::ThreadProcProxy(void *param)
{
	CHttpSearchServer *pthiz = static_cast<CHttpSearchServer*>(param);
	pthiz->ThreadProc();
	return NULL;
}

av_bool CHttpSearchServer::codec(const char* recv_buff, std::string& resp_buff)
{
	av_msg("recv_buff[%s]\n", recv_buff);

	resp_buff.clear();
	//TODO: 分析数据包 回包
	Json::Value recv_msg;
	Json::Reader recv_msg_reader;
	if (false == recv_msg_reader.parse(recv_buff, recv_msg)) {
		av_error("recv[%s] not vaild json\n", recv_buff);
		return av_false;
	}

	Json::Value header = recv_msg["Header"];
	if (!header["Method"].isString() && !header["Method"].asString().compare("Discover")) {
		return av_false;
	}

	Json::Value recv_param = recv_msg["Param"];
	int type;
	if (recv_param["Type"].isString()) {
		for (type = TYPE_ALL; type < TYPE_NR; type++) {
			if (0 == recv_param["Type"].asString().compare(SearchType[type])) {
				break;
			}
		}
		if (TYPE_NR == type) {
			return av_false;
		}
	}

	CAvConfigNetRtsp rtsp;
	rtsp.Update();
	ConfigNetRtsp &conf_rtsp = rtsp.GetConfig();

	CAvConfigNetComm netComm;
	netComm.Update();
	ConfigNetComm conf_net = netComm.GetConfig();
	//FIXME: 1,web 端口写死80, IP端口写死了用LAN0的地址， 设备写死了IPC
	char buf[512];
	sprintf(buf,"{\"Header\":{\"Action\":\"RePly\",\"Method\":\"Discover\",\"Status\":\"OK\"},\"Param\":{\"IP\":\"%s\",\"Port\":%d,\"RtspPort\":%d,\"Type\":\"%s\"}}",
		conf_net.LanAttr.IpAddr, 
		80,
		conf_rtsp.ServicePort,
		"IPC");

	resp_buff.assign(buf);
	return av_true;
}

void CHttpSearchServer::NetUdpSendMsg(const struct sockaddr *addr, const char *msg, int len)
{
	int sockfd;
	sockfd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sockfd <= 0) {
		return;
	}
	sendto(sockfd, msg, len, 0, addr, sizeof(struct sockaddr));
#if defined(_MSC_VER)
	closesocket(sockfd);
#elif defined(__GNUC__)
	close(sockfd);
#else
	static_assert(false, "No implement");
#endif
	return;
}


//FIXME:不是线程安全，但是目前业务流程下不要紧
void CHttpSearchServer::ThreadProc()
{
	THREAD_SET_THREADNAME("CHttpSearchServer");
	while (thread_run_) {
		serverSocketFd_ = NetMulicastServerSocketInit(mulicastip_, mulicastPort);
		if (0 > serverSocketFd_ ) {
			av_error("NetMulicastServerSocketInit failed\n");
			av_msleep(10 * 1000);
		} else {
			break;
		}
	}

	struct sockaddr peeraddr;
	socklen_t socklen = sizeof(peeraddr);
	char recv_buff[512] = {0};
	std::string resp_buff;
	while (thread_run_) {
		memset(recv_buff, 0, sizeof(recv_buff));
		int recv_length = recvfrom(serverSocketFd_, recv_buff, 512, 0, &peeraddr, &socklen);
		if (recv_length < 0) {
			av_error("HttpSearchServer recvfrom < 0, %s\n", strerror(errno));
		} else {
			if (codec(recv_buff, resp_buff)) {
				NetUdpSendMsg(&peeraddr, resp_buff.c_str(), resp_buff.size());
			}
		}
	}

	if (-1 != serverSocketFd_) {
	#if defined(_MSC_VER)
		closesocket(serverSocketFd_);
	#elif defined(__GNUC__)
		close(serverSocketFd_);
	#else
		static_assert(false, "No implement");
	#endif
		serverSocketFd_ = -1;
	}
}
