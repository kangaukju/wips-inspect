/*
 * airsock.cpp
 *
 *  Created on: 2015. 5. 4.
 *      Author: root
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <signal.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <sys/un.h>
#include "airsock.h"


#define EAGAIN_DELAY  100


/**
 * @brief recv_t (recv_tcp)
 * @return -1: error, 0: timeout,  0<: success
 */
int recv_t(int sock, uint8_t *buf, size_t buflen, struct timeval wait, int flag)
{
	int ret = -1;
	fd_set	rset;
	struct timeval tv;
	size_t n = 0;
	int bytes = 0;

	tv.tv_sec = wait.tv_sec;
	tv.tv_usec = wait.tv_usec;
RE_RECV:
	if (tv.tv_sec || tv.tv_usec) {
		FD_ZERO(&rset);
		FD_SET(sock, &rset);
		ret = select(sock+1, &rset, (fd_set*)0, (fd_set*)0, &tv);
		if (ret == -1) {
			if (errno == EINTR) {
				goto RE_RECV;
			}
			return -1;
		} else if (ret == 0) {
			return 0;
		}
		if (!FD_ISSET(sock, &rset)) {
			return -1;
		}
		do {
			bytes = recv(sock, buf+n, buflen-n, MSG_DONTWAIT | flag);
			if (bytes <= 0) {
				if (errno == EINTR || errno == EAGAIN) {
					usleep(EAGAIN_DELAY);
					continue;
				}
				return -1;
			}
			n += bytes;
		} while (n < buflen);
		return n;
	}
	do {
		bytes = recv(sock, buf+n, buflen-n, flag);
		if (bytes <= 0) {
			if (errno == EINTR || errno == EAGAIN) {
				usleep(EAGAIN_DELAY);
				continue;
			}
			return -1;
		}
		n += bytes;
	} while (n < buflen);
	return n;
}

/**
 * @brief recv_u (recv_udp)
 * @return -1: error, 0: timeout, 0<: success
 */
int recv_u(int sock, uint8_t *buf, size_t buflen, struct timeval wait)
{
	return recvfrom_u(sock, buf, buflen, wait, NULL);
}
int recvfrom_u(int sock, uint8_t *buf, size_t buflen, struct timeval wait, struct sockaddr_in *from)
{
	int bytes = 0;
	int ret = -1;
	fd_set	rset;
	struct timeval tv;
	static struct sockaddr_in clientaddr;
	static socklen_t clilen = sizeof(clientaddr);

	tv.tv_sec = wait.tv_sec;
	tv.tv_usec = wait.tv_usec;
RE_RECV:
	if (tv.tv_sec || tv.tv_usec) {
		FD_ZERO(&rset);
		FD_SET(sock, &rset);
		ret = select(sock+1, &rset, (fd_set*)0, (fd_set*)0, &tv);
		if (ret == -1) {
			if (errno == EINTR) {
				goto RE_RECV;
			}
			return -1;
		} else if (ret == 0) {
			return 0;
		}
		if (!FD_ISSET(sock, &rset)) {
			return -1;
		}
		bytes = recvfrom(sock, buf, buflen, MSG_DONTWAIT,
				from ?
						(struct sockaddr *)from :
						(struct sockaddr *)&clientaddr, &clilen);
		if ( bytes <= 0) {
			return -1;
		}
		return bytes;
	}
	bytes = recvfrom(sock, buf, buflen, 0,
			from ?
					(struct sockaddr *)from :
					(struct sockaddr *)&clientaddr, &clilen);
	if (bytes <= 0) {
		return -1;
	}
	return bytes;
}

/**
 * @brief send_t (send_tcp)
 * @return -1: error, 0: timeout, 0<: success
 */
int send_t(int sock, uint8_t *buf, size_t buflen, struct timeval wait, int flag)
{
	int ret = -1;
	fd_set	wset;
	struct timeval tv;
	size_t n = 0;
	int bytes = 0;

	if (wait.tv_sec && wait.tv_usec) {
		tv.tv_sec = wait.tv_sec;
		tv.tv_usec = wait.tv_usec;
RE_SEND:
		FD_ZERO(&wset);
		FD_SET(sock, &wset);
		ret = select(sock+1, (fd_set*)0, &wset, (fd_set*)0, &tv);
		if (ret == -1) {
			if (errno == EINTR) {
				goto RE_SEND;
			}
			return -1;
		} else if (ret == 0) {
			return 0;
		}
		if (!FD_ISSET(sock, &wset)) {
			return -1;
		}
		do {
			bytes = send(sock, buf+n, buflen-n, MSG_DONTWAIT | flag);
			if (bytes <= 0) {
				if (errno == EINTR || errno == EAGAIN) {
					usleep(EAGAIN_DELAY);
					continue;
				}
				return -1;
			}
			n += bytes;
		} while (n < buflen);
		return n;
	}
	do {
		bytes = send(sock, buf+n, buflen-n, flag);
		if (bytes <= 0) {
			if (errno == EINTR || errno == EAGAIN) {
				usleep(EAGAIN_DELAY);
				continue;
			}
			return -1;
		}
		n += bytes;
	} while (n < buflen);
	return n;
}

/**
 * @brief send_u (send_udp)
 * @return -1: error, 0: timeout, 0<: success
 */
int send_u(int sock, uint8_t *buf, size_t buflen, struct timeval wait)
{
	return sendto_u(sock, buf, buflen, wait, NULL);
}
int sendto_u(int sock, uint8_t *buf, size_t buflen, struct timeval wait, struct sockaddr_in *to)
{
	int bytes = 0;
	int ret = -1;
	fd_set	wset;
	struct timeval tv;
	static socklen_t svrlen = sizeof(*to);

	if (wait.tv_sec && wait.tv_usec) {
		tv.tv_sec = wait.tv_sec;
		tv.tv_usec = wait.tv_usec;
RE_SEND:
		FD_ZERO(&wset);
		FD_SET(sock, &wset);
		ret = select(sock+1, (fd_set*)0, &wset, (fd_set*)0, &tv);
		if (ret == -1) {
			if (errno == EINTR) {
				goto RE_SEND;
			}
			return -1;
		}
		else if (ret == 0) {
			return 0;
		}
		if (!FD_ISSET(sock, &wset)) {
			return -1;
		}
		bytes = sendto(sock, buf, buflen, MSG_DONTWAIT,
				(struct sockaddr *)to, svrlen);
		if (bytes <= 0) {
			return -1;
		}
		return bytes;
	}
	bytes = sendto(sock, buf, buflen, 0,
			(struct sockaddr *)to, svrlen);
	if (bytes <= 0) {
		return -1;
	}
	return bytes;
}

/**
 * @brief connect_nb: connect non-block
 * @return -1: error, 0: timeout, 1: success
 */
static int connect_nb(int sock, struct sockaddr *saddr, int saddr_size, int sec)
{
	int new_stat = 0;
	int old_stat = 0;
	int res = 0, n = 0;
	fd_set rset, wset;
	struct timeval tv;
	int error = 0;
	int esize = 0;

	if ((new_stat = fcntl(sock, F_GETFL, NULL)) < 0) {
		return -1;
	}

	old_stat = new_stat;
	new_stat |= O_NONBLOCK;

	if (fcntl(sock, F_SETFL, new_stat) < 0) {
		return -1;
	}
	if ((res = connect(sock, saddr, saddr_size)) < 0) {
		if (errno != EINPROGRESS) {
			return -1;
		}
	}
	if (res == 0) {
		fcntl(sock, F_SETFL, old_stat);
		return 1; // connect success
	}

	FD_ZERO(&rset);
	FD_SET(sock, &rset);
	wset = rset;
	tv.tv_sec = sec;
	tv.tv_usec = 0;

	if ((n = select(sock + 1, &rset, &wset, NULL, &tv)) == 0) {
		errno = ETIMEDOUT;
		return 0; // connect timeout
	}
	if (FD_ISSET(sock, &rset) || FD_ISSET(sock, &wset)) {
		esize = sizeof(int);
		if ((n = getsockopt(sock, SOL_SOCKET, SO_ERROR, &error,
				(socklen_t *)&esize)) < 0) {
			return -1;
		}
	}
	else {
		return -1;
	}

	fcntl(sock, F_SETFL, old_stat);
	if (error) {
		errno = error;
		return -1;
	}
	return 1;
}

/**
 * @brief _connect
 */
static int _connect(int sock, const char *src, const char *dst, uint16_t port, int blocking)
{
	static int on = 1;
	struct sockaddr_in sock_addr;
	struct sockaddr_in dst_addr;

	memset(&sock_addr, 0x00, sizeof(sock_addr));
	memset(&dst_addr, 0x00, sizeof(dst_addr));

	if (src && *src) {
		sock_addr.sin_family = AF_INET;
		sock_addr.sin_addr.s_addr = inet_addr(src);
		sock_addr.sin_port = 0;

		setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

		if (bind(sock, (struct sockaddr*) &sock_addr, sizeof(sock_addr)) < 0) {
			close(sock);
			return -1;
		}
	}

	dst_addr.sin_family = AF_INET;
	dst_addr.sin_addr.s_addr = inet_addr(dst);
	dst_addr.sin_port = htons(port);

	if (blocking) {
		if (connect(sock, (struct sockaddr*)&dst_addr, sizeof(dst_addr)) < 0) {
			close(sock);
			return -1;
		}
	}
	else {
		// connect error or connect timeout
		if (connect_nb(sock,
				(struct sockaddr*)&dst_addr, sizeof(dst_addr), 1) <= 0) {
			close(sock);
			return -1;
		}
	}
#if 0
	if (!set_keepalive(sock, 1, 60, 10)) {
		close(sock);
		return -1;
	}
#endif
	return sock;
}


/**
 * @brief _unix_connect
 */
static int _unix_connect(int sock, const char *sockfile, int blocking)
{
	struct sockaddr_un sock_addr;
	struct sockaddr_un dst_addr;

	memset(&sock_addr, 0x00, sizeof(sock_addr));
	memset(&dst_addr, 0x00, sizeof(dst_addr));

	dst_addr.sun_family = AF_LOCAL;
    snprintf(dst_addr.sun_path, sizeof(dst_addr.sun_path), "%s", sockfile);

	if (blocking) {
		if (connect(sock, (struct sockaddr*)&dst_addr, sizeof(dst_addr)) < 0) {
			close(sock);
			return -1;
		}
	}
	else {
		// connect error or connect timeout
		if (connect_nb(sock, (struct sockaddr*)&dst_addr, sizeof(dst_addr), 1) <= 0) {
			close(sock);
			return -1;
		}
	}
#if 0
	// TODO: Operation not supported ????
	if (!set_keepalive(sock, 1, 60, 10)) {
		close(sock);
		return -1;
	}
#endif
	return sock;
}

/**
 * @brief unix_connect_t (tcp)
 */
int unix_connect_t(const char *sockfile, int blocking)
{
	int sock = socket(PF_LOCAL, SOCK_STREAM, 0);
	if (sock < 0) {
		return -1;
	}
	return _unix_connect(sock, sockfile, blocking);
}

/**
 * @brief unix_connect_u (udp)
 */
int unix_connect_u(const char *sockfile, int blocking)
{
	int sock = socket(PF_LOCAL, SOCK_DGRAM, 0);
	if (sock < 0) {
		return -1;
	}
	return _unix_connect(sock, sockfile, blocking);
}

/**
 * @brief connect_t (tcp)
 */
int connect_t(const char *src, const char *dst, uint16_t port, int blocking)
{
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		return -1;
	}
	return _connect(sock, src, dst, port, blocking);
}

/**
 * @brief connect_u (udp)
 */
int connect_u(const char *src, const char *dst,
		uint16_t port, int blocking)
{
	int sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		return -1;
	}
	return _connect(sock, src, dst, port, blocking);
}

/**
 * @brief accept_t (only tcp)
 */
int accept_t(int sock, struct timeval wait, struct sockaddr_in *client)
{
	fd_set	rset;
	struct timeval tv;
	int ret = -1;
	static socklen_t client_len = sizeof(struct sockaddr_in);
	int csock = -1;

	tv.tv_sec = wait.tv_sec;
	tv.tv_usec = wait.tv_usec;
RE_DO:
	if (tv.tv_sec || tv.tv_usec) {
		FD_ZERO(&rset);
		FD_SET(sock, &rset);
		ret = select(sock+1, &rset, (fd_set*)0, (fd_set*)0, &tv);
		if (ret == -1) {
			if (errno == EINTR) {
				goto RE_DO;
			}
			return -1;
		} else if (ret == 0) {
			return 0;
		}
	}
	csock = accept(sock, (struct sockaddr *)client, &client_len);
	if (csock < 0) {
		return -1;
	}
	if (!set_keepalive(csock, 1, 60, 10)) {
		close(csock);
		return -1;
	}
	return csock;
}

/**
 * @brief unix_accept_t (only tcp)
 */
int unix_accept_t(int sock, struct timeval wait, struct sockaddr_un *client)
{
	fd_set	rset;
	struct timeval tv;
	int ret = -1;
	static socklen_t client_len = sizeof(struct sockaddr_un);
	int csock = -1;

	tv.tv_sec = wait.tv_sec;
	tv.tv_usec = wait.tv_usec;
RE_DO:
	if (tv.tv_sec || tv.tv_usec) {
		FD_ZERO(&rset);
		FD_SET(sock, &rset);
		ret = select(sock+1, &rset, (fd_set*)0, (fd_set*)0, &tv);
		if (ret == -1) {
			if (errno == EINTR) {
				goto RE_DO;
			}
			return -1;
		} else if (ret == 0) {
			return 0;
		}
	}
	csock = accept(sock, (struct sockaddr *)client, &client_len);
	if (csock < 0) {
		return -1;
	}
#if 0
	// TODO: Operation not supported ????
	if (!set_keepalive(csock, 1, 60, 10)) {
		close(csock);
		return -1;
	}
#endif
	return csock;
}

/**
 * @brief create_unix_server_t (tcp)
 */
int unix_server_t(const char* sockfile, int blocking)
{
	int server_sock = 0;
	int on = 1;
	int stat = 0;
	struct sockaddr_un serveraddr;

	server_sock = socket(AF_UNIX/*PF_LOCAL*/, SOCK_STREAM, 0);
	if (server_sock < 0) {
		return -1;
	}
	{
		on = 1;
		setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	}
	if (!blocking) {
		stat = fcntl(server_sock, F_GETFL, NULL);
		stat |= O_NONBLOCK;
		fcntl(server_sock, F_SETFL, stat);
	}

	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sun_family = AF_LOCAL;
	snprintf(serveraddr.sun_path, sizeof(serveraddr.sun_path), "%s", sockfile);

	if (bind(server_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0) {
		close(server_sock);
		return -1;
	}
	if (listen(server_sock, 5) < 0) {
		close(server_sock);
		return -1;
	}
	signal(SIGPIPE, SIG_IGN);

	return server_sock;
}

/**
 * @brief create_unix_server_u (udp)
 */
int unix_server_u(const char* sockfile, int blocking)
{
	int server_sock = 0;
	int on = 1;
	int stat = 0;
	struct sockaddr_un serveraddr;

	server_sock = socket(AF_UNIX/*PF_LOCAL*/, SOCK_DGRAM, 0);
	if (server_sock < 0) {
		return -1;
	}
	{
		on = 1;
		setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	}
	if (!blocking) {
		stat = fcntl(server_sock, F_GETFL, NULL);
		stat |= O_NONBLOCK;
		fcntl(server_sock, F_SETFL, stat);
	}

	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sun_family = AF_LOCAL;
    snprintf(serveraddr.sun_path, sizeof(serveraddr.sun_path), "%s", sockfile);

	if (bind(server_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0) {
		close(server_sock);
		return -1;
	}
	signal(SIGPIPE, SIG_IGN);

	return server_sock;
}

int server_t(const char* ipaddr, int port, int blocking)
{
	int server_sock = 0;
	int on = 1;
	int stat = 0;
	struct sockaddr_in serveraddr;

	server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (server_sock < 0) {
		return -1;
	}
	{
		on = 1;
		setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	}
	if (!blocking) {
		stat = fcntl(server_sock, F_GETFL, NULL);
		stat |= O_NONBLOCK;
		fcntl(server_sock, F_SETFL, stat);
	}

	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	if (!ipaddr || !ipaddr[0]) {
		serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	} else {
		serveraddr.sin_addr.s_addr = inet_addr(ipaddr);
	}
	serveraddr.sin_port = htons(port);

	if (bind(server_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0) {
		close(server_sock);
		return -1;
	}
	if (listen(server_sock, 5) < 0) {
		close(server_sock);
		return -1;
	}
	signal(SIGPIPE, SIG_IGN);

	return server_sock;
}

/**
 * @brief server_u (udp)
 */
int server_u(const char* ipaddr, int port, int blocking)
{
	int server_sock = 0;
	int on = 1;
	int stat = 0;
	struct sockaddr_in serveraddr;

	server_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (server_sock < 0) {
		return -1;
	}
	{
		on = 1;
		setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	}
	if (!blocking) {
		stat = fcntl(server_sock, F_GETFL, NULL);
		stat |= O_NONBLOCK;
		fcntl(server_sock, F_SETFL, stat);
	}

	memset(&serveraddr, 0, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
//	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
/*
SO_REUSEADDR
              Indicates that the rules used in validating  addresses  supplied
              in  a  bind(2)  call  should allow reuse of local addresses. For
              PF_INET sockets this means that a socket may bind,  except  when
              there  is an active listening socket bound to the address.  When
              the listening socket is bound to INADDR_ANY with a specific port
              then  it  is  not  possible  to  bind to this port for any local
              address.
 */
	serveraddr.sin_addr.s_addr = inet_addr(ipaddr /*"127.0.0.1"*/);
	serveraddr.sin_port = htons(port);

	if (bind(server_sock, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0) {
		close(server_sock);
		return -1;
	}
	signal(SIGPIPE, SIG_IGN);

	return server_sock;
}

int set_keepalive(int sock, int keep_count, int keep_idle, int keep_interval) {
	int on = 1;

	if (setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof(on)) < 0) {
		perror("SO_KEEPALIVE");
		return 0;
	}
	if (setsockopt(sock, SOL_TCP, TCP_KEEPIDLE, &keep_idle, sizeof(keep_count)) < 0) {
		perror("TCP_KEEPIDLE");
		return 0;
	}
	if (setsockopt(sock, SOL_TCP, TCP_KEEPCNT, &keep_count, sizeof(keep_count)) < 0) {
		perror("TCP_KEEPCNT");
		return 0;
	}
	if (setsockopt(sock, SOL_TCP, TCP_KEEPINTVL, &keep_interval, sizeof(keep_interval)) < 0) {
		perror("TCP_KEEPINTVL");
		return 0;
	}
	return 1;
}


int set_rst_close(int sock) {
	struct linger ling;
	ling.l_onoff = 1;
	ling.l_linger = 0;

	if (setsockopt(sock, SOL_SOCKET, SO_LINGER, &ling, sizeof(ling)) < 0) {
		return 0;
	}
	return 1;
}

#ifdef TEST_SOCK
// g++ check_sock.cpp sock.cpp -o check_sock


#define TEST_PORT (30000)

void do_server()
{
	struct sockaddr_in clientaddr;
	int s = create_local_server_t(TEST_PORT, 0);
	int c = 0;

	do {
		c = accept_t(s, 1, 0, &clientaddr);
		if (c <= 0) {
			printf("accept_t timeout... retry after 1 secs\n");
			continue;
		}

		printf("success accept client %d\n", c);

		while (1) {
#if 0
			printf("valid sock = %s\n", check_valid_sock_t(c) ? "connected" :"disconnected");
#else
			char buf[1024];
			int bytes = 0;

RE_DO:
			bytes = recv_t(c, (uint8_t*)buf, sizeof(buf), 1, 0);
			if (bytes < 0) {
				printf("close client\n");
				break;
			} else if (bytes == 0) {
				printf("recv timeout\n");
				goto RE_DO;
			} else {
				printf("recv : %s\n", buf);
			}
#endif

			sleep(1);
		}

	} while (1);
}

void do_client()
{
	int c = 0;

	do {
		c = connect_t(NULL, "127.0.0.1", TEST_PORT, 0);
		if (c < 0) {
			printf("failed to connect... retry after 1 sec\n");
			sleep(1);
			continue;
		}

		printf("connected server (socket:%d)\n", c);
		printf("waiting...\n");
		while (1) {
			sleep(5);
			char data[1024];
			int r = 0;

			snprintf(data, sizeof(data), "kinow");
			r = send_t(c, (uint8_t*)data, sizeof(data), 0, 0);
			printf("send %d\n", r);
		}

	} while (1);
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		exit(1);
	}

	if (argv[1][0] == 's') {
		do_server();
	}
	else if (argv[1][0] == 'c') {
		do_client();
	}
	return 0;
}

#endif
