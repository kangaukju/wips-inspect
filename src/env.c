/*
 * env.c
 *
 *  Created on: 2017. 2. 7.
 *      Author: root
 */

#include <stdio.h>
#include <stdlib.h>

#include "env.h"
#include "common.h"

int get_air_env(AIR_ENV *env)
{
	if (!getenv("DB_HOME")) {
		fprintf(stderr, "DB_HOME is not defined\n");
		return -1;
	}
	if (!getenv("CONF_HOME")) {
		fprintf(stderr, "CONF_HOME is not defined\n");
		return -1;
	}
	if (!getenv("PCAP_HOME")) {
		fprintf(stderr, "PCAP_HOME is not defined\n");
		return -1;
	}

	SNP(env->DB_HOME,    "%s",    getenv("DB_HOME"));
	SNP(env->CONF_HOME,  "%s",    getenv("CONF_HOME"));
	SNP(env->PCAP_HOME,  "%s",    getenv("PCAP_HOME"));
	SNP(env->PROFILE_DB, "%s/%s", env->DB_HOME, "profiles.db");

	/*
	printf("DB_HOME=%s\n", env->DB_HOME);
	printf("CONF_HOME=%s\n", env->CONF_HOME);
	printf("PCAP_HOME=%s\n", env->PCAP_HOME);
	printf("PROFILE_DB=%s\n", env->PROFILE_DB);
	*/
	return 0;
}

