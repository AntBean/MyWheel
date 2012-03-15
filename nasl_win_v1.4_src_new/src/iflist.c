/*
 * Copyright (c) 1999 - 2002
 *	Politecnico di Torino.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that: (1) source code distributions
 * retain the above copyright notice and this paragraph in its entirety, (2)
 * distributions including binary code include the above copyright notice and
 * this paragraph in its entirety in the documentation or other materials
 * provided with the distribution, and (3) all advertising materials mentioning
 * features or use of this software display the following acknowledgement:
 * ``This product includes software developed by the Politecnico
 * di Torino, and its contributors.'' Neither the name of
 * the University nor the names of its contributors may be used to endorse
 * or promote products derived from this software without specific prior
 * written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#include <pcap.h>
#include <packet32.h>
#ifndef WIN32
#include <sys/socket.h>
#include <netinet/in.h>
#else
#include <winsock.h>
#endif

void ifprint(int n, pcap_if_t *d);
char *iptos(u_long in);

int iflist()
{
  pcap_if_t *alldevs;
  pcap_if_t *d;
  char errbuf[PCAP_ERRBUF_SIZE+1];
  int i;

  printf("WinPCap version: %s\n", PacketGetVersion());

  // Retrieve the interfaces list 
  if (pcap_findalldevs(&alldevs, errbuf) == -1)
  {
    fprintf(stderr, "Error in pcap_findalldevs: %s\n", errbuf);
    return 0;
  }

  // Scan the list printing every entry 
  for (i=0,d=alldevs; d; i++,d=d->next)
  {
    ifprint(i, d);
  }

  return i;
}

// Print all the available information on the given interface 
void ifprint(int n, pcap_if_t *d)
{
  pcap_addr_t *a;

  // Name 
  printf("%d - %s\n", n+1, d->name);

  // Description 
  if (d->description)
    printf("\tDescription: %s\n", d->description);

  // Loopback Address
  printf("\tLoopback: %s\n", (d->flags & PCAP_IF_LOOPBACK)?"yes":"no");

  // IP addresses 
  for (a=d->addresses; a; a=a->next) {
    printf("\tAddress Family: #%d\n", a->addr->sa_family);
  
    switch (a->addr->sa_family)
    {
      case AF_INET:
        printf("\tAddress Family Name: AF_INET\n");
        if (a->addr)
          printf("\tAddress: %s\n", iptos(((struct sockaddr_in *)a->addr)->sin_addr.s_addr));
        if (a->netmask)
          printf("\tNetmask: %s\n", iptos(((struct sockaddr_in *)a->netmask)->sin_addr.s_addr));
        if (a->broadaddr)
          printf("\tBroadcast Address: %s\n", iptos(((struct sockaddr_in *)a->broadaddr)->sin_addr.s_addr));
        if (a->dstaddr)
          printf("\tDestination Address: %s\n", iptos(((struct sockaddr_in *)a->dstaddr)->sin_addr.s_addr));
        break;
      default:
        printf("\tAddress Family Name: Unknown\n");
        break;
    }
  }
  printf("\n");
}

