#include "port.h"
#include "tcpscan.h"

int GetPortArray(char *szLine, BOOL *PortPool)
{
    char *p = NULL;
    char *q = NULL;
    char *r = NULL;
    char *s = NULL;
    char temp[1024];
    char *t;
    int i;
    
    int nMin, nMax, nCount = 0;
    
    for (i=0; i<MAX_PORT_NUM; i++)
        PortPool[i] = FALSE;
    
    p = szLine;
    for (q=r=p; q; q++) {
        if ((*q==',') || (*q=='\0')) {
            s = NULL;
            strncpy(temp, r, q-r);
            *(temp+(q-r)) = 0;
            if (s = strstr(temp, "-")) {
                *s = 0;
                s++;
                t = temp;
                while (*t == ' ' || *t == '\t') t++;
                nMin = atoi(t);
                if (nMin < 0) nMin = 0;
                else if (nMin >65535) nMin = 65535;
                
                while (*s == ' ' || *s == '\t') s++;
                nMax = atoi(s);
                if (nMax < 0) nMax =0;
                else if (nMax >65535) nMax = 65535;
                
                for (i=nMin; i<=nMax; i++) {
                    PortPool[i] = TRUE;
                    nCount++;
                }
            }
            else {
                t = temp;
                while (*t == ' ' || *t == '\t' ) t++;
                nMin = atoi(t);
                if (nMin < 0) nMin = 0;
                else if (nMin >65535) nMin = 65535;
                PortPool[nMin] = TRUE;
                nCount++;
            }
            r = q+1;
        }
        if (*q == '\0') break;
    }
    
    return nCount;
    
}

void BlockSocket(SOCKET sock, BOOL block)
{
    ULONG ulInBuffer;

    if (block) ulInBuffer = 0L;
    else ulInBuffer = 1L;

    if (SOCKET_ERROR == ioctlsocket(sock, FIONBIO, &ulInBuffer))
        printf("ioctlsocket failed. Error: %d\n", WSAGetLastError());

    return;
}

int FastTcpScan(char *szHostName, BOOL *PortPool, struct arglist *desc)
{
    int             nCount = 0,
                    nCheckPortNum = 0,
                    nCurrentSocket = 0,
                    nOutNum = 0,
                    nDeadIndex = 0,
                    nRtn = 0,
                    sockaddr_in_len = 0;                      
    char            temp[1024],
                    buf[65536];
    SOCKET          ServerSock[MAX_PARALLEL_SOCKNUM] = {0};
    int             PortArray[MAX_PORT_NUM] = {0},
                    DeadStack[MAX_PARALLEL_SOCKNUM] = {0},
                    PortNo[MAX_PARALLEL_SOCKNUM] = {0};
    SOCKADDR_IN     destination_sin,
                    stranger;        
    fd_set          fdsWrite,
                    fdsRead,
                    fdsError;
    DWORD           dwLastTime, dwTimeOut;

    int             i, j, k;
    BOOL            bIsSet;
    struct timeval  tsWait;

    int             nVerbose = 0;                   //是否显示详细扫描信息
    int             *pnTimeout;                     //插件超时
    char            *szCurrentSchedule = NULL;
    char            szTempBuf[1024] = {0};
    char            szLine[1024] = {0};

    nVerbose = plug_get_parameter(desc, "verbose");
    pnTimeout = plug_get_parameter(desc, "Timeout");
    szCurrentSchedule = (char *)plug_get_parameter(desc, "CurrentSchedule");

    FD_ZERO(&fdsRead);
    FD_ZERO(&fdsWrite);
    FD_ZERO(&fdsError);
    
    memset(PortArray, 0, sizeof(PortArray));
    
    // initialize port array
    for (i=0, j=0; ; i++) {
        if (PortPool[i]) PortArray[j++] = i;    
        if (i == MAX_PORT_NUM -1) break;
    }
    nCheckPortNum = j;
    
    for (i=0; i<MAX_PARALLEL_SOCKNUM; i++) {
        DeadStack[nDeadIndex++] = i;
        PortNo[i] = 0;
    }
    nDeadIndex--;

    for (i=0; i<MAX_PARALLEL_SOCKNUM; i++)
        ServerSock[i] = INVALID_SOCKET;

    // Fill out the server socket's address information.
    destination_sin.sin_family = AF_INET;
    destination_sin.sin_addr.s_addr = inet_addr(szHostName);
    
    dwTimeOut = SELECT_TIMEOUT;

    j = 0;

    tsWait.tv_sec = 0;
    tsWait.tv_usec = 0;

    while (PortArray[j] != 0 || nOutNum != 0) {
        for (i=nOutNum; i<MAX_PARALLEL_SOCKNUM && PortArray[j]!=0; j++, i++) {
            if (dwTimeOut) Sleep(1);
            nCurrentSocket = DeadStack[nDeadIndex--];
            if (nCurrentSocket >= MAX_PARALLEL_SOCKNUM) nCurrentSocket = MAX_PARALLEL_SOCKNUM-1;
            if (nCurrentSocket < 0) nCurrentSocket = 0;
            nOutNum++;
            PortNo[nCurrentSocket] = PortArray[j];

            if ((ServerSock[nCurrentSocket] = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
                printf("Allocate socket failed.\n");
                return -1;
            }
#ifdef _XSCAN_PLUG
            // Display the current schedule
            GetString("PORT-PORT", szLine, NULL, NULL);
            sprintf(szTempBuf, "%s: %d", szLine, PortArray[j]);
            GetString("ACT-CHECKING", szLine, szTempBuf, NULL);
            strcpy(szCurrentSchedule, szLine);
            if (pnTimeout) *pnTimeout = 0;
#endif
            fprintf(stdout, "%d\r", PortArray[j]);

            // Convert to network ordering.
            destination_sin.sin_port = htons(PortArray[j]);
            BlockSocket(ServerSock[nCurrentSocket], FALSE);
            // Establish a connection to the server socket.
            if (0 == connect(ServerSock[nCurrentSocket], 
                (PSOCKADDR) &destination_sin, 
                sizeof (destination_sin))) {
                closesocket(ServerSock[nCurrentSocket]);
                if (AlertUser(desc, PortNo[nCurrentSocket])) {
                    nCount++;
                }
            }
            else {
                switch (WSAGetLastError()) {
                case WSAEWOULDBLOCK :
                    BlockSocket(ServerSock[nCurrentSocket], TRUE);
                case WSAEALREADY:
                    FD_SET(ServerSock[nCurrentSocket], &fdsWrite);
                    FD_SET(ServerSock[nCurrentSocket], &fdsRead);
                    FD_SET(ServerSock[nCurrentSocket], &fdsError);
                    break;
                case WSAECONNREFUSED:
                    DeadStack[++nDeadIndex] = nCurrentSocket;
                    nOutNum--;
                    PortNo[nCurrentSocket] = 0;
                    closesocket(ServerSock[nCurrentSocket]);
                    break;
                default:
                    closesocket(ServerSock[nCurrentSocket]);
                    printf("Strange error in connect\n");
                    return -1;
                }
            }
            if (dwTimeOut) dwLastTime = GetTickCount();
        }

        if (PortArray[j] == 0 && dwTimeOut) Sleep(2000);

        if ((nRtn = select(0, &fdsRead, &fdsWrite, &fdsError, &tsWait)) < 0) {
            printf("select error=%d, %d\n", nRtn, WSAGetLastError());
            return -1;
        }

        for (k=MAX_PARALLEL_SOCKNUM-1; k>=0; k--) {
            if (dwTimeOut) Sleep(1);
            if (!PortNo[k]) continue;
            bIsSet = FALSE;
            if (FD_ISSET(ServerSock[k], &fdsWrite)
                && FD_ISSET(ServerSock[k], &fdsRead)) {
                if (AlertUser(desc, PortNo[k])) {
                    nCount++;
                }
                bIsSet = TRUE;
                FD_CLR(ServerSock[k], &fdsWrite);
                FD_CLR(ServerSock[k], &fdsRead);
            }
            else if (FD_ISSET(ServerSock[k], &fdsWrite)) {
                if (AlertUser(desc, PortNo[k])) {
                    nCount++;
                }
                bIsSet = TRUE;
                FD_CLR(ServerSock[k], &fdsWrite);
            }
            else if (FD_ISSET(ServerSock[k], &fdsRead)) {
                if (AlertUser(desc, PortNo[k])) {
                    nCount++;
                }
                bIsSet = TRUE;
                FD_CLR(ServerSock[k], &fdsRead);
            }
            else if (FD_ISSET(ServerSock[k], &fdsError)) {
                int type = 0;
                if (!plug_get_key(desc, "PortScan/ClosePort", &type))
                    plug_set_key(desc, "PortScan/ClosePort", ARG_INT, PortNo[k]);
                bIsSet = TRUE;
                FD_CLR(ServerSock[k], &fdsError);
            }
            else if (dwTimeOut) {
                FD_SET(ServerSock[k], &fdsWrite);
                FD_SET(ServerSock[k], &fdsRead);
                FD_SET(ServerSock[k], &fdsError);
            }

            if (bIsSet || !dwTimeOut) {
                DeadStack[++nDeadIndex] = k;
                nOutNum--;
                closesocket(ServerSock[k]);
                PortNo[k] = 0;
                if (dwTimeOut) dwLastTime = GetTickCount();
            }
            else if (GetTickCount()-dwLastTime > dwTimeOut) {
		//2012.3.15
		//Edit:   
		//	   在小于64个端口时,nDeadIndex过大,DeadStack数组越界
		//
		nDeadIndex = -1;
                for (k=MAX_PARALLEL_SOCKNUM-1; k>=0; k--) {
                    FD_CLR(ServerSock[k], &fdsWrite);
                    FD_CLR(ServerSock[k], &fdsRead);
                    FD_CLR(ServerSock[k], &fdsError);
                    DeadStack[++nDeadIndex] = k;
                    closesocket(ServerSock[k]);
                    PortNo[k] = 0;
                }
                nOutNum = 0;
                dwLastTime = GetTickCount();
                break;
            }
        }
    }

    return nCount;
}
