#ifndef _UPNPLIB_H_
#define _UPNPLIB_H_

#ifdef __cplusplus
extern "C" {
#endif 

int Ipcam_upnpc(char *local_ip, int local_port, char*mapping_name, char *mapping_protocol, 
						char *router_ip, int *mapped_port);
#ifdef __cplusplus
}
#endif

#endif

