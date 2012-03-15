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
#include "nasl_raw.h"
#include <ws2tcpip.h>

#include "nasl_tree.h"
#include "nasl_global_ctxt.h"
#include "nasl_func.h"
#include "nasl_var.h"
#include "nasl_lex_ctxt.h"
#include "exec.h"

#include "nasl_debug.h"
#include <pcap.h>
#include "capture_packet.h"
//#include "strutils.h"
#include "nasl_packet_forgery.h"

/*--------------[ cksum ]-----------------------------------------*/

/*
 * Checksum routine for Internet Protocol family headers (C Version)
 * From ping examples in W.Richard Stevens "UNIX NETWORK PROGRAMMING" book.
 */
static int np_in_cksum(p,n)
u_short *p; int n;
{
  register u_short answer;
  register long sum = 0;
  u_short odd_byte = 0;

  while( n > 1 )  { sum += *p++; n -= 2; }

  /* mop up an odd byte, if necessary */
  if( n == 1 ) {
      *(u_char *)(&odd_byte) = *(u_char *)p;
      sum += odd_byte;
  }

  sum = (sum >> 16) + (sum & 0xffff);	/* add hi 16 to low 16 */
  sum += (sum >> 16);			/* add carry */
  answer = (int)~sum;			/* ones-complement, truncate*/
  return (answer);
}



/*--------------[ IP ]--------------------------------------------*/



tree_cell* forge_ip_packet(lex_ctxt* lexic)
{
  tree_cell	*retc;
  struct ip	*pkt;
  char		*s;
  struct arglist * script_infos = lexic->script_infos;
  struct in_addr * dst_addr;
  char * data;
  int data_len;
  
  
  
  dst_addr = plug_get_host_ip(script_infos);
  
  if( dst_addr == NULL )
  	return NULL;
  
  data = get_str_local_var_by_name(lexic, "data");
  data_len = get_local_var_size_by_name(lexic, "data");
  
  
  retc = alloc_tree_cell(0, NULL);
  retc->type = CONST_DATA;
  retc->size = sizeof(struct ip) + data_len;
 
  pkt = (struct ip*)emalloc(sizeof(struct ip) + data_len);
  retc->x.str_val = (char*)pkt;
   
  pkt->ip_hl = get_int_local_var_by_name(lexic, "ip_hl", 5);
  pkt->ip_v = get_int_local_var_by_name(lexic, "ip_v", 4);
  pkt->ip_tos = get_int_local_var_by_name(lexic, "ip_tos", 0);
 /* pkt->ip_len = FIX(get_int_local_var_by_name(lexic, "ip_len", 20 + data_len));*/

  pkt->ip_len = FIX(20 + data_len);
  
  pkt->ip_id = htons(get_int_local_var_by_name(lexic, "ip_id", rand()));
  pkt->ip_off = get_int_local_var_by_name(lexic, "ip_off", 0);
  pkt->ip_off = FIX(pkt->ip_off);
  pkt->ip_ttl = get_int_local_var_by_name(lexic, "ip_ttl", 64);
  pkt->ip_p = get_int_local_var_by_name(lexic, "ip_p", 0);
  pkt->ip_sum = htons(get_int_local_var_by_name(lexic, "ip_sum", 0));
  /* source */
  s = get_str_local_var_by_name(lexic, "ip_src");
  if (s != NULL)
    inet_aton(s, &pkt->ip_src);
  /* else this host address? */

  /* I know that this feature looks dangerous, but anybody can edit an IP 
   * packet with the string functions */
  s = get_str_local_var_by_name(lexic, "ip_dst");
  if (s != NULL)
    inet_aton(s, &pkt->ip_dst);
  else
  pkt->ip_dst.s_addr = dst_addr->s_addr; 
  
  if( data != NULL )
  {
   bcopy(data, retc->x.str_val + sizeof(struct ip), data_len);
  }
    

  if(! pkt->ip_sum)
    {
      if(get_int_local_var_by_name(lexic, "ip_sum", -1) < 0)
	pkt->ip_sum = np_in_cksum((u_short *)pkt, sizeof(struct ip));
    }

  return retc;
}


tree_cell * get_ip_element(lex_ctxt * lexic)

{
  tree_cell * retc;
  struct ip * ip = (struct ip*)get_str_local_var_by_name(lexic, "ip");
  char * element = get_str_local_var_by_name(lexic, "element");
  char   ret_ascii[32] = {0};
  int    ret_int = 0;
  int    flag = 0;

  if( ip == NULL )
  {
   nasl_perror(lexic, "get_ip_element : no valid 'ip' argument!\n");
   return NULL;
  }
  
  if( element == NULL)
  {
   nasl_perror(lexic, "get_ip_element : no valid 'element' argument!\n");
   return NULL;
  }
  
  if(!strcmp(element, "ip_v"))		{ ret_int = ip->ip_v; flag ++;		}
  else if(!strcmp(element, "ip_id"))	{ ret_int = UNFIX(ip->ip_id);  flag ++ ;	}
  else if(!strcmp(element, "ip_hl"))	{ ret_int = ip->ip_hl;  flag ++;	}
  else if(!strcmp(element, "ip_tos"))	{ ret_int = ip->ip_tos; flag ++;	}
  else if(!strcmp(element, "ip_len"))	{ ret_int = UNFIX(ip->ip_len); flag ++;	}
  else if(!strcmp(element, "ip_off"))	{ ret_int = UNFIX(ip->ip_off); flag ++; }
  else if(!strcmp(element, "ip_ttl"))	{ ret_int = ip->ip_ttl; flag ++;	}
  else if(!strcmp(element, "ip_p"))	{ ret_int = ip->ip_p; flag ++;		}
  else if(!strcmp(element, "ip_sum"))	{ ret_int = UNFIX(ip->ip_sum); flag ++;	}
  
  if(flag != 0)
  {
   retc = alloc_tree_cell(0, NULL);
   retc->type = CONST_INT;
   retc->x.i_val = ret_int;
   return retc;
  }
  

  
  if(!strcmp(element, "ip_src"))     {	snprintf(ret_ascii, sizeof(ret_ascii)-1, "%s", inet_ntoa(ip->ip_src)); flag ++;}
  else if(!strcmp(element, "ip_dst")){  snprintf(ret_ascii, sizeof(ret_ascii)-1, "%s", inet_ntoa(ip->ip_dst)); flag ++;}
 
 
  if( flag == 0) {
  	printf("%s : unknown element\n", element);
	return NULL;
	}
 
 retc = alloc_tree_cell(0, NULL);
 retc->type = CONST_DATA;
 retc->size = strlen(ret_ascii);
 retc->x.str_val = estrdup(ret_ascii);
 
 return retc;
}


tree_cell * set_ip_elements(lex_ctxt * lexic)
{
 struct ip * o_pkt = (struct ip*)get_str_local_var_by_name(lexic, "ip");
 int size = get_var_size_by_name(lexic, "ip");
 tree_cell * retc = alloc_tree_cell(0, NULL);
 struct ip * pkt;
 char * s;
 
 
 
 if(o_pkt == NULL)
 {
  nasl_perror(lexic, "set_ip_elements: missing <ip> field\n");
  return NULL;
 }
 
 pkt = (struct ip*)emalloc(size);
 bcopy(o_pkt, pkt, size);
 
 
 pkt->ip_hl = get_int_local_var_by_name(lexic, "ip_hl", pkt->ip_hl);
 pkt->ip_v  = get_int_local_var_by_name(lexic, "ip_v",  pkt->ip_v);
 pkt->ip_tos= get_int_local_var_by_name(lexic, "ip_tos", pkt->ip_tos);
 pkt->ip_len= FIX(get_int_local_var_by_name(lexic, "ip_len", UNFIX(pkt->ip_len)));
 pkt->ip_id = htons(get_int_local_var_by_name(lexic, "ip_id", pkt->ip_id));
 pkt->ip_off= FIX(get_int_local_var_by_name(lexic, "ip_off", UNFIX(pkt->ip_off)));
 pkt->ip_ttl= get_int_local_var_by_name(lexic, "ip_ttl", pkt->ip_ttl);
 pkt->ip_p  = get_int_local_var_by_name(lexic, "ip_p", pkt->ip_p);
 
 s = get_str_local_var_by_name(lexic, "ip_src");
 if (s != NULL)
    inet_aton(s, &pkt->ip_src);
 
  pkt->ip_sum = htons(get_int_local_var_by_name(lexic, "ip_sum", 0));
  if(pkt->ip_sum == 0) 
    pkt->ip_sum = np_in_cksum((u_short *)pkt, sizeof(struct ip));
  
 retc->type = CONST_DATA;
 retc->size = size;
 retc->x.str_val = (char*)pkt;
 
 return retc;
}


tree_cell * insert_ip_options(lex_ctxt * lexic)
{
 struct ip * ip = (struct ip *)get_str_local_var_by_name(lexic, "ip");
 int code = get_int_local_var_by_name(lexic, "code", 0);
 int len = get_int_local_var_by_name(lexic, "length", 0);
 char * value = get_str_local_var_by_name(lexic, "value");
 int value_size = get_var_size_by_name(lexic, "value");
 tree_cell * retc;
 struct ip * new_packet;
 char * p;
 int size = get_var_size_by_name(lexic, "ip");
 u_char uc_code, uc_len;
 int pad_len;
 char zero = '0';
 int i;
 int hl;
 



 if( ip == NULL )
 {
   nasl_perror(lexic, "Usage : insert_ip_options(ip:<ip>, code:<code>, length:<len>, value:<value>\n");
   return NULL;
 }
 
 pad_len = 4 - ((sizeof(uc_code) + sizeof(uc_len) + value_size) % 4);
 if ( pad_len == 4 ) pad_len = 0;
 
 hl =  ip->ip_hl * 4 < UNFIX(ip->ip_len) ? ip->ip_hl * 4 : UNFIX(ip->ip_len);
 new_packet = emalloc(size + 4 + value_size + pad_len);
 bcopy(ip, new_packet, hl);
 
 uc_code = (u_char)code;
 uc_len =  (u_char)len;
 

 p = (char*)new_packet;
 bcopy(&uc_code, p+hl,sizeof(uc_code));
 bcopy(&uc_len,  p+hl+sizeof(uc_code), sizeof(uc_len));
 bcopy(value,    p+hl+sizeof(uc_code)+sizeof(uc_len), value_size);
 
 
 
 zero = 0;
 for(i=0;i<pad_len;i++)
 {
  bcopy(&zero, p+hl+sizeof(uc_code)+sizeof(uc_len)+value_size+i, 1);
 }
 

 p = (char*)ip;
 bcopy(p + hl, new_packet + (sizeof(uc_code) + sizeof(uc_len) + value_size + pad_len) + hl, size - hl);
 
 
 new_packet->ip_hl  = (hl + (sizeof(uc_code) + sizeof(uc_len) + value_size + pad_len)) / 4;
 new_packet->ip_len = FIX(size + sizeof(uc_code) + sizeof(uc_len) + value_size + pad_len);
 new_packet->ip_sum = 0;
 new_packet->ip_sum = np_in_cksum((u_short*)new_packet, new_packet->ip_hl*4 > UNFIX(new_packet->ip_len) ? UNFIX(new_packet->ip_len):new_packet->ip_hl*4  );
 
 retc = alloc_tree_cell(0, NULL);
 retc->type = CONST_DATA;
 retc->size = size + value_size + sizeof(uc_code) + sizeof(uc_len) + pad_len;
 retc->x.str_val = (char*)new_packet;
 
 return retc;
}



tree_cell * dump_ip_packet(lex_ctxt * lexic)
{
 int i;
 
 for(i=0;;i++)
 {
  struct ip * ip = (struct ip*)get_str_var_by_num(lexic, i);
  if(ip == NULL)
   break;
  else
   {
    printf("------\n");
    printf("\tip_hl : %d\n", ip->ip_hl);
    printf("\tip_v  : %d\n", ip->ip_v);
    printf("\tip_tos: %d\n", ip->ip_tos);
    printf("\tip_len: %d\n", UNFIX(ip->ip_len));
    printf("\tip_id : %d\n", ntohs(ip->ip_id));
    printf("\tip_off: %d\n", UNFIX(ip->ip_off));
    printf("\tip_ttl: %d\n", ip->ip_ttl);
    switch(ip->ip_p)
    {
     case IPPROTO_TCP : printf("\tip_p  : IPPROTO_TCP (%d)\n", ip->ip_p);
     		        break;
     case IPPROTO_UDP : printf("\tip_p  : IPPROTO_UDP (%d)\n", ip->ip_p);
     			break;
     case IPPROTO_ICMP: printf("\tip_p  : IPPROTO_ICMP (%d)\n", ip->ip_p);
     			break;
     default :
     			printf("\tip_p  : %d\n", ip->ip_p);	
			break;
     }					
    printf("\tip_sum: 0x%x\n", ntohs(ip->ip_sum));
    printf("\tip_src: %s\n", inet_ntoa(ip->ip_src));
    printf("\tip_dst: %s\n", inet_ntoa(ip->ip_dst));
    printf("\n");
   }
  }
 
 return FAKE_CELL;
}
/*--------------[ 	TCP 	]--------------------------------------------*/

struct pseudohdr
{
        struct in_addr saddr;
        struct in_addr daddr;
        u_char zero;
        u_char protocol;
        u_short length;
        struct tcphdr tcpheader;
};

tree_cell*	forge_tcp_packet(lex_ctxt* lexic)
{
  tree_cell	*retc;
  char		*data;
  int		len;
  u_char	* pkt;
  struct ip	*ip, * tcp_packet;
  struct tcphdr	* tcp;
  int		ipsz;


  ip = (struct ip*)get_str_local_var_by_name(lexic, "ip");
  if (ip == NULL)
    {
      nasl_perror(lexic, 
	      "forge_tcp_packet : You must supply the 'ip' argument !");
      return NULL;
    }
    
   ipsz = get_local_var_size_by_name(lexic, "ip");
   if(ipsz > ip->ip_hl * 4)
   	ipsz = ip->ip_hl * 4;
  
  

  data = get_str_local_var_by_name(lexic, "data");
  len = data == NULL ? 0 : get_var_size_by_name(lexic, "data");
  
  retc = alloc_tree_cell(0, NULL);
  retc->type = CONST_DATA;
  tcp_packet = (struct ip*)emalloc(ipsz + sizeof(struct tcphdr) + len);
  retc->x.str_val = (char*)tcp_packet;
  pkt = (u_char*) tcp_packet;
  
  bcopy(ip, tcp_packet, ipsz );
  /* recompute the ip checksum, because the ip length changed */
  if(UNFIX(tcp_packet->ip_len) <= tcp_packet->ip_hl * 4)
    {
      if(get_int_local_var_by_name(lexic, "update_ip_len", 1))
	{
	  tcp_packet->ip_len = FIX(tcp_packet->ip_hl*4 + sizeof(struct tcphdr) + len);
	  tcp_packet->ip_sum = 0;
	  tcp_packet->ip_sum = np_in_cksum((u_short*)tcp_packet, sizeof(struct ip));
	}
    }
  tcp = (struct tcphdr *)((char*)tcp_packet + tcp_packet->ip_hl*4);

  tcp->th_sport = ntohs(get_int_local_var_by_name(lexic, "th_sport", 0));
  tcp->th_dport = ntohs(get_int_local_var_by_name(lexic, "th_dport", 0));
  tcp->th_seq = htonl(get_int_local_var_by_name(lexic, "th_seq", rand()));
  tcp->th_ack = htonl(get_int_local_var_by_name(lexic, "th_ack", 0));
  tcp->th_x2 = get_int_local_var_by_name(lexic, "th_x2", 0);
  tcp->th_off = get_int_local_var_by_name(lexic, "th_off", 5);
  tcp->th_flags = get_int_local_var_by_name(lexic, "th_flags", 0);
  tcp->th_win = htons(get_int_local_var_by_name(lexic, "th_win", 0));
  tcp->th_sum = get_int_local_var_by_name(lexic, "th_sum", 0);
  tcp->th_urp = get_int_local_var_by_name(lexic, "th_urp", 0);
   
  if(data != NULL)
    bcopy(data, (char*)tcp + sizeof(struct tcphdr), len);
    

  if(!tcp->th_sum)
    {
      struct pseudohdr pseudoheader;
      char * tcpsumdata = emalloc(sizeof(struct pseudohdr) + (len % 2 ? len + 1 : len ) );
      struct in_addr source, dest;

      source.s_addr = ip->ip_src.s_addr;
      dest.s_addr = ip->ip_dst.s_addr;
   
      bzero(&pseudoheader, 12+sizeof(struct tcphdr));
      pseudoheader.saddr.s_addr=source.s_addr;
      pseudoheader.daddr.s_addr=dest.s_addr;
 
      pseudoheader.protocol=IPPROTO_TCP;
      pseudoheader.length=htons(sizeof(struct tcphdr)+len);
      bcopy((char *) tcp,(char *) &pseudoheader.tcpheader,sizeof(struct tcphdr));
      /* fill tcpsumdata with data to checksum */
      bcopy((char *) &pseudoheader, tcpsumdata ,sizeof(struct pseudohdr));
      if( data != NULL ) bcopy((char *) data, tcpsumdata + sizeof(struct pseudohdr), len );
      tcp->th_sum = np_in_cksum((unsigned short *)tcpsumdata,12+sizeof(struct tcphdr) + len );
      efree(&tcpsumdata );
    }


 
  retc->size = ipsz + sizeof(struct tcphdr) + len;
  return retc;
}

tree_cell * get_tcp_element(lex_ctxt * lexic)
{
 u_char * packet = get_str_local_var_by_name(lexic, "tcp");
 struct ip * ip;
 int	ipsz;
 struct tcphdr * tcp;
 char * element;
 int ret;
 tree_cell * retc;
 
 
 ipsz = get_local_var_size_by_name(lexic, "tcp");
 
 
 if(packet == NULL)
 {
  nasl_perror(lexic, "get_tcp_element : Error ! No valid 'tcp' argument !\n");
  return NULL;
 }
 
 ip = (struct ip * )packet;
 
 if(ip->ip_hl * 4 > ipsz)
 	return NULL;	/* Invalid packet */

 if(UNFIX(ip->ip_len) > ipsz)
 	return NULL;	/* Invalid packet */
 
 tcp = (struct tcphdr*)(packet + ip->ip_hl * 4);
 
 element = get_str_local_var_by_name(lexic, "element");
 if(!element)
 {
  nasl_perror(lexic, "get_tcp_element : Error ! No valid 'element' argument !\n");
  return NULL;
 }
 

 if(!strcmp(element, "th_sport"))ret = ntohs(tcp->th_sport);
 else if(!strcmp(element, "th_dsport"))ret = ntohs(tcp->th_dport);
 else if(!strcmp(element, "th_seq"))ret = ntohl(tcp->th_seq);
 else if(!strcmp(element, "th_ack"))ret = ntohl(tcp->th_ack);
 else if(!strcmp(element, "th_x2"))ret = tcp->th_x2;
 else if(!strcmp(element, "th_off"))ret = tcp->th_off;
 else if(!strcmp(element, "th_flags"))ret = tcp->th_flags;
 else if(!strcmp(element, "th_win"))ret = ntohs(tcp->th_win);
 else if(!strcmp(element, "th_sum"))ret = tcp->th_sum;
 else if(!strcmp(element, "th_urp"))ret = tcp->th_urp;
 else if(!strcmp(element, "data")){
 	retc = alloc_tree_cell(0, NULL);
	retc->type = CONST_DATA;
	retc->size = UNFIX(ip->ip_len) - ntohl(tcp->th_off) * 4;
	retc->x.str_val = emalloc(retc->size);
	bcopy(tcp + ntohl(tcp->th_off) * 4, retc->x.str_val, retc->size);
	return retc;
}
 else {
 	nasl_perror(lexic, "Unknown tcp field %s\n", element);
	return NULL;
	}
	
 retc = alloc_tree_cell(0, NULL);
 retc->type = CONST_INT;
 retc->x.i_val = ret;
 return retc;
}

tree_cell * set_tcp_elements(lex_ctxt * lexic)
{
 char * pkt = get_str_local_var_by_name(lexic, "tcp");
 struct ip * ip = (struct ip*)pkt;
 int	pktsz = get_local_var_size_by_name(lexic, "tcp");
 struct tcphdr * tcp;
 tree_cell * retc;
 char * data = get_str_local_var_by_name(lexic, "data");
 int data_len = get_local_var_size_by_name(lexic, "data");
 char * npkt;
 
 if( pkt == NULL )
 {
  nasl_perror(lexic, "set_tcp_elements : Invalid value for the argument 'tcp'\n");
  return NULL;
 }
 
 if(ip->ip_hl * 4 > pktsz)
 	tcp = (struct tcphdr*)(pkt + 20);		/* ip->ip_hl is bogus, we work around that */
 else
	tcp =  (struct tcphdr*)(pkt + ip->ip_hl * 4);
 
 
 if(pktsz < UNFIX(ip->ip_len))
 	return NULL;
	
 
 if(data_len == 0)
 {
  data_len = UNFIX(ip->ip_len) - (ip->ip_hl * 4) - (tcp->th_off * 4);
  data = (char*)((char*)tcp + tcp->th_off * 4);
 }
 
 npkt = emalloc(ip->ip_hl * 4 + tcp->th_off * 4 + data_len);
 bcopy(pkt, npkt, UNFIX(ip->ip_len));
 
 ip  = (struct ip*)(npkt);
 tcp = (struct tcphdr*)(npkt + ip->ip_hl * 4);
 
 tcp->th_sport = htons(get_int_local_var_by_name(lexic, "th_sport", ntohs(tcp->th_sport)));
 tcp->th_dport = htons(get_int_local_var_by_name(lexic, "th_dport", ntohs(tcp->th_dport)));
 tcp->th_seq   = htonl(get_int_local_var_by_name(lexic, "th_seq", ntohl(tcp->th_seq)));
 tcp->th_ack   = htonl(get_int_local_var_by_name(lexic, "th_ack", ntohl(tcp->th_ack)));
 tcp->th_x2    = get_int_local_var_by_name(lexic, "th_x2", tcp->th_x2);
 tcp->th_off   = get_int_local_var_by_name(lexic, "th_off", tcp->th_off);
 tcp->th_flags = get_int_local_var_by_name(lexic, "th_flags", tcp->th_flags);
 tcp->th_win   = htons(get_int_local_var_by_name(lexic, "th_win", ntohs(tcp->th_win)));
 tcp->th_sum   = get_int_local_var_by_name(lexic, "th_sum", 0);
 tcp->th_urp   = get_int_local_var_by_name(lexic, "th_urp", tcp->th_urp);
 bcopy(data, (char*)tcp + tcp->th_off * 4, data_len);
 
 if(get_int_local_var_by_name(lexic, "update_ip_len", 1) != 0)
 {
 ip->ip_len = ip->ip_hl * 4 + tcp->th_off * 4 + data_len;
 ip->ip_sum = 0;
 ip->ip_sum = np_in_cksum((u_short *)pkt, ip->ip_hl * 4);
 }
 
if(tcp->th_sum == 0)
  {
    struct pseudohdr pseudoheader;
    char * tcpsumdata = emalloc(sizeof(struct pseudohdr) + data_len + (data_len % 2) );
    struct in_addr source, dest;
    
    source.s_addr = ip->ip_src.s_addr;
    dest.s_addr = ip->ip_dst.s_addr;
   
    bzero(&pseudoheader, sizeof(pseudoheader));
    pseudoheader.saddr.s_addr=source.s_addr;
    pseudoheader.daddr.s_addr=dest.s_addr;
 
    pseudoheader.protocol = IPPROTO_TCP;
    pseudoheader.length = htons(sizeof(struct tcphdr) + data_len);
    bcopy((char *) tcp,(char *) &pseudoheader.tcpheader,sizeof(struct tcphdr));
    /* fill tcpsumdata with data to checksum */
    bcopy((char *) &pseudoheader, tcpsumdata ,sizeof(struct pseudohdr));
    bcopy((char *) data, tcpsumdata + sizeof(struct pseudohdr), data_len );
    tcp->th_sum = np_in_cksum((unsigned short *)tcpsumdata,sizeof(pseudoheader) + data_len );
    efree(&tcpsumdata );
  }
  
  retc = alloc_tree_cell(0, NULL);
  retc->type = CONST_DATA;
  retc->size = (ip->ip_hl * 4) + (tcp->th_off * 4) + data_len;
  retc->x.str_val = npkt;
  return retc;
}



tree_cell * dump_tcp_packet(lex_ctxt * lexic)
{
 int i = 0;
 u_char * pkt;
 while((pkt = (char*)get_str_var_by_num(lexic, i++)) != NULL)
 {
    int a=0;
    struct ip * ip = (struct ip*)pkt;
    struct tcphdr * tcp = (struct tcphdr *)(pkt + ip->ip_hl * 4);
    int j;
    int limit;
    char * c;
    limit = get_var_size_by_num(lexic, i - 1);
    printf("------\n");
    printf("\tth_sport : %d\n", ntohs(tcp->th_sport));
    printf("\tth_dport : %d\n", ntohs(tcp->th_dport));
    printf("\tth_seq   : %u\n", (unsigned int)ntohl(tcp->th_seq));
    printf("\tth_ack   : %u\n", (unsigned int)ntohl(tcp->th_ack));
    printf("\tth_x2    : %d\n", tcp->th_x2);
    printf("\tth_off   : %d\n",tcp->th_off);
    printf("\tth_flags : ");
    if(tcp->th_flags & TH_FIN){printf("TH_FIN");a++;}
    if(tcp->th_flags & TH_SYN){if(a)printf("|");printf("TH_SYN");a++;}
    if(tcp->th_flags & TH_RST){if(a)printf("|");printf("TH_RST");a++;}
    if(tcp->th_flags & TH_PUSH){if(a)printf("|");printf("TH_PUSH");a++;}
    if(tcp->th_flags & TH_ACK){if(a)printf("|");printf("TH_ACK");a++;}
    if(tcp->th_flags & TH_URG){if(a)printf("|");printf("TH_URG");a++;}
    if(!a)printf("0");
    else printf(" (%d)", tcp->th_flags);
    printf("\n");
    printf("\tth_win   : %d\n", ntohs(tcp->th_win));
    printf("\tth_sum   : 0x%x\n", tcp->th_sum);
    printf("\tth_urp   : %d\n", tcp->th_urp);
    printf("\tData     : ");
    c = (char*)((char*)tcp+sizeof(struct tcphdr));
    if(UNFIX(ip->ip_len)>(sizeof(struct ip)+sizeof(struct tcphdr)))
    	for(j=0;j<UNFIX(ip->ip_len)-sizeof(struct ip)-sizeof(struct tcphdr) && j < limit;j++)
     		printf("%c", isprint(c[j])?c[j]:'.');
    printf("\n"); 
     
    printf("\n");
 
  }
 return NULL;
}

/*--------------[ 	UDP 	]--------------------------------------------*/
struct pseudo_udp_hdr
{
        struct in_addr saddr;
        struct in_addr daddr;
        char nothing;
	char proto;
	unsigned short len;
	struct udphdr udpheader;
};


tree_cell * forge_udp_packet(lex_ctxt * lexic)
{
 tree_cell * retc;
 struct ip * ip = (struct ip *)get_str_local_var_by_name(lexic, "ip");
 
 if(ip != NULL)
 {
  char * data = get_str_local_var_by_name(lexic, "data");
  int data_len = get_local_var_size_by_name(lexic, "data");
  u_char * pkt; 
  struct ip * udp_packet;
  struct udphdr * udp;
  





  pkt = emalloc(sizeof(struct udphdr)+ip->ip_hl*4+sizeof(struct udphdr) + data_len);	
 
  
  udp_packet = (struct ip*)pkt;
  udp = (struct udphdr*)(pkt + ip->ip_hl*4);
  
  
  udp->uh_sport = htons(get_int_local_var_by_name(lexic, "uh_sport", 0));
  udp->uh_dport = htons(get_int_local_var_by_name(lexic, "uh_dport", 0));
  udp->uh_ulen  = htons(get_int_local_var_by_name(lexic, "uh_ulen", data_len + sizeof(struct udphdr)));
  
  
 /* printf("len : %d %s\n", len, data);*/
  if(data_len != 0 && data != NULL)bcopy(data, (pkt + ip->ip_hl * 4 + sizeof(struct udphdr)), data_len);
 
  udp->uh_sum = get_int_local_var_by_name(lexic, "uh_sum", 0);
  bcopy((char*)ip, pkt, ip->ip_hl*4);
  if(udp->uh_sum == 0)
  {
   struct pseudo_udp_hdr pseudohdr;
   struct in_addr source, dest;
   char * udpsumdata = (char*)emalloc(sizeof(struct pseudo_udp_hdr) + (data_len % 2 ? data_len + 1 : data_len));
   
   source.s_addr = ip->ip_src.s_addr;
   dest.s_addr   = ip->ip_dst.s_addr;
   
   bzero(&pseudohdr, sizeof(struct pseudo_udp_hdr));
   pseudohdr.saddr.s_addr = source.s_addr;
   pseudohdr.daddr.s_addr = dest.s_addr;
   
   pseudohdr.proto = IPPROTO_UDP;
   pseudohdr.len = htons(sizeof(struct udphdr) + data_len);
   bcopy((char*)udp, (char*)&pseudohdr.udpheader, sizeof(struct udphdr));
   bcopy((char*)&pseudohdr, udpsumdata, sizeof(pseudohdr));
   if(data != NULL)
   {
   bcopy((char*)data, udpsumdata + sizeof(pseudohdr), data_len );
   }
   udp->uh_sum = np_in_cksum((unsigned short*)udpsumdata, 12 + sizeof(struct udphdr) + data_len);
   efree(&udpsumdata);
  }
  
  
  if(UNFIX(udp_packet->ip_len) <= udp_packet->ip_hl * 4)
  {
   int v = get_int_local_var_by_name(lexic, "update_ip_len", 1);
   if(v != 0)
   {
    udp_packet->ip_len = FIX(ntohs(udp->uh_ulen)+(udp_packet->ip_hl*4));
    udp_packet->ip_sum = 0;
    udp_packet->ip_sum = np_in_cksum((u_short*)udp_packet, udp_packet->ip_hl*4);
   }
  }
  
  retc = alloc_tree_cell(0, NULL);
  retc->type = CONST_DATA;
  retc->x.str_val = pkt;
  retc->size = 8 + ip->ip_hl * 4 + data_len;
  return retc;
 }
 else printf("Error ! You must supply the 'ip' argument !\n");
 
 return NULL;
}


tree_cell * get_udp_element(lex_ctxt * lexic)
{
 tree_cell * retc;
 char * udp;
 char * element;
 struct ip * ip;
 int	ipsz;
 struct udphdr * udphdr;
 int ret;
 
 
 udp = get_str_local_var_by_name(lexic, "udp");
 ipsz  = get_local_var_size_by_name(lexic, "udp");
 
 
 element = get_str_local_var_by_name(lexic, "element");
 if(udp == NULL || element == NULL )
 	{
 	printf("get_udp_element() usage :\n");
	printf("element = get_udp_element(udp:<udp>,element:<element>\n");
	return NULL;
	}
ip = (struct ip*)udp;

if(ip->ip_hl * 4 + sizeof(struct udphdr) > ipsz)
	return NULL;
	

udphdr = (struct udphdr*)(udp+ip->ip_hl*4);
if(!strcmp(element, "uh_sport"))
  ret = ntohs(udphdr->uh_sport);
else if(!strcmp(element, "uh_dport"))
  ret = ntohs(udphdr->uh_dport);
else if(!strcmp(element, "uh_ulen"))
  ret = ntohs(udphdr->uh_ulen);
else if(!strcmp(element, "uh_sum"))
  ret = ntohs(udphdr->uh_sum);
else if(!strcmp(element, "data"))
 {
  int sz;
  retc = alloc_tree_cell(0, NULL);
  retc->type = CONST_DATA;
  sz = ntohs(udphdr->uh_ulen) - sizeof(struct udphdr);
  
  
  if(ntohs(udphdr->uh_ulen) - ip->ip_hl * 4 - sizeof(struct udphdr) > ipsz)
  	sz = ipsz - ip->ip_hl * 4 - sizeof(struct udphdr);
	
  retc->x.str_val = emalloc(sz);
  retc->size = sz ;
  bcopy(udp + ip->ip_hl*4 + sizeof(struct udphdr), retc->x.str_val, sz);
  return retc;
 }
 else {
 	printf("%s is not a value of a udp packet\n", element);
	return NULL;
  }

 retc = alloc_tree_cell(0, NULL);
 retc->type = CONST_INT;
 retc->x.i_val = ret;
 return retc;
}



tree_cell * set_udp_elements(lex_ctxt * lexic)
{
 struct ip * ip = (struct ip*)get_str_local_var_by_name(lexic, "udp");
 int sz = get_local_var_size_by_name(lexic, "udp");
 char * data = get_str_local_var_by_name(lexic, "data");
 int data_len = get_local_var_size_by_name(lexic, "data");
 
 if( ip != NULL )
 {
  char * pkt = emalloc(sz + data_len);
  struct udphdr * udp;
  tree_cell * retc;
  int old_len;
  
  
  if(ip->ip_hl * 4 + sizeof(struct udphdr) > sz)
  	return NULL;
	
  if(data != NULL)
   {
   sz = ip->ip_hl * 4 + sizeof(struct udphdr) + data_len;
   pkt = emalloc(sz);
   bcopy(ip, pkt, ip->ip_hl * 4 + sizeof(struct udphdr));
   }
  else
   {
   pkt = emalloc(sz);
   bcopy(ip, pkt, sz);
   }
  
   
   
  ip = (struct ip *)pkt;
  if(data != NULL)
  {
   ip->ip_len = FIX(sz);
   ip->ip_sum = 0;
   ip->ip_sum = np_in_cksum(ip, ip->ip_hl * 4);
  }
  udp = (struct udphdr*)(pkt + ip->ip_hl * 4);
  
 
  udp->uh_sport = htons(get_int_local_var_by_name(lexic, "uh_sport", ntohs(udp->uh_sport)));
  udp->uh_dport = htons(get_int_local_var_by_name(lexic, "uh_dport", ntohs(udp->uh_dport)));
  old_len = ntohs(udp->uh_ulen);
  udp->uh_ulen  = htons(get_int_local_var_by_name(lexic, "uh_ulen", ntohs(udp->uh_ulen)));
  udp->uh_sum   = get_int_local_var_by_name(lexic, "uh_sum", 0);
  
  if(data != NULL)
   {
   bcopy(data, pkt + ip->ip_hl * 4 + sizeof(struct udphdr), data_len);
   udp->uh_ulen = htons(sizeof(struct udphdr) + data_len);
  }
  
  if(udp->uh_sum == 0)
  {
   struct pseudo_udp_hdr pseudohdr;
   struct in_addr source, dest;
   int len = old_len - sizeof(struct udphdr);
   char * udpsumdata;
   char * ptr = NULL;
   
   if(data != NULL)
   {
    len = data_len;
   }
   
   if(len > 0)
   {
    ptr = (char*)udp + sizeof(struct udphdr);
   }
   
   
   udpsumdata = (char*)emalloc(sizeof(struct pseudo_udp_hdr) + (len % 2 ? len + 1 : len));
   
   source.s_addr = ip->ip_src.s_addr;
   dest.s_addr   = ip->ip_dst.s_addr;
   
   bzero(&pseudohdr, sizeof(struct pseudo_udp_hdr));
   pseudohdr.saddr.s_addr = source.s_addr;
   pseudohdr.daddr.s_addr = dest.s_addr;
   
   pseudohdr.proto = IPPROTO_UDP;
   pseudohdr.len = htons(sizeof(struct udphdr) + len);
   bcopy((char*)udp, (char*)&pseudohdr.udpheader, sizeof(struct udphdr));
   bcopy((char*)&pseudohdr, udpsumdata, sizeof(pseudohdr));
   if(ptr != NULL)
   {
   bcopy((char*)ptr, udpsumdata + sizeof(pseudohdr), len );
   }
   udp->uh_sum = np_in_cksum((unsigned short*)udpsumdata, 12 + sizeof(struct udphdr) + len);
   efree(&udpsumdata);
  }
  retc = alloc_tree_cell(0, NULL);
  retc->type = CONST_DATA;
  retc->size = sz;
  retc->x.str_val = pkt;
  return retc;
 }
 else printf("Error ! You must supply the 'udp' argument !\n");
 
 return NULL;
}


tree_cell * dump_udp_packet(lex_ctxt * lexic)
{
 int i = 0;
 u_char * pkt;
 while((pkt = get_str_var_by_num(lexic, i++)) != NULL)
 {
   struct udphdr * udp = (struct udphdr*)(pkt+sizeof(struct ip));
   int j;
   char * c;
   int limit = get_var_size_by_num(lexic, i - 1);
   printf("------\n");
   printf("\tuh_sport : %d\n", ntohs(udp->uh_sport));
   printf("\tuh_dport : %d\n", ntohs(udp->uh_dport));
   printf("\tuh_sum   : 0x%x\n", udp->uh_sum);
   printf("\tuh_ulen  : %d\n", ntohs(udp->uh_ulen));
   printf("\tdata     : ");
   c = (char*)(udp + sizeof(struct udphdr));
   if(udp->uh_ulen > sizeof(struct udphdr))
   for(j=0;j<(ntohs(udp->uh_ulen)-sizeof(struct udphdr)) && j < limit;j++)
   	printf("%c", isprint(c[j])?c[j]:'.');
	
  printf("\n");			   
 }
 return NULL;
}
/*--------------[  ICMP  ]--------------------------------------------*/

tree_cell*	forge_icmp_packet(lex_ctxt* lexic)
{
  tree_cell	*retc = NULL;
  struct ip	*ip;
  struct ip	*ip_icmp;
  int    ip_sz;
  struct icmp	*icmp;
  char		*data, *p;
  int		len;
  u_char	*pkt;
  int		t;

  ip = (struct ip*)get_str_local_var_by_name(lexic, "ip");
  ip_sz = get_local_var_size_by_name(lexic, "ip");
  if (ip != NULL)
    {
      data = get_str_local_var_by_name(lexic, "data");
      len = data == NULL ? 0 : get_var_size_by_name(lexic, "data");
    
      
      t = get_int_local_var_by_name(lexic, "icmp_type", 0);
      if(t == 13 || t == 14) len += 3 * sizeof(time_t);
      
      if(ip->ip_hl * 4 > ip_sz)
      	return NULL;
	
      pkt = emalloc(sizeof(struct icmp) + ip_sz + len);
      ip_icmp = (struct ip*) pkt;
  
      
  
      bcopy(ip, ip_icmp, ip_sz);
      if(UNFIX(ip_icmp->ip_len) <= (ip_icmp->ip_hl * 4))
	{
	  if (get_int_local_var_by_name(lexic, "update_ip_len", 1) != 0)
	    {
	      ip_icmp->ip_len = FIX(ip->ip_hl*4 + 8 + len);
	      ip_icmp->ip_sum = 0;
	      ip_icmp->ip_sum = np_in_cksum((u_short*)ip_icmp, ip->ip_hl*4);
	    }
	}
      p = (char*)(pkt + (ip->ip_hl*4));
      icmp = (struct icmp*)p;
  
      icmp->icmp_code = get_int_local_var_by_name(lexic, "icmp_code", 0);
      icmp->icmp_type = t;
      icmp->icmp_seq = htons(get_int_local_var_by_name(lexic, "icmp_seq", 0));
      icmp->icmp_id = htons(get_int_local_var_by_name(lexic, "icmp_id", 0));
     
      if(data != NULL)bcopy(data, &(p[8]), len);
  
      if(get_int_local_var_by_name(lexic, "icmp_cksum", -1) == -1 )
      	icmp->icmp_cksum = np_in_cksum((u_short *) icmp, len + 8);
      else
        icmp->icmp_cksum = htons(get_int_local_var_by_name(lexic, "icmp_cksum", 0));
       
      
      retc = alloc_tree_cell(0, NULL);
      retc->type = CONST_DATA;
      retc->x.str_val = pkt;
      retc->size = ip_sz + len + 8;
    }
  else
    nasl_perror(lexic, "forge_icmp_packet: missing 'ip' parameter\n");

  return retc;
}

tree_cell * get_icmp_element(lex_ctxt * lexic)
{
 struct icmp * icmp;
 char * p;
 

 if((p = get_str_local_var_by_name(lexic, "icmp")) != NULL)
 {
  char * elem = get_str_local_var_by_name(lexic, "element");
  int value;
  struct ip * ip = (struct ip*)p;
  tree_cell * retc;
  
  icmp = (struct icmp*)(p+ip->ip_hl*4);
  
  if( elem == NULL )
   return NULL;
   
  if(!strcmp(elem, "icmp_id"))value = ntohs(icmp->icmp_id);
  else if(!strcmp(elem, "icmp_code"))value = icmp->icmp_code;
  else if(!strcmp(elem, "icmp_type"))value = icmp->icmp_type;
  else if(!strcmp(elem, "icmp_seq"))value = ntohs(icmp->icmp_seq);
  else if(!strcmp(elem, "icmp_cksum"))value = ntohs(icmp->icmp_cksum);
  else if(!strcmp(elem, "data"))
  {
   retc = alloc_tree_cell(0, NULL);
   retc->type = CONST_DATA;
   retc->size = get_var_size_by_name(lexic, "icmp") - (ip->ip_hl * 4) - 8;
   if ( retc->size > 0 )
     retc->x.str_val = nasl_strndup( &(p[ip->ip_hl*4+8]), retc->size);
   else {
     retc->x.str_val = NULL;
     retc->size = 0;
   }
   return retc;
  }
  else return NULL;
  
  retc = alloc_tree_cell(0, NULL);
  retc->type = CONST_INT;
  retc->x.i_val = value;
  return retc;
  }

 return NULL;
}
  
  
  


  
  
  
  
  
/*--------------[  IGMP  ]--------------------------------------------*/







struct igmp {
 	unsigned char type;
	unsigned char code;
	unsigned short cksum;
	struct in_addr group;
	};
	
tree_cell * forge_igmp_packet(lex_ctxt * lexic)
{
 struct ip * ip = (struct ip*) get_str_local_var_by_name(lexic, "ip");
 
 if( ip != NULL )
 {
  char * data = get_str_local_var_by_name(lexic, "data");
  int len = data ? get_local_var_size_by_name(lexic, "data"):0;
  u_char * pkt = emalloc(sizeof(struct igmp)+ ip->ip_hl * 4 + len);
  struct ip * ip_igmp = (struct ip*)pkt;
  struct igmp * igmp;
  char * p;
  char * grp;
  tree_cell * retc;
  int ipsz = get_local_var_size_by_name(lexic, "ip");
  
  bcopy(ip, ip_igmp, ipsz);
  
  
  if(UNFIX(ip_igmp->ip_len) <= ip_igmp->ip_hl * 4)
   {
   int v = get_int_local_var_by_name(lexic, "update_ip_len", 1);
   if(v != 0)
    {
    ip_igmp->ip_len = FIX(ip->ip_hl * 4 + sizeof(struct igmp) + len);
    ip_igmp->ip_sum = 0;
    ip_igmp->ip_sum = np_in_cksum((u_short*)ip_igmp, ip->ip_hl * 4);
    }
   }
  p = (char*)(pkt + ip_igmp->ip_hl*4);
  igmp = (struct igmp *)p;
  
  igmp->code = get_int_local_var_by_name(lexic, "code", 0);
  igmp->type = get_int_local_var_by_name(lexic, "type", 0);
  grp = get_str_local_var_by_name(lexic, "group");
  
  if( grp != NULL )
  {
   inet_aton(grp, &igmp->group);
  }
  
  igmp->cksum = np_in_cksum((u_short*)igmp, sizeof(struct igmp));
  if(data != NULL)
  {
   char * p = (char*)(pkt + ip->ip_hl * 4 + sizeof(struct igmp));
   bcopy(p, data, len);
  }
   retc = alloc_tree_cell(0, NULL);
   retc->type = CONST_DATA;
   retc->x.str_val = pkt;
   retc->size = ip->ip_hl * 4 + sizeof(struct igmp) + len;
   return retc;
  }
 
  return NULL;
}
  
  
/*---------------------------------------------------------------------------*/

tree_cell * nasl_tcp_ping(lex_ctxt * lexic)
{
 int port;
 u_char packet[sizeof(struct ip)+sizeof(struct tcphdr)];
 int soc;
 struct ip * ip = (struct ip *)packet;
 struct tcphdr * tcp = (struct tcphdr *)(packet + sizeof(struct ip));
 struct arglist *  script_infos = lexic->script_infos;
 struct in_addr * dst = plug_get_host_ip(script_infos);
 struct in_addr src;
 struct sockaddr_in soca;
 int flag = 0;
 int i = 0;
 int bpf;
 char filter[255] = {0};
 struct ip * pk = NULL;
 tree_cell * retc;
 int opt = 1;
#define rnd_tcp_port() (rand() % 65535 + 1024)

 
 
 int sports[]= { 0,     0,   0,  0,  0, 1023, 0,  0,    0,    0,  0,   0,    0,    0,    0,  0,   0,   0,    0,    0, 53,   0,    0,    20,   0,  25,   0,    0, 0};
 int ports[] = { 139, 135, 445, 80, 22, 515, 23, 21, 6000, 1025, 25, 111, 1028, 9100, 1029, 79, 497, 548, 5000, 1917, 53, 161, 9001, 65535, 443, 113, 993, 8080, 0};
 
 
 
 
 int num_ports = 0;
 
 
 if( dst == NULL ) 
  	return NULL;
	
 for(i=0;i < sizeof(sports) / sizeof(int); i ++)
 {
  if ( sports[i] == 0 ) sports[i] = rnd_tcp_port();
 }
 



 for(i=0;ports[i];i++)num_ports ++;
 i = 0;
 
 
 soc = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
 if(soc < 0)
  return NULL;
//#ifdef IP_HDRINCL 
 if(setsockopt(soc, IPPROTO_IP, IP_HDRINCL, (char*)&opt, sizeof(opt))<0)
    fprintf(stderr, "setsockopt() failed, error code: %d\n", WSAGetLastError());
	//perror("setsockopt ");
//#endif



 port = get_int_local_var_by_name(lexic, "port", -1);
 if(port == -1)
	 port = plug_get_host_open_port(script_infos);
 
  if(islocalhost(dst) > 0 )src.s_addr = dst->s_addr;
  else 
   {
    bzero(&src, sizeof(src));
    routethrough(dst, &src);
  }
 
 set_pktlock();

 snprintf(filter, sizeof(filter)-1, "ip and src host %s", inet_ntoa(*dst));
 bpf = init_capture_device(*dst, src, filter);
 
 
 if(islocalhost(dst) != 0)flag++;
 else
 {
  while((i < 20) && (!flag))
  {
   bzero(packet, sizeof(packet));
  /* IP */
  ip->ip_hl  = 5;	ip->ip_off = FIX(0);
  ip->ip_v   = 4;	ip->ip_len = FIX(40);
  ip->ip_tos = 0;	ip->ip_p   = IPPROTO_TCP;
  ip->ip_id  = rand();	ip->ip_ttl = 0x40;
  ip->ip_src = src; 	ip->ip_dst = *dst;
  ip->ip_sum = 0;	ip->ip_sum = np_in_cksum((u_short *)ip, 20);
  
  
  /* TCP */ 
  tcp->th_sport = port ? htons(rnd_tcp_port()) : htons(sports[i%num_ports]);  tcp->th_flags = TH_SYN;
  tcp->th_dport = port ? htons(port):htons(ports[i%num_ports]);  
  tcp->th_seq = rand();
  tcp->th_ack = 0;	tcp->th_x2  = 0;
  tcp->th_off = 5;	tcp->th_win = htons(2048);
  tcp->th_urp = 0;	tcp->th_sum = 0;
 
 /* CKsum */
  {
    struct in_addr source, dest;
    struct pseudohdr pseudoheader;
    source.s_addr = ip->ip_src.s_addr;
    dest.s_addr = ip->ip_dst.s_addr;
   
    bzero(&pseudoheader, 12+sizeof(struct tcphdr));
    pseudoheader.saddr.s_addr=source.s_addr;
    pseudoheader.daddr.s_addr=dest.s_addr;

    pseudoheader.protocol=6;
    pseudoheader.length=htons(sizeof(struct tcphdr));
    bcopy((char *) tcp,(char *) &pseudoheader.tcpheader,sizeof(struct tcphdr));
    tcp->th_sum = np_in_cksum((unsigned short *)&pseudoheader,12+sizeof(struct tcphdr));
  } 

 bzero(&soca, sizeof(soca));
 soca.sin_family = AF_INET;
 soca.sin_addr = ip->ip_dst;
 sendto(soc, (const void*)ip, 40, 0, (struct sockaddr *)&soca, sizeof(soca));
 if(bpf >= 0 && (pk = capture_next_packet(bpf, 0, NULL)))flag++;	
 i++;
  }
 }


 retc = alloc_tree_cell(0, NULL);
 retc->type = CONST_INT;
 retc->x.i_val = flag;
 if(bpf >= 0)bpf_close(bpf);

 unset_pktlock();
 
 closesocket(soc);
 return retc;
}

/*---------------------------------------------------------------------------*/

tree_cell*	nasl_send_packet(lex_ctxt* lexic)
{
  tree_cell	*retc = FAKE_CELL;
  int		bpf = -1;
  u_char	* answer;
  int	 	answer_sz;
  struct sockaddr_in	sockaddr;
  char		*ip = NULL;
  struct ip	*sip = NULL;
  int		vi = 0, b, len = 0;
  int		soc;
  int		use_pcap = get_int_local_var_by_name(lexic, "pcap_active", 1);
  int		to = get_int_local_var_by_name(lexic, "pcap_timeout", 5);
  char		*filter = get_str_local_var_by_name(lexic, "pcap_filter");
  int		dfl_len = get_int_local_var_by_name(lexic, "length", -1);
  int	    i = 1;
  struct arglist	*script_infos = lexic->script_infos;
  struct in_addr	*dstip = plug_get_host_ip(script_infos);


 soc = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
 if(soc < 0)
  return NULL;
//#ifdef IP_HDRINCL 
 if(setsockopt(soc, IPPROTO_IP, IP_HDRINCL, (char*)&i, sizeof(i))<0)
    fprintf(stderr, "setsockopt() failed, error code: %d\n", WSAGetLastError());
	//perror("setsockopt ");
//#endif

  set_pktlock();

  while ((ip = get_str_var_by_num(lexic, vi)) != NULL)
    {
      int sz = get_var_size_by_num(lexic, vi);
      vi ++;
      
      if ( sz < sizeof(struct ip) )
      {
       nasl_perror(lexic, "send_packet(): packet is too short!\n");
       continue;
      }
      sip = (struct ip *)ip;
     
      if( use_pcap != 0 && bpf < 0)
	bpf = init_capture_device(sip->ip_dst, sip->ip_src, filter);

      bzero(&sockaddr, sizeof(struct sockaddr_in));
      sockaddr.sin_family = AF_INET;
      sockaddr.sin_addr = sip->ip_dst;
      if (dstip != NULL && sockaddr.sin_addr.s_addr != dstip->s_addr)
	{
	  char	txt1[64], txt2[64];
	  strncpy(txt1, inet_ntoa(sockaddr.sin_addr), sizeof(txt1));
 	  txt1[sizeof(txt1)-1] = '\0';
	  strncpy(txt2, inet_ntoa(*dstip), sizeof(txt2));
 	  txt2[sizeof(txt2)-1] = '\0';
	  nasl_perror(lexic, "send_packet: malicious or buggy script is trying to send packet to %s instead of designated target %s\n", txt1, txt2);
#if 1
          if(bpf >= 0)bpf_close(bpf);  
          closesocket(soc);
          unset_pktlock();
	  return NULL;
#else
	  sip->ip_dst = *dstip;
	  sip->ip_sum = np_in_cksum((u_short *)sip, sizeof(struct ip));
#endif
	}
      
      if(dfl_len > 0 && dfl_len < sz)
	len = dfl_len;
      else
	len = sz;
	
    	
   
      b = sendto(soc, (u_char*)ip, len, 0, (struct sockaddr *)&sockaddr, sizeof(sockaddr));
      /* if(b < 0) perror("sendto "); */
      if(b >= 0 && use_pcap != 0 && bpf >= 0)
	{
	  if(islocalhost(&sip->ip_dst))
	    {
	      answer = (u_char*) capture_next_packet(bpf, to, &answer_sz);
	      while(answer != NULL && (!memcmp(answer, (char*)ip, sizeof(struct ip))))
		{
		  efree(&answer);
		  answer = (u_char*)capture_next_packet( bpf, to, &answer_sz);
		}
	      	
	    }
	  else 
	    answer = (u_char*)capture_next_packet(bpf, to, &answer_sz);
	
	  if(answer)
	    {
  	      retc = alloc_tree_cell(0, NULL);
  	      retc->type = CONST_DATA;
	      retc->x.str_val = answer;
	      retc->size = answer_sz;
              break;
	    }
	}
    }

  unset_pktlock();

  if(bpf >= 0)bpf_close(bpf);  
  closesocket(soc);
  return retc;
}

   
   
/*---------------------------------------------------------------------------*/
   
tree_cell * nasl_pcap_next(lex_ctxt* lexic)
{
 char * ifinterface = get_str_local_var_by_name(lexic, "interface");
 int bpf = -1;
 static char errbuf[PCAP_ERRBUF_SIZE];
 int is_ip = 0;
 struct ip * ret = NULL;
 char *filter = get_str_local_var_by_name(lexic, "pcap_filter");
 int timeout = get_int_local_var_by_name(lexic, "timeout", 5);
 tree_cell * retc;
 int sz;
 struct in_addr * dst = plug_get_host_ip(lexic->script_infos);

 set_pktlock();

 if(ifinterface == NULL )
 	ifinterface = getif(errbuf);//pcap_lookupdev(errbuf);
	
 if(ifinterface != NULL)
  bpf = bpf_open_live(ifinterface, filter);
 
 
 if(bpf < 0)
  {
   unset_pktlock();
   nasl_perror(lexic, "pcap_next: Could not get a bpf\n");
   return NULL;
  }
 else
 {
  int len;
  int dl_len = get_datalink_size(bpf_datalink(bpf));
  char * packet;
  DWORD then, now;
  
  if (timeout < 1000) timeout *= 1000;
  then = GetTickCount();
  for(;;)
  {
  packet = (char*)bpf_next(bpf, &len);
  
  if(packet != NULL)
  	break;
	
  if(timeout != 0)
   {
    now = GetTickCount();
    if(now - then >= timeout)break;
   }
  }

  if(packet)
  {
   struct ip * ip;   
   ip = (struct ip*)(packet + dl_len);
   sz = UNFIX(ip->ip_len);
   ret = emalloc(sz);
  
   is_ip = (ip->ip_v == 4);
   if(is_ip)
   {
	
	 bcopy(ip, ret, sz);
   }
   else {
	   sz = len - dl_len;
	   bcopy(ip, ret, sz);
   }
  }
  else {
    bpf_close(bpf);
    unset_pktlock();
    return NULL;
  }
 }
 bpf_close(bpf);

 unset_pktlock();
 
 retc = alloc_tree_cell(0, NULL);
 
 retc->type = CONST_DATA;
 retc->x.str_val = (char*)ret;
 retc->size = sz;
 
 return retc;
}
