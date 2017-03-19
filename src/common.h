/* 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * is provided AS IS, WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, and
 * NON-INFRINGEMENT.  See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston,
 * MA 02111-1307, USA.
 *
 * In addition, as a special exception, the copyright holders give
 * permission to link the code of portions of this program with the
 * OpenSSL library under certain conditions as described in each
 * individual source file, and distribute linked combinations
 * including the two.
 * You must obey the GNU General Public License in all respects
 * for all of the code used other than OpenSSL.  If you modify
 * file(s) with this exception, you may extend this exception to your
 * version of the file(s), but you are not obligated to do so.  If you
 * do not wish to do so, delete this exception statement from your
 * version.  If you delete this exception statement from all source
 * files in the program, then also delete it here.
 */
#ifndef _COMMON_H_
#define _COMMON_H_

#include <sys/time.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#if defined (__CYGWIN32__) && !defined(__CYGWIN64__)
int fseeko64(FILE* fp, int64_t offset, int whence);
int64_t ftello64(FILE * fp);
#undef fseek
#define fseek fseeko64
#undef ftello
#define ftello ftello64
#endif

#if defined(__FreeBSD__) || defined(__OpenBSD__)
#undef rand
#define rand lrand48
#undef srand
#define srand srand48
#endif

#include <time.h>
#include <stdio.h>

#define SNP(v, ...) ({ snprintf((v), sizeof(v), __VA_ARGS__); })

#define SWAP(x,y) { unsigned char tmp = x; x = y; y = tmp; }

#define SWAP32(x)       \
    x = ( ( ( x >> 24 ) & 0x000000FF ) | \
          ( ( x >>  8 ) & 0x0000FF00 ) | \
          ( ( x <<  8 ) & 0x00FF0000 ) | \
          ( ( x << 24 ) & 0xFF000000 ) );

#define PCT { struct tm *lt; time_t tc = time( NULL ); \
              lt = localtime( &tc ); printf( "%02d:%02d:%02d  ", \
              lt->tm_hour, lt->tm_min, lt->tm_sec ); }

#ifndef MAX
	#define MAX(x,y) ( (x)>(y) ? (x) : (y) )
#endif

#ifndef MIN
	#define MIN(x,y) ( (x)>(y) ? (y) : (x) )
#endif

#ifndef ABS
	#define ABS(a)          ((a)>=0?(a):(-(a)))
#endif

// For later use in aircrack-ng
#define CPUID_MMX_AVAILABLE 1
#define CPUID_SSE2_AVAILABLE 2
#define CPUID_NOTHING_AVAILABLE 0

#if defined(__i386__) || defined(__x86_64__)
	#define CPUID() shasse2_cpuid()
#else
	#define CPUID() CPUID_NOTHING_AVAILABLE
#endif

void calctime(time_t t, float calc);

#include <stdint.h>
#include <string.h>
#define _MAC_FMT_ "%02x:%02x:%02x:%02x:%02x:%02x"
#define _MAC_FMT_FILL_(x) \
	((uint8_t*)(x))[0], ((uint8_t*)(x))[1], ((uint8_t*)(x))[2], ((uint8_t*)(x))[3], ((uint8_t*)(x))[4], ((uint8_t*)(x))[5]
#define _IP_FMT_ "%d.%d.%d.%d"
#if __BYTE_ORDER == __BIG_ENDIAN
    #define _IP_FMT_FILL_(x) \
        ((uint8_t*)(x))[0], ((uint8_t*)(x))[1], ((uint8_t*)(x))[2], ((uint8_t*)(x))[3]

    #define IS_MCAST_IP(x) \
        ((((uint8_t*)(x))[0] >> 4) == 0x0E)
#else
    #define _IP_FMT_FILL_(x) \
        ((uint8_t*)(x))[3], ((uint8_t*)(x))[2], ((uint8_t*)(x))[1], ((uint8_t*)(x))[0]

    #define IS_MCAST_IP(x) \
        ((((uint8_t*)(x))[3] >> 4) == 0x0E)
#endif
#define MACUINT64(x) \
       (((uint64_t)x[0] << (5 * 8)) | \
        ((uint64_t)x[1] << (4 * 8)) | \
        ((uint64_t)x[2] << (3 * 8)) | \
        ((uint64_t)x[3] << (2 * 8)) | \
        ((uint64_t)x[4] << (1 * 8)) | \
        ((uint64_t)x[5] << (0 * 8)))

#ifndef boolean
typedef enum {FALSE, TRUE} boolean;
#endif

#ifndef NULL_MAC
#define NULL_MAC  (unsigned char*)"\x00\x00\x00\x00\x00\x00"
#endif

#ifndef BCAST
#define BCAST (unsigned char*)"\xFF\xFF\xFF\xFF\xFF\xFF"
#endif

#ifndef FILL_MAC_ARRAY
#define FILL_MAC_ARRAY(x) {(x),(x),(x),(x),(x),(x)}
#endif

#ifndef mac_t
#define mac_t(m) unsigned char m[6]
#endif

#ifndef oui_t
#define oui_t(m) unsigned char m[3];
#endif

typedef struct mac_seen_t {
	boolean seen;
	mac_t(mac);
} mac_seen_t;

#include <strings.h>
#if 1
	#define mac_init              {0, }
#else
	#define mac_init(pMac)        (memset((void*)(pMac), 0, 6))
#endif
#define mac_cmp(pMac1, pMac2) (memcmp((void*)(pMac1), (void*)(pMac2), 6) == 0)
#define mac_cmp_bcast(pMac)   (memcmp((void*)(pMac),  (void*)(BCAST), 6) == 0)
#define mac_cmp_null(pMac)    (memcmp((void*)(pMac),  (void*)(NULL_MAC), 6) == 0)
#define mac_cpy(pMac1, pMac2) (memcpy((void*)(pMac1), (void*)(pMac2), 6))
#define mac_cpy_bcast(pMac)   (memcpy((void*)(pMac),  (void*)(BCAST), 6))

#define oui_cmp(p1, p2)       (memcmp((void*)(p1),    (void*)(p2), 3) == 0)
#define oui_cpy(p1, p2)       (memcpy((void*)(p1),    (void*)(p2), 3))

#if 1
	#define mac_seen_init \
		{ .seen = FALSE, .mac = {0, } }
#else
	#define mac_seen_init(pCtx) \
		do { \
			((mac_seen_t*)(pCtx))->seen = FALSE; \
			mac_init(((mac_seen_t*)(pCtx))->mac); \
		} while (0)
#endif

#define mac_seen_mac_cpy(pCtx, m) \
	do { \
		((mac_seen_t*)(pCtx))->seen = TRUE; \
		mac_cpy(((mac_seen_t*)(pCtx))->mac, m); \
	} while (0)

#define mac_seen_cpy(pCtx1, pCtx2) \
	do { \
		((mac_seen_t*)(pCtx1))->seen = ((mac_seen_t*)(pCtx2))->seen; \
		mac_cpy(((mac_seen_t*)(pCtx1))->mac, ((mac_seen_t*)(pCtx2))->mac); \
	} while (0)

#define mac_seen_cmp(pCtx1, pCtx2) \
		( (((mac_seen_t*)(pCtx1))->seen == ((mac_seen_t*)(pCtx2))->seen) && \
		 (mac_cmp(((mac_seen_t*)(pCtx1))->mac, ((mac_seen_t*)(pCtx2))->mac)) )


#include <sys/time.h>
typedef struct _elapsed_tv {
	struct timeval start;
	struct timeval end;
} elapsed_tv;


struct timeval msec2usec(time_t msec);
long usec2msec(struct timeval *tv);
int is_unset_time(const struct timeval *tv);
struct timeval diff_timeval(const struct timeval* start, const struct timeval* end);
struct timeval elapsed_timeval(const elapsed_tv *tv);
void start_timeval(elapsed_tv *tv);
void end_timeval(elapsed_tv *tv);
void timebase_rand(uint8_t randbuf[], int size);

int valid_macstr(const char* macstr);
int str2mac(const char *str, uint8_t mac[]);
void print_hexstr(uint8_t *data, int len);
void print_hexdump(uint8_t *data, int len);
int hex2binarr(uint8_t tbuf[], int tlen, const char *hexstr);
uint8_t hex2bin(const char *hexstr);
char vchar(char ch);
int vhex(char* buf, size_t buflen, uint8_t *data, size_t datalen);

char *linux_path(const char*cmd);

char* utoa(unsigned int num, int base);
char* ltoa(unsigned long num, int base);
char* itoa(int num, int base);
char* u64toa(uint64_t num, int base);
char* mactoa(const uint8_t *mac);
char* mactostr(uint8_t* bytes, char str[18]);

void print_args(int argc, char** argv);
char* read_file_content(const char* filename);

int mkdir_r(const char* dir);
int write_cur_pid(const char* pname);
int is_null_str(const char* str);

#endif
