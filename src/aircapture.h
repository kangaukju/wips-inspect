/*
 * aircapture.h
 *
 *  Created on: 2016. 11. 23.
 *      Author: root
 */

#ifndef SRC_AIRCAPTURE_H_
#define SRC_AIRCAPTURE_H_

#include "airconf-parse.h"
#include "hashtable.h"
#include "osdep/osdep.h"
#include "osdep/common.h"
#include "common.h"
#include "wifi_setup.h"
#include "wi_packet.h"
#include "log.h"
#include "airresult.h"

#define NB_PWR  5       /* size of signal power ring buffer */
#define NB_PRB 10       /* size of probed ESSID ring buffer */

struct ch_dwell {
	uint8_t channel;
	uint32_t dwell;
};

struct dwell_list {
	struct ch_dwell hops[MAX_NUM_CHANNEL];
	int hops_size;
	int hop_index;
};

/**
 * @brief aircapture global config
 */
typedef struct AIRCAP_G {
	struct wif *wi;
	int debug;
	char confdir[256];
	char conffile[256];
	int do_exit;
	/* running time */
	uint32_t capture_timer;
	struct timeval capture_start;
	struct airconf *conf;
	struct dwell_list dwell_list;
	FILE *pcap_fp;
	char pcap_file[128];
} AIRCAP_G;

struct aircap_summary {
	uint8_t type;
	uint8_t subtype;
	uint8_t addr1[6];
	uint8_t addr2[6];
	uint8_t addr3[6];
	cv_arr_def(addr4, uint8_t, 6);
	/* 00:adhoc, 10:fromds, 10:tods, 11:wds */
	uint8_t ds;
	uint16_t seq;
	/* deauth reason code */
	cv_def(deauthcode, uint16_t);
};

struct aircap_data {
	struct airconf *conf;
	struct timeval capture_tv;
	uint8_t rawdata[4096];
	size_t rawdatalen;
	struct aircap_summary summary;
	struct rx_info ri;
};

typedef void (*aircap_handler_t)(AIRCAP_G *, void *);

int  aircap_g_init(AIRCAP_G* g);
void aircap_g_free(AIRCAP_G* g);
int  aircap_conf_verify(AIRCAP_G *g);
int  aircap_matching(AIRCAP_G *g, aircap_handler_t);
int  aircap_summary(uint8_t *h80211, int caplen, struct aircap_summary *);
void aircap_summary_print(struct aircap_summary *pkt_summary);

// write pcap file
FILE *new_pcap_file(const char* pcapfile);
int write_pcap_file(FILE *fp, uint8_t *h80211, size_t caplen, struct rx_info *ri);

// calc channel dwell time
struct ch_dwell* get_channel_dwell(struct dwell_list *list, uint8_t ch);
void set_channel_dwell(struct dwell_list *list, struct ch_dwell *ch_dwell);
void dwell_coordination(struct airconf *conf, struct dwell_list *list);

#endif /* SRC_AIRCAPTURE_H_ */
