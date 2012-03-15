#include <winsock2.h>
#include <process.h>

#include "port.h"
#include "synscan.h"

#define MAX_PACK_LEN        1024*8          //���յ����IP����
#define MAX_ADDR_LEN        16+MAX_PATH+2   //���ʮ���Ƶ�ַ����󳤶�+������

//��������
USHORT checksum(USHORT *buffer, int size) 
{
    unsigned long cksum=0;

    while (size > 1) {
        cksum += *buffer++;
        size -= sizeof(USHORT);
    }

    if (size) {
        cksum += *(UCHAR*)buffer;
    }
    cksum = (cksum >> 16) + (cksum & 0xffff);
    cksum += (cksum >> 16);
    return (USHORT)(~cksum);
}

int tcp_status(int sd, char *type, DWORD timeout)
{
    fd_set rset, wset, eset;
    fd_set FAR *prset = NULL, *pwset = NULL, *peset = NULL;
    struct timeval tval;
    int i, status, err_no = 0;
    DWORD t1, t2;
    MSG msg;

    if (timeout <1000) timeout = timeout * 1000;

    tval.tv_sec = 0;
    tval.tv_usec = 0;
    t1 = GetTickCount();
    t2 = t1;
    while(t2-t1 <= timeout)
    {
        Sleep(10);
        FD_ZERO(&rset);
        FD_ZERO(&wset);
        FD_ZERO(&eset);

        for(i = 0; i<(int)strlen(type); i++)
        {
            if(type[i] == 'r') { FD_SET(sd, &rset); prset = &rset; }
            if(type[i] == 'w') { FD_SET(sd, &wset); pwset = &wset; }
            if(type[i] == 'e') { FD_SET(sd, &eset); peset = &eset; }
        }
        status = select(-1/*sd+1*/, prset, pwset, peset, &tval);
        err_no = WSAGetLastError();
        t2 = GetTickCount();
        if(status == 0)
        {
            if(PeekMessage(&msg, 0, NULL, NULL, PM_REMOVE))
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
                if(msg.message == WM_QUIT) return -1;
            }
            if(t2-t1 <timeout) continue;
            else
            {
                if(prset) FD_CLR((UINT)sd,&rset);
                if(pwset) FD_CLR((UINT)sd,&wset);
                if(peset) FD_CLR((UINT)sd,&eset);
                SetLastError(WSAETIMEDOUT);
                return -10;
            }
        }

        if(peset && FD_ISSET(sd, peset))
        {
            if(prset != NULL) FD_CLR((UINT)sd,&rset);
            if(pwset != NULL) FD_CLR((UINT)sd,&wset);
            if(peset != NULL) FD_CLR((UINT)sd,&eset);
            /*
            len = sizeof(errno);
            getsockopt(sd, SOL_SOCKET, SO_ERROR, (char *)&errno, &len);
            */
            return -1;
        }
        if((prset && FD_ISSET(sd, prset)) || (pwset && FD_ISSET(sd, pwset)))
        {
            /*
            len = sizeof(errno);
            getsockopt(sd, SOL_SOCKET, SO_ERROR, (char *)&errno, &len);
            */
        }
        if(prset != NULL) FD_CLR((UINT)sd,&rset);
        if(pwset != NULL) FD_CLR((UINT)sd,&wset);
        if(peset != NULL) FD_CLR((UINT)sd,&eset);

        if(err_no == WSAEINTR) return WSAEINTR;
        if(err_no)
        {
            return -1;
        }
        else break;

    }
    return 0;
}

DWORD WINAPI DoRecvPacket(LPVOID pParam)
{
    char            szSrcHost[128];
    char            szRemoteHost[128];
    char            szRemoteIP[50];
    int             nScanMethod;
    int             *pnNodata = NULL;
    int             *pnLoop = NULL;
    int             *pnLocalThreads = NULL;
    int             *pnTotalThreads = NULL;
    int             *pnOpened = NULL;
    struct arglist  *desc = NULL;
    struct arglist  *key = NULL;
    char            szLine[MAX_PATH] = {0};

    char            pRecvBuf[RECV_BUFSIZE+1] = {0};
    unsigned int    nBufSize = 0;
    SOCKET          nSockListen = 0;
    DWORD           dwDest;
    long            *lPhe;
    HOSTENT         *phe;
    int             nPort = 0;
    DWORD           dwTimeout = 0;
    DWORD           dwStartTime = 0;
    int             nErrorCode;
    BOOL            bRecvPacket = FALSE;

    IP_HEADER       *ip_hdr = NULL;
    TCP_HEADER      *tcp_hdr = NULL;
    USHORT          ip_hdr_len = 0;
    UINT            ack = 0;
    USHORT          sport = 0;
    UCHAR           flag = 0;
    DWORD           source_addr = 0;
    
    DWORD           dwBufferLen[10] ;
    DWORD           dwBufferInLen = 1 ;
    DWORD           dwBytesReturned = 0 ;
    int             nSourcePort = 7000;
    SOCKADDR_IN     sa;
    int             nProtocol = 0;

    RECV_THREAD_INFO *thread_info = NULL;
    thread_info = (RECV_THREAD_INFO*)pParam;

    strcpy(szSrcHost, thread_info->szSrcHost);
    strcpy(szRemoteHost, thread_info->szRemoteHost);
    strcpy(szRemoteIP, thread_info->szRemoteIP);
    nScanMethod = thread_info->nScanMethod;
    pnLoop = thread_info->pnLoop;
    pnOpened = thread_info->pnOpened;
    desc = thread_info->desc;
    key = arg_get_value(desc, "key");

    nSockListen = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    CheckSockError(nSockListen, "socket");

    while (nSourcePort < 65536) {
        //��ñ���IP
        sa.sin_addr.s_addr = inet_addr(szSrcHost);
        sa.sin_family = AF_INET;
        sa.sin_port = htons(nSourcePort);
        nErrorCode = bind(nSockListen, (PSOCKADDR)&sa, sizeof(sa));
        if (nErrorCode != SOCKET_ERROR)
            break;
        nSourcePort++;
    }

    if (nErrorCode == SOCKET_ERROR) goto f_exit;

    //����SOCK_RAWΪSIO_RCVALL���Ա�������е�IP��
    nErrorCode = WSAIoctl(nSockListen, SIO_RCVALL, &dwBufferInLen, sizeof(dwBufferInLen),
        &dwBufferLen, sizeof(dwBufferLen), &dwBytesReturned, NULL, NULL);
    CheckSockError(nErrorCode, "Ioctl");
    if (nErrorCode == SOCKET_ERROR) goto f_exit;

    //���Ŀ��������ַ
    dwDest = inet_addr(szRemoteIP);
    if (dwDest == INADDR_NONE) {
        phe = gethostbyname(szRemoteIP);
        if (phe) {
            lPhe = (long*)(*phe->h_addr_list);
            dwDest = *lPhe;
        }
    }

    //��������
    dwTimeout = RECV_TIMEOUT;
    dwStartTime = GetTickCount();

    while (1) {
        if (*pnOpened && !bRecvPacket) break;

        //��ʱ,RECV_TIMEOUT s��ʱ
        if ((GetTickCount()-dwStartTime) > dwTimeout) {
            if ( ((*pnLoop) > 0) && (nScanMethod == SCAN_SYN) ) {
                dwStartTime = GetTickCount();//��ʱ����
            }
            else goto f_exit;
        }

        if (tcp_status(nSockListen, "r", 1) < 0)
            continue;

        memset(pRecvBuf, 0, sizeof(pRecvBuf));
        nErrorCode = recv(nSockListen, pRecvBuf, RECV_BUFSIZE, 0);
        CheckSockError(nErrorCode, "recv");
        nBufSize = nErrorCode;
        if (nBufSize <= 0) goto f_exit;

        //����SOCK_RAWΪSIO_RCVALL���Ա�������е�IP��
        nErrorCode = WSAIoctl(nSockListen, SIO_RCVALL, &dwBufferInLen, sizeof(dwBufferInLen),
            &dwBufferLen, sizeof(dwBufferLen), &dwBytesReturned, NULL, NULL);
        CheckSockError(nErrorCode, "Ioctl");
        if (nErrorCode == SOCKET_ERROR) goto f_exit;

        ip_hdr = (IP_HEADER *)pRecvBuf;
        ip_hdr_len = sizeof(unsigned long) * (ip_hdr->h_lenver & 0xf);
        source_addr = ip_hdr->sourceIP;

        nProtocol = ip_hdr->proto;
        switch (nProtocol) {
        case IPPROTO_TCP:
            if ( nBufSize < (ip_hdr_len + sizeof(TCP_HEADER)) ) {
                fprintf(stderr, "Packet length error: %d, request length: %d\n", nBufSize, ip_hdr_len + sizeof(TCP_HEADER));
                continue;
            }

            tcp_hdr = (TCP_HEADER*)(pRecvBuf + ip_hdr_len);
            sport = tcp_hdr->th_sport;
            ack = tcp_hdr->th_ack;
            flag = tcp_hdr->th_flag;
            nPort = ntohs(sport); 

            //��ַ�Ƿ���ȷ
            if(source_addr != dwDest)
                continue;

            bRecvPacket = TRUE;

            //RST/ACK - �˿�δ����
            if ((flag & ~TH_URG) == (TH_RST | TH_ACK)) {
                int type = 0;
                if (!plug_get_key(desc, "PortScan/ClosePort", &type))
                    plug_set_key(desc, "PortScan/ClosePort", ARG_INT, nPort);
                dwStartTime = GetTickCount(); //��ʱ����
                continue;
            }

            //SYN/ACK - �˿ڿ���
            if ((flag & ~TH_URG) == (TH_SYN | TH_ACK)) {
                if (nScanMethod == SCAN_SYN) {
                    if (AlertUser(desc, nPort)) {
                        (*pnOpened)++;
                    }
                    dwStartTime = GetTickCount(); //��ʱ����
                }
            }
        break;
        case IPPROTO_UDP:
        break;
        case IPPROTO_ICMP:
        break;
        default: break;
        }

    }

f_exit:

    closesocket(nSockListen);
    
    return 1;
}

int SendSyn(SOCKADDR_IN sa, SOCKADDR_IN dest, int nPort)
{
    int             nDataSize;
    DWORD           dwSeq;
    unsigned char   ucTtl;
    USHORT          usWin;
    int             nSourcePort;
    long            *lPhe = 0;
    HOSTENT         *phe = NULL;
    IP_HEADER       ip_header;
    TCP_HEADER      tcp_header;
    PSD_HEADER      psd_header;
    IP_HEADER       *ip_hdr = NULL;
    TCP_HEADER      *tcp_hdr = NULL;
    
    unsigned short  ip_hdr_len = 0;
    unsigned int    ack = 0;
    USHORT          sport = 0;
    unsigned char   flag = 0;
    DWORD           source_addr = 0;

    char            pSendBuf[128] = {0};
    char            pRecvBuf[RECV_BUFSIZE+1] = {0};
    SOCKET          nSockRaw = 0;

    BOOL            bOpt = TRUE;
    int             nErrorCode;
    int             nRtn = 0;

    dwSeq = SEQ_VALUE + nPort * 2;
    ucTtl = nPort % 20 + 50;
    nSourcePort = nPort % 20000 + SOURCE_PORT;

    usWin = htons(1024 * (ucTtl % 4 + 1));
    nSockRaw = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
    CheckSockError(nSockRaw, "socket()");

    //����IPͷ����ѡ��
    bOpt = TRUE;
    nErrorCode = setsockopt(nSockRaw, IPPROTO_IP, IP_HDRINCL,
        (char *)&bOpt, sizeof(bOpt));
    CheckSockError(nErrorCode, "setsockopt()"); 
    
    if (nErrorCode == SOCKET_ERROR) {
        closesocket(nSockRaw);
        return -1;
    }

    //printf("source address: %s\n",inet_ntoa(sa.sin_addr));
    //printf("target address: %s\n",inet_ntoa(dest.sin_addr));

    //���IP�ײ�
    ip_header.h_lenver = (4<<4 | sizeof(ip_header)/sizeof(unsigned long));
    //����λIP�汾�ţ�����λ�ײ�����
    ip_header.total_len = htons(sizeof(IP_HEADER) + sizeof(TCP_HEADER)); //16λ�ܳ��ȣ��ֽڣ�
    ip_header.ident = 1; //16λ��ʶ
    ip_header.frag_and_flags = 0; //3λ��־λ
    ip_header.ttl = ucTtl; //8λ����ʱ��TTL
    ip_header.proto = IPPROTO_TCP; //8λЭ��(TCP,UDP��)
    ip_header.checksum = 0; //16λIP�ײ�У���
    ip_header.sourceIP = sa.sin_addr.s_addr; //32λԴIP��ַ
    ip_header.destIP = dest.sin_addr.s_addr; //32λĿ��IP��ַ

    //���TCP�ײ�
    tcp_header.th_sport = htons(nSourcePort); //Դ�˿ں�
    tcp_header.th_dport = htons(nPort); //Ŀ�Ķ˿ں�
    tcp_header.th_seq = htonl(dwSeq); //SYN���к�
    tcp_header.th_ack = 0; //ACK���к���Ϊ0
    tcp_header.th_lenres = (sizeof(TCP_HEADER)/4<<4|0); //TCP���Ⱥͱ���λ
    tcp_header.th_flag = 2; //SYN ��־
    tcp_header.th_win = usWin; //���ڴ�С
    tcp_header.th_urp = 0; //ƫ��
    tcp_header.th_sum = 0; //У���

    //���TCPα�ײ������ڼ���У��ͣ������������ͣ�
    psd_header.saddr = ip_header.sourceIP;
    psd_header.daddr = ip_header.destIP;
    psd_header.mbz = 0;
    psd_header.ptcl = IPPROTO_TCP;
    psd_header.tcpl = htons(sizeof(tcp_header));

    //����TCPУ��ͣ�����У���ʱ��Ҫ����TCP pseudo header 
    memcpy(pSendBuf, &psd_header, sizeof(psd_header)); 
    memcpy(pSendBuf + sizeof(psd_header), &tcp_header, sizeof(tcp_header));
    tcp_header.th_sum = checksum((USHORT *)pSendBuf, sizeof(psd_header) + sizeof(tcp_header));

    //����IPУ���
    memset(pSendBuf, 0, sizeof(pSendBuf));
    memcpy(pSendBuf, &ip_header, sizeof(ip_header));
    memcpy(pSendBuf + sizeof(ip_header), &tcp_header, sizeof(tcp_header));
    //memset(pSendBuf + sizeof(ip_header) + sizeof(tcp_header), 0, 4);
    nDataSize = sizeof(ip_header) + sizeof(tcp_header);
    ip_header.checksum = checksum((USHORT *)&ip_header, sizeof(ip_header));

    //��䷢�ͻ�����
    if ( !memcpy(pSendBuf, &ip_header, sizeof(ip_header)) ) {
        closesocket(nSockRaw);
        return -1;
    }

    //����TCP����
    nErrorCode = sendto(nSockRaw, pSendBuf, nDataSize, 0,
        (struct sockaddr*) &dest, sizeof(dest));
    if (nErrorCode == SOCKET_ERROR) {

        //printf("source address - %s: %d\n", inet_ntoa(sa.sin_addr), ntohs(sa.sin_port));
        //printf("target address - %s: %d\n", inet_ntoa(dest.sin_addr), ntohs(dest.sin_port));
        
        CheckSockError(nErrorCode, "sendto()");
        closesocket(nSockRaw);
        return -1;
    }
    else
        nRtn = 1;

    closesocket(nSockRaw);

    return nRtn;
}

