#ifndef _TCPSCAN_H
#define _TCPSCAN_H

int GetPortArray(char *szLine, BOOL *PortPool);
int FastTcpScan(char *szHostName, BOOL *PortPool, struct arglist *key);

#endif