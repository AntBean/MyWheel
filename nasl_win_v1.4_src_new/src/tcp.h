#ifndef TCP_H_
#define TCP_H_

int tcp_init();
int tcp_exit();

int islocalhost(struct in_addr *addr);
int is_local_ip(struct in_addr addr);
char *iptos(u_long in);

#endif