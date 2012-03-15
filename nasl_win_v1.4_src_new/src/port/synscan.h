#ifndef _SYNSCAN_H
#define _SYNSCAN_H

#define SCAN_TCP        0x0001
#define SCAN_FASTTCP    0x0002
#define SCAN_SYN        0x0003

#define SOURCE_PORT     7614
#define SEQ_VALUE       0x19760104
#define RECV_TIMEOUT    1000*5
#define RECV_BUFSIZE    1024*10

#define TH_FIN          0x01
#define TH_SYN          0x02
#define TH_RST          0x04
#define TH_PUSH         0x08
#define TH_ACK          0x10
#define TH_URG          0x20

typedef struct _IP_HEADER            //定义IP首部
{
    unsigned char    h_lenver;       //4位首部长度+4位IP版本号
    unsigned char    tos;            //8位服务类型TOS
    unsigned short   total_len;      //16位总长度（字节）
    unsigned short   ident;          //16位标识
    unsigned short   frag_and_flags; //3位标志位+13位片偏移
    unsigned char    ttl;            //8位生存时间 TTL
    unsigned char    proto;          //8位协议 (TCP, UDP 或其他)
    unsigned short   checksum;       //16位IP首部校验和
    unsigned int     sourceIP;       //32位源IP地址
    unsigned int     destIP;         //32位目的IP地址
} IP_HEADER;

typedef struct _TCP_HEADER           //定义TCP首部
{
    USHORT th_sport;                 //16位源端口
    USHORT th_dport;                 //16位目的端口
    UINT   th_seq;                   //32位序列号
    UINT   th_ack;                   //32位确认号
    UCHAR  th_lenres;                //4位首部长度/6位保留字
    UCHAR  th_flag;                  //6位标志位
    USHORT th_win;                   //16位窗口大小
    USHORT th_sum;                   //16位校验和
    USHORT th_urp;                   //16位紧急数据偏移量
} TCP_HEADER; 

typedef struct _PSD_HEADER			//定义TCP伪首部
{
    unsigned long saddr;            //源地址
    unsigned long daddr;            //目的地址
    char mbz;
    char ptcl;                      //协议类型
    unsigned short tcpl;            //TCP长度
} PSD_HEADER;

typedef struct _UDP_HEADER          //定义UDP首部
{
    unsigned short uh_sport;        //16位源端口
    unsigned short uh_dport;        //16位目的端口
    unsigned short uh_len;          //16位长度
    unsigned short uh_sum;          //16位校验和
} UDP_HEADER;

typedef struct _ICMP_HEADER         //定义ICMP首部
{
    BYTE   i_type;                  //8位类型
    BYTE   i_code;                  //8位代码
    USHORT i_cksum;                 //16位校验和 
    USHORT i_id;                    //识别号（一般用进程号作为识别号）
    USHORT i_seq;                   //报文序列号    
    ULONG  timestamp;               //时间戳
}ICMP_HEADER;

DWORD WINAPI DoRecvPacket(LPVOID pParam);
int SendSyn(SOCKADDR_IN sa, SOCKADDR_IN dest, int nPort);

#endif