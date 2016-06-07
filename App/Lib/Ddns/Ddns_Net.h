#ifndef _DDNS_PUBLIC_H
#define _DDNS_PUBLIC_H

int Ddns_SendPack(int SocketFd, char *buf, int buflen, int waitsec);
int Ddns_RecvPack(int SocketFd, char *buf, int buflen, int waitsec);
int Ddns_GetOutIP(char *pOutIp);


#endif //_DDNS_PUBLIC_H

