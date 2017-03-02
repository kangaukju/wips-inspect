/*
 * log.h
 *
 *  Created on: 2016. 12. 14.
 *      Author: root
 */

#ifndef SRC_LOG_H_
#define SRC_LOG_H_

#include <sys/types.h>
#include <stdint.h>
#include <stdio.h>

typedef enum {
	L_CONSOLE = 1,
	L_FILE    = 2,
	L_SYSLOG  = 4,
} L_SYS;


#define L_CRI 1
#define L_ERR 2
#define L_INF 4
#define L_DBG 8
#define L_ALL (L_CRI|L_ERR|L_INF|L_DBG)

typedef struct LCTX
{
	int lv;
	L_SYS sys;
	int color;
	char dir[256];
	char prefix[32];
	char file[256];
	int disable;
} LCTX;

/* API Common log */
void llc(LCTX* ctx, const char *fmt, ...);
void lle(LCTX* ctx, const char *fmt, ...);
void lli(LCTX* ctx, const char *fmt, ...);
void lld(LCTX* ctx, const char *fmt, ...);
void llcx(LCTX* ctx, uint8_t *hex, size_t len, const char *fmt, ...);
void llex(LCTX* ctx, uint8_t *hex, size_t len, const char *fmt, ...);
void llix(LCTX* ctx, uint8_t *hex, size_t len, const char *fmt, ...);
void lldx(LCTX* ctx, uint8_t *hex, size_t len, const char *fmt, ...);

/* API Console log */
void console_log_disable();
void console_log_enable();
void clc(const char *fmt, ...);
void cld(const char *fmt, ...);
void cle(const char *fmt, ...);
void cli(const char *fmt, ...);
void clcx(uint8_t *hex, size_t len, const char *fmt, ...);
void cldx(uint8_t *hex, size_t len, const char *fmt, ...);
void clex(uint8_t *hex, size_t len, const char *fmt, ...);
void clix(uint8_t *hex, size_t len, const char *fmt, ...);

#endif /* SRC_LOG_H_ */
