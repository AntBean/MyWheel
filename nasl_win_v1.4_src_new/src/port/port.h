#ifndef PORT_H_
#define PORT_H_

#include <stdio.h>
#include <conio.h>
#include <process.h>
#include <assert.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iprtrmib.h>
#include <sys/timeb.h>
#include "mstcpip.h"

#include "../includes.h"
#include "../libnessus.h"

#ifndef bzero
#define bzero(s, n) memset((s), 0, (n))
#endif

#define MAX_PORT_NUM 65536
#define MAX_PARALLEL_SOCKNUM (FD_SETSIZE)
#define SELECT_TIMEOUT 10000
#define LOCAL_SELECT_TIMEOUT 0

#define PORTSCAN_PLUGIN_NAME    "TCP port"

typedef struct _PORT_THREAD_INFO
{
    char    szRemoteHost[128]; 
    char    szRemoteIP[50];
    char    szLocalIP[50];
    int     nPort;
    int     nScanMethod;
    int     *pnTotalThreads;
    int     *pnLocalThreads;
    int     *pnLeavings;
    int     *pnOpened;
    struct arglist *key;
} PORT_THREAD_INFO; 

typedef struct _RECV_THREAD_INFO
{
    char    szSrcHost[128];
    char    szRemoteHost[128];
    char    szRemoteIP[50];
    int     nScanMethod;
    int     *pnLoop;
    int     *pnOpened;
    struct arglist *desc;
} RECV_THREAD_INFO; 


void CheckSockError(int nErrorCode, char *pErrorMsg);

#endif