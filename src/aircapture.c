#include "aircapture.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <termios.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <assert.h>

#include "pcap.h"
#include "h80211_types.h"
#include "ieee802_11_defs.h"
#include "aircapture.h"
#include "airsock.h"


#if 0
/**
 * @brief is_extended_iv
 */
static int is_extended_iv(uint8_t buf[])
{
	/*
byte                  4                    4                      0-2304             8        4      4(CRC)
    +------------------------------+-----------------------+----------------+--------------+-----+ -----+
    |            IV / Key ID       |     Extended IV       |  MSDU Payload  | MIC (M0~M7)  | ICV |  FCS |
    +------------------------------+-----------------------+----------------+--------------+-----+ -----+
bit | 8  | 8  | 8  |   5   | 1 | 2 |  8  |  8  |  8  |  8  |
    |TSC1|WEP |TSCO|Reserve|EXT|KEY| TSC2| TSC3| TSC4| TSC5|<----------- Encrypted -------------->
    |    |seed|    |       |IV |ID |

               buf[0]  buf[1]    buf[2]  buf[3]
	EXT IV = xxxxxxxx|xxxxxxxx|xxxxxxxx|00000100
	                                     buf[3] & 0x20 == 1 ?

	 */
	return (buf[3] & 0x20);
}
#endif

/**
 * @brief test matching only one airconf
 */
static int aircap_matching_conf(struct airconf *conf, uint8_t *h80211, int h80211_len, struct rx_info *ri)
{
	int len = 0;
	wi_hdr *pH = NULL; // 80211 header
	wi_ctrl_hdr *pcH = NULL; // 80211 control header
	struct tagconf *cur_tag = NULL;
	size_t left = 0;
	const uint8_t *pos = NULL;
	uint8_t id, elen;
	int tag_elements = 0;
	int data_sign_len = 0;
	uint8_t *p_data = NULL;
	int i;
	int matched = 0;
	int tagable = 0;


	pH  = (wi_hdr *)    h80211;
	pcH = (wi_ctrl_hdr*)h80211;

	// check version
	if (cv_is_enable(conf->version)) {
		if (conf->type != pH->fc.version) {
			return -1;
		}
	}
	// check type
	if (cv_is_enable(conf->type)) {
		if (conf->type != pH->fc.type) {
			return -1;
		}
	}
	// check subtype
	if (cv_is_enable(conf->subtype)) {
		if (conf->subtype != pH->fc.subtype) {
			return -1;
		}
	}
	// check ToDS
	if (cv_is_enable(conf->tods)) {
		if (conf->tods != pH->fc.flags.tods) {
			return -1;
		}
	}
	// check FromDS
	if (cv_is_enable(conf->fromds)) {
		if (conf->fromds != pH->fc.flags.fromds) {
			return -1;
		}
	}
	// check protected of flags
	if (cv_is_enable(conf->protect)) {
		if (conf->protect != pH->fc.flags.protect) {
			return -1;
		}
	}
	// check pwr sleep of flags
	if (cv_is_enable(conf->pwr_sleep)) {
		if (conf->pwr_sleep != pH->fc.flags.pwr_mgt) {
			return -1;
		}
	}

	/**
	 * [ADHOC MAC 프레임 주소 필드 정의]
	 *                                 4              10             16
	 * +--------------+--------------+--------------+--------------+--------------+--------------+
	 * | To DS(AP)    | From DS(AP)  |   Address1   |  Address2    |   Address3   |   Address4   |
	 * +--------------+--------------+--------------+--------------+--------------+--------------+
	 * |      0       |     0        |    DA(STA)   |    SA(STA)   |    BSSID(AP) |              | ADHOC
	 * +--------------+--------------+--------------+--------------+--------------+--------------+
	 * |      0       |     1        |    DA(STA)   |   BSSID(AP)  |    SA        |              | From AP
	 * +--------------+--------------+--------------+--------------+--------------+--------------+
	 * |      1       |     0        |   BSSID(AP)  |   SA(STA)    |    DA        |              | To AP
	 * +--------------+--------------+--------------+--------------+--------------+--------------+
	 * |      1       |     1        |   RA(rx AP)  |   TA(tx AP)  |  DA(rx STA)  |  SA(tx STA)  | WDS
	 * +--------------+--------------+--------------+--------------+--------------+--------------+
	 *                                 4              10             16
	 */

	switch (pH->fc.type)
	{
	case frame_mgnt:
		len = sizeof(wi_hdr);

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

		/*
		 * management frame check address
		 */
		// check address
		if (cv_is_enable(conf->addr1)) {
			if (!mac_cmp(pH->addr1, conf->addr1.da)) return -1;
		} else if (cv_is_enable(conf->addr2)) {
			if (!mac_cmp(pH->addr1, conf->addr2.da)) return -1;
			if (!mac_cmp(pH->addr2, conf->addr2.sa)) return -1;
		} else if (cv_is_enable(conf->addr3)) {
			if (!mac_cmp(pH->addr1, conf->addr3.da)) return -1;
			if (!mac_cmp(pH->addr2, conf->addr3.sa)) return -1;
			if (!mac_cmp(pH->addr3, conf->addr3.bssid)) return -1;
		} else {
			// check any address
			if (cv_is_enable(conf->any_addr)) {
				if (!mac_cmp(pH->addr1, conf->any_addr.addr)
						&&
						!mac_cmp(pH->addr2, conf->any_addr.addr)
						&&
						!mac_cmp(pH->addr3, conf->any_addr.addr)) return -1;
			} else {
				// check ap address
				if (cv_is_enable(conf->ap_addr)) {
					if (pH->fc.subtype != mgnt_probe_req) {
						if (!mac_cmp(pH->addr3, conf->ap_addr.ap)) return -1;
					}
				}
				// check sta address
				if (cv_is_enable(conf->sta_addr)) {
					switch (pH->fc.subtype)
					{
					case mgnt_beacon: // not exist sta
						break;
					case mgnt_probe_req:
					case mgnt_assoc_req:
					case mgnt_reassoc_req:
						if (!mac_cmp(pH->addr2, conf->sta_addr.sta)) return -1;
						break;
					case mgnt_probe_resp:
					case mgnt_assoc_resp:
					case mgnt_reassoc_resp:
						if (!mac_cmp(pH->addr1, conf->sta_addr.sta)) return -1;
						break;
					case mgnt_auth:
					case mgnt_deauth:
					case mgnt_action:
						if (mac_cmp(pH->addr1, pH->addr3)) {
							if (!mac_cmp(pH->addr2, conf->sta_addr.sta)) return -1;
						}
						else
						if (mac_cmp(pH->addr2, pH->addr3)) {
							if (!mac_cmp(pH->addr1, conf->sta_addr.sta)) return -1;
						}
					}
				}
			}
		}

		// check deauth reason code when catch deauthentication packet
		if (cv_is_enable(conf->deauth_reason)) {
			if (pH->fc.subtype == mgnt_deauth) {
				wi_deauth_fixed *fix = (wi_deauth_fixed *)(h80211 + len);
				if (fix->reason != conf->deauth_reason) return -1;
			}
		}

		/*
		 * check length of fixed parameter for tag list
		 */
		switch (pH->fc.subtype)
		{
		case mgnt_assoc_req:
			// mgnt_assoc_req = 0, fixed = wi_assoc_req_fixed (4 byte)
			len += sizeof(wi_assoc_req_fixed);
			tagable = 1;
			break;
		case mgnt_assoc_resp:
			// mgnt_assoc_resp = 1, fixed = wi_assoc_resp_fixed (6 byte)
			len += sizeof(wi_assoc_resp_fixed);
			tagable = 1;
			break;
		case mgnt_reassoc_req:
			// mgnt_reassoc_req = 2, fixed = wi_reassoc_req_fixed (10 byte)
			len += sizeof(wi_reassoc_req_fixed);
			tagable = 1;
			break;
		case mgnt_reassoc_resp:
			// mgnt_reassoc_resp = 3, fixed = wi_reassoc_resp_fixed (6 byte)
			len += sizeof(wi_reassoc_resp_fixed);
			tagable = 1;
			break;
		case mgnt_probe_req:
			// mgnt_probe_req = 4, fixed = 0
			tagable = 1;
			break;
		case mgnt_probe_resp:
			// mgnt_probe_resp = 5, fixed = wi_beacon_fixed (12 byte)
			len += sizeof(wi_bcn_fixed);
			tagable = 1;
			break;
		case mgnt_beacon:
			// mgnt_beacon = 8, fixed = wi_beacon_fixed (12 byte)
			len += sizeof(wi_bcn_fixed);
			tagable = 1;
			break;
		case mgnt_atim:
			// mgnt_atim = 9, fixed = 0
			break;
		case mgnt_disassoc:
			// mgnt_disassoc = 10, fixed = wi_deauth_fixed (2 byte)
			len += sizeof(wi_deauth_fixed);
			break;
		case mgnt_auth:
			// mgnt_auth = 11, fixed = wi_auth_fixed (6 byte)
			len += sizeof(wi_auth_fixed);
			tagable = 1;
			break;
		case mgnt_deauth:
			// mgnt_deauth = 12, fixed = wi_deauth_fixed (2 byte)
			len += sizeof(wi_deauth_fixed);
			break;
		case mgnt_action:
			// mgnt_action = 13, fixed = .....
			break;
		}
		break;


	case frame_ctrl:
		len = sizeof(wi_ctrl_hdr);

		switch (pH->fc.subtype)
		{
		// 2-address packets
		case ctrl_block_ack_req:
		case ctrl_block_ack:
		case ctrl_ps_poll:
		case ctrl_rts:
		case ctrl_cf_end:
		case ctrl_cf_end_ack:
			// check address
			if (cv_is_enable(conf->addr1)) {
				if (!mac_cmp(pcH->addr1, conf->addr1.da)) return -1;
			} else if (cv_is_enable(conf->addr2)) {
				if (!mac_cmp(pcH->addr1, conf->addr2.da)) return -1;
				if (!mac_cmp(pcH->addr2, conf->addr2.sa)) return -1;
			} else if (cv_is_enable(conf->addr3)) {
				if (!mac_cmp(pcH->addr1, conf->addr3.da)) return -1;
				if (!mac_cmp(pcH->addr2, conf->addr3.sa)) return -1;
				// TODO: mismatch address conf address and packet address
				// check address3 but this packet is 2-address
/*
				if (!mac_cmp(((uint8_t*)pcH->addr2)+6, conf->addr3.bssid)) return -1;
*/
			} else {
				// check any address
				if (cv_is_enable(conf->any_addr)) {
					if (!mac_cmp(pcH->addr1, conf->any_addr.addr)
							&&
							!mac_cmp(pcH->addr2, conf->any_addr.addr)) return -1;
				} else {
					// check ap address
					if (cv_is_enable(conf->ap_addr)) {
						if (!mac_cmp(pcH->addr1, conf->ap_addr.ap)
								&&
								!mac_cmp(pcH->addr2, conf->ap_addr.ap)) return -1;
					}
					// check sta address
					if (cv_is_enable(conf->sta_addr)) {
						if (!mac_cmp(pcH->addr1, conf->sta_addr.sta)
								&&
								!mac_cmp(pcH->addr2, conf->sta_addr.sta)) return -1;
					}
				}
			}
			break;
		// 1-address packets
		case ctrl_cts:
		case ctrl_ack:
			// check address
			if (cv_is_enable(conf->addr1)) {
				if (!mac_cmp(pcH->addr1, conf->addr1.da)) return -1;
			} else if (cv_is_enable(conf->addr2)) {
				if (!mac_cmp(pcH->addr1, conf->addr2.da)) return -1;
			} else if (cv_is_enable(conf->addr3)) {
				if (!mac_cmp(pcH->addr1, conf->addr3.da)) return -1;
			} else {
				// check any address
				if (cv_is_enable(conf->any_addr)) {
					if (!mac_cmp(pcH->addr1, conf->any_addr.addr)) return -1;
				} else {
					// check ap address
					if (cv_is_enable(conf->ap_addr)) {
						if (!mac_cmp(pcH->addr1, conf->ap_addr.ap)) return -1;
					}
					// check sta address
					if (cv_is_enable(conf->sta_addr)) {
						if (!mac_cmp(pcH->addr1, conf->sta_addr.sta)) return -1;
					}
				}
			}
			break;
		}
		break;

	case frame_data:
		len = sizeof(wi_hdr);
		/*
		 * data frame check address for ADHOC
		 */
		if ((pH->fc.flags.fromds == 0) && (pH->fc.flags.tods == 0)) {
			// check address
			if (cv_is_enable(conf->addr1)) {
				if (!mac_cmp(pH->addr1, conf->addr1.da)) return -1;
			} else if (cv_is_enable(conf->addr2)) {
				if (!mac_cmp(pH->addr1, conf->addr2.da)) return -1;
				if (!mac_cmp(pH->addr2, conf->addr2.sa)) return -1;
			} else if (cv_is_enable(conf->addr3)) {
				if (!mac_cmp(pH->addr1, conf->addr3.da)) return -1;
				if (!mac_cmp(pH->addr2, conf->addr3.sa)) return -1;
				if (!mac_cmp(pH->addr3, conf->addr3.bssid)) return -1;
			} else {
				// check any address
				if (cv_is_enable(conf->any_addr)) {
					if (!mac_cmp(pH->addr1, conf->any_addr.addr)
							&&
							!mac_cmp(pH->addr2, conf->any_addr.addr)
							&&
							!mac_cmp(pH->addr3, conf->any_addr.addr)) return -1;
				} else {
					// check ap address
					if (cv_is_enable(conf->ap_addr)) {
						if (!mac_cmp(pH->addr3, conf->ap_addr.ap)) return -1;
					}
					// check sta address
					if (cv_is_enable(conf->sta_addr)) {
						// address1 and address2 is STA node
						if (!mac_cmp(pH->addr1, conf->sta_addr.sta)
								&&
								!mac_cmp(pH->addr2, conf->sta_addr.sta)) return -1;
					}
				}
			}
		}
		/*
		 * data frame check address for WDS
		 */
		else if ((pH->fc.flags.fromds == 1) && (pH->fc.flags.tods == 1)) {
			// check address
			if (cv_is_enable(conf->addr1)) {
				if (!mac_cmp(pH->addr3, conf->addr1.da)) return -1;
			} else if (cv_is_enable(conf->addr2)) {
				if (!mac_cmp(pH->addr3, conf->addr2.da)) return -1;
				if (!mac_cmp(pH->addr4, conf->addr2.sa)) return -1;
			} else if (cv_is_enable(conf->addr4)) {
				if (!mac_cmp(pH->addr1, conf->addr4.ra)) return -1;
				if (!mac_cmp(pH->addr2, conf->addr4.ta)) return -1;
				if (!mac_cmp(pH->addr3, conf->addr4.da)) return -1;
				if (!mac_cmp(pH->addr4, conf->addr4.sa)) return -1;
			} else {
				// check any address
				if (cv_is_enable(conf->any_addr)) {
					if (!mac_cmp(pH->addr1, conf->any_addr.addr)
							&&
							!mac_cmp(pH->addr2, conf->any_addr.addr)
							&&
							!mac_cmp(pH->addr3, conf->any_addr.addr)
							&&
							!mac_cmp(pH->addr4, conf->any_addr.addr)) return -1;
				} else {
					// check ap address
					// RA/TA is AP
					if (cv_is_enable(conf->ap_addr)) {
						if (!mac_cmp(pH->addr1, conf->ap_addr.ap)
								&&
								!mac_cmp(pH->addr2, conf->ap_addr.ap)) return -1;
					}
					// check sta address
					// DA/SA is STA
					if (cv_is_enable(conf->sta_addr)) {
						if (!mac_cmp(pH->addr3, conf->sta_addr.sta)
								&&
								!mac_cmp(pH->addr4, conf->sta_addr.sta)) return -1;
					}
				}
			}
			len += 6; // for address 4
		}
		/*
		 * data frame check address for To AP
		 */
		else if (pH->fc.flags.tods == 1) {
			// check address
			if (cv_is_enable(conf->addr1)) {
				if (!mac_cmp(pH->addr3, conf->addr1.da)) return -1;
			} else if (cv_is_enable(conf->addr2)) {
				if (!mac_cmp(pH->addr3, conf->addr2.da)) return -1;
				if (!mac_cmp(pH->addr2, conf->addr2.sa)) return -1;
			} else if (cv_is_enable(conf->addr3)) {
				if (!mac_cmp(pH->addr3, conf->addr3.da)) return -1;
				if (!mac_cmp(pH->addr2, conf->addr3.sa)) return -1;
				if (!mac_cmp(pH->addr1, conf->addr3.bssid)) return -1;
			} else {
				// check any address
				if (cv_is_enable(conf->any_addr)) {
					if (!mac_cmp(pH->addr1, conf->any_addr.addr)
							&&
							!mac_cmp(pH->addr2, conf->any_addr.addr)
							&&
							!mac_cmp(pH->addr3, conf->any_addr.addr)) return -1;
				} else {
					// check ap address
					if (cv_is_enable(conf->ap_addr)) {
						if (!mac_cmp(pH->addr1, conf->ap_addr.ap)) return -1;
					}
					// check sta address
					if (cv_is_enable(conf->sta_addr)) {
						// src, dst address is wireless STA
						if (!mac_cmp(pH->addr2, conf->sta_addr.sta)
								&&
								!mac_cmp(pH->addr3, conf->sta_addr.sta)) return -1;
					}
				}
			}
		}
		/*
		 * data frame check address for From AP
		 */
		else if (pH->fc.flags.fromds == 1) {
			// check address
			if (cv_is_enable(conf->addr1)) {
				if (!mac_cmp(pH->addr1, conf->addr1.da)) return -1;
			} else if (cv_is_enable(conf->addr3)) {
				if (!mac_cmp(pH->addr3, conf->addr2.sa)) return -1;
				if (!mac_cmp(pH->addr1, conf->addr2.da)) return -1;
			} else if (cv_is_enable(conf->addr3)) {
				if (!mac_cmp(pH->addr3, conf->addr3.sa)) return -1;
				if (!mac_cmp(pH->addr2, conf->addr3.bssid)) return -1;
				if (!mac_cmp(pH->addr1, conf->addr3.da)) return -1;
			} else {
				// check any address
				if (cv_is_enable(conf->any_addr)) {
					if (!mac_cmp(pH->addr1, conf->any_addr.addr)
							&&
							!mac_cmp(pH->addr2, conf->any_addr.addr)
							&&
							!mac_cmp(pH->addr3, conf->any_addr.addr)) return -1;
				} else {
					// check ap address
					if (cv_is_enable(conf->ap_addr)) {
						if (!mac_cmp(pH->addr2, conf->ap_addr.ap)) return -1;
					}
					// check sta address
					if (cv_is_enable(conf->sta_addr)) {
						// src, dst address is wireless STA
						if (!mac_cmp(pH->addr3, conf->sta_addr.sta)
								&&
								!mac_cmp(pH->addr1, conf->sta_addr.sta)) return -1;
					}
				}
			}
		}

		// data signature
		data_sign_len = strlen(conf->data_sign);
		if (
				(data_sign_len > 0)
				&&
				(data_sign_len < conf->data_size)
				&&
				(conf->data_size < (h80211_len-len))
		) {
			p_data = h80211 + len;
			for (i=0; i<h80211_len-len; i++) {
				// first match
				if (conf->data_sign[0] == p_data[i]) {
					if (data_sign_len > 3) {
						if (
							(conf->data_sign[1] == p_data[i+1])
							&&
							(conf->data_sign[2] == p_data[i+2])
							&&
							(conf->data_sign[3] == p_data[i+3])
						) {
							if (memcmp(conf->data_sign, &p_data[i], data_sign_len) == 0) {
								matched = 1;
								break;
							}
							i += data_sign_len;
						}
					}
					else if (data_sign_len > 2) {
						if (
								(conf->data_sign[1] == p_data[i+1])
								&&
								(conf->data_sign[2] == p_data[i+2])
						) {
							if (memcmp(conf->data_sign, &p_data[i], data_sign_len) == 0) {
								matched = 1;
								break;
							}
							i += data_sign_len;
						}
					}
					else if (data_sign_len > 1) {
						if (conf->data_sign[1] == p_data[i+1]) {
							if (memcmp(conf->data_sign, &p_data[i], data_sign_len) == 0) {
								matched = 1;
								break;
							}
							i += data_sign_len;
						}
					}
				}
			}

			if (matched == 0) {
				return -1;
			}
		}

		break;

	default:
			//cld("This packet is malformed, unknown frame type %d\n", pH->fc.type);
		return -1;
	}

	// tag is not data
	if (
		pH->fc.flags.protect   ||
		pH->fc.flags.more_data ||
		pH->fc.flags.more_frag
	) {
		goto end_match;
	}

	// tagalbe frame
	if (!tagable) {
		goto end_match;
	}

	// tag compare
	cur_tag = conf->tags;
	while (cur_tag) {
		tag_elements++;
		cur_tag = cur_tag->next;
	}

	cur_tag = conf->tags;
	while (cur_tag) {
		left = h80211_len - len;
		pos  = h80211 + len;

		while (left >= 2) {
			id    = *pos++;
			elen  = *pos++;
			left -= 2;

			if (elen > left) { // malformed packet
				return -1;
			}

			if (cur_tag->tag == id) {
				tag_elements--;
				if (cur_tag->len != elen) {
					return -1;
				}
				if (memcmp(cur_tag->data, pos, elen)) {
					return -1;
				}
			}

			left -= elen;
			pos  += elen;
		}
		cur_tag = cur_tag->next;
	}

	// left tag element
	if (tag_elements) {
		return -1;
	}

end_match:
	return 0;
}

/**
 * @brief test matching all airconf
 */
static void aircap_matching_confs(AIRCAP_G *g, uint8_t *capbuf, int caplen, struct rx_info *ri, aircap_handler_t handler)
{
	struct airconf *conf = g->conf;
	struct timeval tv;
	struct aircap_data result;

	while (conf) {
		if (g->do_exit) return;

		// matched
		if (conf->enable && aircap_matching_conf(conf, capbuf, caplen, ri) == 0) {
			gettimeofday(&tv, NULL);

			result.conf = conf;
			result.capture_tv = tv;
			result.rawdatalen = caplen;
			memcpy(result.rawdata, capbuf, caplen);
			memcpy(&result.ri, ri, sizeof(struct rx_info));

			aircap_summary(capbuf, caplen, &result.summary);

			if (conf->pcap && g->pcap_fp) {
				if (!write_pcap_file(g->pcap_fp, capbuf, caplen, ri)) {
					fflush(g->pcap_fp);
				}
			}

			// run result hander
			handler(g, (void*)&result);
		}
		conf = conf->next;
	}
}

/**
 * @brief
 */
int aircap_g_init(AIRCAP_G* g)
{
	console_log_disable();
	memset(g, 0, sizeof(AIRCAP_G));
	return 0;
}

/**
 * @brief
 */
void aircap_g_free(AIRCAP_G* g)
{
	if (g->conf) {
		airconf_free(g->conf);
		g->conf = NULL;
	}
	close_wifi(g->wi);
}

/**
 * @brief
 */
int aircap_conf_verify(AIRCAP_G *g)
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

			if (g->conf) airconf_free(g->conf); // prevent memory leak

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
		if (g->conf) airconf_free(g->conf); // prevent memory leak

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

static void chhop_thread(void *arg) {
	AIRCAP_G *g = (AIRCAP_G *)arg;
	struct ch_dwell *cd;

	while (g->do_exit == 0) {
		g->dwell_list.hop_index =
				((g->dwell_list.hop_index) % g->dwell_list.hops_size);
		cd = &g->dwell_list.hops[g->dwell_list.hop_index];

		if (wi_set_channel(g->wi, cd->channel)) {
			cle("failed to change %s channel %d", wi_get_ifname(g->wi), cd->channel);
		}

		g->dwell_list.hop_index++;
		usleep(cd->dwell * 1000); // usec -> msec
	}
}

void aircap_summary_print(struct aircap_summary *pkt_summary) {
	printf(
			"type:%d/%d, seq:%d, ds:%d, "
			"addr["_MAC_FMT_","_MAC_FMT_","_MAC_FMT_","_MAC_FMT_"]\n",
			pkt_summary->type, pkt_summary->subtype, pkt_summary->seq, pkt_summary->ds,
			_MAC_FMT_FILL_(pkt_summary->addr1), _MAC_FMT_FILL_(pkt_summary->addr2),
			_MAC_FMT_FILL_(pkt_summary->addr3), _MAC_FMT_FILL_(pkt_summary->addr4));
}

int aircap_summary(uint8_t *h80211, int caplen, struct aircap_summary *pkt_summary)
{
	wi_hdr *pH = (wi_hdr *)h80211;
	wi_tag *tag = NULL;
	int i;

	memset(pkt_summary, 0, sizeof(struct aircap_summary));
	pkt_summary->type = pH->fc.type;
	pkt_summary->subtype = pH->fc.subtype;
	pkt_summary->seq = ((h80211[22] >> 4) + (h80211[23] << 4));
	pkt_summary->ds = (pH->fc.flags.fromds << 1) | pH->fc.flags.tods;
	mac_cpy(pkt_summary->addr1, pH->addr1);
	mac_cpy(pkt_summary->addr2, pH->addr2);
	mac_cpy(pkt_summary->addr3, pH->addr3);
	if (pkt_summary->ds == 3) {
		mac_cpy(pkt_summary->addr4, pH->addr4);
		cv_set_enable(pkt_summary->addr4);
	}

	if (pH->fc.type == frame_mgnt && pH->fc.subtype == mgnt_deauth) {
		wi_deauth_fixed *fix = (wi_deauth_fixed *)(h80211 +  sizeof(wi_hdr));
		pkt_summary->deauthcode = letoh16(fix->reason);
		cv_set_enable(pkt_summary->deauthcode);
	}
	return 0;
}

/**
 * @brief match_capture_process
 */
int aircap_matching(AIRCAP_G *g, aircap_handler_t handler)
{
	struct timeval now;
	fd_set rfds;
	struct timeval wait_tv;
	struct rx_info ri;
	uint8_t capbuf[4096];
	int caplen = 0;
	pthread_t chhop_tid;
	int fd = wi_fd(g->wi);

	/* multi channel - switch channel */
	if (g->dwell_list.hops_size > 1) {
		if (pthread_create(&chhop_tid, NULL, (void*)chhop_thread, g) != 0) {
			clc("failed to create channel hop pthread - %s", strerror(errno));
			exit(1);
		}
	}
	// single channel
	else {
		if (wi_set_channel(g->wi, g->dwell_list.hops[0].channel)) {
			clc("failed to change %s channel %d",
					wi_get_ifname(g->wi), g->dwell_list.hops[0]);
			exit(1);
		}
	}

	gettimeofday(&g->capture_start, NULL);

	while (1) {
		if (g->do_exit) {
			goto cleanup;
		}

		gettimeofday(&now, NULL);

		if (g->capture_timer > 0) {
			if ((now.tv_sec - g->capture_start.tv_sec) >= g->capture_timer) {
				goto cleanup;
			}
		}

		FD_ZERO(&rfds);
		FD_SET(fd, &rfds);

		wait_tv.tv_sec = 0;
		wait_tv.tv_usec = 10;

		if (select(fd+1, &rfds, NULL, NULL, &wait_tv) < 0) {
			if (errno == EINTR) {
				continue;
			}
			cle("failed to select - %s", strerror(errno));
			goto cleanup;
		}

		if (!FD_ISSET(fd, &rfds)) {
			continue;
		}

		caplen = wi_read(g->wi, capbuf, sizeof(capbuf), &ri);
		if (caplen == -1) {
			continue;
		}
		aircap_matching_confs(g, capbuf, caplen, &ri, handler);
	}

cleanup:
	return 0;
}

int write_pcap_file(FILE *fp, uint8_t *h80211, size_t caplen, struct rx_info *ri)
{
	struct pcap_pkthdr pkh;
	struct timeval tv;
	size_t n = sizeof(pkh);

	if (fp == NULL) return -1;

	pkh.caplen = pkh.len = caplen;

	gettimeofday(&tv, NULL);

	pkh.tv_sec  =  tv.tv_sec;
	pkh.tv_usec = (tv.tv_usec & ~0x1ff) + ri->ri_power + 64;

	if (fwrite(&pkh, 1, n, fp) != n) {
		return -1;
	}

	n = pkh.caplen;

	if (fwrite( h80211, 1, n, fp) != n) {
		return -1;
	}
	return 0;
}

FILE *new_pcap_file(const char* pcapfile)
{
	struct pcap_file_header pfh;
	FILE *fp;

	fp = fopen(pcapfile, "wb+");
	if (fp == NULL) {
		return NULL;
	}

	pfh.magic           = TCPDUMP_MAGIC;
	pfh.version_major   = PCAP_VERSION_MAJOR;
	pfh.version_minor   = PCAP_VERSION_MINOR;
	pfh.thiszone        = 0;
	pfh.sigfigs         = 0;
	pfh.snaplen         = 65535;
	pfh.linktype        = LINKTYPE_IEEE802_11;

	if (fwrite(&pfh, 1, sizeof(pfh), fp) != (size_t)sizeof(pfh)) {
		fclose(fp);
		return NULL;
	}
	return fp;
}

struct ch_dwell* get_channel_dwell(struct dwell_list *list, uint8_t ch)
{
	int i;
	for (i=0; i<MAX_NUM_CHANNEL; i++) {
		if (ch == list->hops[i].channel) {
			return &list->hops[i];
		}
	}
	return NULL;
}

void set_channel_dwell(struct dwell_list *list, struct ch_dwell *ch_dwell)
{
	int i;
	struct ch_dwell *sel = NULL;

	for (i=0; i<list->hops_size; i++) {
		if (ch_dwell->channel == list->hops[i].channel) {
			sel = &list->hops[i];
			break;
		}
	}
	if (sel == NULL) {
		list->hops[list->hops_size].channel = ch_dwell->channel;
		list->hops[list->hops_size].dwell = ch_dwell->dwell;
		list->hops_size++;
	} else {
		sel->dwell = ch_dwell->dwell;
	}
}

// capture에 다중 채널인 경우 max dwell 시간을 채널별로 보정한다
void dwell_coordination(struct airconf *conf, struct dwell_list *list)
{
	struct ch_dwell *cur;
	struct ch_dwell set;

	// initialize channel dwell
	memset(list, 0, sizeof(struct dwell_list));

	while (conf)
	{
		if (cv_is_enable(conf->channel) && cv_is_enable(conf->dwell)) {
			cur = get_channel_dwell(list, conf->channel);

			set.channel = conf->channel;
			set.dwell   = conf->dwell;

			if (cur) {
				if (cur->dwell < conf->dwell) {
					set_channel_dwell(list, &set);
				}
			} else {
				set_channel_dwell(list, &set);
			}
		}
		conf = conf->next;
	}

	if (0) {
		printf("channel list =====> [size:%d]\n", list->hops_size);
		int i;
		for (i=0; i<list->hops_size; i++) {
			printf("ch:%d, dwell:%d\n",
					list->hops[i].channel,
					list->hops[i].dwell);
		}
	}
}
