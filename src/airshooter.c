/*
 * airshooter.c
 *
 *  Created on: 2016. 12. 1.
 *      Author: root
 */

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <signal.h>

#include "airshooter.h"
#include "wi_packet.h"
#include "wifi_setup.h"
#include "h80211_types.h"
#include "common.h"
#include "ieee802_11_defs.h"
#include "airsock.h"

extern const char* h80211_frame_string[];
extern const char* h80211_mgnt_string[];
extern const char* h80211_ctrl_string[];
extern const char* h80211_data_string[];

/**
 * @brief build_data_sample
 * @param buf Array packet buffer
 * @param qos Is QoS data?
 * @param nodata Is no data?
 * @param data_size If be filled data, size of data
 * @return write data length
 */
static int build_data_sample(AIRSHOOT_G *g, struct airconf *conf,
		uint8_t buf[], boolean isqos, boolean isdata)
{
	int i = 0;
	wi_qos_control *pQos = NULL;
	wi_data_llc *pLLC = NULL;
	uint8_t *realdata = NULL;
	int len = 0;
	int sign_len = 0;
	time_t t = time(NULL);
	static uint8_t qos_priority[] = {
			0, // not effect
			1, // background (background)
			2,
			3, // excellent effort (best effort)
			4, // controlled load (video)
			5, // video (video)
			6, // voice (voice)
			7, // network control (voice)
	};

	// QoS Control
	if (isqos) {
		pQos = (wi_qos_control*)(buf + len);
		memset(pQos, 0, sizeof(wi_qos_control));
		pQos->priority = qos_priority[rand() % (sizeof(qos_priority))];
		len += sizeof(*pQos);
	}
	// TKIP/CCMP or WEP
	if (conf->protect) {
#define RSN_WEP  0
#define RSN_CCMP 1
#define RSN_TKIP 2

		int rsn = RSN_TKIP;

		if (rsn == RSN_WEP) {
			timebase_rand(buf+len, 4);
			// exten iv bit off
			buf[len-1+4] &= (~0x20);
			len += 4;
		}
		else if (rsn == RSN_CCMP) {
//			printf("ccmp\n");
			memcpy(buf+len, "\x07\x11\x00\x20\x00\x00\x00\x00", 8);
			buf[len+3] = 0x20;
			len += 8;
		}
		else if (rsn == RSN_TKIP) {
//			printf("tkip\n");
			memcpy(buf+len, "\x0a\x2a\x00\x20\x00\x00\x00\x00", 8);
			buf[len+3] = 0x20;
			len += 8;
		}
	}
	// LLC
	else {
		pLLC = (wi_data_llc*)(buf + len);
		pLLC->dsap = 0xaa;
		pLLC->ssap = 0xaa;
		pLLC->ctrl = 0x03;
		pLLC->oui[0] = 0; // Encapsulated Ethernet
		pLLC->oui[1] = 0;
		pLLC->oui[2] = 0;

		t = t % 3; // random
		if (t == 0) {
			pLLC->pid[0] = 0x88; // 802.1x
			pLLC->pid[1] = 0x8e;
		}
		else if (t == 1) {
			pLLC->pid[0] = 0x08; // IPv4
			pLLC->pid[1] = 0x00;
		}
		else if (t == 2) {
			pLLC->pid[0] = 0x08; // ARP
			pLLC->pid[1] = 0x06;
		}
		len += sizeof(*pLLC);
	}
	// Data
	if (isdata) {
		// filled random data
		realdata = buf + len;

		sign_len = strlen(conf->data_sign);
		if ((sign_len > 0) && (sign_len <= conf->data_size)) {
			memcpy(realdata, conf->data_sign, sign_len);
		}
		for (i = sign_len; i < conf->data_size-sign_len; i++) {
			realdata[i] = rand() & 0xFF;
		}
		len += conf->data_size;
	}
	return len;
}

static int build_pspoll_sample(AIRSHOOT_G *g, struct airconf *conf, uint8_t buf[])
{
	return 0;
}

/*
 점 조정 함수 (PCF - point coordination function)
 이 방식은 인프라스트럭처(infrastructure) 모드에서만 사용 가능하다.
 실제로 이 기능이 적용된 AP나 Wi-Fi 어댑터들은 얼마 되지 않는다.
 AP는 규칙적으로 beacon 프레임을 보낸다.
 (일반적으로 매 0.1초 마다 한개의 beacon 프레임을 전송한다.)
 이 beacon 프레임 사이에 PCF는 두가지 구간을 정한다.
 1. 비경쟁구간(Contention Free Period - CFP)
    비경쟁구간에서 AP는 Contention-Free-Poll (CF-Poll)패킷을
    각 station에 차례차례 전송하게된다.
    CF-Poll을 받은 station은 패킷을 전송할 권한을 갖는다.
    즉 AP가 경쟁에 관한 중재자가 된다.
 2. 경쟁 구간(Contention Period - CP)
    경쟁구간에서는 DCF가 사용된다.
 */
static int build_cf_end_sample(AIRSHOOT_G *g, struct airconf *conf,
		uint8_t buf[], boolean isack)
{
	return 0;
}

static int build_auth_sample(AIRSHOOT_G *g, struct airconf *conf, uint8_t buf[])
{
	// 6 byte
	wi_auth_fixed *fix = (wi_auth_fixed*)buf;
#if 0
	uint16_t auth_algo[] = {
			0, // open system
			1, // shared key
	};
#endif
	fix->algorithm = 0;
	fix->seq = 0;
	fix->status = 0; // successful

	return sizeof(*fix);
}

static int build_deauth_sample(AIRSHOOT_G *g, struct airconf *conf,
		uint8_t buf[], uint16_t reason)
{
	wi_deauth_fixed *fix = (wi_deauth_fixed*)buf;
	uint16_t reasonlist[] = {
			IEEE80211_REASON_UNSPECIFIED, // 1
			IEEE80211_REASON_AUTH_EXPIRE, // 2
			IEEE80211_REASON_AUTH_LEAVE, // 3
			IEEE80211_REASON_ASSOC_EXPIRE, // 4
			IEEE80211_REASON_ASSOC_TOOMANY, // 5
			IEEE80211_REASON_NOT_AUTHED, // 6
			IEEE80211_REASON_NOT_ASSOCED,	// 7
			IEEE80211_REASON_ASSOC_LEAVE,	// 8
			IEEE80211_REASON_ASSOC_NOT_AUTHED, // 9

			IEEE80211_REASON_RSN_REQUIRED, // 11
			IEEE80211_REASON_RSN_INCONSISTENT, // 12
			IEEE80211_REASON_IE_INVALID, // 13
			IEEE80211_REASON_MIC_FAILURE,	// 14
	};

	if (reason == 0) {
		fix->reason = reasonlist[rand() % sizeof(reasonlist)];
	} else {
		fix->reason = reason;
	}
	return sizeof(*fix);
}

static int build_action_sample(AIRSHOOT_G *g, struct airconf *conf, uint8_t buf[])
{
	int len = 0;
	uint8_t category = 0;
	uint8_t action = 0;

	category = 3;
	action = 0;

	// High Throughput
	if (category == 7) {
		// category code: High Throughput
		buf[len++] = category;
		// action code: Spatial Multiplexing (SM) Power Save (1)
		buf[len++] = action;
		// Spatial Multiplexing (SM) Power Control
		buf[len++] = 0;
	}
	// for block ack
	else if (category == 3) {
		// category code: block ack
		buf[len++] = category;
		// action code: Add Block Ack Request (0)
		// action code: Add Block Ack Response (1)
		buf[len++] = action;
		// dialog token
		buf[len++] = 9;
		// Block Ack Request
		if (action == 0) {
			// block ack parameters
			memcpy(&buf[len], "\xc8\x00", 2);
			len += 2;
			// block ack timeout
			memcpy(&buf[len], "\x01\x00", 2);
			len += 2;
			// block ack starting sequence control
			memcpy(&buf[len], "\x00\x00", 2);
			len += 2;
		}
		// Block Ack Response
		else if (action == 1) {
			// status code: successful
			memcpy(&buf[len], "\x00\x00", 2);
			len += 2;
			// Block Ack Parameters
			memcpy(&buf[len], "\x03\x00", 2);
			len += 2;
			// Block Ack Timeout
			memcpy(&buf[len], "\x00\x00", 2);
			len += 2;
		}
	}

	return len;
}

/**
 * @brief build_block_ack_sample
 * @param buf Array packet buffer
 * @param isack Is ack packet?
 * @return write data length
 */
static int build_block_ack_sample(AIRSHOOT_G *g, struct airconf *conf,
		uint8_t buf[], boolean isack)
{
	wi_ctrl_ba_ctrl   *ba_ctrl = NULL;
	wi_ctrl_ba_ssc    *ba_scc = NULL;
	wi_ctrl_ba_bitmap *ba_bitmap = NULL;
	int len = 0;

	// Block Ack Request Control
	{
		ba_ctrl = (wi_ctrl_ba_ctrl*) (buf + len);
		memset(ba_ctrl, 0, sizeof(wi_ctrl_ba_ctrl));
		ba_ctrl->immediate_ack = rand() % 1;
		ba_ctrl->compress_bitmap = rand() % 1;
		len += sizeof(*ba_ctrl);
	}
	// Block Ack Starting Sequence Control (SSC)
	{
		ba_scc = (wi_ctrl_ba_ssc*) (buf + len);
		memset(ba_scc, 0, sizeof(wi_ctrl_ba_ssc));
		ba_scc->starting_seq = rand() % 0x0FFF;
		len += sizeof(*ba_scc);
	}
	if (isack) {
		return len;
	}

	// Block Ack Bitmap
	{
		ba_bitmap = (wi_ctrl_ba_bitmap*) (buf + len);
		memset(ba_bitmap, 0, sizeof(*ba_bitmap));
		len += sizeof(*ba_bitmap);
	}
	return len;
}

static void add_channel_tags(AIRSHOOT_G *g, struct airconf *conf, boolean HT)
{
	struct tagconf *tc = NULL;
	struct tagconf *last = NULL;

	last = tagconf_last(conf->tags);

	if (conf->channel == 0) {
		return;
	}

	// 2GHz
	if (conf->channel <= 14) {
		// DS Parameter set
		struct tagconf *channel_tag = tagconf_find(conf->tags, 3);
		if (channel_tag) {
			if (channel_tag->data == NULL) {
				channel_tag->data = (uint8_t*)malloc(1);
			}
			channel_tag->data[0] = conf->channel;
		} else {
			if (last) {
				last = tagconf_new(last, 3, 1, &conf->channel);
				// sort tags
				conf->tags = tagconf_sort(conf->tags);
				last = tagconf_last(conf->tags);
			} else {
				last = tagconf_new(NULL, 3, 1, &conf->channel);
				conf->tags = last;
			}
			//printf("append chaanel %d\n", channel);
		}
	}
	// 5GHz
	else { /* noop */ }

	// High Throughput - 802.11n
	if (HT) {
		// HT Information (802.11n D1.10)
		tc = tagconf_find(conf->tags, 61);
		if (tc) {
			if (tc->len > 0 && tc->data) {
				tc->data[0] = conf->channel;
			}
		} else {
			uint8_t ht_info[22] = {0, };
			ht_info[0] = conf->channel; // primary channel
			ht_info[1] = 0x0d; // HT Information Subset (1 of 3)
			ht_info[2] = 0x12; // HT Information Subset (2 of 3)
			ht_info[3] = 0x00;
			ht_info[4] = 0x00; // HT Information Subset (3 of 3)
			ht_info[5] = 0x00;
			//ht_info[6] ~ ht_info[21]; // RX Suuported MCS - set Basic MSC set

			if (last) {
				last = tagconf_new(last, 61, sizeof(ht_info), ht_info);
				// sort tags
				conf->tags = tagconf_sort(conf->tags);
				last = tagconf_last(conf->tags);
			} else {
				last = tagconf_new(NULL, 61, sizeof(ht_info), ht_info);
				conf->tags = last;
			}
		}
	}
}

static int build_reassoc_resp_sample(AIRSHOOT_G *g, struct airconf *conf, uint8_t buf[])
{
	static uint16_t assoc_id = 1;
	// 6 byte
	wi_reassoc_resp_fixed *fix = (wi_reassoc_resp_fixed*)buf;

	fix->capa.ess_capa = 1;
	fix->capa.privacy = conf->protect;
	fix->capa.preamble = 1;

	fix->status = 0;

	fix->assoc_id = assoc_id++;

	add_channel_tags(g, conf, TRUE);

	return sizeof(*fix);
}

static int build_assoc_resp_sample(AIRSHOOT_G *g, struct airconf *conf, uint8_t buf[])
{
	static uint16_t assoc_id = 1;
	// 6 byte
	wi_assoc_resp_fixed *fix = (wi_assoc_resp_fixed*)buf;

	fix->capa.ess_capa = 1;
	fix->capa.privacy = conf->protect;
	fix->capa.preamble = 1;

	fix->status = 0;

	fix->assoc_id = assoc_id++;

	add_channel_tags(g, conf, TRUE);

	return sizeof(*fix);
}

static int build_assoc_req_sample(AIRSHOOT_G *g, struct airconf *conf, uint8_t buf[])
{
	// 4 byte
	wi_assoc_req_fixed *fix = (wi_assoc_req_fixed*)buf;

	fix->capa.ess_capa = 1;
	fix->capa.privacy = conf->protect;
	fix->capa.preamble = 1;

	fix->listen_intv = htons(10);

	add_channel_tags(g, conf, TRUE);

	return sizeof(*fix);
}

static int build_reassoc_req_sample(AIRSHOOT_G *g, struct airconf *conf, uint8_t buf[])
{
	struct timeval tv;
	// 10 byte
	wi_reassoc_req_fixed *fix = (wi_reassoc_req_fixed*)buf;

	fix->capa.ess_capa = 1;
	fix->capa.privacy = conf->protect;
	fix->capa.preamble = 1;

	fix->listen_intv = htons(10);

	gettimeofday(&tv, NULL);
	fix->cur_ap[0] = 0;
	fix->cur_ap[1] = (tv.tv_sec) & 0xFF;
	fix->cur_ap[2] = (tv.tv_sec >> 8) & 0xFF;
	fix->cur_ap[3] = (tv.tv_sec >> 16) & 0xFF;
	fix->cur_ap[4] = (tv.tv_usec) & 0xFF;
	fix->cur_ap[5] = (tv.tv_usec >> 8) & 0xFF;

	add_channel_tags(g, conf, TRUE);

	return sizeof(*fix);
}

static int build_beacon_sample(AIRSHOOT_G *g, struct airconf *conf, uint8_t buf[])
{
	struct timeval tv;
	// 12 byte
	wi_bcn_fixed *fix = (wi_bcn_fixed*)buf;

	gettimeofday(&tv, NULL);

	fix->timestamp = (tv.tv_sec << 32) + tv.tv_usec;
	fix->intv = 0x0064;

	// Transmitter is an AP
	fix->capa.ess_capa = 1;
	// RSN
	if (tagconf_find(conf->tags, 48)) {
		fix->capa.privacy = 1;
	}
	// WPA
	if (tagconf_vendor_find(conf->tags, (uint8_t*)"\x00\x50\xf2")) {
		fix->capa.privacy = 1;
	}
	// HT Capability, HT Information
	if (tagconf_find(conf->tags, 45) ||
		tagconf_find(conf->tags, 61)) {
		fix->capa.preamble = 0;
	} else {
		fix->capa.preamble = 1;
	}

	// add tag for channel
	add_channel_tags(g, conf, TRUE);

	return sizeof(*fix);
}

static int build_taglist(uint8_t* buf, struct tagconf *first)
{
	wi_tag *pT = NULL;
	struct tagconf *next = first;
	int len = 0;

//	print_tagdump(first);

	while (next) {
		pT = (wi_tag*) (buf + len);

		pT->tag = next->tag;
		pT->len = next->len;

		if (next->data == NULL) {
			pT->len = 0;
		} else {
			memcpy(pT->data, next->data, pT->len);
		}

		// header + data length
		len += sizeof(wi_tag) + pT->len;

		next = next->next;
	}
	return len;
}

/**
 * @brief debug_airshooter_packet
 */
void airshoot_summary_print(uint8_t *h80211, size_t h80211_len)
{
	wi_hdr *h = (wi_hdr *) h80211;
	const char* type_str = NULL;
	const char* subtype_str = NULL;
	const char *ds_str = NULL;
	char buffer[1024];
	size_t len = 0;

	if (!valid_h80211_frame(h->fc.type)) {
		cle("[airshooter pacekt] incorrect frame %d", h->fc.type);
		return;
	}

	type_str = h80211_frame_string[h->fc.type];
	switch (h->fc.type)
	{
	case frame_mgnt:
		if (!valid_h80211_mgnt(h->fc.subtype)) {
			cle("[airshooter pacekt] incorrect mgnt frame %d", h->fc.subtype);
			return;
		}
		subtype_str = h80211_mgnt_string[h->fc.subtype];
		break;
	case frame_ctrl:
		if (!valid_h80211_ctrl(h->fc.subtype)) {
			cle("[airshooter pacekt] incorrect ctrl frame %d", h->fc.subtype);
			return;
		}
		subtype_str = h80211_ctrl_string[h->fc.subtype];
		break;
	case frame_data:
		if (!valid_h80211_data(h->fc.subtype)) {
			cle("[airshooter pacekt] incorrect data frame %d", h->fc.subtype);
			return;
		}
		subtype_str = h80211_data_string[h->fc.subtype];
		break;
	}

	len += snprintf(buffer+len, sizeof(buffer)-len, "[%s|%s]", type_str, subtype_str);
	if (h->fc.flags.fromds == 1 &&
		h->fc.flags.tods == 1) {
		ds_str = "WDS";
	} else if (h->fc.flags.fromds == 1) {
		ds_str = "AP->STA";
	} else if (h->fc.flags.tods == 1) {
		ds_str = "STA->AP";
	} else {
		ds_str = "ADHOC";
	}
	len += snprintf(buffer+len, sizeof(buffer)-len, "(%s)", ds_str);

	len += snprintf(buffer+len, sizeof(buffer)-len,
			"addr1:"_MAC_FMT_" "
			"addr2:"_MAC_FMT_" "
			"addr3:"_MAC_FMT_" "
			"len:%ld",
			_MAC_FMT_FILL_(h->addr1),
			_MAC_FMT_FILL_(h->addr2),
			_MAC_FMT_FILL_(h->addr3),
			h80211_len);
	cld("%s", buffer);

	vhex(buffer, sizeof(buffer), h80211, h80211_len);
	cld("%s", buffer);
}

/**
 * @brief shooter_one_airconf
 * @param g
 * @param conf
 * @return -1: build failure, else length of build packet
 */
int airshoot_build_packet(AIRSHOOT_G *g, struct airconf *conf)
{
	uint8_t *shootbuf = &conf->pktbuf[0];
	wi_hdr *pH = (wi_hdr*)(shootbuf);
	wi_ctrl_hdr *cpH = NULL;
	int len = 0;

	pH->fc.version = conf->version;
	pH->fc.type    = conf->type;
	pH->fc.subtype = conf->subtype;
	pH->duration   = conf->duration;
	pH->fc.flags.protect
	               = conf->protect;

	if (conf->protect) {
		pH->fc.flags.fromds = conf->fromds;
		pH->fc.flags.tods   = conf->tods;
	}

	switch (pH->fc.type)
	{
    /**
      * [관리 MAC 프레임 주소 필드 정의]
      *                                 4              10             16
      * +--------------+--------------+--------------+--------------+--------------+--------------+
      * | To DS(AP)    | From DS(AP)  |   Address1   |  Address2    |   Address3   |   Address4   |
      * +--------------+--------------+--------------+--------------+--------------+--------------+
      * |      0       |     0        |    DA        |      SA      |    BSSID     |              |
      * +--------------+--------------+--------------+--------------+--------------+--------------+
      * |           probe-req         ff:ff:ff:ff:ff:ff     sta     ff:ff:ff:ff:ff:ff             |
      * |           assoc-req              bssid            sta          bssid                    |
      * |        re-assoc-req              bssid            sta          bssid                    |
      * |            beacon           ff:ff:ff:ff:ff:ff    bssid         bssid                    |
      * |           probe-resp              sta            bssid         bssid                    |
      * |           assoc-resp              sta            bssid         bssid                    |
      * |        re-assoc-resp              sta            bssid         bssid                    |
      * |       authentication           sta (bssid)    bssid (Sta)      bssid                    |
      * |      de-authentication         sta (bssid)    bssid (Sta)      bssid                    |
      * |           action               sta (bssid)    bssid (Sta)      bssid                    |
      * +--------------+--------------+--------------+--------------+--------------+--------------+
      * |            ADHOC                 DA               SA           BSSID                    |
      * +--------------+--------------+--------------+--------------+--------------+--------------+
      *                                 4              10             16
      */
		case frame_mgnt:
		{
			addrconf_3 *addr = &conf->addr3;

			len = sizeof(wi_hdr);

			switch (pH->fc.subtype)
			{
			case mgnt_probe_req:
				if (cv_is_enable(conf->sta_addr)) {
					mac_cpy_bcast(pH->addr1);
					mac_cpy(pH->addr2, conf->sta_addr.sta);
					mac_cpy_bcast(pH->addr3);
				}
				else {
					mac_cpy(pH->addr1, addr->da);
					mac_cpy(pH->addr2, addr->sa);
					mac_cpy(pH->addr3, addr->bssid);
				}
				break;
			case mgnt_assoc_req:
			case mgnt_reassoc_req:
				if (cv_is_enable(conf->ap_addr) &&
						cv_is_enable(conf->sta_addr)
				) {
					mac_cpy(pH->addr1, conf->ap_addr.ap);
					mac_cpy(pH->addr2, conf->sta_addr.sta);
					mac_cpy(pH->addr3, conf->ap_addr.ap);
				}
				else {
					mac_cpy(pH->addr1, addr->da);
					mac_cpy(pH->addr2, addr->sa);
					mac_cpy(pH->addr3, addr->bssid);
				}
				break;
			case mgnt_probe_resp:
			case mgnt_assoc_resp:
			case mgnt_reassoc_resp:
				if (cv_is_enable(conf->ap_addr) && cv_is_enable(conf->sta_addr)) {
					mac_cpy(pH->addr1, conf->sta_addr.sta);
					mac_cpy(pH->addr2, conf->ap_addr.ap);
					mac_cpy(pH->addr3, conf->ap_addr.ap);
				}
				else {
					mac_cpy(pH->addr1, addr->da);
					mac_cpy(pH->addr2, addr->sa);
					mac_cpy(pH->addr3, addr->bssid);
				}
				break;
			case mgnt_atim:
			case mgnt_auth:
			case mgnt_disassoc:
			case mgnt_deauth:
			case mgnt_action:
				mac_cpy(pH->addr1, addr->da);
				mac_cpy(pH->addr2, addr->sa);
				mac_cpy(pH->addr3, addr->bssid);
				break;
			case mgnt_beacon:
				if (cv_is_enable(conf->ap_addr)) {
					mac_cpy_bcast(pH->addr1);
					mac_cpy(pH->addr2, conf->ap_addr.ap);
					mac_cpy(pH->addr3, conf->ap_addr.ap);
				}
				else {
					mac_cpy_bcast(pH->addr1);
					mac_cpy(pH->addr2, addr->bssid);
					mac_cpy(pH->addr3, addr->bssid);
				}
				break;
			default:
				cle("Not support management subtype: %d", pH->fc.subtype);
				return -1;
			}

			switch (pH->fc.subtype)
			{
			case mgnt_assoc_req:
				// mgnt_assoc_req = 0, fixed = wi_assoc_req_fixed (4 byte)
				len += build_assoc_req_sample(g, conf, shootbuf+len);
				break;
			case mgnt_assoc_resp:
				// mgnt_assoc_resp = 1, fixed = wi_assoc_resp_fixed (6 byte)
				len += build_assoc_resp_sample(g, conf, shootbuf+len);
				break;
			case mgnt_reassoc_req:
				// mgnt_reassoc_req = 2, fixed = wi_reassoc_req_fixed (10 byte)
				len += build_reassoc_req_sample(g, conf, shootbuf+len);
				break;
			case mgnt_reassoc_resp:
				// mgnt_reassoc_resp = 3, fixed = wi_reassoc_resp_fixed (6 byte)
				len += build_reassoc_resp_sample(g, conf, shootbuf+len);
				break;
			case mgnt_probe_req:
				// mgnt_probe_req = 4, fixed = 0
				break;
			case mgnt_probe_resp:
				// mgnt_probe_resp = 5, fixed = wi_beacon_fixed (12 byte)
				len += build_beacon_sample(g, conf, shootbuf+len);
				break;

			case mgnt_beacon:
				// mgnt_beacon = 8, fixed = wi_beacon_fixed (12 byte)
				len += build_beacon_sample(g, conf, shootbuf+len);
				break;
			case mgnt_atim:
				// mgnt_atim = 9, fixed = 0
				len += build_data_sample(g, conf, shootbuf+len, FALSE/*isqos*/, TRUE/*isdata*/);
				break;
			case mgnt_disassoc:
				// mgnt_disassoc = 10, fixed = wi_deauth_fixed (2 byte)
				len += build_deauth_sample(g, conf, shootbuf+len, conf->deauth_reason);
				break;
			case mgnt_auth:
				// mgnt_auth = 11, fixed = wi_auth_fixed (6 byte)
				len += build_auth_sample(g, conf, shootbuf+len);
				break;
			case mgnt_deauth:
				// mgnt_deauth = 12, fixed = wi_deauth_fixed (2 byte)
				len += build_deauth_sample(g, conf, shootbuf+len, conf->deauth_reason);
				break;
			case mgnt_action:
				// mgnt_action = 13, fixed = .....
				len += build_action_sample(g, conf, shootbuf+len);
				break;
			}

			// add tag list
			len += build_taglist(shootbuf+len, conf->tags);
		}
		break;

		case frame_ctrl:
		{
			len = sizeof(wi_ctrl_hdr);
			cpH = (wi_ctrl_hdr*)shootbuf;

			// 1: STA will go to sleep
			// 0: STA will stay up
			cpH->fc.flags.pwr_mgt = conf->pwr_sleep ? 1 : 0;

			switch (cpH->fc.subtype)
			{
			case ctrl_block_ack_req:
			case ctrl_block_ack:
			case ctrl_rts:
			case ctrl_cf_end: // addr3
				mac_cpy(cpH->addr1, conf->addr3.da);
				mac_cpy(cpH->addr2, conf->addr3.sa);
				mac_cpy(((uint8_t*)cpH->addr2)+6, conf->addr3.bssid);
				len += 12;
				break;
			case ctrl_cf_end_ack:
			case ctrl_ps_poll:
				mac_cpy(cpH->addr1, conf->addr2.da);
				mac_cpy(cpH->addr2, conf->addr2.sa);
				len += 6;
				break;
			case ctrl_cts:
			case ctrl_ack:
				mac_cpy(cpH->addr1, conf->addr1.da);
				break;
			default:
				cle("Not support control subtype: %d", pH->fc.subtype);
				return -1;
			}

			// add control data
			switch (cpH->fc.subtype)
			{
			case ctrl_block_ack:
				len += build_block_ack_sample(g, conf, shootbuf+len, FALSE);
				break;
			case ctrl_block_ack_req:
				len += build_block_ack_sample(g, conf, shootbuf+len, TRUE);
				break;
			case ctrl_cf_end:
				len += build_cf_end_sample(g, conf, shootbuf+len, FALSE);
				break;
			case ctrl_cf_end_ack:
				len += build_cf_end_sample(g, conf, shootbuf+len, TRUE);
				break;
			case ctrl_ps_poll:
				len += build_pspoll_sample(g, conf, shootbuf+len);
				break;
			}

			len += build_data_sample(g, conf, shootbuf+len, FALSE, FALSE);
		}
		break;

		case frame_data:
		{
			len = sizeof(wi_hdr);
			// 4 address
			if ((conf->fromds == 1) && (conf->tods == 1)) {
				addrconf_4 *addr = &conf->addr4;
				mac_cpy(pH->addr1, addr->ra);
				mac_cpy(pH->addr2, addr->ta);
				mac_cpy(pH->addr3, addr->da);
				mac_cpy(pH->addr4, addr->sa);
				len += 6;
			}
			// AP -> STA
			else if (conf->fromds == 1) {
				addrconf_3 *addr = &conf->addr3;
				mac_cpy(pH->addr1, addr->da);
				mac_cpy(pH->addr2, addr->bssid);
				mac_cpy(pH->addr3, addr->sa);
			}
			// STA -> AP
			else if (conf->tods == 1) {
				addrconf_3 *addr = &conf->addr3;
				mac_cpy(pH->addr1, addr->bssid);
				mac_cpy(pH->addr2, addr->sa);
				mac_cpy(pH->addr3, addr->da);
			}
			// ADHOC
			else {
				addrconf_3 *addr = &conf->addr3;
				mac_cpy(pH->addr3, addr->bssid);
				mac_cpy(pH->addr1, addr->da);
				mac_cpy(pH->addr2, addr->sa);
			}

			boolean is_nodata = (pH->fc.subtype & 0x04);
			boolean is_qos    = (pH->fc.subtype & 0x08);

			// add data
			len += build_data_sample(g, conf, shootbuf+len, is_qos, !is_nodata);
		}
		break;
	}
	return len;
}

/**
 * @brief airshoot_one_shoot
 * @param g
 * @param conf
 * @param handler after shoot, call handler function
 * @return 0: success, -1: failure
 */
static int airshoot_one_shoot(AIRSHOOT_G *g, struct airconf *conf, airshoot_handler_t handler)
{
	int ret = -1;
	struct airshoot_result result;
	static uint32_t seq = 0;
	wi_hdr *pH = (wi_hdr*)(conf->pktbuf);
	struct timeval now;

	if (pH->fc.type != frame_ctrl) {
		if (seq == 0x0FFF) {
			seq = 0;
		}
		pH->frag_seq.seq_number = (seq++) & 0x0FFF;
	}

	result.conf = conf;

	if (g->do_exit) {
		goto cleanup;
	}
	if (g->shooter_timer > 0) {
		gettimeofday(&now, NULL);
		if ((now.tv_sec - g->shooter_start.tv_sec) > g->shooter_timer) {
			goto cleanup;
		}
	}

	if (wi_set_channel(g->wi, conf->channel)) {
		cle("failed '%s' wi_set_channel to %d", wi_get_ifname(g->wi), conf->channel);
		ret = -1;
		goto cleanup;
	}

retry_send:

	ret = wi_write(g->wi, conf->pktbuf, conf->pktbuf_len, NULL);

	// fail shoot
	if (ret == -1) {
		switch (errno)
		{
		case EAGAIN:
		case ENOBUFS:
			goto retry_send;
		default:
			cle("failed write packet - %s\n", strerror(errno));
			goto cleanup;
		}
	}
	else {
		gettimeofday(&result.shoot_tv, NULL);
		conf->shooter_tv = result.shoot_tv;

		// call result handler
		if (handler) {
			handler(g, (void*)&result);
		}
		if (g->debug) {
			airshoot_summary_print(conf->pktbuf, conf->pktbuf_len);
		}
	}

cleanup:
	return ret;
}

/**
 * @brief shooter_process
 */
int airshoot_shoot(AIRSHOOT_G *g, airshoot_handler_t handler)
{
	struct timeval now;
	struct timeval diff;
	struct airconf *conf = NULL;
	struct timeval timeout;
	uint32_t min_timer;

#if 0
	// 최소 dwell time(msec) 에서 airconf 수 를 나눈 시간이 sleep하는 시간이다
	min_timer = (min_dwell_time(g->conf) * 1000) / airconf_count(g->conf);
	if (min_timer < 10) {
		min_timer = 10;
	}
#else
	min_timer = 1000; // 1 msec
#endif

	// shoot할 패킷을 미리 만들어 놓는다.
	for (conf = g->conf; conf; conf = conf->next) {
		if (conf->enable) {
			conf->pktbuf_len = airshoot_build_packet(g, conf);
			if (conf->pktbuf_len == -1) {
				return -1;
			}
		}
	}

	gettimeofday(&g->shooter_start, NULL);

	while (1) {
		if (g->do_exit) {
			break;
		}

		gettimeofday(&now, NULL);

		if (g->shooter_timer > 0) {
			if ((now.tv_sec - g->shooter_start.tv_sec) > g->shooter_timer) {
				return 0;
			}
		}
		for (conf = g->conf; conf; conf = conf->next) {
			if (g->do_exit) {
				return 0;
			}

			if (conf->enable) {
				// check first shoot delay time
				if (conf->delay > 0) {
					if (is_unset_time(&conf->shooter_tv)) {
						diff = diff_timeval(&g->shooter_start, &now);
						if (usec2msec(&diff) < conf->delay) {
							continue;
						}
					}
				}

				// check send interval (dwell)
				if (!is_unset_time(&conf->shooter_tv)) {
					diff = diff_timeval(&conf->shooter_tv, &now);
					if (usec2msec(&diff) < conf->dwell) {
						continue;
					}
				}

				airshoot_one_shoot(g, conf, handler);
			}

			// sleep
			{
				timeout.tv_sec = 0;
				timeout.tv_usec = min_timer;
				select(0 ,NULL, NULL, NULL, &timeout);
			}
		}
	}
	return 0;
}

/**
 * @brief
 */
int airshoot_conf_verify(AIRSHOOT_G *g)
{
	llist *files = NULL;
	llist *cur = NULL;
	const char* filename = NULL;

	// verify files in directory
	if (g->confdir[0]) {
		// files is
		airconf_filelist(&files, g->confdir);

		for (cur = files; cur; cur = cur->next) {
			filename = (const char*)cur->obj;

			g->conf = airconf_from_file(filename, g->debug);
			if (g->conf) {
				cli("[GOOD] verify - %s", filename);
				airconf_free(g->conf);
				g->conf = NULL;
			}
			else {
				cle("[BAD] verify - %s", filename);
			}
		}
		delall_list(files, LLIST_OBJ_FREE);
	}

	// verify single file
	if (g->conffile[0]) {
		g->conf = airconf_from_file(g->conffile, g->debug);
		if (g->conf) {
			cli("[GOOD] verify - %s", g->conffile);
			airconf_free(g->conf);
			g->conf = NULL;
		}
		else {
			cle("[BAD] verify - %s", g->conffile);
		}
	}
	return 0;
}

/**
 * @brief init_airshooter_g
 */
int airshoot_g_init(AIRSHOOT_G* g)
{
	console_log_disable();
	memset(g, 0, sizeof(*g));
	return 0;
}

/**
 * @brief free_airshooter_g
 */
void airshoot_g_free(AIRSHOOT_G* g)
{
	if (g->conf) {
		airconf_free(g->conf);
		g->conf = NULL;
	}
	close_wifi(g->wi);
}

