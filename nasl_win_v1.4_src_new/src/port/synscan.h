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

typedef struct _IP_HEADER            //����IP�ײ�
{
    unsigned char    h_lenver;       //4λ�ײ�����+4λIP�汾��
    unsigned char    tos;            //8λ��������TOS
    unsigned short   total_len;      //16λ�ܳ��ȣ��ֽڣ�
    unsigned short   ident;          //16λ��ʶ
    unsigned short   frag_and_flags; //3λ��־λ+13λƬƫ��
    unsigned char    ttl;            //8λ����ʱ�� TTL
    unsigned char    proto;          //8λЭ�� (TCP, UDP ������)
    unsigned short   checksum;       //16λIP�ײ�У���
    unsigned int     sourceIP;       //32λԴIP��ַ
    unsigned int     destIP;         //32λĿ��IP��ַ
} IP_HEADER;

typedef struct _TCP_HEADER           //����TCP�ײ�
{
    USHORT th_sport;                 //16λԴ�˿�
    USHORT th_dport;                 //16λĿ�Ķ˿�
    UINT   th_seq;                   //32λ���к�
    UINT   th_ack;                   //32λȷ�Ϻ�
    UCHAR  th_lenres;                //4λ�ײ�����/6λ������
    UCHAR  th_flag;                  //6λ��־λ
    USHORT th_win;                   //16λ���ڴ�С
    USHORT th_sum;                   //16λУ���
    USHORT th_urp;                   //16λ��������ƫ����
} TCP_HEADER; 

typedef struct _PSD_HEADER			//����TCPα�ײ�
{
    unsigned long saddr;            //Դ��ַ
    unsigned long daddr;            //Ŀ�ĵ�ַ
    char mbz;
    char ptcl;                      //Э������
    unsigned short tcpl;            //TCP����
} PSD_HEADER;

typedef struct _UDP_HEADER          //����UDP�ײ�
{
    unsigned short uh_sport;        //16λԴ�˿�
    unsigned short uh_dport;        //16λĿ�Ķ˿�
    unsigned short uh_len;          //16λ����
    unsigned short uh_sum;          //16λУ���
} UDP_HEADER;

typedef struct _ICMP_HEADER         //����ICMP�ײ�
{
    BYTE   i_type;                  //8λ����
    BYTE   i_code;                  //8λ����
    USHORT i_cksum;                 //16λУ��� 
    USHORT i_id;                    //ʶ��ţ�һ���ý��̺���Ϊʶ��ţ�
    USHORT i_seq;                   //�������к�    
    ULONG  timestamp;               //ʱ���
}ICMP_HEADER;

DWORD WINAPI DoRecvPacket(LPVOID pParam);
int SendSyn(SOCKADDR_IN sa, SOCKADDR_IN dest, int nPort);

#endif