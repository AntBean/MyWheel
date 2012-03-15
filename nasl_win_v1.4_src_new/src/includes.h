#ifndef ___INCLUDES_H__
#define ___INCLUDES_H__

#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <ctype.h>
#include <fcntl.h>
#include <memory.h>
#include <string.h>                                                    
#include <time.h>                                               
#include <sys/stat.h>

#ifndef HAVE_SSL
#define HAVE_SSL
#endif
#ifndef HAVE_OPENSSL_MD2_H
#define HAVE_OPENSSL_MD2_H
#endif
#ifndef HAVE_OPENSSL_MD4_H
#define HAVE_OPENSSL_MD4_H
#endif
#ifndef HAVE_OPENSSL_MD5_H
#define HAVE_OPENSSL_MD5_H
#endif

#ifdef HAVE_SSL
#include <openssl/ssl.h>
#endif

#ifndef bcmp
#define bcmp(s1, s2, n)	memcmp ((s1), (s2), (n))
#endif
#ifndef bcopy
#define bcopy(s, d, n)	memcpy ((d), (s), (n))
#endif
#ifndef bzero
#define bzero(s, n)	memset ((s), 0, (n))
#endif

#define snprintf _snprintf
//#define close(s) closesocket(s)

#define pid_t   int
#define MAXPATHLEN MAX_PATH

#define FIX(n) htons(n)
#define UNFIX(n) ntohs(n)

typedef unsigned int u_int32_t;
typedef unsigned short n_short;
typedef unsigned short u_short;
typedef unsigned short u_int16_t;
typedef unsigned long n_time;

#endif /* not defined(___INCLUDES_H) */


