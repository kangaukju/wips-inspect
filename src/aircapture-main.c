/*
 * aircapture-main.c
 *
 *  Created on: 2016. 12. 16.
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

#include "ezxml/ezxml.h"
#include "llist.h"
#include "aircapture.h"
#include "airsock.h"
#include "env.h"

static AIRCAP_G g;

void usage(int argc, char** argv)
{
//	print_args(argc, argv);

	printf("\n");
	printf("Usage: aircapture\n");
	printf("options:\n\n");
	printf("\t -i <iface>        : Wi-Fi Interface name\n");
	printf("\t -f <config>       : air config XML file\n");
	printf("\t -r <dir>          : air config directory\n");
	printf("\t -v <verify>       : verify config content\n");
	printf("\t -t <capture time> : capture time second (default: 0 is infinite)\n");
	printf("\t -p <profile id>   : profile id\n");
	printf("\t -d                : debug\n");
	printf("\t -h -?             : help\n");
	printf("\n");
}

void sighandler(int signum)
{
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

static unsigned long result_xid = 0;
void result_data_handler(AIRCAP_G *g, void *data)
{
	struct aircap_data *result = (struct aircap_data*)data;
	struct timeval diff;
	char *outxml;
	int outxml_len;

	result_xid++;
	diff = diff_timeval(&g->capture_start, &result->capture_tv);

	cli("[%s] xid:%ld, capture %ld.%06ld, elapsed:%ld",
			result->conf->desc,
			result_xid,
			result->capture_tv.tv_sec, result->capture_tv.tv_usec,
			usec2msec(&diff));
	aircap_summary_print(&result->summary);
}

int main(int argc, char **argv)
{
	int conf_verify = 0;
	int opt = 0;
	int ret = 0;
	int i = 0;
	AIR_ENV env;
	char xmlconf[128];
	struct sqlairconf *sqlairconf = NULL, *cur;
	cv_arr_def(profileid, char, 128);
	char ifname[MAX_IFACE_NAME] = {0, };

	memset(profileid, 0, sizeof(profileid));
	cv_set_disable(profileid);

	// initialize global config
	aircap_g_init(&g);

	while ((opt = getopt(argc, argv, "i:r:f:t:p:vdh?")) != -1) {
		switch (opt)
		{
		case 'p':
			SNP(profileid, "%s", optarg);
			cv_set_enable(profileid);
			break;
		case 'i':
			SNP(ifname, "%s", optarg);
			break;
		case 'r':
			SNP(g.confdir, "%s", optarg);
			break;
		case 'f':
			SNP(g.conffile, "%s", optarg);
			break;
		case 't':
			g.capture_timer = atoi(optarg);
			break;
		case 'v':
			conf_verify = 1;
			break;
		case 'd':
			g.debug = 1;
			console_log_enable();
			break;
		case 'h':
		case '?':
		default:
			if (ifname[0] == 0) {
				ret = system("airmon-ng");
			}
			usage(argc, argv);
			goto cleanup;
		}
	}

	// load airshoot global config
	if (get_air_env(&env)) {
		clc("failure load env");
		goto cleanup;
	}

	if (ifname[0] == 0) {
		fprintf(stderr, "missing!!! interface name <-i>\n");
		ret = system("airmon-ng");
		usage(argc, argv);
		goto cleanup;
	}

	// initialize wifi device
	g.wi = wi_open(ifname);
	if (!g.wi) {
		clc("failure init wifi driver '%s'", ifname);
		goto cleanup;
	}

	// verify config file
	if (conf_verify) {
		aircap_conf_verify(&g);
		goto cleanup;
	}

	if (cv_is_enable(profileid)) {
		sqlairconf = sqlairconf_load(env.PROFILE_DB, profileid);
		if (!sqlairconf) {
			clc("failure load config of profile [%s] from sqlite3", profileid);
			goto cleanup;
		}

		if (conf_verify) {
			for (cur = sqlairconf; cur; cur = cur->next) {
				// check verify capture xml
				SNP(g.conffile, "%s", cur->capturexml);
				aircap_conf_verify(&g);
				// check verify shooter xml
				SNP(g.conffile, "%s", cur->shooterxml);
				aircap_conf_verify(&g);
			}
			goto cleanup;
		}

		// test load airconf xml file
		for (cur = sqlairconf; cur; cur = cur->next) {
			SNP(xmlconf, "%s/%s", env.CONF_HOME, cur->capturexml);
			g.conf = airconf_from_file(xmlconf, g.debug);
			SNP(xmlconf, "%s/%s", env.CONF_HOME, cur->shooterxml);
			airconf_last(g.conf)->next = airconf_from_file(xmlconf, g.debug);

			if (!g.conf) {
				clc("Error!!! load config profile: %s capture: %s", cur->profileid, cur->configid);
				goto cleanup;
			}
			airconf_free(g.conf);
			g.conf = NULL;
		}

		if (setuid(getuid()) == -1) {
			clc("failed to setuid - %s", strerror(errno));
			goto cleanup;
		}

		signal(SIGINT, sighandler);
		signal(SIGTERM, sighandler);

		for (cur = sqlairconf; cur; cur = cur->next) {
			if (g.do_exit) {
				break;
			}
			// capture할 airconf 설정을 불러온다 (shooter에 대한 capture도 포함한다.)
			SNP(xmlconf, "%s/%s", env.CONF_HOME, cur->capturexml);
			g.conf = airconf_from_file(xmlconf, 0);
			SNP(xmlconf, "%s/%s", env.CONF_HOME, cur->shooterxml);
			airconf_last(g.conf)->next = airconf_from_file(xmlconf, 0);

			// capture에 다중 채널인 경우 max dwell 시간을 채널별로 보정한다
			dwell_coordination(g.conf, &g.dwell_list);

			// pcap 파일 디렉토리 생성 위치
			SNP(g.pcap_file, "%s/%s/%s", env.PCAP_HOME, cur->profileid, cur->configid);
			if (mkdir_r(g.pcap_file)) {
				clc("Error!!! can't create pcap directory - %s (%s)", g.pcap_file, strerror(errno));
			} else {
				//  initialize pcap - pcap file open
				SNP(g.pcap_file, "%s/%s/%s/%ld.pcap",
						env.PCAP_HOME,
						cur->profileid,
						cur->configid,
						time(NULL));
				g.pcap_fp = new_pcap_file(g.pcap_file);
				if (!g.pcap_fp) {
					clc("Error!!! initialize pcap file - %s\n", g.pcap_file);
				}
			}

			// long time process
			cli("start capture profile: %s, capture: %s", cur->profileid, cur->configid);

			aircap_matching(&g, result_data_handler);

			// close pcap
			fclose(g.pcap_fp);

			cli("pcap write to [%s]\n", g.pcap_file);

			// airconf 설정 메모리 해제
			airconf_free(g.conf);
			g.conf = NULL;
		}
	}
	else {
		// single airconf file
		if (g.conffile[0]) {
			char *tok;
			tok = strtok(g.conffile, " ");
			g.conf = airconf_from_file(tok, g.debug);

			while ((tok = strtok(NULL, " "))) {
				if (g.conf) {
					airconf_last(g.conf)->next = airconf_from_file(tok, g.debug);
				}
			}
		}
		// directory airconf files
		else if (g.confdir[0]) {
			if (airconf_from_path(&g.conf, g.confdir, g.debug) == -1) {
				goto cleanup;
			}
		}

		if (!g.conf) {
			usage(argc, argv);
			goto cleanup;
		}

		dwell_coordination(g.conf, &g.dwell_list);

		if (setuid(getuid()) == -1) {
			clc("failed to setuid - %s", strerror(errno));
			goto cleanup;
		}

		signal(SIGINT, sighandler);
		signal(SIGTERM, sighandler);

		ret = aircap_matching(&g, result_data_handler);
	}

cleanup:
	aircap_g_free(&g);
	sqlairconf_free(sqlairconf);
	return ret;
}
