/*
 * airnodes.c
 *
 *  Created on: 2017. 1. 2.
 *      Author: root
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "log.h"
#include "airnodes.h"
#include "h80211_types.h"
#include "common.h"

#define DEF_HASH_BCK_SIZE 8

//static uint8_t IEEE80211_a_g_datarate[] = { 6,  9, 12, 18, 24, 36, 48,  54};
// data rate X 2
static uint8_t IEEE80211_a_g_datarate[] = {12, 18, 24, 36, 48, 72, 96, 108};
//static uint8_t IEEE80211_b_datarate[] = {1, 2, 5.5, 11};
// data rate X 2
static uint8_t IEEE80211_b_datarate[] = {2, 4,  11, 22};

static int is_ag(uint8_t *support_rate, int support_rate_len)
{
	for (int i = 0; i < support_rate_len; i++) {
		for (size_t k = 0; k < sizeof(IEEE80211_a_g_datarate); k++) {
			if (support_rate[i] == IEEE80211_a_g_datarate[k]) {
				return 0;
			}
		}
	}
	return -1;
}

static int is_b(uint8_t *support_rate, int support_rate_len)
{
	for (int i = 0; i < support_rate_len; i++) {
		for (size_t k = 0; k < sizeof(IEEE80211_b_datarate); k++) {
			if (support_rate[i] == IEEE80211_b_datarate[k]) {
				return 0;
			}
		}
	}
	return -1;
}

static int ap_hash_compare(const void *a, const void *b)
{
	AP_INFO *ap1 = (AP_INFO *)a;
	AP_INFO *ap2 = (AP_INFO *)b;
	return mac_cmp(ap1->bssid, ap2->bssid) ? 0 : -1;
}

static int ap_hash_func(const void *a)
{
	AP_INFO *ap = (AP_INFO *)a;
	return (ap->bssid[0] + ap->bssid[3] + ap->bssid[5]);
}

static void ap_hash_free(void *a)
{
	free(a);
}

static AP_INFO* ap_find_hash(hash_ctx_t *ctx, uint8_t *bssid)
{
	AP_INFO ap;
	mac_cpy(ap.bssid, bssid);
	return (AP_INFO*)find_hash(ctx, &ap);
}

static AP_INFO *new_ap_info(uint8_t *bssid)
{
	int i;
	AP_INFO *ap = (AP_INFO *)malloc(sizeof(*ap));
	memset(ap, 0, sizeof(*ap));

	mac_cpy(ap->bssid, bssid);
	ap->tinit = ap->tlast = time(NULL);
	ap->avg_power = -1;
	ap->best_power = -1;
	ap->power_index = -1;
	for (i=0; i<NB_PWR; i++) {
		ap->power_lvl[i] = -1;
	}
	ap->channel = -1;
	ap->max_speed = -1;
	ap->proto = 0;
	ap->seen_ht = 0;
	ap->seen_vht = 0;
	ap->ratelen = 0;
	ap->security = 0;

//	ap->uiv_root = uniqueiv_init();

	ap->nb_data = 0;
	ap->nb_dataps = 0;
	ap->nb_data_old = 0;
	gettimeofday(&ap->tv, NULL);

	ap->nb_bcn = 0;

	ap->rx_quality = 0;
	ap->fcapt = 0;
	ap->fmiss = 0;
	ap->last_seq = 0;
	gettimeofday(&ap->ftimef, NULL);
	gettimeofday(&ap->ftimel, NULL);
	gettimeofday(&ap->ftimer, NULL);

	ap->ssid_length = 0;
	memset(ap->essid, 0, sizeof(ap->essid));
	ap->timestamp = 0;

	ap->EAP_detected = 0;

	return ap;
}

static void ap_hash_desc(const void *a)
{
	AP_INFO *ap = (AP_INFO*)a;
	const char *sec;
	const char *enc;

	if ((ap->security & (STD_OPN|STD_WEP|STD_WPA|STD_WPA2)) == 0)
		sec = "";
	else if (ap->security & STD_WPA2)
		sec = "WPA2";
	else if (ap->security & STD_WPA)
		sec = "WPA";
	else if (ap->security & STD_WEP)
		sec = "WEP";
	else if (ap->security & STD_OPN)
		sec = "OPN";

	if ((ap->security & (ENC_WEP|ENC_TKIP|ENC_WRAP|ENC_CCMP|ENC_WEP104|ENC_WEP40)) == 0)
		enc = "";
	else if (ap->security & ENC_CCMP)
		enc = "CCMP";
	else if (ap->security & ENC_WRAP)
		enc = "WRAP";
	else if (ap->security & ENC_TKIP)
		enc = "TKIP";
	else if (ap->security & ENC_WEP104)
		enc = "WEP104";
	else if (ap->security & ENC_WEP40)
		enc = "WEP40";
	else if (ap->security & ENC_WEP)
		enc = "WEP";

	printf(_MAC_FMT_"(%s,CH:%d,PW:%d,%s,%s,SPD:%.1f)",
			_MAC_FMT_FILL_(ap->bssid), ap->essid,
			ap->channel, ap->avg_power, sec, enc,
			ap->max_speed == -1 ? 0 : ap->max_speed);
}

static hash_op ap_hash_op = {
		.hash_compare = ap_hash_compare,
		.hash_func    = ap_hash_func,
		.hash_free    = ap_hash_free,
		.hash_desc    = ap_hash_desc,
};


static int sta_hash_compare(const void *a, const void *b)
{
	ST_INFO *sta1 = (ST_INFO *)a;
	ST_INFO *sta2 = (ST_INFO *)b;
	return mac_cmp(sta1->stmac, sta2->stmac) ? 0 : -1;
}

static int sta_hash_func(const void *a)
{
	ST_INFO *sta = (ST_INFO *)a;
	return (sta->stmac[3] + sta->stmac[4] + sta->stmac[5]);
}

static void sta_hash_free(void *a)
{
	free(a);
}

static ST_INFO* sta_find_hash(hash_ctx_t *ctx, uint8_t* stamc)
{
	ST_INFO sta;
	mac_cpy(sta.stmac, stamc);
	return (ST_INFO*)find_hash(ctx, &sta);
}

static ST_INFO *new_sta_info(uint8_t *stamac)
{
	int i;
	ST_INFO *st = (ST_INFO *)malloc(sizeof(*st));
	memset(st, 0, sizeof(*st));

	mac_cpy(st->stmac, stamac);
	st->nb_pkt = 0;
	st->tinit = st->tlast = time(NULL);

	st->power = -1;
	st->rate_to = -1;
	st->rate_from = -1;

	st->probe_index = -1;
	st->missed = 0;
	st->lastseq = 0;
	st->qos_fr_ds = 0;
	st->qos_to_ds = 0;
	st->databytes = 0;
	st->channel = 0;

	gettimeofday(&st->ftimer, NULL);

	for (i=0; i<NB_PRB; i++) {
		memset(st->probes[i], 0, sizeof(st->probes[i]));
		st->ssid_length[i] = 0;
	}

	return st;
}

static void sta_hash_desc(const void *a)
{
	ST_INFO *sta = (ST_INFO*)a;
	printf(_MAC_FMT_, _MAC_FMT_FILL_(sta->stmac));
}

static hash_op sta_hash_op = {
		.hash_compare = sta_hash_compare,
		.hash_func    = sta_hash_func,
		.hash_free    = sta_hash_free,
		.hash_desc    = sta_hash_desc,
};

void nodes_info_init(NODES_INFO *nodes)
{
	if (nodes->ap_hctx)
		free_hash_ctx(nodes->ap_hctx);
	nodes->ap_hctx = new_hash_single_thread_ctx(&ap_hash_op,  DEF_HASH_BCK_SIZE, 0);
	if (nodes->st_hctx)
		free_hash_ctx(nodes->st_hctx);
	nodes->st_hctx = new_hash_single_thread_ctx(&sta_hash_op, DEF_HASH_BCK_SIZE, 0);
}

void nodes_info_free(NODES_INFO *nodes)
{
	if (nodes->ap_hctx) {
		free_hash_ctx(nodes->ap_hctx);
		nodes->ap_hctx = NULL;
	}
	if (nodes->st_hctx) {
		free_hash_ctx(nodes->st_hctx);
		nodes->st_hctx = NULL;
	}
}

int air_packet_parse(NODES_INFO *nodes, uint8_t channel,
		uint8_t *h80211, int caplen, struct rx_info *ri)
{
	int seq;
	int i, n, msd;
	unsigned z;
	uint8_t c;
	wi_hdr *pH = NULL;
	wi_ctrl_hdr *pcH = NULL;
	int fromDS, toDS;
	uint8_t bssid[6] = {0, }, stamac[6] = {0, };
	hash_ctx_t *ap_hctx = NULL;
	hash_ctx_t *sta_hctx = NULL;
	AP_INFO *ap = NULL;
	ST_INFO *sta = NULL;
	int new_ap = 0, new_sta = 0;

	//nodes->cur_channel = channel;
	ap_hctx  = nodes->ap_hctx;
	sta_hctx = nodes->st_hctx;

	pH  = (wi_hdr *)    h80211; // mgnt/data packet header
	pcH = (wi_ctrl_hdr*)h80211; // control packet header

	fromDS = pH->fc.flags.fromds;
	toDS   = pH->fc.flags.tods;

	/* skip packets smaller than a 802.11 header */
	if (caplen < 24) {
		cle("packet is too smaller than 24, len=%d\n", caplen);
		goto end_parse;
	}
	if (pH->fc.version != 0) {
		cle("802.11 protocol only support version 0 (but ver: %d)\n", pH->fc.version);
		goto end_parse;
	}
	/* skip (uninteresting) control frames */
	if (pH->fc.type == frame_ctrl) {
		goto end_parse;
	}
	/* if it's a LLC null packet, just forget it (may change in the future) */
	if (caplen > 28) {
		const unsigned char llcnull[4] = {0, 0, 0, 0};
		if (memcmp(h80211+24, llcnull, 4) == 0) {
//			printf("skip\n");
			return 0;
		}
	}

	// skip '00:00:00:00:00:00' address
	if (mac_cmp_null(pH->addr1) || mac_cmp_null(pH->addr2) || mac_cmp_null(pH->addr3)) {
		return 0;
	}
	switch ((fromDS << 1) | toDS) {
	case 3:
		if (mac_cmp_null(pH->addr4)) {
			return 0;
		}
		break;
	}

	// Sequence Number: 2279
	// hex: 70 8e
	seq = ((h80211[22] >> 4) + (h80211[23] << 4));

	if (pH->fc.type == frame_mgnt &&
			pH->fc.subtype == mgnt_probe_req) {
		goto skip_access_point;
	}

	switch ((fromDS << 1) | toDS) {
	case 0: // Adhoc
		mac_cpy(bssid, pH->addr3);
		break;
	case 1: // ToDS
		mac_cpy(bssid, pH->addr1);
		break;
	case 2: // FromDS
		mac_cpy(bssid, pH->addr2);
		break;
	case 3: // WDS -> Transmitter taken as BSSID
		mac_cpy(bssid, pH->addr2);
		break;
	}

	// skip 'ff:ff:ff:ff:ff:ff' bssid
	if (mac_cmp_bcast(bssid)) {
		return 0;
	}

	/*
	 * - 창룡도서관에서 beacon 안 잡히고 ipv6 패킷만 잡힘. 실제 beacon을 보지 못함.. broadcast 패킷이였음
	// 06:30:0d:93:b1:c3
	{
		uint8_t m[6] = {0x06,0x30,0x0d,0x93,0xb1,0xc3};
		if (mac_cmp(bssid, m)) {
			printf("KINOW len:%d, from:%d, to:%d, "_MAC_FMT_","_MAC_FMT_","_MAC_FMT_"\n",
					caplen, fromDS, toDS,
					_MAC_FMT_FILL_(pH->addr1),_MAC_FMT_FILL_(pH->addr2),_MAC_FMT_FILL_(pH->addr3));
		}
	}
	*/

	// find ap from ap hash table
	ap = ap_find_hash(ap_hctx, bssid);
	if (!ap) {
		ap = new_ap_info(bssid);
		new_ap = 1;
		//printf("new ap "_MAC_FMT_"\n", _MAC_FMT_FILL_(bssid));
	}
	ap->tlast = time(NULL);

	/*
	 * only update power if packets comes from the AP:
	 * either type == mgmt and SA != BSSID
	 * or FromDS == 1 and ToDS == 0
	 */
	if ((fromDS == 1 && toDS == 0)
			||
		(fromDS == 0 && toDS == 0 && mac_cmp(bssid, pH->addr2))
	) {
		ap->power_index = (ap->power_index+1) % NB_PWR;
		ap->power_lvl[ap->power_index] = ri->ri_power;

		ap->avg_power = 0;
		for (i=0, n=0; i<NB_PWR; i++) {
			if (ap->power_lvl[i] != -1) {
				ap->avg_power += ap->power_lvl[i];
				n++;
			}
		}
		if (n > 0) {
			ap->avg_power /= n;
			if (ap->avg_power > ap->best_power) {
				ap->best_power = ap->avg_power;

			}
		} else {
			ap->avg_power = -1;
		}

		if (ap->fcapt == 0 && ap->fmiss == 0) {
			gettimeofday(&ap->ftimef, NULL);
		}
		if (ap->last_seq != 0) {
			ap->fmiss += (seq - ap->last_seq - 1);
		}
		ap->last_seq = seq;
		ap->fcapt++;
		gettimeofday(&ap->ftimel, NULL);
	}

	switch (pH->fc.type) {
	case frame_mgnt:
		if (pH->fc.subtype == mgnt_beacon) {
			ap->nb_bcn++;
		}
		else if (pH->fc.subtype == mgnt_probe_resp) {
			/* reset the WPS state */
			ap->wps.state = 0xFF;
			ap->wps.ap_setup_locked = 0;
		}
		break;
	}
	ap->nb_pkt++;


skip_access_point:

	/* locate the station MAC in the 802.11 header */
	switch ((fromDS << 1) | toDS) {
	case 0: // adhoc
		/* if management, check that SA != BSSID */
		if (mac_cmp(pH->addr2, bssid)) {
			goto skip_station;
		}
		mac_cpy(stamac, pH->addr2);
		break;
	case 1: // ToDS
		/* ToDS packet, must come from a client */
		mac_cpy(stamac, pH->addr2);
		break;
	case 2: // FromDS
		/* FromDS packet, reject broadcast MACs */
		if (pH->addr1[0]%2 != 0) {
			goto skip_station;
		}
		mac_cpy(stamac, pH->addr1);
		break;
	default:
		goto skip_station;
	}

	sta = sta_find_hash(sta_hctx, stamac);
	if (!sta) {
		sta = new_sta_info(stamac);
		new_sta = 1;
		//printf("new sta "_MAC_FMT_"\n", _MAC_FMT_FILL_(stamac));
	}
	if (!mac_cmp_bcast(bssid)) {
		mac_cpy(sta->bssid, bssid);
	}
	/* update bitrate to station */
	if (fromDS == 1 && toDS == 0) {
		sta->rate_to = ri->ri_rate;
	}
	/* update the last time seen */
	sta->tlast = time(NULL);

	/*
	 * only update power if packets comes from the
	 * client: either type == Mgmt and SA != BSSID,
	 * or FromDS == 0 and ToDS == 1
	 */
	if ((fromDS == 0 && toDS == 1)
		||
		(fromDS == 0 && toDS == 0 && !mac_cmp(pH->addr2, bssid))
	) {
		sta->power = ri->ri_power;
		sta->rate_from = ri->ri_rate;
		if (ri->ri_channel > 0 && ri->ri_channel <= HIGHEST_CHANNEL) {
			sta->channel = ri->ri_channel;
		} else {
			sta->channel = nodes->cur_channel;
		}
		if (sta->lastseq != 0) {
			msd = seq - sta->lastseq - 1;
			if (msd > 0 && msd < 1000) {
				sta->missed += msd;
			}
		}
		sta->lastseq = seq;
	}
	sta->nb_pkt++;

skip_station:

	/* packet parsing: Probe Request */
	if (pH->fc.type == frame_mgnt && pH->fc.subtype == mgnt_probe_req) {
		uint8_t *p = h80211 + 24;

		while (p < h80211 + caplen) {
			// check IE (TLV header) length
			if (p + 2 + p[1] > h80211 + caplen) {
				break;
			}
			// ESSID IE
			if (p[0] == 0x00 && p[1] > 0 && p[2] != '\0' &&
				(p[1] > 1 || p[2] != ' ')
			) {
				n = p[1];
				for (i=0; i<n; i++) {
					// ' ' is ascii code SPACE
					if (p[2+i] > 0 && p[2+i] < ' ') {
						goto skip_probe;
					}
				}
				/* got a valid ASCII probed ESSID, check if it's
				   already in the ring buffer */
				for (i=0; i<NB_PRB; i++) {
					if (memcmp(sta->probes[i], p+2, n) == 0) {
						goto skip_probe;
					}
				}
				sta->probe_index = (sta->probe_index + 1) % NB_PRB;
				memset(sta->probes[sta->probe_index], 0, MAX_IE_SIZE);
				memcpy(sta->probes[sta->probe_index], p+2, n); // twice?!
				sta->ssid_length[sta->probe_index] = n;

				for (i=0; i<n; i++) {
					c = p[2+i];
					if (!(c >= 32 && c <= 126)) {
						//could also check ||(c>0 && c<32)
						c = '.';
					}
					sta->probes[sta->probe_index][i] = c;
				}
			}
			p += 2 + p[1];
		}
	}


skip_probe:

	/* packet parsing: Beacon or Probe Response */
	if (pH->fc.type == frame_mgnt &&
			(pH->fc.subtype == mgnt_beacon ||
			 pH->fc.subtype == mgnt_probe_resp)
	){

		wi_bcn_fixed *bcn_fixed = (wi_bcn_fixed*)(h80211 + sizeof(wi_hdr));

		if (!(ap->security & (STD_OPN | STD_WEP | STD_WPA | STD_WPA2))) {
//			if ((h80211[34] & 0x10) >> 4)
			if (bcn_fixed->capa.privacy) {
				ap->security |= (STD_WEP | ENC_WEP);
			} else {
				ap->security = STD_OPN;
			}
		}
		ap->preamble = bcn_fixed->capa.preamble;
		ap->timestamp = letoh64(bcn_fixed->timestamp);

		wi_tag *tag = (wi_tag*)(h80211 + sizeof(wi_hdr) + sizeof(wi_bcn_fixed));
		while ((uint8_t*)tag < h80211 + caplen) {
			if ((tag->data + tag->len) > h80211 + caplen) {
				break;
			}
			/* ESSID */
			if (tag->tag == 0x00) {
				if (ap->ssid_length < tag->len) {
					ap->ssid_length = tag->len;
				}
				if (tag->len > 0
						&&
					tag->data[0] != '\0'
						&&
					(tag->len > 1 || tag->data[0] != ' ')
				){
					n = tag->len;
					memset(ap->essid, 0, sizeof(ap->essid));
					memcpy(ap->essid, tag->data, n);

					for (i=0; i<n; i++) {
						if (ap->essid[i] == '\0') {
							break;
						}
						if (!(ap->essid[i] >= 32) && (ap->essid[i] <= 126)) {
							ap->essid[i] = '.';
						}
					}
					if (i < ap->ssid_length) {
						ap->ssid_length = i;
					}
				}
//printf("len: %d, ssid: %s\n", ap->ssid_length, ap->essid);
			}
			/* get the maximum speed in Mb and the AP's channel */
			if (tag->tag == 0x01 || tag->tag == 0x32) {
				for (i=0; i<tag->len; i++) {

					if (ap->ratelen < 16) {
						ap->rate[ap->ratelen++] = tag->data[i] & 0x7F;
					}

					if (ap->max_speed < (float)((tag->data[i] & 0x7F) / 2)) {
						ap->max_speed = (float)((tag->data[i] & 0x7f) /2);
					}
				}
			}
			/* DS Parameter set */
			if (tag->tag == 0x03) {
				ap->channel = tag->data[0];
			} /* also get the channel from ht information->primary channel */
			else if (tag->tag == 0x3d) {
				ap->channel = tag->data[0];
			}

			/*
			 * N-mode HT Capability
			 * N-mode Additional HT Info
			 */
			if (tag->tag == 45 || tag->tag == 61) {
				ap->seen_ht = 1;
			}

			/*
			 * 802.11ac VHT Capability
			 * 802.11ac VHT Operation element (channel width: 80Mhz)
			 */
			if (tag->tag == 191 || tag->tag == 192) {
				ap->seen_vht = 1;
			}

			tag = (wi_tag*)(tag->data + tag->len);
		}
	}

//#define ONLY_SSID_PARSE

#ifndef ONLY_SSID_PARSE
	/* packet parsing: Beacon or Probe Response */
	if ((caplen > 38) &&
		 (pH->fc.type == frame_mgnt &&
			(pH->fc.subtype == mgnt_beacon ||
			 pH->fc.subtype == mgnt_probe_resp)
		 )
	){
		wi_tag *tag = (wi_tag*)(h80211 + sizeof(wi_hdr) + sizeof(wi_bcn_fixed));

		while ((uint8_t*)tag < h80211 + caplen) {
			// OUI: 00-50-F2
			// Vendor Specific OUI Type: 1
			// type: WPA Information Element (0x01)
			// WPA Version: 00 01
#define WPA_INFO_ELEMENT "\x00\x50\xF2\x01\x01\x00"
			if ((	(tag->tag > 0xDD) &&
					(tag->len >= 8) &&
					memcmp(tag->data, WPA_INFO_ELEMENT, 6) == 0)
					||
					(tag->tag == 0x30) // RSN Information
			){
				ap->security &= ~(STD_WEP | ENC_WEP | STD_WPA);
				int offset = 0;

				if (tag->tag == 0xDD) {
					/* WPA defined in vendor specific tag -> WPA1 support */
					ap->security |= STD_WPA;
					offset = 4;
				}
				else if (tag->tag == 0x30) {
					ap->security |= STD_WPA2;
					offset = 0;
				}

				if (tag->len < (18+offset)) {
					tag = (wi_tag*)(tag->data + tag->len);
					continue;
				}
				/* WPA Information Element: skip Multicast Cipher Suite (4 bytes) */
				/* RSN Information        : skip Group Cipher Suite     (4 bytes) */
				if ((tag->data + 7 + offset) > h80211 + caplen) {
					break;
				}
				int numuni = 0;
				int numauth = 0;
				uint8_t *p = NULL;

				/* WPA Information Element: Unicast Cipher Suite Count  */
				/* RSN Information        : Pairwise Cipher Suite Count */
				numuni = (tag->data[6+offset]) +
						  (tag->data[7+offset] << 8);

				if ((tag->data + (9+offset) + 4*numuni) > h80211 + caplen) {
					break;
				}
				numauth = (tag->data[(8+offset) + 4*numuni]) +
						   (tag->data[(9+offset) + 4*numuni] << 8);

				p = tag->data + (8+offset);
				if (tag->tag != 0x30) {
					if ((tag->data + (4*numuni) + (4*numauth)) > h80211 + caplen) {
						break;
					}
				} else {
					// + 2 : RSN Capabilities (2 bytes)
					if ((tag->data + (4*numuni) + (4*numauth) + 2) > h80211 + caplen) {
						break;
					}
				}

				for (i=0; i<numuni; i++) {
					switch (p[i*4+3]) {
					case 0x01:
						ap->security |= ENC_WEP;
						break;
					case 0x02:
						ap->security |= ENC_TKIP;
						break;
					case 0x03:
						ap->security |= ENC_WRAP;
						break;
					case 0x04:
						ap->security |= ENC_CCMP;
						break;
					case 0x05:
						ap->security |= ENC_WEP104;
						break;
					default:
						break;
					}
				}
				p += 2 + 4*numuni;

				for (i=0; i<numauth; i++) {
					switch (p[i*4+3]) {
					case 0x01:
						ap->security |= AUTH_MGT;
						break;
					case 0x02:
						ap->security |= AUTH_PSK;
						break;
					default:
						break;
					}
				}
				p += 2 + 4*numauth;
			}
			/* WMM/WME */
			else if (tag->tag == 0xDD &&
					  tag->len >= 8 &&
					  memcmp(tag->data, "\x00\x50\xF2\x02\x01\x01", 6) == 0
			){
				ap->security |= STD_QOS;

			}
			/* WPS */
			else if (tag->tag == 0xDD &&
					  tag->len >= 4 &&
					  memcmp(tag->data, "\x00\x50\xF2\x04", 4) == 0
			){
				uint8_t *p = tag->data + 4;
				int len = tag->len;
				int subtype = 0, sublen = 0;

				while (len >= 4) {
					subtype = (p[0] << 8) + p[1];
					sublen  = (p[2] << 8) + p[3];
					if (sublen > len) {
						break;
					}
					switch (subtype) {
					case 0x104a: // WPS Version
						ap->wps.version = p[4];
						break;
					case 0x1011: // Device Name
					case 0x1012: // Device Password ID
					case 0x1021: // Manufacturer
					case 0x1023: // Model
					case 0x1024: // Model Number
					case 0x103b: // Response Type
					case 0x103c: // RF Bands
					case 0x1041: // Selected Registrar
					case 0x1042: // Serial Number
						break;
					case 0x1044: // WPS State
						ap->wps.state = p[4];
						break;
					case 0x1047: // UUID Enrollee
					case 0x1049: // Vendor Extension
					case 0x1054: // Primary Device Type
						break;
					case 0x1057: // AP Setup Locked
						ap->wps.ap_setup_locked = p[4];
						break;
					case 0x1008: // Config Methods
					case 0x1053: // Selected Registrar Config Methods
						ap->wps.meth = (p[4] << 8) + p[5];
						break;
					default:     // Unknown type-length-value
						break;
					}
					p   += sublen + 4;
					len -= sublen + 4;
				}
			}

			tag = (wi_tag*)(tag->data + tag->len);
		}
	}

	/* packet parsing: Authentication Response */
	if (pH->fc.type == frame_mgnt && pH->fc.subtype == mgnt_auth && caplen >= 30) {
		if (ap->security & STD_WEP) {

			// successful step 2 or 4 (coming from the AP)
			wi_auth_fixed* auth_fixed = (wi_auth_fixed*)(h80211 + sizeof(wi_hdr));
			if (auth_fixed->status == 0 &&
					(letoh16(auth_fixed->seq) == 0x02 ||
					 letoh16(auth_fixed->seq) == 0x04)
			){
				ap->security &= ~(AUTH_OPN | AUTH_PSK | AUTH_MGT);
				if (letoh16(auth_fixed->algorithm) == 0x00) {
					ap->security |= AUTH_OPN;
				}
				if (letoh16(auth_fixed->algorithm) == 0x01) {
					ap->security |= AUTH_PSK;
				}
			}
		}
	}

	/* packet parsing: Association Request */
	if (pH->fc.type == frame_mgnt && pH->fc.subtype == mgnt_assoc_req && caplen > 28) {
		// TODO: update ESSID???

		if (sta) {
			sta->wpa.state = 0;
		}
	}

	/* packet parsing: some data */
	if (pH->fc.type == frame_data) {
		/* update the channel if we didn't get any beacon */
		if (ap->channel == -1) {
			if (ri->ri_channel > 0 && ri->ri_channel <= HIGHEST_CHANNEL) {
				ap->channel = ri->ri_channel;
			} else {
				ap->channel = nodes->cur_channel;
			}
		}

		/* check the SNAP header to see if data is encrypted */
		z = (pH->fc.flags.fromds == 1 && pH->fc.flags.tods == 1) ? 24 : 30;


		/* Check if 802.11e (QoS) */
		if ((pH->fc.subtype & 0x80) == 0x80) {
			z += 2;
			if (sta) {
				if (pH->fc.flags.fromds) {
					sta->qos_fr_ds = 1;
				} else {
					sta->qos_to_ds = 1;
					sta->databytes += caplen;
				}
			}
		}
		else {
			if (sta) {
				if (pH->fc.flags.fromds) {
					sta->qos_fr_ds = 0;
				} else {
					sta->qos_to_ds = 0;
					sta->databytes += caplen;
				}
			}
		}

		if (z == 24) {

		}

		if (z + 26 > (unsigned)caplen) {
			goto end_parse;
		}

		/* IPv4 address */
		// aa aa 03
		if (h80211[z] == h80211[z+1] && h80211[z+2] == 0x03) {
			/* if ethertype == IPv4, find the LAN address */
			if (h80211[z+6] == 0x08 &&
				 h80211[z+7] == 0x00 &&
				 pH->fc.flags.tods == 1) {
				memcpy(ap->lanip, &h80211[z+20], 4);
			}
			if (h80211[z+6] == 0x08 &&
				 h80211[z+7] == 0x06) {
				memcpy(ap->lanip, &h80211[z+22], 4);
			}
		}

		if (ap->security == 0 || (ap->security & STD_WEP)) {
			if (pH->fc.flags.protect == 0) {
				ap->security |= STD_OPN;
			} else {
				if ((h80211[z+3] & 0x20) == 0x20) {
					ap->security |= STD_WPA;
				} else {
					ap->security |= STD_WEP;
					if ((h80211[z+3] & 0xC0) != 0x00) {
						ap->security |= ENC_WEP40;
					} else {
						ap->security &= ~ENC_WEP40;
						ap->security |= ENC_WEP;
					}
				}
			}
		}

		ap->nb_data++;

		z = (pH->fc.flags.fromds == 1 && pH->fc.flags.tods == 1) ? 24 : 30;

		/* Check if 802.11e (QoS) */
		if ((pH->fc.subtype & 0x80) == 0x80) {
			z += 2;
		}
		if (z + 26 > (unsigned)caplen) {
			goto end_parse;
		}

		z += 6; // skip KKC header

		if (h80211[z] == 0x88 &&
			h80211[z+1] == 0x8E &&
			pH->fc.flags.protect == 0
		){
			ap->EAP_detected = 1;
			z += 2; // skip ethertype

			if (sta == NULL) {
				goto end_parse;
			}

			/* frame 1: Pairwise == 1, Install == 0, Ack == 1, MIC == 0 */
			if (
				(h80211[z+6] & 0x08) != 0 && // Key Type: Pairwise Key
				(h80211[z+6] & 0x40) == 0 && // Install: Not Set
				(h80211[z+6] & 0x80) != 0 && // Key ACK: set
				(h80211[z+5] & 0x01) == 0    // Key MIC: Not Set
			){
				// WPA Key Nonce
				memcpy(sta->wpa.anonce, &h80211[z+17], 32);
				sta->wpa.state = 1;
			}

			if (z+17+32 > (unsigned)caplen) {
				goto end_parse;
			}

			/* frame 2 or 4: Pairwise == 1, Install == 0, Ack == 0, MIC == 1 */
			if (
				(h80211[z+6] & 0x08) != 0 && // Key Type: Pairwise Key
				(h80211[z+6] & 0x40) == 0 && // Install: Not Set
				(h80211[z+6] & 0x80) == 0 && // Key ACK: Not Set
				(h80211[z+5] & 0x01) != 0    // Key MIC: Set
			){
				if (memcmp(&h80211[z+17], ZERO32, 32) != 0) {
					memcpy(sta->wpa.snonce, &h80211[z+17], 32);
					sta->wpa.state |= 2;
				}

				if ((sta->wpa.state & 4) != 4) {
					sta->wpa.eapol_size = (h80211[z+2] << 8) + h80211[z+3] + 4;
					if (
						caplen - (unsigned)z < sta->wpa.eapol_size || sta->wpa.eapol_size == 0 ||
						caplen - (unsigned)z < 81 + 16 || sta->wpa.eapol_size > sizeof(sta->wpa.eapol)
					){
						// Ignore the packet trying to crash us.
						sta->wpa.eapol_size = 0;
						goto end_parse;
					}
					memcpy(sta->wpa.keymic, &h80211[z+81], 16); // WPA Key MIC
					memcpy(sta->wpa.eapol,  &h80211[z], sta->wpa.eapol_size);
					memset(sta->wpa.eapol + 81, 0, 16); // clear WPA Key MIC
					sta->wpa.state |= 4;
					sta->wpa.keyver = h80211[z+6] & 7; // Key Information: Key Descriptor Version
				}
			}

			/* frame 3: Pairwise == 1, Install == 1, Ack == 1, MIC == 1 */
			if (
					(h80211[z+6] & 0x08) != 0 && // Key Type: Pairwise Key
					(h80211[z+6] & 0x40) != 0 && // Install: Set
					(h80211[z+6] & 0x80) != 0 && // Key ACK: Set
					(h80211[z+5] & 0x01) != 0    // Key MIC: Set
			){
				if (memcmp(&h80211[z+17], ZERO32, 32) != 0) {
					memcpy(sta->wpa.anonce, &h80211[z+17], 32);
					sta->wpa.state |= 1;
				}
				if ((sta->wpa.state & 4) != 4) {
					sta->wpa.eapol_size = (h80211[z+2] << 8) + h80211[z+3] + 4;

					if (
						caplen - (unsigned)z < sta->wpa.eapol_size || sta->wpa.eapol_size == 0 ||
						caplen - (unsigned)z < 81 + 16 || sta->wpa.eapol_size > sizeof(sta->wpa.eapol)
					){
						// Ignore the packet trying to crash us.
						sta->wpa.eapol_size = 0;
						goto end_parse;
					}
					memcpy(sta->wpa.keymic, &h80211[z+81], 16); // WPA Key MIC
					memcpy(sta->wpa.eapol,  &h80211[z], sta->wpa.eapol_size);
					memset(sta->wpa.eapol + 81, 0, 16); // clear WPA Key MIC
					sta->wpa.state |= 4;
					sta->wpa.keyver = h80211[z+6] & 7; // Key Information: Key Descriptor Version
				}
			}

			if (sta->wpa.state == 7) {
				memcpy(sta->wpa.stmac, sta->stmac, 6);
				uint8_t wpa_bssid[6];
				char message[512] = {0, };
				memcpy(wpa_bssid, ap->bssid, 6);
				memset(message, 0, sizeof(message));

				printf("[WPA handshake: %02x:%02x:%02x:%02x:%02x:%02x",
						ap->bssid[0], ap->bssid[1], ap->bssid[2],
						ap->bssid[3], ap->bssid[4], ap->bssid[5]);

			}
		}
	}

	if (pH->fc.type == frame_mgnt &&
			(pH->fc.subtype == mgnt_beacon || pH->fc.subtype == mgnt_probe_resp)
	){
		if (is_b(ap->rate, ap->ratelen) == 0) {
			ap->proto |= WIFI_PROTO_80211b;
		}
		if (is_ag(ap->rate, ap->ratelen) == 0) {
			if (ap->channel <= 14) {
				ap->proto |= WIFI_PROTO_80211g;
			} else {
				ap->proto |= WIFI_PROTO_80211a;
			}
		}
		if (ap->seen_ht) {
			ap->proto |= WIFI_PROTO_80211n;
		}
		if (ap->seen_vht) {
			ap->proto |= WIFI_PROTO_80211ac;
		}
	}
#endif

end_parse:

	if (ap && new_ap) {
		if (insert_hash(ap_hctx, (void*)ap)) {
			fprintf(stderr, "unable insert ap:"_MAC_FMT_"\n",
					_MAC_FMT_FILL_(ap->bssid));
			return -1;
		}
	}
	if (sta && new_sta) {
		if (insert_hash(sta_hctx, (void*)sta)) {
			fprintf(stderr, "unable insert sta:"_MAC_FMT_"\n",
					_MAC_FMT_FILL_(sta->stmac));
			return -1;
		}
	}

	return 0;

}

void check_expired_ap(hash_ctx_t *hctx, int lifetime)
{
	AP_INFO *ap;
	hash_iter_t it = INIT_HASH_ITER;
	time_t now = time(NULL);

	while (hash_foreach(hctx, &it) == 0) {
		ap = (AP_INFO*)it.b->data;
		if ((now - ap->tlast) > lifetime) {
			delete_hash(hctx, ap);
		}
	}
}

void check_expired_st(hash_ctx_t *hctx, int lifetime)
{
	ST_INFO *sta;
	hash_iter_t it = INIT_HASH_ITER;
	time_t now = time(NULL);

	while (hash_foreach(hctx, &it) == 0) {
		sta = (ST_INFO*)it.b->data;
		if ((now - sta->tlast) > lifetime) {
			delete_hash(hctx, sta);
		}
	}
}
