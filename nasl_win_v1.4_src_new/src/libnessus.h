/* Nessuslib -- the Nessus Library
 * Copyright (C) 1998 Renaud Deraison
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */   

#ifndef _NESSUS_NESSUSLIB_H
#define _NESSUS_NESSUSLIB_H

#ifdef HAVE_SSL
#include <openssl/ssl.h>
#endif

#ifndef ExtFunc
#define ExtFunc
#endif

void nessus_lib_version(int *, int *, int *);
 
/*
 * Arglist definition and defines
 */
 
struct arglist {
	char * name;
	int type;
	void * value;
	long length;
	struct arglist * next;
	int hash;
	};

	
#define ARG_STRING 	1
#define ARG_PTR 	2
#define ARG_INT 	3
#define ARG_ARGLIST 	4
#define ARG_STRUCT	5


/*
 * Plugin standard function templates
 */

typedef int(*plugin_init_t)(struct arglist *);
typedef int(*plugin_run_t)(struct arglist *);      

#ifndef _SIZE_T_DEFINED
typedef unsigned int size_t;
#endif

/*
 * Network-related functions
 */

/* Plugin specific network functions */
int open_sock_tcp(struct arglist * , unsigned int, int );
int open_sock_udp(struct arglist * , unsigned int );
int open_sock_option(struct arglist * , unsigned int , int , int, int);
int recv_line(int, char *, size_t);
int nrecv(int, void*, int, int);
int socket_close(int);

/* Additional functions -- should not be used by the plugins */
int open_sock_tcp_hn(const char * , unsigned int );
int open_sock_opt_hn(const char * , unsigned int , int , int, int );
struct in_addr nn_resolve (const char *); 

#ifdef __GNUC__
void auth_printf(struct arglist *, char * , ...) __attribute__ (( format (printf, 2, 3)));
#else
void auth_printf(struct arglist *, char * , ...);
#endif
void scanner_add_port(struct arglist*, int, char *);
void auth_send(struct arglist *, char *);
char * auth_gets(struct arglist *, char * , size_t);
int ping_host(struct in_addr);

unsigned short *getpts(char *, int *);

/* 
 * Management of the arglists --should not be used directly by
 * the plugins
 */

void arg_add_value(struct arglist *, const char *, int, long, void *);	   
int arg_set_value(struct arglist *, const char *, long, void *);	
int arg_set_type(struct arglist *, const char *, int);
void * arg_get_value(struct arglist *, const char *);
int arg_get_type(struct arglist *, const char *);
int arg_get_length(struct arglist *, const char *);
void arg_dump(struct arglist *, int);
void arg_dup(struct arglist *, struct arglist *);
void arg_free(struct arglist *);
void arg_free_all(struct arglist *);
void arg_free_name(char*);



/*
 * Arglist management at plugin-level
 */
 
void plug_set_name(struct arglist *, const char *, const char *);
char*plug_get_name(struct arglist*);

void plug_set_path(struct arglist *, const char *);
char*plug_get_path(struct arglist *);

void plug_set_fname(struct arglist *, const char *);
char*plug_get_fname(struct arglist *);


void plug_set_version(struct arglist *, const char *);
char*plug_get_version(struct arglist *);

void plug_set_timeout(struct arglist *, int);
int  plug_get_timeout(struct arglist *);

void plug_set_launch(struct arglist *, int);
int plug_get_launch(struct arglist *);

void plug_set_summary(struct arglist *, const char *, const char*);
char*plug_get_summary(struct arglist *);

void plug_set_description(struct arglist *, const char *,const char *);
char*plug_get_description(struct arglist *);

void plug_set_category(struct arglist *, int);
int  plug_get_category(struct arglist *);

void plug_set_copyright(struct arglist *, const char *, const char*);
char*plug_get_copyright(struct arglist *);

void plug_set_family(struct arglist * , const char *, const char *);
char*plug_get_family(struct arglist *);

void plug_set_dep(struct arglist *, const char *);
struct arglist * plug_get_deps(struct arglist*);

void plug_set_id(struct arglist *, int);
int  plug_get_id(struct arglist *);

void plug_set_cve_id(struct arglist *, char *);
char*plug_get_cve_id(struct arglist *);

void plug_set_bugtraq_id(struct arglist *, char *);
char*plug_get_bugtraq_id(struct arglist *);

void plug_set_xref(struct arglist *, char *, char *);
char * plug_get_xref(struct arglist *);

void plug_set_see_also(struct arglist *, char *);
struct arglist * plug_get_see_also(struct arglist *);

#ifdef HAVE_SSL
void plug_set_ssl_cert(struct arglist*, char*);
void plug_set_ssl_key(struct arglist*, char*);
void plug_set_ssl_pem_password(struct arglist*, char*);
int nessus_SSL_init(char*);
void sslerror(char*);
void nessus_install_passwd_cb(SSL_CTX*, char*);

#endif



void plug_add_dep(struct arglist *, char *, char *);


void plug_add_port(struct arglist *, int);


const char * plug_get_hostname(struct arglist *);
const char * plug_get_host_fqdn(struct arglist *);
unsigned int plug_get_host_open_port(struct arglist * desc);
void plug_add_host(struct arglist *, struct arglist *);

char* plug_get_cve_id(struct arglist*);
char* plug_get_bugtraq_id(struct arglist*);

void plug_require_key(struct arglist *, const char *);
struct arglist * plug_get_required_keys(struct arglist *);

void plug_exclude_key(struct arglist *, const char *);
struct arglist * plug_get_excluded_keys(struct arglist *);

void plug_require_port(struct arglist *, const char *);
struct arglist * plug_get_required_ports(struct arglist *);

void plug_require_udp_port(struct arglist*, const char *);
struct arglist * plug_get_required_udp_ports(struct arglist *);

int plug_get_port_transport(struct arglist*, int);
int comm_send_status(struct arglist*, char*, char*, int, int);
int islocalhost(struct in_addr *);



/*
 * Reporting functions
 */
 
/* Plugin-specific : */
void proto_post_hole(struct arglist *, int, const char *, const char *);
void post_hole(struct arglist *, int, const char *);
void post_hole_udp(struct arglist *, int, const char *);
#define post_hole_tcp post_hole

void proto_post_info(struct arglist *, int, const char *, const char *);
void post_info(struct arglist *, int, const char *);
void post_info_udp(struct arglist *, int, const char *);
#define post_info_tcp post_info

void proto_post_note(struct arglist *, int, const char *, const char *);
void post_note(struct arglist *, int, const char *);
void post_note_udp(struct arglist *, int, const char *);
#define post_note_tcp post_note

#ifndef _WIN32
/* returns a full duplex data file stream */
FILE * ptyexecvp (const char *file, const char **argv, pid_t *child);
#endif /* _WIN32 */
char ** append_argv (char **argv, char *opt);
void    destroy_argv (char **argv);
void (*pty_logger(void(*)(const char *, ...)))(const char *, ...);

//ExtFunc FILE*	nessus_popen(const char*, char *const[], pid_t*);
//ExtFunc FILE*	nessus_popen4(const char*, char *const[], pid_t*, int);
//int	nessus_pclose(FILE*, pid_t);

/* 
 * Management of the portlists
 */


void host_add_port(struct arglist *, int, int);
void host_add_port_udp(struct arglist *, int, int);
int host_get_port_state(struct arglist *, int);
int host_get_port_state_udp(struct arglist *, int);
/* Not implemented
char * host_get_port_banner(struct arglist *, int);
*/






/*
 * Miscellaneous functions
 */
 
struct in_addr * plug_get_host_ip(struct arglist *);
char * plug_get_host_name(struct arglist *);
char * get_preference(struct arglist *, const char *);
#define PREF_CHECKBOX "checkbox"
#define PREF_ENTRY "entry"
#define PREF_RADIO "radio"
#define PREF_PASSWORD "password"
#define PREF_FILE "file"
void add_plugin_preference(struct arglist *, const char *, const char *, const char *);
char *get_plugin_preference(struct arglist *, const char *);
const char *get_plugin_preference_fname(struct arglist*, const char*);

/*
 * Replacement for system related functions
 */
 


void * emalloc(size_t);
char * estrdup(const char *);
void * erealloc(void*, size_t);
void efree(void *);
size_t estrlen(const char *, size_t);


#ifdef HUNT_MEM_LEAKS
void * __hml_malloc(char*, int, size_t);
char * __hml_strdup(char*, int, char*);
void   __hml_free(char*, int, void*);
void * __hml_realloc(char*, int, void*, size_t);



#define emalloc(x) __hml_malloc(__FILE__, __LINE__, x)
#define estrdup(x) __hml_strdup(__FILE__, __LINE__, x)
#define efree(x)   __hml_free(__FILE__, __LINE__, x)

#undef strdup

#define malloc(x) __hml_malloc(__FILE__, __LINE__, x)
#define strdup(x) __hml_strdup(__FILE__, __LINE__, x)
#define free(x)   __hml_free(__FILE__, __LINE__, &x)
#define realloc(x, y) __hml_realloc(__FILE__, __LINE__, x, y)

#endif

/* 
 * Inter Plugins Communication functions
 */
struct kb_item ** plug_get_kb(struct arglist *);
void plug_set_key(struct arglist *, char *, int, void *);
void plug_replace_key(struct arglist *, char *, int, void *);
void * plug_get_key(struct arglist *, char *, int *);
void * plug_get_fresh_key(struct arglist *, char *, int *);
/*
 * FTP Functions
 */
int ftp_log_in(int , char * , char * );
int ftp_get_pasv_address(int , struct sockaddr_in * );


char* addslashes(char*);
char* rmslashes(char*);

char* nessuslib_version();



/*
 * Pcap utils
 */
//#include "winpcap/pcap.h"
#include <winsock2.h>
 
int get_datalink_size(int);
char *routethrough(struct in_addr *, struct in_addr *);

int is_local_ip(struct in_addr);

int get_mac_addr(struct in_addr, char**);

/* 
 * Misc. defines
 */
/* Actions types of the plugins */
#define ACT_LAST		ACT_END
#define ACT_FIRST		ACT_INIT

#define ACT_END			10
#define ACT_FLOOD		9
#define ACT_KILL_HOST		8
#define ACT_DENIAL 		7
#define ACT_DESTRUCTIVE_ATTACK 	6
#define ACT_MIXED_ATTACK 	5
#define ACT_ATTACK 		4
#define ACT_GATHER_INFO 	3
#define ACT_SETTINGS		2
#define ACT_SCANNER 		1
#define ACT_INIT		0



#define	LAUNCH_DISABLED 0
#define LAUNCH_RUN	1
#define LAUNCH_SILENT	2


/*
 * Type of "transport layer", for encapsulated connections
 * Only SSL is supported at this time.
 * (Bad) examples of other layers could be SOCKS, httptunnel, icmptunnel,
 * RMI over HTTP, DCOM over HTTP, TCP over TCP, etc.
 */
#define NESSUS_ENCAPS_IP	1
#define NESSUS_ENCAPS_SSLv23	2 /* Ask for compatibility options */
#define NESSUS_ENCAPS_SSLv2	3
#define NESSUS_ENCAPS_SSLv3	4
#define NESSUS_ENCAPS_TLSv1	5

#define IS_ENCAPS_SSL(x) ((x) >= NESSUS_ENCAPS_SSLv23 && (x) <= NESSUS_ENCAPS_TLSv1)

/*
 * Transport layer options 
 */
#define NESSUS_CNX_IDS_EVASION_SPLIT	1L  /* Try to evade NIDS by spliting sends */
#define NESSUS_CNX_IDS_EVASION_INJECT	2L /* Split + insert garbage */
#define NESSUS_CNX_IDS_EVASION_SHORT_TTL 4L /* Split + too short ttl for garbage */
#define NESSUS_CNX_IDS_EVASION_FAKE_RST  8L /* Send a fake RST from our end after each established connection */

#define NESSUS_CNX_IDS_EVASION_SEND_MASK (NESSUS_CNX_IDS_EVASION_SPLIT|NESSUS_CNX_IDS_EVASION_INJECT|NESSUS_CNX_IDS_EVASION_SHORT_TTL)


int    open_stream_connection(struct arglist *, unsigned int, int, int);
int    open_stream_connection_unknown_encaps(struct arglist *, unsigned int, int, int *);
int    open_stream_auto_encaps(struct arglist *, unsigned int, int);
int    write_stream_connection (int, void * buf, int n);
int    read_stream_connection (int, void *, int);
int    read_stream_connection_min(int, void*, int, int);
int    close_stream_connection(int);
int    nsend(int, void*, int, int);
const char* get_encaps_name(int);
const char* get_encaps_through(int);

int    stream_set_timeout(int, int);
int    stream_set_options(int, int, int);

int	stream_set_buffer(int, int);
int	stream_get_buffer_sz (int);

#ifdef HAVE_SSL
       X509*   stream_get_server_certificate(int);
       char*   stream_get_ascii_server_certificate(int);
#endif


int 	is_shell_command_present(char*);
char* 	find_in_path(char*, int);

char*	build_encode_URL(struct arglist*, char*, char*, char*, char*);


#ifdef HAVE_SSL
int nessus_register_connection(int, SSL*);
#else
int nessus_register_connection(int, void*);
#endif
int nessus_deregister_connection(int);
int nessus_get_socket_from_connection(int);


void nessus_init_random();
int stream_zero(fd_set*);
int stream_set(int, fd_set*);
int stream_isset(int, fd_set*);


int bpf_server();
int bpf_open_live(char*, char*);
u_char* bpf_next(int, int *);
void bpf_close(int);
int bpf_datalink(int);

void initsetproctitle(int argc, char *argv[], char *envp[]);
#ifndef HAVE_SETPROCTITLE
void setproctitle( const char *fmt, ... );
#endif


struct in_addr socket_get_next_source_addr();
int set_socket_source_addr(int, int);
void socket_source_init(struct in_addr *);

struct arglist * store_plugin(struct arglist *,  char *);
struct arglist * store_load_plugin(char *, char *,  struct arglist*);
int		 store_init_sys(char *);
int		 store_init_user(char *);


int		nessus_init_svc();
/*-----------------------------------------------------------------*/

#define KB_TYPE_INT ARG_INT
#define KB_TYPE_STR ARG_STRING

struct kb_item {
	char * name;
 	char type;
	union {
		char * v_str;
		int v_int;
	} v;
	struct kb_item * next;
};


struct kb_item ** kb_new();


struct kb_item * kb_item_get_single(struct kb_item **, char *, int );
char * kb_item_get_str(struct kb_item **, char *);
int    kb_item_get_int(struct kb_item **, char *);

struct kb_item * kb_item_get_all(struct kb_item **, char *);
struct kb_item * kb_item_get_pattern(struct kb_item **, char *);
void   kb_item_get_all_free(struct kb_item *);


int    kb_item_add_str(struct kb_item **, char *, char *);
int    kb_item_set_str(struct kb_item **, char *, char *);
int    kb_item_add_int(struct kb_item **, char *, int   );
int    kb_item_set_int(struct kb_item **, char *, int   );

struct arglist * plug_get_oldstyle_kb(struct arglist * );


#define NEW_KB_MGMT



/*-----------------------------------------------------------------*/
/*
struct http_msg {
	int type;		// Who should read this message  
	pid_t owner;		// Process who sent that message 
	unsigned short port;
	int total_len;
	int transport;
	int data_len;
	char data[1];
	};
*/	
int http_share_exists(struct arglist *);	
struct http_msg * http_share_mkmsg(int, int, int, char*);
void http_share_freemsg(struct http_msg*);
struct http_msg * http_share_send_recv_msg(struct arglist *, struct http_msg *);
	
	
//pid_t http_share_init(struct arglist *);
//int http_share_close(struct arglist *, pid_t);

int http_recv_headers(int , char ** , int * );



int os_send(int, void*, int, int);
int os_recv(int, void*, int, int);


#define INTERNAL_COMM_MSG_TYPE_CTRL	(1 << 16)
#define INTERNAL_COMM_MSG_TYPE_KB	(1 << 17)
#define INTERNAL_COMM_MSG_TYPE_DATA	(1 << 18)
#define INTERNAL_COMM_MSG_SHARED_SOCKET (1 << 19)


#define INTERNAL_COMM_KB_REPLACE	1
#define INTERNAL_COMM_KB_GET		2
#define INTERNAL_COMM_KB_SENDING_INT	4
#define INTERNAL_COMM_KB_SENDING_STR	8
#define INTERNAL_COMM_KB_ERROR	 	16	


#define INTERNAL_COMM_CTRL_FINISHED	1
#define INTERNAL_COMM_CTRL_ACK          2


#define INTERNAL_COMM_SHARED_SOCKET_REGISTER	1
#define INTERNAL_COMM_SHARED_SOCKET_ACQUIRE	2
#define INTERNAL_COMM_SHARED_SOCKET_RELEASE	4
#define INTERNAL_COMM_SHARED_SOCKET_DESTROY	8

#define INTERNAL_COMM_SHARED_SOCKET_DORECVMSG	16
#define INTERNAL_COMM_SHARED_SOCKET_BUSY 	32
#define INTERNAL_COMM_SHARED_SOCKET_ERROR	64


int internal_send(int, char *, int);
int internal_recv(int, char **, int *, int *);
int internal_finished(int);
	
int fd_is_stream(int);

int send_fd(int, int);
int recv_fd(int);

int shared_socket_register ( struct arglist *, int, char *);
int shared_socket_acquire  ( struct arglist *, char * );
int shared_socket_release  ( struct arglist *, char * );
int shared_socket_destroy  ( struct arglist *, char * );

#endif


