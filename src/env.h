/*
 * env.h
 *
 *  Created on: 2017. 2. 7.
 *      Author: root
 */

#ifndef SRC_ENV_H_
#define SRC_ENV_H_


typedef struct _AIR_ENV {
	char DB_HOME[128];
	char CONF_HOME[128];
	char PCAP_HOME[128];
	char PROFILE_DB[128];
} AIR_ENV;

int get_air_env(AIR_ENV *env);

#endif /* SRC_ENV_H_ */
