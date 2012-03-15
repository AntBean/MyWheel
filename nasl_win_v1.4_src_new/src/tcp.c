
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>

#include "tcp.h"

int wsa_ok = 0, err_no = 0;
void peek_message(void);
int get_local_ip(char *ip);

int tcp_init()
{
    WSADATA wsa;

    if(WSAStartup(MAKEWORD(1, 1), &wsa) != 0)
        return -1;
    wsa_ok = 1;
    return 0;
}

int tcp_exit()
{
    if(wsa_ok) WSACleanup();

    return 0;
}

int islocalhost(struct in_addr *addr)
{
    char hostname[128] = {0};
    struct hostent *phe;
    int res = 0, i = 0;

    if (addr->S_un.S_addr == inet_addr("127.0.0.1")) {
        res = 1;
    }
    else if( gethostname(hostname, sizeof(hostname)-1 ) == 0 ) {
        phe = gethostbyname(hostname);
        for( i = 0; phe != NULL && phe->h_addr_list[i] != NULL; i++ ) {
            if ( addr->S_un.S_un_b.s_b1 == (UINT)((UCHAR*)phe->h_addr_list[i])[0] &&
                 addr->S_un.S_un_b.s_b2 == (UINT)((UCHAR*)phe->h_addr_list[i])[1] &&
                 addr->S_un.S_un_b.s_b3 == (UINT)((UCHAR*)phe->h_addr_list[i])[2] &&
                 addr->S_un.S_un_b.s_b4 == (UINT)((UCHAR*)phe->h_addr_list[i])[3] ) {
                res = 1; break;
            }
        }
    }

    return res;
}

int is_local_ip(struct in_addr addr)
{
    return islocalhost(&addr);
}

char *routethrough(struct in_addr *dest, struct in_addr *source)
{
    char hostname[128] = {0};
    struct hostent *phe;
    int i = 0;

    if( gethostname(hostname, sizeof(hostname)-1 ) == 0 ) {
        phe = gethostbyname(hostname);
        for( i = 0; phe != NULL && phe->h_addr_list[i] != NULL; i++ ) {
            source->S_un.S_un_b.s_b1 = (UINT)((UCHAR*)phe->h_addr_list[i])[0];
            source->S_un.S_un_b.s_b2 = (UINT)((UCHAR*)phe->h_addr_list[i])[1];
            source->S_un.S_un_b.s_b3 = (UINT)((UCHAR*)phe->h_addr_list[i])[2];
            source->S_un.S_un_b.s_b4 = (UINT)((UCHAR*)phe->h_addr_list[i])[3];
        }
    }

    return NULL;
}

/* From tcptraceroute, convert a numeric IP address to a string */
#define IPTOSBUFFERS	12
char *iptos(u_long in)
{
	static char output[IPTOSBUFFERS][3*4+3+1];
	static short which;
	u_char *p;

	p = (u_char *)&in;
	which = (which + 1 == IPTOSBUFFERS ? 0 : which + 1);
	sprintf(output[which], "%d.%d.%d.%d", p[0], p[1], p[2], p[3]);
	return output[which];
}
