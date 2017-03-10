/*
 * aircurrent.c
 *
 *  Created on: 2017. 1. 7.
 *      Author: root
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
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
#include <time.h>
#include <pthread.h>
#include <sqlite3.h>

#include "airnodes.h"
#include "ezxml/ezxml.h"
#include "airconf-parse.h"
#include "hashtable.h"
#include "osdep/osdep.h"
#include "osdep/common.h"
#include "common.h"
#include "wifi_setup.h"
#include "wi_packet.h"
#include "log.h"
#include "sqldb.h"
#include "airsock.h"
#include "airresult.h"


typedef struct _AIRCUR_G
{
	struct wif *wi;
	int debug;
	uint8_t channel;
	int do_exit;
	int report_intv;
	int ap_lifetime;
	int st_lifetime;
	NODES_INFO *nodes_info;
} AIRCUR_G;

static AIRCUR_G g;


struct xml_buffer_list {
	int buf_seq;
	uint8_t  **buf;
	int *len;
	int total_len;
};

static void xml_buffer_list_init(struct xml_buffer_list *xmllist, size_t size) {
	xmllist->buf_seq = 0;
	xmllist->total_len = 0;
	xmllist->buf = (uint8_t**)malloc(sizeof(uint8_t*) * size);
	xmllist->len = (int*)malloc(sizeof(int) * size);
}

static void xml_buffer_list_free(struct xml_buffer_list *xmllist) {
	int i=0;
	for (i=0; i<xmllist->buf_seq; i++) {
		if (xmllist->buf[i])
			free(xmllist->buf[i]);
	}
	if (xmllist->len)
		free(xmllist->len);
	xmllist->buf_seq = 0;
	xmllist->total_len = 0;
}

static void xml_buffer_list_print(struct xml_buffer_list *xmllist) {
	int i=0;
	for (i=0; i<xmllist->buf_seq; i++) {
		printf("%s\n", xmllist->buf[i]);
	}
}

static int ar_sock = -1;
static int aircurrent_open()
{
	int csock;

	csock = connect_t(NULL, "127.0.0.1", AIRRESULT_CURRENT_PORT, 0);
	if (csock < 0) {
		return -1;
	}
	set_rst_close(csock); // SO_LINGER
	return csock;
}

static void aircurrent_close(int *csock)
{
	if (!csock) return;

	if (*csock > 0) {
		close(*csock);
	}
	*csock = -1;
}

static int aircurrent_send(int *csock, uint8_t *data, size_t len)
{
	struct timeval tv;
	int retry = 5;
	int bytes = 0;

	if (!csock) return -1;

	if (*csock < 0) {
		*csock = aircurrent_open();
		if (*csock < 0) return -1;
	}

	while (retry) {
		tv.tv_sec = 1; tv.tv_usec = 0;
		bytes = send_t(*csock, data, len, tv, 0);
		if (bytes > 0) {
			break;
		} else {
			clc("error send for result data (%s)\n", strerror(errno));
			usleep(100);
			retry--;
		}
	}
	return bytes;
}

void aircurrent_xml_aplist_report(void *data, void *arg)
{
	const AP_INFO *ap = (const AP_INFO *)data;
	struct xml_buffer_list *buflist = (struct xml_buffer_list*)arg;
	ezxml_t xml;
	char *ssid;
	uint8_t *buf;
	int len;

	xml = ezxml_new("ap");
	ezxml_set_attr(xml, "bssid", mactoa(ap->bssid));
	ezxml_set_attr(xml, "ch",    itoa(ap->channel, 10));
	if (ap->ssid_length > 0 && ap->essid[0]) {
		ssid = (char *)malloc(ap->ssid_length+1);
		memcpy(ssid, ap->essid, ap->ssid_length);
		ssid[ap->ssid_length] = '\0';
		ezxml_set_attr(xml, "ssid",  ssid);
	}
	ezxml_set_attr(xml, "proto", itoa(ap->proto, 10));
	ezxml_set_attr(xml, "pwr",   itoa(ap->avg_power, 10));
	ezxml_set_attr(xml, "sec",   itoa(ap->security, 10));
	ezxml_set_attr(xml, "tinit", ltoa(ap->tinit, 10));
	ezxml_set_attr(xml, "tlast", ltoa(ap->tlast, 10));
	buf = ezxml_toxml(xml);
	len = strlen(buf);
	buflist->buf[buflist->buf_seq] = buf;
	buflist->len[buflist->buf_seq] = len;
	buflist->total_len += len;
	buflist->buf_seq++;
	ezxml_free(xml);
}

void aircurrent_xml_stlist_report(void *data, void *arg)
{
	const ST_INFO *st = (const ST_INFO *)data;
	struct xml_buffer_list *buflist = (struct xml_buffer_list*)arg;
	ezxml_t xml;
	uint8_t *buf;
	int len;

	xml = ezxml_new("sta");
	ezxml_set_attr(xml, "stamac", mactoa(st->stmac));
	ezxml_set_attr(xml, "bssid",  mactoa(st->bssid));
	ezxml_set_attr(xml, "ch",     itoa(st->channel, 10));
	ezxml_set_attr(xml, "pwr",    itoa(st->power, 10));
	ezxml_set_attr(xml, "rate",   itoa(st->rate_from/1000000, 10));
	ezxml_set_attr(xml, "pkt",    ltoa(st->nb_pkt, 10));
	ezxml_set_attr(xml, "bytes",  u64toa(st->databytes, 10));
	ezxml_set_attr(xml, "tinit",  ltoa(st->tinit, 10));
	ezxml_set_attr(xml, "tlast",  ltoa(st->tlast, 10));
	buf = ezxml_toxml(xml);
	len = strlen(buf);
	buflist->buf[buflist->buf_seq] = buf;
	buflist->len[buflist->buf_seq] = len;
	buflist->total_len += len;
	buflist->buf_seq++;
	ezxml_free(xml);
}

void usage(int argc, char** argv)
{
	print_args(argc, argv);

	printf("\n");
	printf("Usage: aircurrent\n");
	printf("options:\n\n");
	printf("\t -i <iface>         : Wi-Fi Interface name\n");
	printf("\t -c <channel>       : channel\n");
	printf("\t -t <report intval> : capture report interval time\n");
	printf("\t -a <ap  lifetime>  : ap  lifetime seconds (default: 60sec)\n");
	printf("\t -s <sta lifetime>  : sta lifetime seconds (default: 60sec)\n");
	printf("\t -d                 : debug\n");
	printf("\t -h -?              : help\n");
	printf("\n");
}

void sighandler(int signum)
{
//fprintf(stderr, "catch signal %d\n", signum);
	switch (signum)
	{
	case SIGUSR1:
		break;
	case SIGUSR2:
		break;
	case SIGINT:
	case SIGTERM:
		break;
	case SIGCHLD:
//		wait(NULL);
		break;
	default:
		break;
	}

	g.do_exit = 1;
}

void aircurrent_g_free(AIRCUR_G* g)
{
	if (g->nodes_info) {
		nodes_info_free(g->nodes_info);
		free(g->nodes_info);
		g->nodes_info = NULL;
	}
	close_wifi(g->wi);
}

void aircurrent_g_init(AIRCUR_G *g)
{
	console_log_disable();
	memset(g, 0, sizeof(AIRCUR_G));
	{
		g->ap_lifetime = 60;
		g->st_lifetime = 60;
	}
	{
		g->nodes_info = (NODES_INFO*)malloc(sizeof(NODES_INFO));
		memset(g->nodes_info, 0, sizeof(NODES_INFO));
		nodes_info_init(g->nodes_info);
	}
}

static void* channel_hop_thread(void *arg)
{
	AIRCUR_G *g = (AIRCUR_G *)arg;
	int ch_idx = 0;
	static uint8_t all_channels[] = {
			1,2,3,4,5,6,7,8,9,10,11,12,13,14,
			36,37,38,39,40,41,42,43,44,45,46,47,48,52,53,54,55,56,57,58,59,60,64,100,104,108,112,116,120,124,128,132,136,140,149,150,152,153,157,161,165,160
	};

	while (g->do_exit == 0)
	{
		if (wi_set_channel(g->wi, all_channels[ch_idx])) {
			clc("Error!!! change channel [%d]", all_channels[ch_idx]);
		} else {
			g->nodes_info->cur_channel = all_channels[ch_idx];

			usleep(10 * 1000);
			if (g->do_exit) break;
		}

		ch_idx++;
		ch_idx = ch_idx % sizeof(all_channels);
	}
	return 0;
}

static void aircurrent_update(void *data, void *arg)
{
	AP_INFO *ap = (AP_INFO *)data;
	// power clear
	ap->avg_power = -1;
	ap->best_power = -1;
	ap->power_index = -1;
}

static void aircurrent_repotring(AIRCUR_G *g)
{
	int i;
	uint8_t *buf;
	int len, len_be, total_len;
	struct xml_buffer_list ap_xml_buflist;
	struct xml_buffer_list st_xml_buflist;
	int bytes;

	hash_ctx_t *ap_hctx = g->nodes_info->ap_hctx;
	hash_ctx_t *st_hctx = g->nodes_info->st_hctx;

	check_expired_ap(ap_hctx, g->ap_lifetime);
	check_expired_st(st_hctx, g->st_lifetime);

	if (1) {
		static const char *xml_start = "<aircurrent>";
		const int xml_startlen = strlen(xml_start);
		static const char *xml_end = "</aircurrent>";
		const int xml_endlen = strlen(xml_end);

		if (ap_hctx->num_elements > 0) {
			xml_buffer_list_init(&ap_xml_buflist, ap_hctx->num_elements);
			ap_hctx->op->hash_dosomething = aircurrent_xml_aplist_report;
			dosomething_hash(ap_hctx, &ap_xml_buflist);
		}
		if (st_hctx->num_elements > 0) {
			xml_buffer_list_init(&st_xml_buflist, st_hctx->num_elements);
			st_hctx->op->hash_dosomething = aircurrent_xml_stlist_report;
			dosomething_hash(st_hctx, &st_xml_buflist);
		}

		if ((ap_hctx->num_elements > 0) || (st_hctx->num_elements > 0)) {
			// send total length
			total_len = ap_xml_buflist.total_len + st_xml_buflist.total_len +
					xml_startlen + xml_endlen;
			len_be = htonl(total_len);
			bytes = aircurrent_send(&ar_sock, (uint8_t*)&len_be, sizeof(len_be));
			if (bytes <= 0) {
				clc("Error!!! send aircurrent data length %u (send: %d)", total_len, bytes);
			}

			// send aircurrent data
			bytes = aircurrent_send(&ar_sock, (uint8_t*)xml_start, xml_startlen);
			if (bytes <= 0) {
				clc("Error!!! send aircurrent xml start (%s)", xml_start);
			}
			{
				// send ap
				for (i=0; i<ap_xml_buflist.buf_seq; i++) {
					buf = ap_xml_buflist.buf[i];
					len = ap_xml_buflist.len[i];
					bytes = aircurrent_send(&ar_sock, buf, len);
				}
				// send sta
				for (i=0; i<st_xml_buflist.buf_seq; i++) {
					buf = st_xml_buflist.buf[i];
					len = st_xml_buflist.len[i];
					bytes = aircurrent_send(&ar_sock, buf, len);
				}
			}
			bytes = aircurrent_send(&ar_sock, (uint8_t*)xml_end, xml_endlen);
			if (bytes <= 0) {
				clc("Error!!! send aircurrent xml end (%s)", xml_end);
			}

		}
		if (ap_hctx->num_elements > 0) {
			xml_buffer_list_free(&ap_xml_buflist);
		}
		if (st_hctx->num_elements > 0) {
			xml_buffer_list_free(&st_xml_buflist);
		}
	}

	if (0) {
		ap_hctx->op->hash_dosomething = aircurrent_update;
		dosomething_hash(ap_hctx, NULL);
	}

#if 1
	printf("+++ AP reporting +++\n");
	{
		debug_hash2(g->nodes_info->ap_hctx);
		printf("\n\n\n");
	}
	printf("+++ STA reporting +++\n");
	{
		debug_hash2(g->nodes_info->st_hctx);
	}
#endif
}


int main(int argc, char** argv)
{
	int opt = 0, ret = 0;
	int fd;
	time_t t1, t2;
	fd_set rfds;
	struct timeval wait_tv;
	struct rx_info ri;
	uint8_t capbuf[4096];
	int caplen = 0;
	pthread_t chhop_tid;
	char ifname[MAX_IFACE_NAME] = {0, };

	// initialize global config
	aircurrent_g_init(&g);


	while ((opt = getopt(argc, argv, "i:c:t:a:s:dh?")) != -1) {
			switch (opt)
			{
			case 'i':
				SNP(ifname, "%s", optarg);
				break;
			case 'c':
				g.channel = atoi(optarg);
				break;
			case 't':
				g.report_intv = atoi(optarg);
				break;
			case 'd':
				g.debug = 1;
				console_log_enable();
				break;
			case 'a':
				if (atoi(optarg)) {
					g.ap_lifetime = atoi(optarg);
				}
				break;
			case 's':
				if (atoi(optarg)) {
					g.st_lifetime = atoi(optarg);
				}
				break;
			case 'h':
			case '?':
			default:
				usage(argc, argv);
				goto cleanup;
			}
	}

	if (ifname[0] == 0) {
		clc("missing!!! interface name <-i>\n");
		ret = system("airmon-ng");
		usage(argc, argv);
		goto cleanup;
	}

	g.wi = wi_open(ifname);
	if (!g.wi) {
		clc("failure init aircurrent wifi driver '%s'", ifname);
		goto cleanup;
	}

	if (setuid(getuid()) == -1) {
		clc("failed to setuid - %s", strerror(errno));
		goto cleanup;
	}

	signal(SIGINT, sighandler);
	signal(SIGTERM, sighandler);

	if (g.channel == 0) {
		if (pthread_create(&chhop_tid, NULL, channel_hop_thread, &g)) {
			clc("failed to pthread create - %s", strerror(errno));
			goto cleanup;
		}
	}

	fd = wi_fd(g.wi);

	t1 = time(NULL);

	while (1) {
		t2 = time(NULL);
		if ((t2 - t1) >= g.report_intv) {
			/* aircurrent reporting */
			aircurrent_repotring(&g);

			t1 = t2;
		}
		if (g.do_exit) break;

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

		caplen = wi_read(g.wi, capbuf, sizeof(capbuf), &ri);
		if (caplen <= 0) {
			continue;
		}

		if (air_packet_parse(g.nodes_info, g.channel, capbuf, caplen, &ri) == -1) {
			cle("Error!!! parsing air packet");
		}
	}

	if (g.channel == 0) {
		pthread_join(chhop_tid, NULL);
	}

cleanup:
	aircurrent_g_free(&g);
	aircurrent_close(&ar_sock);
	return 0;
}
