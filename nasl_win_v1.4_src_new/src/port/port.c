/*****************************************************************
  文件名        : port.cpp
  描述          : 端口扫描插件(vc++)
  作者          : glacier
  最后修改日期  : 2003-10-20

  Copyright (C) 2001 http://www.xfocus.org   All Rights Reserved
 *****************************************************************/


#include <process.h>

#include "port.h"
#include "synscan.h"
#include "tcpscan.h"

//SOCK错误处理程序
void CheckSockError(int nErrorCode, char *pErrorMsg)
{
    char szLine[MAX_PATH] = {0};
    if(nErrorCode==SOCKET_ERROR) {
        int nErr = WSAGetLastError();
        fprintf(stderr, "%s Error: %d\n", pErrorMsg, nErr);
    }
}

BOOL GetLocalIP(char *szLocalIP)
{
    char szHostName[128+1] = "\0";
    HOSTENT *phe;
    int i;
    if( gethostname(szHostName, 128 ) == 0 ) {
        // Get host adresses
        phe = gethostbyname(szHostName);
        for( i = 0; phe != NULL && phe->h_addr_list[i]!= NULL; i++ )
        {
            sprintf(szLocalIP, "%d.%d.%d.%d", 
                (UINT)((UCHAR*)phe->h_addr_list[i])[0],
                (UINT)((UCHAR*)phe->h_addr_list[i])[1],
                (UINT)((UCHAR*)phe->h_addr_list[i])[2], 
                (UINT)((UCHAR*)phe->h_addr_list[i])[3]);
        }
    }
    else
        return FALSE;

    return TRUE;
}

BOOL GetRemoteIP(char *szRemoteHost, char *szRemoteIP)
{
    long            *lPhe;
    HOSTENT         *phe;
    SOCKADDR_IN     dest;
    DWORD           dwAddress;

    dwAddress = inet_addr(szRemoteHost);
    if (dwAddress == INADDR_NONE) {
        phe = gethostbyname(szRemoteHost);
        if (phe) {
            lPhe = (long*)(*phe->h_addr_list);
            dest.sin_addr.s_addr = *lPhe;
        }
    }
    else {
        dest.sin_addr.s_addr = dwAddress;
    }
    strcpy(szRemoteIP, inet_ntoa(dest.sin_addr));

    return strlen(szRemoteIP);
}

int GetPortCount(BOOL *PortPool)
{
    int i, nCount = 0;

    for (i=0; i<MAX_PORT_NUM; i++) {
        if (PortPool[i]) nCount++;
    }

    return nCount;
}

BOOL TestRawSocket()
{
    int nErrorCode = 0;
    BOOL bRtn = FALSE;
    int nTestSocket = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    
    if (nTestSocket != INVALID_SOCKET) {
        DWORD dwBufferLen[10] ;
        DWORD dwBufferInLen = 1 ;
        DWORD dwBytesReturned = 0 ;
        int nSourcePort = 7000;
        char szLocalIP[MAX_PATH] = {0};
        SOCKADDR_IN sa;

        GetLocalIP(szLocalIP); //获得本地IP
        while (nSourcePort < 65536) {
            sa.sin_addr.s_addr = inet_addr(szLocalIP);
            sa.sin_family = AF_INET;
            sa.sin_port = htons(nSourcePort);
            nErrorCode = bind(nTestSocket, (PSOCKADDR)&sa, sizeof(sa));
            if (nErrorCode != SOCKET_ERROR)
                break;
            nSourcePort++;
        }
        nErrorCode = WSAIoctl(nTestSocket, SIO_RCVALL, &dwBufferInLen, sizeof(dwBufferInLen),
            &dwBufferLen, sizeof(dwBufferLen), &dwBytesReturned , NULL , NULL );
        if (nErrorCode == 0) bRtn = TRUE;
    }

    closesocket(nTestSocket);

    return bRtn;
}

VOID GetServiceName(struct arglist *desc, int nPort, char *szService)
{
    nessus_get_svc_name(desc, nPort, "tcp", szService);
}

BOOL AlertUser(struct arglist *desc, int nPort)
{
    char szLine[1024] = {0};
    int type = 0;

    sprintf(szLine, "Ports/tcp/%d", nPort);
    if (!plug_get_key(desc, szLine, &type)) {
        plug_set_key(desc, szLine, ARG_INT, 1);
        if (!plug_get_key(desc, "PortScan/OpenPort", &type))
            plug_set_key(desc, "PortScan/OpenPort", ARG_INT, nPort);

#ifdef _XSCAN_PLUG
        if (!plug_get_parameter(desc, "Port/IdentifyService")) {
            GetServiceName(desc, nPort, szLine);
            post_note(desc, nPort, szLine);
        }
        else {
            char szAlert[MAX_PATH] = {0};
            char szMember[MAX_PATH] = {0};

            GetString("PORT-STATE", szMember, NULL, NULL);
            sprintf(szAlert, "%s: %d/tcp", szMember, nPort);
            PutAlert(desc, 0x102, szAlert);
        }
#else
        text_color(15);
        printf("Found %s\n", szLine);
        text_color(7);
#endif
        return TRUE;
    }
    return FALSE;
}

int port_plugin_init(struct arglist *desc)
{
    struct kb_item **kb = plug_get_kb(desc);
    if (!kb) {
        kb = kb_new();
        arg_add_value(desc, "key", ARG_PTR, -1, kb);
    }
    plug_set_key(desc, "Host/scanned", ARG_INT, 1);
    arg_add_value(desc, "NAME", ARG_STRING, strlen(PORTSCAN_PLUGIN_NAME), estrdup(PORTSCAN_PLUGIN_NAME));

    return 0;
}

int port_plugin_run(struct arglist *desc)
{
    char            szHostName[MAX_PATH] = {0};     //主机地址
    int             nMaxThread = 0;                 //最大线程数量
    int             *pnCurrentThreads;              //当前所有线程总数量
    int             *pnPluginThreads;               //插件线程总数
    int             *pnTimeout;                     //插件超时
    int             nVerbose = 0;                   //是否显示详细扫描信息
    char            *szCurrentSchedule = NULL;
    struct kb_item  **kb = NULL;

    WSADATA         wsaData;
    char            szPortList[MAX_PATH+1] = {0};
    char            szLocalHost[MAX_PATH] = {0};
    char            szLocalIP[50] = {0};
    char            szRemoteIP[50] = {0};
    HANDLE          hThreads[64] = {0};
    int             nThreadNum = 0;
    DWORD           dwThreadID = 0, dwExitCode = 0;
    int             nTotalPort = 0;
    int             nLeavings = 0;
    int             nOpenPort = 0;
    int             nScanMethod = 0;
    int             *pnCurrHosts;
    int             nSleepTime = 10;

    SOCKADDR_IN     sa, dest;
    struct hostent  *phe = NULL;
    int             i = 0, j = 0;
    int             nErrorCode = 0;
    char            *ptr1 = NULL, *ptr2 = NULL;
    char            szScanMethod[MAX_PATH] = {0};
    BOOL            PortPool[MAX_PORT_NUM];
    char            szLine[1024] = {0};
    char            szTempBuf[1024] = {0};

    RECV_THREAD_INFO *recv_thread_info = NULL;

    kb = plug_get_kb(desc);
    if (!kb) return 0;
    strcpy(szHostName, plug_get_hostname(desc));
    nVerbose = plug_get_parameter(desc, "verbose");
    strcpy(szScanMethod, plug_get_parameter(desc, "scan_method"));
    strcpy(szPortList, plug_get_parameter(desc, "port_range"));
    nMaxThread = plug_get_parameter(desc, "max_threads");
    pnCurrentThreads = plug_get_parameter(desc, "curr_threads");
    pnPluginThreads = plug_get_parameter(desc, "plugin_threads");
    pnTimeout = plug_get_parameter(desc, "Timeout");
    szCurrentSchedule = (char *)plug_get_parameter(desc, "CurrentSchedule");

    if (stricmp(szScanMethod, "syn") == 0)
        nScanMethod = SCAN_SYN;
    else if (stricmp(szScanMethod, "fasttcp") == 0)
        nScanMethod = SCAN_FASTTCP;
    else
        nScanMethod = SCAN_TCP;

    if ((nScanMethod == SCAN_SYN) && (!TestRawSocket()))
        nScanMethod = SCAN_FASTTCP;
    
    if (nScanMethod == 0)
        nScanMethod = SCAN_TCP;

    GetRemoteIP(szHostName, szRemoteIP);
    GetPortArray(szPortList, PortPool);
    nTotalPort = GetPortCount(PortPool);
    nLeavings = nTotalPort;

#ifndef _XSCAN_PLUG
    if (nVerbose) printf("%d ports shoud be scanned.\n", nTotalPort);
#endif

    switch (nScanMethod) {
    case SCAN_TCP:
    case SCAN_FASTTCP:
#ifdef _XSCAN_PLUG
        while ((*pnCurrentThreads) >= nMaxThread)
            Sleep(10);
        (*pnCurrentThreads)++;
        (*pnPluginThreads)++;
#endif
        nOpenPort = FastTcpScan(szRemoteIP, PortPool, desc);
#ifdef _XSCAN_PLUG
        (*pnCurrentThreads)--;
        (*pnPluginThreads)--;
#endif
        break;

    case SCAN_SYN:
#ifdef _XSCAN_PLUG
        while ((*pnCurrentThreads) >= nMaxThread)
            Sleep(10);
        (*pnCurrentThreads)++;
        (*pnPluginThreads)++;

        pnCurrHosts = (int *)plug_get_parameter(desc, "CurrentHosts");
        if (pnCurrHosts && (*pnCurrHosts) > 1) nSleepTime = (*pnCurrHosts) * 10;
#endif
        if (gethostname(szLocalHost, sizeof(szLocalHost)-1 ) == 0) {
            phe = gethostbyname(szLocalHost);
            for (i = 0; phe != NULL && phe->h_addr_list[i] != NULL; i++) {
                wsprintf(szLocalIP, "%d.%d.%d.%d",
                    (int)phe->h_addr_list[i][0]&0xFF,
                    (int)phe->h_addr_list[i][1]&0xFF,
                    (int)phe->h_addr_list[i][2]&0xFF,
                    (int)phe->h_addr_list[i][3]&0xFF);

                recv_thread_info = (RECV_THREAD_INFO *)emalloc(sizeof(RECV_THREAD_INFO));
                strcpy(recv_thread_info->szSrcHost, szLocalIP);
                strcpy(recv_thread_info->szRemoteHost, szHostName);
                strcpy(recv_thread_info->szRemoteIP, szRemoteIP);
                recv_thread_info->nScanMethod = nScanMethod;
                recv_thread_info->pnLoop = &nLeavings;
                recv_thread_info->pnOpened = &nOpenPort;
                recv_thread_info->desc = desc;
                hThreads[nThreadNum] = CreateThread(NULL, 0, DoRecvPacket, (LPVOID)recv_thread_info, 0, &dwThreadID);
                nThreadNum++;
                if (nThreadNum >= 64) break;
            }
        }

        memset(&sa, 0, sizeof(dest));
        sa.sin_family = AF_INET;
        memset(&dest, 0, sizeof(dest));
        dest.sin_family = AF_INET;
        dest.sin_addr.s_addr = inet_addr(szRemoteIP);

        for (i=0; i<MAX_PORT_NUM; i++) {
            if (PortPool[i]) {
#ifdef _XSCAN_PLUG
                GetString("PORT-PORT", szLine, NULL, NULL);
                sprintf(szTempBuf, "%s: %d", szLine, i);
                GetString("ACT-CHECKING", szLine, szTempBuf, NULL);
                strcpy(szCurrentSchedule, szLine);
                if (pnTimeout) *pnTimeout = 0;
#else
                fprintf(stdout, "%d\r", i);
#endif
                for (j = 0; phe != NULL && phe->h_addr_list[j] != NULL; j++) {
                    wsprintf(szLocalIP, "%d.%d.%d.%d",
                        (int)phe->h_addr_list[j][0]&0xFF,
                        (int)phe->h_addr_list[j][1]&0xFF,
                        (int)phe->h_addr_list[j][2]&0xFF,
                        (int)phe->h_addr_list[j][3]&0xFF);
                    sa.sin_addr.s_addr = inet_addr(szLocalIP);

                    SendSyn(sa, dest, i);
                }
                Sleep(nSleepTime);
            }
        }

        nLeavings = 0;
        if (WaitForMultipleObjects(nThreadNum, hThreads, TRUE, 1000*20) == WAIT_TIMEOUT) {
            for (i=0; i<nThreadNum; i++) {
                GetExitCodeThread(hThreads[i], &dwExitCode);
                ExitThread(dwExitCode);
                CloseHandle(hThreads[i]);
            }
        }
        for (i=0; i<nThreadNum; i++) {
            CloseHandle(hThreads[i]);
        }

#ifdef _XSCAN_PLUG
        (*pnCurrentThreads)--;
        (*pnPluginThreads)--;
#endif
        break;

    default:
        break;
    }

#ifndef _XSCAN_PLUG
    if (nVerbose) printf("Total %d open ports.\n", nOpenPort);
#endif
    
    return nOpenPort;
}

//---------------------------------------------------------------------------
 