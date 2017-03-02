/*
 * airshooter.h
 *
 *  Created on: 2016. 11. 30.
 *      Author: root
 */

#ifndef SRC_AIRSHOOTER_H_
#define SRC_AIRSHOOTER_H_

#include "airconf-parse.h"
#include "log.h"
#include "common.h"
#include "osdep/osdep.h"
#include "airresult.h"

struct airshoot_result {
	struct airconf *conf;
	struct timeval shoot_tv;
};

typedef struct AIRSHOOT_G {
	struct wif *wi;
	int debug;
	char confdir[256];
	char conffile[256];
	int do_exit;
	// running time
	uint32_t shooter_timer;
	struct timeval shooter_start;
	struct airconf *conf;
} AIRSHOOT_G;

typedef void (*airshoot_handler_t)(AIRSHOOT_G*, void *);

int  airshoot_g_init(AIRSHOOT_G* g);
void airshoot_g_free(AIRSHOOT_G* g);
int  airshoot_conf_verify(AIRSHOOT_G *g);
void airshoot_summary_print(uint8_t *h80211, size_t h80211_len);
int  airshoot_shoot(AIRSHOOT_G *g, airshoot_handler_t);
int  airshoot_build_packet(AIRSHOOT_G *g, struct airconf *conf);

#endif /* SRC_AIRSHOOTER_H_ */
