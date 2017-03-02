/*
 * airshooter-main.c
 *
 *  Created on: 2016. 12. 17.
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
#include "airshooter.h"
#include "env.h"

static AIRSHOOT_G g;

void usage(const char *p)
{
	if (p) {}
	printf("\n");
	printf("Usage: airshooter <options>\n");
	printf("Options:\n\n");
	printf("\t -i <iface>          : Wi-Fi Interface name\n");
	printf("\t -f <config>         : air config XML file\n");
	printf("\t -r <dir>            : air config directory\n");
	printf("\t -v <verify>         : verify config content\n");
	printf("\t -t <shoot timer>    : shooter timer second (default: 0 is infinite)\n");
	printf("\t -d                  : debug\n");
	printf("\t -h -?               : help\n");
	printf("\n");
}

void sighandler(int signum)
{
//	fprintf(stderr, "catch signal %d\n", signum);
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

int main(int argc, char *argv[])
{
	int opt = 0;
	int conf_verify = 0;
	int ret = 0;
	char ifname[MAX_IFACE_NAME] = {0, };

	// initialize global config
	airshoot_g_init(&g);

	while ((opt = getopt(argc, argv, "i:r:f:t:vdh?")) != -1) {
		switch (opt)
		{
		case 'i':
			SNP(ifname, "%s", optarg);
			break;
		case 'f':
			SNP(g.conffile, "%s", optarg);
			break;
		case 't':
			g.shooter_timer = atoi(optarg);
			break;
		case 'v':
			conf_verify = 1;
			break;
		case 'r':
			snprintf(g.confdir, sizeof(g.confdir), "%s", optarg);
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
			usage(argv[0]);
			goto cleanup;
		}
	}

	if (ifname[0] == 0) {
		fprintf(stderr, "missing!!! interface name <-i>\n");
		ret = system("airmon-ng");
		usage(argv[0]);
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
		airshoot_conf_verify(&g);
		goto cleanup;
	}

	// single airconf file
	if (g.conffile[0]) {
		g.conf = airconf_from_file(g.conffile, g.debug);
	}
	// directory airconf files
	else if (g.confdir[0]) {
		if (airconf_from_path(&g.conf, g.confdir, g.debug) == -1) {
			goto cleanup;
		}
	}

	if (!g.conf) {
		usage(argv[0]);
		goto cleanup;
	}

	if (setuid(getuid()) == -1) {
		clc("failed to setuid - %s", strerror(errno));
		goto cleanup;
	}

	signal(SIGINT,  sighandler);
	signal(SIGTERM, sighandler);

	ret = airshoot_shoot(&g, NULL);

cleanup:
	airshoot_g_free(&g);
	return ret;
}

