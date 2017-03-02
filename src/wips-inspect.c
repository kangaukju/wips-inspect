/*
 * wips-inspect.c
 *
 *  Created on: 2016. 12. 18.
 *      Author: root
 */

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
#include <pthread.h>
#include <getopt.h>
#include <libgen.h>

#include "ezxml/ezxml.h"
#include "pcap.h"
#include "airshooter.h"
#include "aircapture.h"
#include "env.h"
#include "airsock.h"

static AIRSHOOT_G shooter_g;
static AIRCAP_G   capture_g;
static AIR_ENV env;
static int do_exit;

void usage(int argc, char** argv)
{
	printf("\n");
	printf("Usage: wips-inspect <options>\n");
	printf("Options:\n");
	printf("\t -s <shooter ifname> : shooter interface name\n");
	printf("\t -c <capture ifname> : capture interface name\n");
	printf("\t -t <timer>          : shooter/capture time second (default: 0 is infinite)\n");
	printf("\t -p <profile id>     : profile id\n");
	printf("\t -d                  : debug\n");
	printf("\t -h -?               : help\n");
	printf("\n");
}

void set_debug() {
	shooter_g.debug = 1;
	capture_g.debug = 1;
	console_log_enable();
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
	shooter_g.do_exit = 1;
	capture_g.do_exit = 1;
	do_exit = 1;
}

static int airresult_open()
{
	int csock;

	csock = connect_t(NULL, "127.0.0.1", AIRRESULT_CAPTURE_PORT, 0);
	if (csock < 0) {
		return -1;
	}
	set_rst_close(csock); // SO_LINGER
	return csock;
}

static void airresult_close(int *csock)
{
	if (!csock) return;

	if (*csock > 0) {
		close(*csock);
	}
	*csock = -1;
}

static int airresult_send(int *csock, uint8_t *data, size_t len)
{
	struct timeval tv;
	int retry = 5;
	int bytes = 0;

	if (!csock) return -1;

	if (*csock < 0) {
		*csock = airresult_open();
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

static unsigned long result_xid = 0;
static int ar_sock = -1;
void result_data_handler(AIRCAP_G *g, void *data)
{
	struct aircap_data *result = (struct aircap_data*)data;
	struct timeval diff;
	char *outxml;
	int outxml_len;
	int len_be;
	int bytes;

	result_xid++;
	diff = diff_timeval(&g->capture_start, &result->capture_tv);

	cli("[%s] [%s/%s], key:%u, xid:%ld, capture %ld.%06ld, elapsed:%ld",
			result->conf->desc,
			result->conf->profileid,
			result->conf->configid,
			result->conf->key,
			result_xid,
			result->capture_tv.tv_sec, result->capture_tv.tv_usec,
			usec2msec(&diff));
	//print_wifi_packet_summary(&result->pkt_summary);


	ezxml_t xml = ezxml_new("aircapture");
	ezxml_t child = ezxml_add_child(xml, "result", 0);
	ezxml_set_attr(child, "profile", strdup(result->conf->profileid));
	ezxml_set_attr(child, "conf",    strdup(result->conf->configid));
	ezxml_set_attr(child, "key",     utoa(result->conf->key, 10));
	ezxml_set_attr(child, "xid",     ltoa(result_xid, 10));
	ezxml_set_attr(child, "sec",     ltoa(result->capture_tv.tv_sec, 10));
	ezxml_set_attr(child, "usec",    ltoa(result->capture_tv.tv_usec, 10));
	ezxml_set_attr(child, "elapsed", ltoa(usec2msec(&diff), 10));
	ezxml_set_attr(child, "pwr",     itoa(result->ri.ri_power, 10));
	ezxml_set_attr(child, "type",    itoa(result->summary.type, 10));
	ezxml_set_attr(child, "subtype", itoa(result->summary.subtype, 10));
	ezxml_set_attr(child, "addr1",   mactoa(result->summary.addr1));
	ezxml_set_attr(child, "addr2",   mactoa(result->summary.addr2));
	ezxml_set_attr(child, "addr3",   mactoa(result->summary.addr3));
	if (cv_is_enable(result->summary.addr4))
		ezxml_set_attr(child, "addr4", mactoa(result->summary.addr4));
	ezxml_set_attr(child, "ds",      itoa(result->summary.ds, 10));
	ezxml_set_attr(child, "seq",     itoa(result->summary.seq, 10));
	if (cv_is_enable(result->summary.deauthcode))
		ezxml_set_attr(child, "deauthcode", itoa(result->summary.deauthcode, 10));

	outxml = ezxml_toxml(xml);
	outxml_len = strlen(outxml);
	if (outxml_len > 0) {
		len_be = htonl(outxml_len);
		bytes = airresult_send(&ar_sock, (uint8_t*)&len_be, sizeof(len_be));
		if (bytes <= 0) {
			clc("Error!!! send airresult data length %u (send: %d)", outxml_len, bytes);
		}
		bytes = airresult_send(&ar_sock, (uint8_t*)outxml, outxml_len);
		if (bytes <= 0) {
			clc("Error!!! send airresult data (send: %d)", bytes);
		}
		cld("%s\n", outxml);
	}
	free(outxml);
	ezxml_free(xml);
}

void airshooter_thread(void *arg)
{
	struct timeval last_wait;

	airshoot_shoot(&shooter_g, NULL);

	last_wait = msec2usec(max_dwell(shooter_g.conf) * 1000);
	select (0 ,NULL, NULL, NULL, &last_wait);

	// stop capture after delay
	capture_g.do_exit = 1;
}

void aircapture_thread(void *arg)
{
	result_xid = 0;
	aircap_matching(&capture_g, result_data_handler);
}

static void airconf_assign_profile_config(struct airconf *to, struct sqlairconf *from)
{
	struct airconf *cur;
	if (!to) return;
	for (cur = to; cur; cur = cur->next) {
		SNP(cur->profileid, "%s", from->profileid);
		SNP(cur->configid,  "%s", from->configid);
	}
}

int main(int argc, char *argv[])
{
	int opt = 0;
	int index = 0;
	int ret = 0;
	pthread_t shooter_tid, capture_tid;
	cv_arr_def(profileid, char, 128);
	struct sqlairconf *sqlairconf = NULL, *cur;
	char xmlconf[128];
	struct airconf *conf;
	char shooter_ifname[MAX_IFACE_NAME] = {0, };
	char capture_ifname[MAX_IFACE_NAME] = {0, };

	memset(profileid, 0, sizeof(profileid));
	cv_set_disable(profileid);

	// initialize global config
	airshoot_g_init(&shooter_g);
	aircap_g_init(&capture_g);

	while ((opt = getopt(argc, argv, "s:c:p:t:vdh?")) != -1) {
		switch (opt)
		{
		case 'p':
			SNP(profileid, "%s", optarg);
			cv_set_enable(profileid);
			break;
		case 's':
			SNP(shooter_ifname, "%s", optarg);
			break;
		case 'c':
			SNP(capture_ifname, "%s", optarg);
			break;
		case 't':
			shooter_g.shooter_timer = atoi(optarg);
			//  여기서 capture 타이머를 설정하지 않고 shooter가 완료된 후 capture를 종료하도록 한다.
//			capture_g.capture_timer = atoi(optarg);
			break;
		case 'd':
			set_debug();
			break;
		case 'h':
		case '?':
		default:
			ret = system("airmon-ng");
			usage(argc, argv);
			goto cleanup;
		}
	}

	// load airshoot global config
	if (get_air_env(&env)) {
		clc("failure load env");
		goto cleanup;
	}

	// check ifname
	if (shooter_ifname[0] == 0) {
		clc("missing!!! shooter-wif\n");
		ret = system("airmon-ng");
		usage(argc, argv);
		goto cleanup;
	}
	if (capture_ifname[0] == 0) {
		clc("missing!!! capture-wif\n");
		ret = system("airmon-ng");
		usage(argc, argv);
		goto cleanup;
	}

	// initialize wifi device
	shooter_g.wi = wi_open(shooter_ifname);
	if (!shooter_g.wi) {
		clc("failure init shooter wifi driver '%s'", shooter_ifname);
		goto cleanup;
	}
	capture_g.wi = wi_open(capture_ifname);
	if (!capture_g.wi) {
		clc("failure init capture wifi driver '%s'", capture_ifname);
		goto cleanup;
	}

	// check profile id
	if (!cv_is_enable(profileid)) {
		clc("missing!!! profile id\n");
		usage(argc, argv);
		goto cleanup;
	}

	sqlairconf = sqlairconf_load(env.PROFILE_DB, profileid);
	if (!sqlairconf) {
		clc("failure load config of profile [%s] from sqlite3\n", profileid);
		goto cleanup;
	}

	for (cur = sqlairconf; cur; cur = cur->next) {
		// test capture airconf
		SNP(xmlconf, "%s/%s", env.CONF_HOME, cur->capturexml);
		conf = airconf_from_file(xmlconf, capture_g.debug);
		if (!conf) {
			clc("Error!!! load '%s' of profile: %s\n", cur->capturexml, cur->profileid);
			airconf_free(conf);
			goto cleanup;
		}
		airconf_free(conf);

		// test shooter airconf
		SNP(xmlconf, "%s/%s", env.CONF_HOME, cur->shooterxml);
		conf = airconf_from_file(xmlconf, shooter_g.debug);
		if (!conf) {
			clc("Error!!! load '%s' of profile: %s\n", cur->shooterxml, cur->profileid);
			airconf_free(conf);
			goto cleanup;
		}
		airconf_free(conf);
	}

	if (setuid(getuid()) == -1) {
		clc("failed to setuid - %s", strerror(errno));
		goto cleanup;
	}

	signal(SIGINT, sighandler);
	signal(SIGTERM, sighandler);

	if (write_cur_pid(basename(argv[0]))) {
		clc("'%s' failed to write pid - %s\n", basename(argv[0]), strerror(errno));
		goto cleanup;
	}

	for (cur = sqlairconf; cur; cur = cur->next) {
		// capture할 airconf 설정을 불러온다 (shooter에 대한 capture도 포함한다.)
		SNP(xmlconf, "%s/%s", env.CONF_HOME, cur->capturexml);
		capture_g.conf = airconf_from_file(xmlconf, 0);
		SNP(xmlconf, "%s/%s", env.CONF_HOME, cur->shooterxml);
		airconf_last(capture_g.conf)->next = airconf_from_file(xmlconf, 0);

		// capture에 다중 채널인 경우 max dwell 시간을 채널별로 보정한다
		dwell_coordination(capture_g.conf, &capture_g.dwell_list);

		// pcap 파일 디렉토리 생성 위치
		SNP(capture_g.pcap_file, "%s/%s/%s/", env.PCAP_HOME, cur->profileid, cur->configid);
		if (mkdir_r(capture_g.pcap_file)) {
			clc("Error!!! can't create pcap directory - %s (%s)\n", capture_g.pcap_file, strerror(errno));
		} else {
			//  initialize pcap - pcap file open
			SNP(capture_g.pcap_file, "%s/%s/%s/%ld.pcap",
					env.PCAP_HOME,
					cur->profileid,
					cur->configid,
					time(NULL));
			capture_g.pcap_fp = new_pcap_file(capture_g.pcap_file);
			if (!capture_g.pcap_fp) {
				clc("Error!!! initialize pcap file - %s\n", capture_g.pcap_file);
			}
		}

		// shoot할 airconf 설정을 불러온다
		shooter_g.conf = airconf_from_file(xmlconf, 0);

		// airconf 기본 값 복사
		airconf_assign_profile_config(capture_g.conf, cur);
		airconf_assign_profile_config(shooter_g.conf, cur);

		// 설정값 초기화
		shooter_g.do_exit = capture_g.do_exit = 0;

		// capture thread
		if (pthread_create(&capture_tid, NULL, (void*)aircapture_thread, &capture_g) != 0) {
			clc("failed to create capture pthread - %s\n", strerror(errno));
			goto cleanup;
		}
		// shooter thread
		if (pthread_create(&shooter_tid, NULL, (void*)airshooter_thread, &shooter_g) != 0) {
			clc("failed to create shooter pthread - %s\n", strerror(errno));
			goto cleanup;
		}

		pthread_join(capture_tid, NULL);
		pthread_join(shooter_tid, NULL);

		// close pcap
		fclose(capture_g.pcap_fp);
		capture_g.pcap_fp = NULL;

		// airconf 설정 메모리 해제
		airconf_free(capture_g.conf);
		capture_g.conf = NULL;
		airconf_free(shooter_g.conf);
		shooter_g.conf = NULL;

		if (do_exit) {
			break;
		}
	}

cleanup:
	sqlairconf_free(sqlairconf);
	airshoot_g_free(&shooter_g);
	aircap_g_free(&capture_g);
	airresult_close(&ar_sock);
	return 0;
}
