/*
 * airconf-parse.c
 *
 *  Created on: 2016. 12. 6.
 *      Author: root
 */

#include "airconf-parse.h"

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <getopt.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <dirent.h>
#include <assert.h>
#include <sqlite3.h>

#include "common.h"
#include "ezxml/ezxml.h"
#include "wifi_setup.h"
#include "h80211_types.h"

const char* h80211_frame_string[] = {
		"mgnt",
		"ctrl",
		"data",
};

const char* h80211_mgnt_string[] = {
		"assoc_req",
		"assoc_resp",
		"reassoc_req",
		"reassoc_resp",
		"probe_req",
		"probe_resp",
		"",
		"",
		"beacon",
		"atim",
		"disassoc",
		"auth",
		"deauth",
		"action",
};

const char* h80211_ctrl_string[] = {
		"","","","","","","","","","",
		"ps_poll",
		"rts",
		"cts",
		"ack",
		"cf_end",
		"cf_end_ack",
};

const char* h80211_data_string[] = {
		"data",
		"data_ack",
		"data_poll",
		"data_ack_poll",
		"null",
		"ack",
		"poll",
		"ack_cf_poll",

		"qos_data",
		"qos_data_ack",
		"qos_data_poll",
		"qos_data_ack_poll",
		"qos_null",
		"reserved"
		"qos_poll",
		"qos_ack",
};

void tagconf_simple_print(struct tagconf *tag)
{
	struct tagconf *cur = tag;
	while (cur) {
		printf("id:%d, len:%d, data:", cur->tag, cur->len);
		print_hexstr(cur->data, cur->len);
		printf("\n");
		cur = cur->next;
	}
}

void tagconf_print(struct tagconf *tag)
{
	struct tagconf *cur = tag;
	uint8_t *buf;

	while (cur) {
		printf("id:%d, len:%d, data:", cur->tag, cur->len);
		print_hexstr(cur->data, cur->len);
		printf("\n");

		// hex dump
		buf = (uint8_t*)malloc(2+cur->len);
		buf[0] = cur->tag;
		buf[1] = cur->len;
		memcpy(&buf[2], cur->data, cur->len);
		print_hexdump(buf, 2+cur->len);
		printf("\n");
		free(buf);

		cur = cur->next;
	}
}


void airconf_print(struct airconf *conf)
{
	int i;
	struct tagconf *tag = conf->tags;
	int len;
	char buf[256];

	len = snprintf(buf, sizeof(buf),
			"[airconf - %s] ++++++++++++++++++++++++++++++++++++++++++++++",
			conf->desc[0] ? conf->desc : "");

	for (i = 0; i < len; i++) {
		printf("+");
	}
	printf("\n");
	printf("%s\n", buf);
	for (i = 0; i < len; i++) {
		printf("+");
	}
	printf("\n");

	printf("key: %u\n", conf->key);
	printf("channel: %d\n", conf->channel);
	printf("dwell: %d, delay: %ld\n", conf->dwell, conf->delay);
	printf("ver=%d/type/subtype: %d %d\n", conf->version, conf->type, conf->subtype);
	printf("ds: tods=%d, fromds=%d\n", conf->tods, conf->fromds);
	printf("protect: %d\n", conf->protect);
	if (conf->duration > 0) printf("duration: %d\n", ntohs(conf->duration));


	printf("addr: count=%d\n", conf->addr_count);
	if (cv_is_enable(conf->any_addr)) {
		printf("\t any_addr="_MAC_FMT_"\n", _MAC_FMT_FILL_(conf->any_addr.addr));
	}
	if (cv_is_enable(conf->ap_addr)) {
		printf("\t ap="_MAC_FMT_"\n", _MAC_FMT_FILL_(conf->ap_addr.ap));
	}
	if (cv_is_enable(conf->sta_addr)) {
		printf("\t sta="_MAC_FMT_"\n", _MAC_FMT_FILL_(conf->sta_addr.sta));
	}
	if (cv_is_enable(conf->addr1)) {
		printf("\t da="_MAC_FMT_"\n", _MAC_FMT_FILL_(conf->addr1.da));
	} else if (cv_is_enable(conf->addr2)) {
		printf("\t da="_MAC_FMT_"\n", _MAC_FMT_FILL_(conf->addr2.da));
		printf("\t sa="_MAC_FMT_"\n", _MAC_FMT_FILL_(conf->addr2.sa));
	} else if (cv_is_enable(conf->addr3)) {
		printf("\t da="_MAC_FMT_"\n", _MAC_FMT_FILL_(conf->addr3.da));
		printf("\t sa="_MAC_FMT_"\n", _MAC_FMT_FILL_(conf->addr3.sa));
		printf("\t bssid="_MAC_FMT_"\n", _MAC_FMT_FILL_(conf->addr3.bssid));
	} else if (cv_is_enable(conf->addr4)) {
		printf("\t da="_MAC_FMT_"\n", _MAC_FMT_FILL_(conf->addr4.da));
		printf("\t ra="_MAC_FMT_"\n", _MAC_FMT_FILL_(conf->addr4.ra));
		printf("\t sa="_MAC_FMT_"\n", _MAC_FMT_FILL_(conf->addr4.sa));
		printf("\t ta="_MAC_FMT_"\n", _MAC_FMT_FILL_(conf->addr4.ta));
	}

	if (tag) printf("taglist:\n");
	while (tag)
	{
		printf("\t tag=%d, len=%d, data=0x", tag->tag, tag->len);
		print_hexstr(tag->data, tag->len);
		printf(" (");
		for (i=0; i<tag->len; i++) {
			printf("%c", vchar(tag->data[i]));
		}
		printf(")\n");

		tag = tag->next;
	}
	printf("\n");
}

/**
 * @brief find tagconf
 */
struct tagconf *tagconf_find(struct tagconf *first, uint8_t id)
{
	struct tagconf *cur = first;
	while (cur) {
		if (cur->tag == id) {
			return cur;
		}
		cur = cur->next;
	}
	return NULL;
}

/**
 * @brief find vendor tagconf
 */
struct tagconf *tagconf_vendor_find(struct tagconf *first, uint8_t *oui)
{
	struct tagconf *cur = first;
	while (cur) {
		if (cur->tag == 221) {
			if (cur->data && oui_cmp(oui, cur->data))
				return cur;
		}
		cur = cur->next;
	}
	return NULL;
}

/**
 * @brief load tagconf from xml buffer
 */
static struct tagconf* tagconf_from_xml(ezxml_t root)
{
	ezxml_t e = NULL;
	struct tagconf *first = NULL;
	struct tagconf *tag = NULL;
	const char *id = NULL;
	const char *len = NULL;
	const char *data = NULL;
	const char *type = NULL;
	uint8_t is_hexdata = 0;

	for (e = ezxml_child(root, "tag"); e; e = e->next) {
		id   = ezxml_attr(e, "id");
		len  = ezxml_attr(e, "len");
		data = ezxml_attr(e, "data");
		type = ezxml_attr(e, "type");

		if (type && (strcmp(type, "hex")==0)) {
			is_hexdata = 1;
		}

		if (!id || !len || !data) {
			if (!id) {
				fprintf(stderr, "missing!!! tag id\n");
				goto fail;
			}
			if (!len) {
				fprintf(stderr, "missing!!! tag len\n");
				goto fail;
			}
			if (len && (atoi(len)>0) && !data) {
				fprintf(stderr, "missing!!! tag data\n");
				goto fail;
			}
		}

		tag = (struct tagconf*)malloc(sizeof(struct tagconf));
		memset(tag, 0, sizeof(struct tagconf));

		tag->tag = atoi(id);
		tag->len = atoi(len);
		tag->data = (uint8_t *)malloc(tag->len+1);

		if (is_hexdata) {
			hex2binarr(tag->data, tag->len, data);
		} else {
			strncpy((char*)tag->data, data, tag->len);
			tag->data[tag->len] = 0;
		}

		if (first == NULL) {
			first = tag;
		} else {
			if (first->next) {
				tag->next = first->next;
				first->next = tag;
			} else {
				first->next = tag;
			}
		}
	}
	return first;

fail:
	while (first) {
		tag = first->next;
		if (tag->data) {
			free(tag->data);
		}
		free(first);
		first = tag;
	}
	return NULL;
}

/**
 * @brief parse addr1 from xml
 */
static int parse_addr1(ezxml_t root, addrconf_1 *addr1)
{
	ezxml_t da = ezxml_child(root, "da");

	if (!da) {
		if (!da) fprintf(stderr, "missing!!! <addr count=\"1\"> <da>\n");
		return -1;
	}
	if (str2mac(da->txt, addr1->da)) {
		fprintf(stderr, "failed parse <addr count=\"1\"> <da>=%s\n", da->txt);
		return -1;
	}
	return 0;
}

/**
 * @brief parse addr2 from xml
 */
static int parse_addr2(ezxml_t root, addrconf_2 *addr2)
{
	ezxml_t da = ezxml_child(root, "da");
	ezxml_t sa = ezxml_child(root, "sa");

	if (!da || !sa) {
		if (!da) fprintf(stderr, "missing!!! <addr count=\"2\"> <da>\n");
		if (!sa) fprintf(stderr, "missing!!! <addr count=\"2\"> <sa>\n");
		return -1;
	}
	if (str2mac(da->txt, addr2->da)) {
		fprintf(stderr, "failed parse <addr count=\"2\"> <da>=%s\n", da->txt);
		return -1;
	}
	if (str2mac(sa->txt, addr2->sa)) {
		fprintf(stderr, "failed parse <addr count=\"2\"> <sa>=%s\n", sa->txt);
		return -1;
	}
	return 0;
}

/**
 * @brief parse addr3 from xml
 */
static int parse_addr3(ezxml_t root, addrconf_3 *addr3)
{
	ezxml_t bssid = ezxml_child(root, "bssid");
	ezxml_t da    = ezxml_child(root, "da");
	ezxml_t sa    = ezxml_child(root, "sa");

	if (!da || !sa || !bssid) {
		if (!da)    fprintf(stderr, "missing!!! <addr count=\"3\"> <da>\n");
		if (!sa)    fprintf(stderr, "missing!!! <addr count=\"3\"> <sa>\n");
		if (!bssid) fprintf(stderr, "missing!!! <addr count=\"3\"> <bssid>\n");
		return -1;
	}
	if (str2mac(da->txt, addr3->da)) {
		fprintf(stderr, "failed parse <addr count=\"3\"> <da>=%s\n", da->txt);
		return -1;
	}
	if (str2mac(sa->txt, addr3->sa)) {
		fprintf(stderr, "failed parse <addr count=\"3\"> <sa>=%s\n", sa->txt);
		return -1;
	}
	if (str2mac(bssid->txt, addr3->bssid)) {
		fprintf(stderr, "failed parse <addr count=\"3\"> <bssid>=%s\n", bssid->txt);
		return -1;
	}
	return 0;
}

/**
 * @brief parse addr4 from xml
 */
static int parse_addr4(ezxml_t root, addrconf_4 *addr4)
{
	ezxml_t ra = ezxml_child(root, "ra");
	ezxml_t da = ezxml_child(root, "da");
	ezxml_t sa = ezxml_child(root, "sa");
	ezxml_t ta = ezxml_child(root, "ta");

	if (!ra || !da || !sa || !ta) {
		if (!da) fprintf(stderr, "missing!!! <addr count=\"4\"> <da>\n");
		if (!sa) fprintf(stderr, "missing!!! <addr count=\"4\"> <sa>\n");
		if (!ta) fprintf(stderr, "missing!!! <addr count=\"4\"> <ta>\n");
		if (!ra) fprintf(stderr, "missing!!! <addr count=\"4\"> <ra>\n");
		return -1;
	}
	if (str2mac(ra->txt, addr4->ra)) {
		fprintf(stderr, "failed parse <addr count=\"4\"> <ra>=%s\n", ra->txt);
		return -1;
	}
	if (str2mac(da->txt, addr4->da)) {
		fprintf(stderr, "failed parse <addr count=\"4\"> <da>=%s\n", da->txt);
		return -1;
	}
	if (str2mac(sa->txt, addr4->sa)) {
		fprintf(stderr, "failed parse <addr count=\"4\"> <sa>=%s\n", sa->txt);
		return -1;
	}
	if (str2mac(ta->txt, addr4->ta)) {
		fprintf(stderr, "failed parse <addr count=\"4\"> <ta>=%s\n", ta->txt);
		return -1;
	}
	return 0;
}

/**
 * @brief parse only one airconf from xml
 * @param def default airconf
 */
static struct airconf* airconf_one_parse(struct airconf* def, ezxml_t root, int isdef)
{
	struct airconf *conf;
	ezxml_t e = NULL;

	conf = (struct airconf*)malloc(sizeof(struct airconf));
	memset(conf, 0, sizeof(struct airconf));

	///////////////////////////////////////////////
	// configure with default
	///////////////////////////////////////////////
	/* enable */
	e = ezxml_child(root, "enable");
	if (!e || !e->txt) {
		conf->enable = 1;
	} else {
		conf->enable = (uint32_t)(atoi(e->txt));
	}
	if (conf->enable == 0) {
		return conf;
	}

	/* desc */
	e = ezxml_child(root, "desc");
	if (e && e->txt) {
		snprintf(conf->desc, sizeof(conf->desc), "%s", e->txt);
		cv_set_enable(conf->desc);
	} else if (def && cv_is_enable(def->desc)) {
		snprintf(conf->desc, sizeof(conf->desc), "%s", def->desc);
		cv_set_enable(conf->desc);
	}

	/* channel */
	e = ezxml_child(root, "channel");
	if (e && e->txt) {
		conf->channel = atoi(e->txt);
		cv_set_enable(conf->channel);
	} else if (def && (def->channel > 0)) {
		conf->channel = def->channel;
		cv_set_enable(conf->channel);
	}
	if (conf->channel == 0) {
		fprintf(stderr, "missing!!! channel\n");
		goto fail;
	}

	/* delay */
	e = ezxml_child(root, "delay");
	if (e && e->txt) {
		conf->delay = atol(e->txt);
		cv_set_enable(conf->delay);
	} else if (def && (def->delay > 0)) {
		conf->delay = def->delay;
		cv_set_enable(conf->delay);
	}

	/* dwell */
	e = ezxml_child(root, "dwell");
	if (e && e->txt) {
		conf->dwell = atoi(e->txt);
		cv_set_enable(conf->dwell);
	} else if (def && (def->dwell > 0)) {
		conf->dwell = def->dwell;
		cv_set_enable(conf->dwell);
	}
	if (conf->dwell == 0) {
		fprintf(stderr, "missing!!! dwell\n");
		goto fail;
	}

	/* data size */
	e = ezxml_child(root, "data_size");
	if (e && e->txt) {
		conf->data_size = atoi(e->txt);
		cv_set_enable(conf->data_size);
	} else if (def && (def->data_size > 0)) {
		conf->data_size = def->data_size;
		cv_set_enable(conf->data_size);
	}

	/* data signature */
	e = ezxml_child(root, "data_sign");
	if (e && e->txt) {
		snprintf(conf->data_sign, sizeof(conf->data_sign), "%s", e->txt);
		cv_set_enable(conf->data_sign);
	} else if (def && cv_is_enable(def->data_sign)) {
		snprintf(conf->data_sign, sizeof(conf->data_sign), "%s", def->data_sign);
		cv_set_enable(conf->data_sign);
	}

	///////////////////////////////////////////////
	// only Individual configure
	///////////////////////////////////////////////
	/* key */
	if (isdef == 0) {
		e = ezxml_child(root, "key");
		if (!e || !e->txt) {
			fprintf(stderr, "missing!!! airconf key\n");
			goto fail;
		}
		conf->key = (uint32_t)(atoi(e->txt));
	}

	/* write pcap file */
	e = ezxml_child(root, "pcap");
	if (e && e->txt) {
		conf->pcap = atoi(e->txt);
		cv_set_enable(conf->pcap);
	}

	/* config sequence */
	e = ezxml_child(root, "seq");
	if (e && e->txt) {
		conf->seq = atoi(e->txt);
		cv_set_enable(conf->seq);
	}

	/* IEEE 802.11 mac frame protocol version */
	e = ezxml_child(root, "version");
	if (e && e->txt) {
		conf->version = atoi(e->txt);
		cv_set_enable(conf->version);
	}

	/* IEEE 802.11 mac frame type */
	e = ezxml_child(root, "type");
	if (e && e->txt) {
		conf->type = atoi(e->txt);
		cv_set_enable(conf->type);
	}

	/* IEEE 802.11 mac frame subtype */
	e = ezxml_child(root, "subtype");
	if (e && e->txt) {
		conf->subtype = atoi(e->txt);
		cv_set_enable(conf->subtype);
	}

	/* fromds or tods */
	e = ezxml_child(root, "ds");
	if (e) {
		const char* tods   = ezxml_attr(e, "tods");
		const char* fromds = ezxml_attr(e, "fromds");
		if (tods) {
			conf->tods   = atoi(tods);
			cv_set_enable(conf->tods);
		}
		if (fromds) {
			conf->fromds = atoi(fromds);
			cv_set_enable(conf->fromds);
		}
	}

	/* power sleep flag */
	e = ezxml_child(root, "pwr_sleep");
	if (e && e->txt) {
		conf->pwr_sleep = atoi(e->txt);
		cv_set_enable(conf->pwr_sleep);
	}

	/* privacy flag */
	e = ezxml_child(root, "protect");
	if (e && e->txt) {
		conf->protect = atoi(e->txt);
		cv_set_enable(conf->protect);
	}

	/* duration */
	e = ezxml_child(root, "duration");
	if (e && e->txt) {
		conf->duration = htons(atoi(e->txt));
		cv_set_enable(conf->duration);
	}

	/* de-authentication reason code */
	e = ezxml_child(root, "deauth_reason");
	if (e && e->txt) {
		conf->deauth_reason = atoi(e->txt);
		cv_set_enable(conf->deauth_reason);
	}

	int dup = 0;
	dup += ezxml_child(root, "ap") ? 1 : 0;
	dup += ezxml_child(root, "sta") ? 1 : 0;
	dup += ezxml_child(root, "addr") ? 1 : 0;
	if (!((dup == 0) || (dup == 1))) {
		fprintf(stderr, "only one select of <ap> or <sta> or <addr>\n");
		goto fail;
	}

	// beacon has not sta address
	if (cv_is_enable(conf->type) && cv_is_enable(conf->subtype)) {
		if (conf->type == frame_mgnt) {
			if (conf->subtype == mgnt_beacon) {
				if (ezxml_child(root, "sta")) {
					fprintf(stderr, "beacon has not <sta>\n");
					goto fail;
				}
			}
		}
	}
	// ctrl frame has not ap address and sta address
	if (cv_is_enable(conf->type)) {
		if (conf->type == frame_ctrl) {
			if (ezxml_child(root, "sta") || ezxml_child(root, "ap")) {
				fprintf(stderr, "beacon has not <ap> or <sta>\n");
				goto fail;
			}
		}
	}
	if (cv_is_enable(conf->tods) && cv_is_enable(conf->fromds)) {
		// WDS address count is '4'
		if (conf->tods == 1 && conf->fromds == 1) {
			e = ezxml_child(root, "addr");
			if (e) {
				const char* count = ezxml_attr(e, "count");
				if (!count) {
					fprintf(stderr, "missing!!! <addr> count\n");
					goto fail;
				}
				if (atoi(count) != 4) {
					fprintf(stderr, "WDS MUST <addr count=\"4\">\n");
					goto fail;
				}
			}
		}
		// Not WDS(address 4)
		else {
			e = ezxml_child(root, "addr");
			if (e) {
				const char* count = ezxml_attr(e, "count");
				if (!count) {
					fprintf(stderr, "missing!!! <addr> count\n");
					goto fail;
				}
				if (atoi(count) == 4) {
					fprintf(stderr, "<addr count=\"4\"> is allow only WDS\n");
					goto fail;
				}
			}
		}
	}

	/* ap mac address (bssid) */
	e = ezxml_child(root, "ap");
	if (e && e->txt) {
		if (str2mac(e->txt, conf->ap_addr.ap)) {
			fprintf(stderr, "failed <ap> convert ap=%s\n", e->txt);
			goto fail;
		}
		cv_set_enable(conf->ap_addr);
	}

	/* station mac address */
	e = ezxml_child(root, "sta");
	if (e && e->txt) {
		if (str2mac(e->txt, conf->sta_addr.sta)) {
			fprintf(stderr, "failed <sta> convert sta=%s\n", e->txt);
			goto fail;
		}
		cv_set_enable(conf->sta_addr);
	}

	/* any mac address */
	e = ezxml_child(root, "any_addr");
	if (e && e->txt) {
		if (str2mac(e->txt, conf->any_addr.addr)) {
			fprintf(stderr, "failed <any_addr> convert addr=%s\n", e->txt);
			goto fail;
		}
		cv_set_enable(conf->any_addr);
	}

	/* 1 or 2 or 3 or 4 address */
	e = ezxml_child(root, "addr");
	if (e) {
		const char* count = ezxml_attr(e, "count");
		if (!count) {
			fprintf(stderr, "missing!!! <addr> count\n");
			goto fail;
		}
		conf->addr_count = atoi(count);

		switch (conf->addr_count)
		{
		case 4:
			if (parse_addr4(e, &conf->addr4)) {
				fprintf(stderr, "failed parse <addr count=\"4\">\n");
				goto fail;
			}
			cv_set_enable(conf->addr4);
			break;
		case 3:
			if (parse_addr3(e, &conf->addr3)) {
				fprintf(stderr, "failed parse <addr count=\"3\">\n");
				goto fail;
			}
			cv_set_enable(conf->addr3);
			break;
		case 2:
			if (parse_addr2(e, &conf->addr2)) {
				fprintf(stderr, "failed parse <addr count=\"2\">\n");
				goto fail;
			}
			cv_set_enable(conf->addr2);
			break;
		case 1:
			if (parse_addr1(e, &conf->addr1)) {
				fprintf(stderr, "failed parse <addr count=\"1\">\n");
				goto fail;
			}
			cv_set_enable(conf->addr1);
			break;
		default: {
				fprintf(stderr, "failed parse <addr count=%d>\n", conf->addr_count);
				goto fail;
			}
		}
	}

	/* tagged parameters */
	e = ezxml_child(root, "taglist");
	if (e) {
		conf->tags = tagconf_from_xml(e);

		if (def && def->tags) {
			if (conf->tags) {
				tagconf_last(conf->tags)->next = tagconf_clone(def->tags);
			} else {
				conf->tags = tagconf_clone(def->tags);
			}
		}
		if (conf->tags) {
			conf->tags = tagconf_sort(conf->tags);
		} else {
			goto fail;
		}
	}
	return conf;

fail:
	if (conf) {
		free(conf);
	}
	return NULL;
}

/**
 * @brief sort tagconf
 */
struct tagconf* tagconf_sort(struct tagconf *tag)
{
	struct tagconf *sort = NULL;
	struct tagconf *cur = tag;
	struct tagconf *find = NULL;
	size_t i = 0;
	char ids[256] = {0, };

	if (!tag) {
		return NULL;
	}

	while (cur) {
		ids[cur->tag] = 1;
		cur = cur->next;
	}

	cur = NULL;
	for (i = 0; i < 256; i++) {
		if (ids[i]) {
			find = tagconf_find(tag, i);
			assert(find != NULL);
			cur = tagconf_new(cur, find->tag, find->len, find->data);

			if (!sort) {
				sort = cur;
			}
		}
	}

	tagconf_free(tag);
	return sort;
}

/**
 * @brief clone tagconf
 */
struct tagconf* tagconf_clone(struct tagconf *tag)
{
	struct tagconf *cur = tag;
	struct tagconf *newt = NULL;
	struct tagconf *newh = NULL;

	if (!tag) {
		return NULL;
	}

	while (cur) {
		newt = tagconf_new(newt, cur->tag, cur->len, cur->data);
		if (newh == NULL) {
			newh = newt;
		}
		cur = cur->next;
	}
	return newh;
}

/**
 * @brief last tagconf
 */
struct tagconf* tagconf_last(struct tagconf *tag)
{
	struct tagconf *cur = tag;

	if (cur == NULL) {
		return NULL;
	}

	while (cur) {
		if (cur->next == NULL) {
			break;
		}
		cur = cur->next;
	}

	return cur;
}

/**
 * @brief make new tagconf
 */
struct tagconf* tagconf_new(struct tagconf *prev, uint8_t tag, uint8_t len, uint8_t *data)
{
	struct tagconf *t = (struct tagconf*)malloc(sizeof(struct tagconf));

	t->tag = tag;
	t->len = len;
	if (len == 0) {
		t->data = NULL;
	} else {
		t->data = (uint8_t*)malloc(len);
		memcpy(t->data, data, len);
	}
	if (prev) {
		prev->next = t;
	}
	t->next = NULL;
	return t;
}

/**
 * @brief free tagconf
 */
int tagconf_free(struct tagconf *first)
{
	struct tagconf *cur = NULL;
	struct tagconf *next = NULL;
	int count = 0;

	if (!first) {
		return 0;
	}

	cur = first;
	while (cur) {
		next = cur->next;

		if (cur->data) {
			free(cur->data);
			cur->data = NULL;
		}
		free(cur);

		cur = next;

		count++;
	}
	return count;
}

/**
 * @brief free airconf
 */
void airconf_free(struct airconf *conf)
{
	struct airconf *cur = conf;
	struct airconf *next = NULL;

	if (!conf) {
		return;
	}

	while (cur) {
		next = cur->next;

		if (cur->tags) {
			tagconf_free(cur->tags);
			cur->tags = NULL;
		}
		free(cur);
		cur = next;
	}
}

/**
 * @brief get max dwell time
 */
uint32_t max_dwell(struct airconf *conf)
{
	uint32_t max = 0;
	struct airconf *cur = conf;

	while (cur) {
		if (cv_is_enable(cur->dwell)
				&& max < cur->dwell) {
			max = cur->dwell;
		}
		cur = cur->next;
	}
	return max;
}

/**
 * @brief get min dwell time
 */
uint32_t min_dwell(struct airconf *conf)
{
	uint32_t min = (uint32_t)-1;
	struct airconf *cur = conf;

	while (cur) {
		if (cv_is_enable(cur->dwell)
				&& min > cur->dwell) {
			min = cur->dwell;
		}
		cur = cur->next;
	}
	return min;
}

/**
 * @brief parse airconf from xml
 */
static struct airconf* airconf_parse(ezxml_t xml, int debug)
{
	struct airconf *first = NULL;
	struct airconf *conf = NULL;
	struct airconf *def = NULL;
	ezxml_t e;

	// default configure
	if ((e = ezxml_child(xml, "default"))) {
		def = airconf_one_parse(NULL, e, 1);
	}

	// airconf configure
	for (e = ezxml_child(xml, "airconf"); e; e = e->next) {
		conf = airconf_one_parse(def, e, 0);
		if (!conf) {
			//fprintf(stderr, "failed parse airconf conf\n");
			goto fail;
		} else {
			if (first == NULL) {
				first = conf;
			} else {
				conf->next = first;
				first->prev = conf;
				first = conf;
			}
			if (debug) {
				airconf_print(conf);
			}
		}
	}

	if (def) airconf_free(def);
	return airconf_sort(first);

fail:
	if (def) airconf_free(def);
	if (first) airconf_free(first);
	return NULL;
}

/**
 * @brief parser airconf from buffer
 */
struct airconf* airconf_from_buf(char *xmlbuf, size_t len, int debug)
{
	struct airconf *conf = NULL;
	ezxml_t xml = ezxml_parse_str(xmlbuf, len);

	if (xml == NULL) return NULL;

	conf = airconf_parse(xml, debug);

	ezxml_free(xml);
	return conf;
}

/**
 * @brief total airconf count
 */
uint32_t airconf_count(struct airconf* head)
{
	uint32_t count = 0;
	struct airconf *cur;
	for (cur = head; cur != NULL; cur = cur->next) {
		count++;
	}
	return count;
}

/**
 * @brief find airconf by key
 */
struct airconf* airconf_find_key(struct airconf* head, uint32_t key)
{
	struct airconf *cur;

	for (cur = head; cur != NULL; cur = cur->next) {
		if (key == cur->key) {
			return cur;
		}
	}
	return NULL;
}

/**
 * @brief find airconf by key and seq
 */
struct airconf* airconf_find_keyseq(struct airconf* head, uint32_t key, uint32_t seq)
{
	struct airconf *cur;

	for (cur = head; cur != NULL; cur = cur->next) {
		if (key == cur->key && seq == cur->seq) {
			return cur;
		}
	}
	return NULL;
}

/**
 * @brief sort airconf by 'seq'
 */
struct airconf* airconf_sort(struct airconf* head)
{
	struct airconf *sort = NULL;
	struct airconf *cur, *tmp, *last;
	uint32_t min;
	uint32_t key = 0;
	uint32_t count;
	uint32_t i;

	count = airconf_count(head);

	for (i = 0; i<count; i++) {
		min = (uint32_t)-1;

		for (cur = head; cur; cur = cur->next) {
			if (cur->seq < min) {
				min = cur->seq;
				key = cur->key;
			}
		}

		tmp = airconf_find_keyseq(head, key, min);
		if (head == tmp) {
			head = tmp->next;
		}
		if (tmp->prev) {
			tmp->prev->next = tmp->next;
		}
		if (tmp->next) {
			tmp->next->prev = tmp->prev;
		}
		tmp->prev = tmp->next = NULL;

		if (sort) {
			for (last = sort; last->next; last = last->next);
			last->next = tmp;
			tmp->prev = last;
		} else {
			sort = tmp;
		}
	}
	return sort;
}

/**
 * @brief airconf_last
 */
struct airconf* airconf_last(struct airconf *conf) {
	struct airconf* cur = NULL;

	if (conf == NULL) return NULL;
	for (cur = conf; cur->next; cur = cur->next) ;
	return cur;
}

/**
 * @brief airconf_from_file
 */
struct airconf* airconf_from_file(const char *filename, int debug)
{
	struct airconf *conf = NULL;
	ezxml_t xml = ezxml_parse_file(filename);
	if (xml == NULL)
		return NULL;

	conf = airconf_parse(xml, debug);

	ezxml_free(xml);
	return conf;
}

/**
 * @brief airconf_filelist
 * @param files file list
 * @param basedir airconf(XML) search base directory
 * @return files is linked list
 */
void airconf_filelist(llist **files, const char *basedir)
{
	DIR *dp = NULL;
	struct dirent *ep = NULL;
	int fnamelen;
	char dir[256];
	char filename[256];

	dp = opendir(basedir);
	if (dp) {
		while ((ep = readdir(dp)) != 0) {
			if ((strcmp(".",  ep->d_name) == 0) ||
				(strcmp("..", ep->d_name) == 0)) {
				continue;
			}

			if ((ep->d_type == DT_DIR)) {
				snprintf(dir, sizeof(dir), "%s/%s", basedir, ep->d_name);
				airconf_filelist(files, dir);
			}

			fnamelen = strlen(ep->d_name);
			if (fnamelen > 4) {
				if (
						(ep->d_type == DT_REG)
						&&
						(strncmp(ep->d_name+fnamelen-4, ".xml", 4) == 0)
				) {
					snprintf(filename, sizeof(filename), "%s/%s", basedir, ep->d_name);
					*files = add_list(*files, strdup(filename));
				}
			}
		}
		closedir(dp);
	}
	return;
}

/**
 * @brief load airconf from directory path
 */
int airconf_from_path(struct airconf** head, const char *basedir, int debug)
{
	DIR *dp = NULL;
	struct dirent *ep = NULL;
	int fnsize;
	struct airconf *conf, *cur, *prev;
	char dir[256];
	int ok = -1;

	dp = opendir(basedir);
	if (dp) {
		while ((ep = readdir(dp)) != 0) {
			if ((strcmp(".",  ep->d_name) == 0) || (strcmp("..", ep->d_name) == 0)) {
				continue;
			}

			if ((ep->d_type == DT_DIR)) {
				snprintf(dir, sizeof(dir), "%s/%s", basedir, ep->d_name);
				ok = airconf_from_path(head, dir, debug);
				if (ok == -1) {
					closedir(dp);
					return -1;
				}
			}

			fnsize = strlen(ep->d_name);
			if (fnsize > 4) {
				if ((ep->d_type == DT_REG) && strncmp(ep->d_name+fnsize-4, ".xml", 4) == 0) {
					snprintf(dir, sizeof(dir), "%s/%s", basedir, ep->d_name);

					conf = airconf_from_file(dir, debug);
					if (conf) {
						if (*head == NULL) {
							*head = conf;
						} else {
							cur = *head;
							while (cur) {
								prev = cur;
								cur = cur->next;
							}
							prev->next = conf;
						}
						ok = 0;
					} else {
						closedir(dp);
						return -1;
					}
				}
			}
		}
		closedir(dp);
	}
	return ok;
}

/**
 * @brief sqlairconf_free
 */
void sqlairconf_free(struct sqlairconf *conf)
{
	struct sqlairconf *tmp;
	struct sqlairconf *cur;

	if (!conf) return;

	cur = conf;
	while (cur) {
		tmp = cur->next;
		free(cur);
		cur = tmp;
	}
}

/**
 * @brief sqlairconf_load
 * @param dbfile sqlite db filename
 * @param profileid
 */
struct sqlairconf* sqlairconf_load(const char *dbfile, const char *profileid)
{
    sqlite3 *conn;
    sqlite3_stmt *stmt;
    int rc;
    struct sqlairconf *head = NULL, *conf = NULL, *last = NULL;
    const char *sql =
    		"select config_id, capturexml, shooterxml from profile_config_ref a, config  b "
    		"where a.config_id = b.id and profile_id = ? "
    		"order by config_id";

	rc = sqlite3_open_v2(dbfile, &conn, SQLITE_OPEN_READONLY, NULL);
	if (rc != SQLITE_OK) {
		fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(conn));
		fprintf(stderr, "dbfile: %s\n", dbfile);
		fprintf(stderr, "query: %s\n", sql);
		sqlite3_close(conn);
		return NULL;
	}

	rc = sqlite3_prepare_v2(conn, sql, -1, &stmt, 0);
	if (rc == SQLITE_OK) {
		sqlite3_bind_text(stmt, 1, profileid, strlen(profileid), SQLITE_TRANSIENT);
	} else {
		fprintf(stderr, "Failed to execute statement: %s\n", sqlite3_errmsg(conn));
		fprintf(stderr, "dbfile: %s\n", dbfile);
		fprintf(stderr, "%s\n", sql);
		sqlite3_close(conn);
		return NULL;
	}

	while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
		conf = (struct sqlairconf *)malloc(sizeof(struct sqlairconf));
		memset(conf, 0, sizeof(struct sqlairconf));
		SNP(conf->profileid, "%s", profileid);
		SNP(conf->configid, "%s", sqlite3_column_text(stmt, 0));
		if (sqlite3_column_type(stmt, 1) != SQLITE_NULL) {
			SNP(conf->capturexml, "%s", sqlite3_column_text(stmt, 1));
		}
		if (sqlite3_column_type(stmt, 2) != SQLITE_NULL) {
			SNP(conf->shooterxml, "%s", sqlite3_column_text(stmt, 2));
		}
		conf->next = NULL;
		if (!head) {
			head = conf;
			last = head;
		} else {
			last->next = conf;
			last = conf;
		}
	}
	sqlite3_finalize(stmt);
	sqlite3_close(conn);
	return head;
}

