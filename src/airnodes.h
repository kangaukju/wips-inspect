/*
 * airnodes.h
 *
 *  Created on: 2017. 1. 2.
 *      Author: root
 */

#ifndef SRC_AIRNODES_H_
#define SRC_AIRNODES_H_

#include <stdint.h>
#include "osdep/osdep.h"
#include "osdep/common.h"
#include "osdep/byteorder.h"
#include "hashtable.h"
#include "wi_packet.h"
#include "eapol.h"

#define STD_OPN     0x0001
#define STD_WEP     0x0002
#define STD_WPA     0x0004
#define STD_WPA2    0x0008

#define STD_FIELD   (STD_OPN | STD_WEP | STD_WPA | STD_WPA2)

#define ENC_WEP     0x0010
#define ENC_TKIP    0x0020
#define ENC_WRAP    0x0040
#define ENC_CCMP    0x0080
#define ENC_WEP40   0x1000
#define ENC_WEP104  0x0100

#define ENC_FIELD   (ENC_WEP | ENC_TKIP | ENC_WRAP | ENC_CCMP | ENC_WEP40 | ENC_WEP104)

#define AUTH_OPN    0x0200
#define AUTH_PSK    0x0400
#define AUTH_MGT    0x0800

#define AUTH_FIELD  (AUTH_OPN | AUTH_PSK | AUTH_MGT)

#define STD_QOS         0x2000

#define QLT_TIME    5
#define QLT_COUNT   25


#define NB_PWR  5       /* size of signal power ring buffer */
#define NB_PRB 10       /* size of probed ESSID ring buffer */
#define MAX_IE_SIZE 256 /* Maximum length of an Information Element */
#define NULL_MAC  (unsigned char*)"\x00\x00\x00\x00\x00\x00"
#define BROADCAST (unsigned char*)"\xFF\xFF\xFF\xFF\xFF\xFF"

#if 0
static unsigned char ZERO8[8] =
"\x00\x00\x00\x00\x00\x00\x00\x00";

static unsigned char ZERO16[16] =
"\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00";
#endif

static unsigned char ZERO32[32] =
"\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00"
"\x00\x00\x00\x00\x00\x00\x00\x00";

enum WIFI_PROTO_TYPE {
	WIFI_PROTO_80211a    = 0x01,
	WIFI_PROTO_80211b    = 0x02,
	WIFI_PROTO_80211g    = 0x04,
	WIFI_PROTO_80211n    = 0x08,
	WIFI_PROTO_80211ac   = 0x10,
};

typedef struct WPS_INFO {
	unsigned char version;    /* WPS Version */
	unsigned char state;      /* Current WPS state */
	unsigned char ap_setup_locked;
	                          /* AP setup locked */
	unsigned int meth;        /* WPS Config Methods */
} WPS_INFO;

typedef struct AP_INFO {
	time_t tinit, tlast;
	int channel;
	float max_speed;
	uint8_t rate[8+8]; // base:8 + extend:8
	uint8_t ratelen;
	uint8_t proto; // WIFI_PROTO_TYPE
	uint8_t seen_ht; // 11n
	uint8_t seen_vht; // 11ac
	int avg_power;
	int best_power;               /* best signal power             */
	int power_index;              /* index in power ring buffer    */
	int power_lvl[NB_PWR];        /* signal power ring buffer      */
	int preamble;                 /* 0 = long, 1 = short           */
	int security;                 /* ENC_*, AUTH_*, STD_*          */
	uint8_t bssid[6];             /* the access point's MAC        */
	int ssid_length;              /* length of ssid                */
	char essid[MAX_IE_SIZE];      /* ascii network identifier      */
	unsigned long long timestamp; /* Timestamp to calculate uptime */
	struct timeval tv;            /* time for data per second      */
	unsigned char lanip[4];       /* last detected ip address      */
	                              /* if non-encrypted network      */
	unsigned char **uiv_root;     /* unique iv root structure      */
	                              /* if wep-encrypted network      */
	int rx_quality;               /* percent of captured beacons   */
	int fcapt;                    /* amount of captured frames     */
	int fmiss;                    /* amount of missed frames       */
	unsigned int last_seq;        /* last sequence number          */
	struct timeval ftimef;        /* time of first frame           */
	struct timeval ftimel;        /* time of last frame            */
	struct timeval ftimer;        /* time of restart               */
	unsigned long nb_bcn;         /* total number of beacons       */
	unsigned long nb_pkt;         /* total number of packets       */
	unsigned long nb_data;        /* number of  data packets       */
	unsigned long nb_data_old;    /* number of data packets/sec    */
	int nb_dataps;                /* number of data packets/sec    */

	int EAP_detected;

	WPS_INFO wps;

} AP_INFO;

typedef struct ST_INFO {
	uint8_t stmac[6];        /* the client's MAC address  */
	uint8_t bssid[6];        /* AP this client belongs to */
	time_t tinit, tlast;     /* first and last time seen  */
	unsigned long nb_pkt;    /* total number of packets   */
	int probe_index;         /* probed ESSIDs ring index  */
	char probes[NB_PRB][MAX_IE_SIZE];
	                         /* probed ESSIDs ring buffer */
	int ssid_length[NB_PRB]; /* ssid lengths ring buffer  */
	int power;               /* last signal power         */
	int rate_to;             /* last bitrate to station   */
	int rate_from;           /* last bitrate from station */
	struct timeval ftimer;   /* time of restart           */
	int missed;              /* number of missed packets  */
	unsigned int lastseq;    /* last seen sequence number */
	struct WPA_hdsk wpa;     /* WPA handshake data        */
	int qos_to_ds;           /* does it use 802.11e to ds */
	int qos_fr_ds;           /* does it receive 802.11e   */
	int channel;             /* Channel station is seen   */
	                         /*  Not used yet             */
	uint64_t databytes;
} ST_INFO;

typedef struct NODES_INFO {
	hash_ctx_t *ap_hctx;
	hash_ctx_t *st_hctx;
	int cur_channel;
} NODES_INFO;


void nodes_info_init(NODES_INFO *nodes);
void nodes_info_free(NODES_INFO *nodes);
int air_packet_parse(NODES_INFO *nodes, uint8_t channel,
		uint8_t *h80211, int caplen, struct rx_info *ri);

void check_expired_ap(hash_ctx_t *hctx, int lifetime);
void check_expired_st(hash_ctx_t *hctx, int lifetime);

#endif /* SRC_AIRNODES_H_ */
