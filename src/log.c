/*
 * log.c
 *
 *  Created on: 2016. 12. 14.
 *      Author: root
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>

#include "log.h"

/*
<List of colors for prompt and Bash>

txtblk='\e[0;30m' # Black - Regular
txtred='\e[0;31m' # Red
txtgrn='\e[0;32m' # Green13
txtylw='\e[0;33m' # Yellow
txtblu='\e[0;34m' # Blue
txtpur='\e[0;35m' # Purple
txtcyn='\e[0;36m' # Cyan
txtwht='\e[0;37m' # White
bldblk='\e[1;30m' # Black - Bold
bldred='\e[1;31m' # Red
bldgrn='\e[1;32m' # Green
bldylw='\e[1;33m' # Yellow
bldblu='\e[1;34m' # Blue
bldpur='\e[1;35m' # Purple
bldcyn='\e[1;36m' # Cyan
bldwht='\e[1;37m' # White
unkblk='\e[4;30m' # Black - Underline
undred='\e[4;31m' # Red
undgrn='\e[4;32m' # Green
undylw='\e[4;33m' # Yellow
undblu='\e[4;34m' # Blue
undpur='\e[4;35m' # Purple
undcyn='\e[4;36m' # Cyan
undwht='\e[4;37m' # White
bakblk='\e[40m'   # Black - Background
bakred='\e[41m'   # Red
bakgrn='\e[42m'   # Green
bakylw='\e[43m'   # Yellow
bakblu='\e[44m'   # Blue
bakpur='\e[45m'   # Purple
bakcyn='\e[46m'   # Cyan
bakwht='\e[47m'   # White
txtrst='\e[0m'    # Text Reset
 */
#ifndef LOG_NO_COLOR
	#define VTC_BLUE   "\x1b[34m" //!< Colour following text blue.
	#define VTC_RED    "\x1b[31m" //!< Colour following text red.
	#define VTC_GREEN  "\x1b[32m" //!< Colour following text creen.
	#define VTC_YELLOW "\x1b[33m" //!< Colour following text yellow.
	#define VTC_BOLD   "\x1b[1m" //!< Embolden following text.
	#define VTC_RESET  "\x1b[0m" //!< Reset terminal text to default style/colour.
#endif

void llc(LCTX* ctx, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	va_end(ap);
}

void lle(LCTX* ctx, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	va_end(ap);
}

void lli(LCTX* ctx, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	va_end(ap);
}

void lld(LCTX* ctx, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	va_end(ap);
}

void llcx(LCTX* ctx, uint8_t *hex, size_t len, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	va_end(ap);
}

void llex(LCTX* ctx, uint8_t *hex, size_t len, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	va_end(ap);
}

void llix(LCTX* ctx, uint8_t *hex, size_t len, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	va_end(ap);
}

void lldx(LCTX* ctx, uint8_t *hex, size_t len, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	va_end(ap);
}

/**
 * console log
 */
static LCTX console_ctx = {
		.lv = L_ALL,
		.sys = L_CONSOLE,
		.color = 1,
};

void console_log_disable() {
	console_ctx.disable = 1;
}

void console_log_enable() {
	console_ctx.disable = 0;
}

static void console_log(LCTX* ctx, int lvl, const char *fmt, va_list ap, const char *color)
{
	struct tm t_now;
	char strtime[256] = {0, };
	char buffer[1024] = {0, };
	time_t now = time(NULL);
	FILE *fp = stdout;

	if (console_ctx.disable) return;
	if (fp == NULL) return;

	localtime_r(&now, &t_now);

	if (ctx->lv >= lvl) {
		strftime(strtime, sizeof(strtime), "%H:%M:%S", &t_now);
		vsnprintf(buffer, sizeof(buffer), fmt, ap);

		switch (lvl) {
		case L_CRI:
			fprintf(fp, "%s%s :CRIT: %s%s\n", color, strtime, buffer, VTC_RESET);
			break;
		case L_ERR:
			fprintf(fp, "%s%s :ERR : %s%s\n", color, strtime, buffer, VTC_RESET);
			break;
		case L_INF:
			fprintf(fp, "%s%s :INFO: %s%s\n", color, strtime, buffer, VTC_RESET);
			break;
		case L_DBG:
			fprintf(fp, "%s%s :DBG : %s%s\n", color, strtime, buffer, VTC_RESET);
			break;
		}
	}
}

static void console_log_x(LCTX* ctx, int lvl, uint8_t *hex, size_t len, const char *fmt, va_list ap, const char *color)
{
	struct tm t_now;
	char strtime[256] = {0, };
	char buffer[1024] = {0, };
	time_t now = time(NULL);
	FILE *fp = stdout;
	int n=0, i=0;

	if (console_ctx.disable) return;
	if (fp == NULL) return;

	localtime_r(&now, &t_now);

	if (ctx->lv >= lvl)
	{
		strftime(strtime, sizeof(strtime), "%H:%M:%S", &t_now);
		n = vsnprintf(buffer, sizeof(buffer), fmt, ap);
		for (i=0; i<(int)len; i++) {
			n += snprintf(buffer + n, sizeof(buffer) - n, " %02x", hex[i]);
		}

		switch (lvl) {
		case L_CRI:
			fprintf(fp, "%s%s :CRIT: (len:%ld) %s%s\n", color, strtime, len, buffer, VTC_RESET);
			break;
		case L_ERR:
			fprintf(fp, "%s%s :ERR : (len:%ld) %s%s\n", color, strtime, len, buffer, VTC_RESET);
			break;
		case L_INF:
			fprintf(fp, "%s%s :INFO: (len:%ld) %s%s\n", color, strtime, len, buffer, VTC_RESET);
			break;
		case L_DBG:
			fprintf(fp, "%s%s :DBG : (len:%ld) %s%s\n", color, strtime, len, buffer, VTC_RESET);
			break;
		}
	}
}

void clc(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	console_log(&console_ctx, L_CRI, fmt, ap, VTC_YELLOW);
	va_end(ap);
}

void cld(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	console_log(&console_ctx, L_DBG, fmt, ap, VTC_RESET);
	va_end(ap);
}

void cle(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	console_log(&console_ctx, L_ERR, fmt, ap, VTC_RED);
	va_end(ap);
}

void cli(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	console_log(&console_ctx, L_INF, fmt, ap, VTC_GREEN);
	va_end(ap);
}

void clcx(uint8_t *hex, size_t len, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	console_log_x(&console_ctx, L_CRI, hex, len, fmt, ap, VTC_YELLOW);
	va_end(ap);
}

void cldx(uint8_t *hex, size_t len, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	console_log_x(&console_ctx, L_DBG, hex, len, fmt, ap, VTC_RESET);
	va_end(ap);
}

void clex(uint8_t *hex, size_t len, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	console_log_x(&console_ctx, L_ERR, hex, len, fmt, ap, VTC_RED);
	va_end(ap);
}

void clix(uint8_t *hex, size_t len, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	console_log_x(&console_ctx, L_INF, hex, len, fmt, ap, VTC_GREEN);
	va_end(ap);
}
