/* $Id: upnpc.c,v 1.95 2012/05/02 20:14:43 nanard Exp $ */
/* Project : miniupnp
 * Author : Thomas Bernard
 * Copyright (c) 2005-2012 Thomas Bernard
 * This software is subject to the conditions detailed in the
 * LICENCE file provided in this distribution. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#ifdef _WIN32
#include <winsock2.h>
#define snprintf _snprintf
#else
/* for IPPROTO_TCP / IPPROTO_UDP */
#include <netinet/in.h>
#endif
#include "miniwget.h"
#include "miniupnpc.h"
#include "upnpcommands.h"
#include "upnperrors.h"
#include "upnplib.h"


/* protofix() checks if protocol is "UDP" or "TCP"
 * returns NULL if not */
static const char * protofix(const char * proto)
{
	static const char proto_tcp[4] = { 'T', 'C', 'P', 0};
	static const char proto_udp[4] = { 'U', 'D', 'P', 0};
	int i, b;
	for(i=0, b=1; i<4; i++)
		b = b && (   (proto[i] == proto_tcp[i])
		          || (proto[i] == (proto_tcp[i] | 32)) );
	if(b)
		return proto_tcp;
	for(i=0, b=1; i<4; i++)
		b = b && (   (proto[i] == proto_udp[i])
		          || (proto[i] == (proto_udp[i] | 32)) );
	if(b)
		return proto_udp;
	return 0;
}

static int DisplayInfos(struct UPNPUrls * urls,
                         struct IGDdatas * data, char *router_ip)
{
	char externalIPAddress[40];
	char connectionType[64];
	char status[64];
	char lastconnerr[64];
	unsigned int uptime;
	unsigned int brUp, brDown;
	time_t timenow, timestarted;
	int r;
	UPNP_GetConnectionTypeInfo(urls->controlURL,
	                           data->first.servicetype,
							   connectionType);
	if(connectionType[0])
		printf("Connection Type : %s\n", connectionType);
	else
		printf("GetConnectionTypeInfo failed.\n");
	UPNP_GetStatusInfo(urls->controlURL, data->first.servicetype,
	                   status, &uptime, lastconnerr);
	printf("Status : %s, uptime=%us, LastConnectionError : %s\n",
	       status, uptime, lastconnerr);
	timenow = time(NULL);
	timestarted = timenow - uptime;
	printf("  Time started : %s", ctime(&timestarted));
	UPNP_GetLinkLayerMaxBitRates(urls->controlURL_CIF, data->CIF.servicetype,
			&brDown, &brUp);
	printf("MaxBitRateDown : %u bps", brDown);
	if(brDown >= 1000000) {
		printf(" (%u.%u Mbps)", brDown / 1000000, (brDown / 100000) % 10);
	} else if(brDown >= 1000) {
		printf(" (%u Kbps)", brDown / 1000);
	}
	printf("   MaxBitRateUp %u bps", brUp);
	if(brUp >= 1000000) {
		printf(" (%u.%u Mbps)", brUp / 1000000, (brUp / 100000) % 10);
	} else if(brUp >= 1000) {
		printf(" (%u Kbps)", brUp / 1000);
	}
	printf("\n");
	r = UPNP_GetExternalIPAddress(urls->controlURL,
	                          data->first.servicetype,
							  externalIPAddress);
	if(r != UPNPCOMMAND_SUCCESS){
		printf("GetExternalIPAddress() returned %d\n", r);
		return -1;
	}
	if(externalIPAddress[0]){
		printf("ExternalIPAddress = %s\n", externalIPAddress);
		sprintf(router_ip, "%s", externalIPAddress);
	}else{
		printf("GetExternalIPAddress failed.\n");
		return -1;
	}
	return 0;
}

static int ListRedirections(struct UPNPUrls * urls,
                             struct IGDdatas * data, char *local_ip, char *mapped_port, char *mapped_name)
{
	int r;
	int i = 0;
	int j = 0;
	int w = 0;
	char index[6];
	char intClient[40];
	char intPort[6];
	char extPort[10];
	char protocol[4];
	char desc[80];
	char enabled[6];
	char rHost[64];
	char duration[16];
	int  mappedport[32];
	/*unsigned int num=0;
	UPNP_GetPortMappingNumberOfEntries(urls->controlURL, data->servicetype, &num);
	printf("PortMappingNumberOfEntries : %u\n", num);*/
	printf(" i protocol exPort->inAddr:inPort description remoteHost leaseTime\n");
	do {
		snprintf(index, 6, "%d", i);
		rHost[0] = '\0'; enabled[0] = '\0';
		duration[0] = '\0'; desc[0] = '\0';
		extPort[0] = '\0'; intPort[0] = '\0'; intClient[0] = '\0';
		r = UPNP_GetGenericPortMappingEntry(urls->controlURL,
		                               data->first.servicetype,
		                               index,
		                               extPort, intClient, intPort,
									   protocol, desc, enabled,
									   rHost, duration);
		if(r==0){
		/*
			printf("%02d - %s %s->%s:%s\tenabled=%s leaseDuration=%s\n"
			       "     desc='%s' rHost='%s'\n",
			       i, protocol, extPort, intClient, intPort,
				   enabled, duration,
				   desc, rHost);
				   */
			#if 1
			printf("%2d %s %5s->%s:%-5s '%s' '%s' %s\n",
			       i, protocol, extPort, intClient, intPort,
			       desc, rHost, duration);
			#endif
			
			if (0 == strcmp(intClient, local_ip) && 0 == strcmp(mapped_port, extPort)){
				sprintf (mapped_name, "%s", desc);
				sprintf (mapped_port, "%s", extPort);
				return 1;
			}else{
				mappedport[j] = atoi(extPort);
				j++;
			}
		}else{
			printf("GetGenericPortMappingEntry() returned %d (%s)\n",
			       r, strupnperror(r));
		}
		i++;
	} while(r==0);
	/*查找可映射点*/
	if (0 != atoi(mapped_port)){
		return 0;
	}
	
	for(i = 0; i < j; i++){
		if ((4000 + w) == mappedport[i]){
			w++;
			i = 0;
			continue;
		}
	}
	sprintf (mapped_port, "%d", 4000 + w);
	return 0;
}


/* Test function
 * 1 - get connection type
 * 2 - get extenal ip address
 * 3 - Add port mapping
 * 4 - get this port mapping from the IGD */
static int SetRedirectAndTest(struct UPNPUrls * urls,
                               struct IGDdatas * data,
							   const char * iaddr,
							   const char * iport,
							   const char * eport,
                               const char * proto,
                               char * mapping_name)
{
	char externalIPAddress[40];
	char intClient[40];
	char intPort[6];
	char duration[16];
	int r;

	proto = protofix(proto);
	if(!proto)
	{
		fprintf(stderr, "invalid protocol\n");
		return -1;
	}

	UPNP_GetExternalIPAddress(urls->controlURL,
	                          data->first.servicetype,
							  externalIPAddress);
	if(externalIPAddress[0])
		printf("ExternalIPAddress = %s\n", externalIPAddress);
	else
		printf("GetExternalIPAddress failed.\n");

	r = UPNP_AddPortMapping(urls->controlURL, data->first.servicetype,
	                        eport, iport, iaddr, mapping_name, proto, 0, 0);
	if(r!=UPNPCOMMAND_SUCCESS){
		printf("AddPortMapping(%s, %s, %s) failed with code %d (%s)\n",
		       eport, iport, iaddr, r, strupnperror(r));
		return -1;
	}
	r = UPNP_GetSpecificPortMappingEntry(urls->controlURL,
	                                 data->first.servicetype,
    	                             eport, proto,
									 intClient, intPort, NULL/*desc*/,
	                                 NULL/*enabled*/, duration);
	if(r!=UPNPCOMMAND_SUCCESS){
		printf("GetSpecificPortMappingEntry() failed with code %d (%s)\n",
		       r, strupnperror(r));
		return -1;
	}

	if(intClient[0]) {
		printf("InternalIP:Port = %s:%s\n", intClient, intPort);
		printf("external %s:%s %s is redirected to internal %s:%s (duration=%s)\n",
		       externalIPAddress, eport, proto, intClient, intPort, duration);
	}
	return 0;
}


/* sample upnp client program */
int Ipcam_upnpc(char *local_ip, int local_port, char*mapping_name, char *mapping_protocol, 
						char *router_ip, int *mapped_port)
{
	struct UPNPDev * devlist = 0;
	char lanaddr[64];	/* my ip address on the LAN */
	int i;
	const char * rootdescurl = 0;
	const char * multicastif = 0;
	const char * minissdpdpath = 0;
	char chlocal_port[5];
	char chmapped_port[5];
	int retcode = 0;
	int error = 0;
	int ipv6 = 0;
	int ret = 0;
	
	printf ("local port = %d mapped_port = %d\n", local_port, *mapped_port);
	
	if( rootdescurl/*mark jsk*/
	  || (devlist = upnpDiscover(2000, multicastif, minissdpdpath,
	                             0/*sameport*/, ipv6, &error)))
	{
		struct UPNPUrls urls;
		struct IGDdatas data;
		if(devlist){
		}else{
			printf("upnpDiscover() error code=%d\n", error);
			return -1;
		}
		i = 1;
		if( (rootdescurl && UPNP_GetIGDFromUrl(rootdescurl, &urls, &data, lanaddr, sizeof(lanaddr)))
		  || (i = UPNP_GetValidIGD(devlist, &urls, &data, lanaddr, sizeof(lanaddr))))
		{
			switch(i) {
			case 1:
				break;
			case 2:
				break;
			case 3:
				break;
			default:
				break;
			}
			printf("Local LAN ip address : %s\n", lanaddr);



			if (-1 == DisplayInfos(&urls, &data, router_ip)){
				FreeUPNPUrls(&urls);
				return -1;
			}
			
			
			sprintf (chlocal_port, "%d", local_port);
			printf ("##UPNPLIB  local port = %d mapped_port = %d\n", local_port, *mapped_port);
			ret = ListRedirections(&urls, &data, local_ip, chmapped_port, mapping_name);
			if (1 == ret){
				*mapped_port = atoi(chmapped_port);
				FreeUPNPUrls(&urls);
				return 1;/*已经映射*/
			}else if (-1 == ret){
				FreeUPNPUrls(&urls);
				return -1;/*查找出错*/
			}else{
				/*找到可映射点*/
			}

			ret = SetRedirectAndTest(&urls,&data,local_ip, chlocal_port,chmapped_port,mapping_protocol,mapping_name);
			*mapped_port = atoi(chmapped_port);
			FreeUPNPUrls(&urls);
			return ret;
		}
		else
		{
			fprintf(stderr, "No valid UPNP Internet Gateway Device found.\n");
			retcode = -1;
		}
		freeUPNPDevlist(devlist); 
		devlist = 0;
	}
	else
	{
		fprintf(stderr, "No IGD UPnP Device found on the network !\n");
		retcode = -1;
	}
	return retcode;
}

