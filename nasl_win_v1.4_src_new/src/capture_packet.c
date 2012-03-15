/* Nessus Attack Scripting Language 
 *
 * Copyright (C) 2002 - 2004 Tenable Network Security
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2,
 * as published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */
#include "includes.h"
#include <pcap.h>
#include <packet32.h>

#include "capture_packet.h"
#include "nasl_raw.h"

static char if_default[1024] = {0};

extern int islocalhost(struct in_addr *);




/*
 * Set up the pcap filter, and select the correct dev_interface.
 *
 * The filter will be changed only if this is necessary
 * 
 */
 
int init_capture_device(struct in_addr src, struct in_addr dst, char * filter)

{
 int ret = -1;
 char * dev_interface = NULL;
 char * a_dst, *a_src;
 char errbuf[PCAP_ERRBUF_SIZE];
 int free_filter = 0;
 
 a_src = estrdup(inet_ntoa(src));
 a_dst = estrdup(inet_ntoa(dst));
 
 if((filter == NULL) || (filter[0]=='\0') || (filter[0]=='0'))
 {
  filter = emalloc(256);
  free_filter = 1;
  if(islocalhost(&src) == 0)
  	snprintf(filter, 256-1, "ip and (src host %s and dst host %s)",
 		a_src, a_dst);
		  
 }
 else {
 	if(islocalhost(&src) == 0)filter = estrdup(filter);
	else filter = emalloc(1);
	free_filter = 1;
 	}
 
 efree(&a_dst);
 efree(&a_src);

 if (if_default && strlen(if_default))
     ret = bpf_open_live(if_default, filter);
 else if (setif(100)) {
     ret = bpf_open_live(if_default, filter);
 }
 else {
    dev_interface = pcap_lookupdev(errbuf);  
    sprintf(dev_interface, "%S", dev_interface);
    ret = bpf_open_live(dev_interface, filter);
 }
     
 if(free_filter != 0)efree(&filter);
 return ret;   
}



struct ip * capture_next_packet(int bpf, int timeout, int * sz)
{
  int len;
  int dl_len;
  char * packet = NULL;
  char * ret = NULL;
  DWORD past, now, then;
 
  if(bpf < 0)
   return NULL;
   
  dl_len =  get_datalink_size(bpf_datalink(bpf));
  if(timeout<1000) timeout*=1000;
  past = GetTickCount();
  for(;;)
  {
   then = past;
   packet = (char*)bpf_next(bpf, &len);
   if(packet != NULL)
     break;
   now = GetTickCount();
  
   if(timeout > 0)
   {
    if((now - past) >= timeout)
    	break;
    }
   else break;
   }	
  
  
  if(packet != NULL)
  {
   struct ip * ip;
   ip = (struct ip *)(packet + dl_len);
#ifdef BSD_BYTE_ORDERING
   ip->ip_len = ntohs(ip->ip_len);
   ip->ip_off = ntohs(ip->ip_off);
#endif   
   ip->ip_id = ntohs(ip->ip_id);
   ret = emalloc(len - dl_len);
   bcopy(ip, ret, len -  dl_len);
   if(sz != NULL)*sz = len - dl_len;
  }
 return((struct ip*)ret);
}

int
get_datalink_size(datalink)
 int datalink;
{
 int offset = -1;
 switch(datalink) {
  case DLT_EN10MB: offset = 14; break;
  case DLT_IEEE802: offset = 22; break;
  case DLT_NULL: offset = 4; break;
  case DLT_SLIP:
#if (FREEBSD || OPENBSD || NETBSD || BSDI)
    offset = 16;
#else
    offset = 24; /* Anyone use this??? */
#endif
    break;
  case DLT_PPP: 
#if (FREEBSD || OPENBSD || NETBSD || BSDI)
    offset = 4;
#else
#ifdef SOLARIS
    offset = 8;
#else
    offset = 24; /* Anyone use this? */
#endif /* ifdef solaris */
#endif /* if freebsd || openbsd || netbsd || bsdi */
    break;
  case DLT_RAW: offset = 0; break;
  }
  return(offset);
}

char *getif(char *device)
{
    if (!strlen(if_default)) setif(100);
    strcpy(device, if_default);
    return device;
}

void setif_byname(char *device)
{
    strcpy(if_default, device);
}

int setif(int num)
{
    pcap_if_t *alldevs;
    pcap_if_t *d;
    char errbuf[PCAP_ERRBUF_SIZE+1];
    char szLine[1024] = {0};
    char szFullLine[1024] = {0};
    int i = 0;

    printf("WinPCap version: %s\n", PacketGetVersion());

    // Retrieve the interfaces list 
    if (pcap_findalldevs(&alldevs, errbuf) == -1) {
        fprintf(stderr, "Error in pcap_findalldevs: %s\n", errbuf);
        return 0;
    }

    // Scan the list printing every entry 
    for (i=0,d=alldevs; d; i++,d=d->next) {
        pcap_addr_t *a;
        for (a=d->addresses; a; a=a->next) {
            if (a->addr->sa_family != AF_INET) continue;
            strcpy(if_default, d->name);
            if (i+1 == num || num <= 0) {
#ifdef _DEBUG
                fprintf(stdout, "Set network interface to:\n%d - %s (%s)\n\n", i+1,
                    d->name,
                    iptos(((struct sockaddr_in *)a->addr)->sin_addr.s_addr));
#endif
                return i+1;
            }
        }
    }

    return i;
}
