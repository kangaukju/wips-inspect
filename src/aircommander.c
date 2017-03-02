/*
 * aircommander.c
 *
 *  Created on: 2017. 1. 16.
 *      Author: root
 */

#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <dirent.h>
#include <fcntl.h>
#include <errno.h>
#include <getopt.h>
#include <stdint.h>

#define PIPE_RD  0
#define PIPE_WR  1

static int AIRCOMMANDER_PID;
static const char *PID_FILE = "/var/run/aircommander.pid";
static const uint16_t AIRCOMMANDER_PORT = "8555";


static char* new_errmsg(const char *msg, ...)
{
	va_list ap;
	int len;
	char *buf;

	buf = (char *)malloc(128);

	va_start(ap, msg);
	len = vsnprintf(buf, 128, msg, ap);
	va_end(ap);

	snprintf(buf+len, 128-len, " (%s)", strerror(errno));
	return buf;
}

static void free_req(REQ **preq)
{
	REQ *req = *preq;
	if (!req) {
		return;
	}
	if (req->data) {
		free(req->data);
	}
	free(*preq);
	*preq = NULL;
}

static REQ *new_req()
{
	REQ *req;

	req = (REQ *)malloc(sizeof(REQ));
	memset(req, 0, sizeof(REQ));
	return req;
}

static int write_lock_file(int fd, int wait)
{
	struct flock lock = {
			.l_type = F_WRLCK,
			.l_whence = SEEK_SET,
			.l_start = 0,
			.l_len = 0,
	};

	if (wait) {
		return fcntl(fd, F_SETLKW, &lock);
	} else {
		return fcntl(fd, F_SETLK, &lock);
	}
}

static int make_command_socket()
{
	struct sockaddr_in saddr;
	int sock;
	int opt;

	sock = socket(PF_INET, SOCK_STREAM, 0);
	if (sock < 0) {
		fprintf(stderr, "unable open command socket %s\n", strerror(errno));
		return -1;
	}

	opt = 1;
	setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	memset(&saddr, 0, sizeof(saddr));
	saddr.sin_family = AF_INET;
	saddr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
	saddr.sin_port = htons(AIRCOMMANDER_PORT);

	if (bind(sock, (struct sockaddr*)&saddr, sizeof(saddr)) < 0) {
		fprintf(stderr, "unable bind command socket %s\n", strerror(errno));
		return -1;
	}
	return sock;
}


static int check_and_pidfile()
{
	char buf[128];
	int buflen;

	AIRCOMMANDER_PID = open(PID_FILE,
			O_RDWR|O_CREAT,
			S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH);

	if (write_lock_file(AIRCOMMANDER_PID, 0) < 0) {
		if (errno == EACCES || errno == EAGAIN) {
			buflen = read(AIRCOMMANDER_PID, buf, sizeof(buf));
			buf[buflen] = '\0';
			return atoi(buf);
		} else {
			fprintf(stderr, "can't lock a file %s\n", PID_FILE);
		}
	}
	return -1;
}

static int check_req(REQ *req)
{
	// check data len
	if (req->datalen == 0) {
		fprintf(stderr, "error request.datalen is 0\n");
		return -1;
	}
	// check type/subtype
	switch (ntohs(req->hdr.type)) {

	case REQ_HDR_TYPE_COMMAND:
		if (	req->hdr.subtype != REQ_HDR_SUBTYPE_COMMAND_RET_RESULT &&
				req->hdr.subtype != REQ_HDR_SUBTYPE_COMMAND_RET_IMMEDIATELY) {
			fprintf(stderr, "unknown request.head.subtype (%d)\n", req->hdr.subtype);
			return -1;
		}
		break;

	/*
	case another...
	*/

	default:
		fprintf(stderr, "unknown request.head.type (%d)\n", ntohs(req->hdr.type));
		return -1;
	}
	// check code
	if (req->hdr.code != REQ_HDR_CODE_REQ_REQUEST) {
		fprintf(stderr, "unknown request.head.code (%d)\n", req->hdr.code);
		return -1;
	}
	return 0;
}

static void show_req_packet(REQ *req)
{
	printf("REQ: len     : %d\n", ntohl(req->hdr.len));
	printf("REQ: type    : %d\n", req->hdr.type);
	printf("REQ: subtype : %d\n", req->hdr.subtype);
	printf("REQ: code    : %d\n", req->hdr.code);
	if (req->data && req->datalen > 0) {
		if (req->hdr.type == REQ_HDR_TYPE_COMMAND) {
			printf("REQ: data    : %s %.200s\n",
				req->hdr.code == REQ_HDR_CODE_REQ_REQUEST ? "REQ" : "ACK",
				req->data);
		}
	}
}

static int send_ack_msg(int sock, uint8_t code, uint16_t type, uint8_t subtype, char *retmsg)
{
	REQ *req;
	int rc;

	req = new_req();
	req->hdr.code = code;
	req->hdr.type = htons(type);
	req->hdr.subtype = subtype;
	req->hdr.len = sizeof(REQ_HDR);
	if (retmsg && strlen(retmsg) > 0) {
		req->hdr.len += strlen(retmsg);
		req->data = strdup(retmsg);
	}
	req->hdr.len = htonl(req->hdr.len);

	rc = send_ack(sock, req);

	free_req(&req);
	return rc;
}

static int send_ack(int sock, REQ *req)
{
	uint8_t *buf;
	ssize_t bytes;
	uint32_t len = ntohl(req->hdr.len);

	buf = (uint8_t *)malloc(len);
	memset(buf, 0, len);
	memcpy(buf, &req->hdr, sizeof(REQ_HDR));

	if (req->datalen > 0) {
		memcpy(buf+sizeof(REQ_HDR), req->data, req->datalen);
	}
	bytes = write(sock, buf, len);
	if (bytes <= 0) {
		fprintf(stderr, "unable send ack %s\n", strerror(errno));
		free(buf);
		return -1;
	}
	free(buf);
	return 0;
}

static REQ *recv_req(int sock)
{
	REQ *req;
	ssize_t bytes;
	fd_set fdset;
	int rc;
	struct timeval tv = { .tv_sec = 3, .tv_usec = 0 };

	req = new_req();
	FD_ZERO(&fdset);

re_select:
	rc = select(sock+1, &fdset, NULL, NULL, &tv);
	if (rc < 0) {
		if (errno == EINTR) {
			goto re_select;
		}
		fprintf(stderr, "error client request %s\n", strerror(errno));
		free_req(&req);
		return NULL;
	} else if (rc == 0) {
		fprintf(stderr, "error client request timeout %d sec%s\n", tv.tv_sec);
		free_req(&req);
		return NULL;
	}

	if (!FD_ISSET(sock, &fdset)) {
		fprintf(stderr, "error client request %s\n", strerror(errno));
		free_req(&req);
		return NULL;
	}

	bytes = read(sock, (void *)&req->hdr, sizeof(REQ_HDR));
	if (bytes < sizeof(REQ_HDR)) {
		free_req(&req);
		return NULL;
	}

	req->datalen = ntohl(req->hdr.len) - sizeof(REQ_HDR);
	if (check_req(req)) {
		req->hdr.len = htonl(sizeof(REQ_HDR));
		req->hdr.code = REQ_HDR_CODE_ACK_FAILURE;
		req->datalen = 0;
		req->data = NULL;
		send_ack(sock, req);
		free_req(&req);
		return NULL;
	}

	req->data = (uint8_t *)malloc(req->datalen + 1);
	memset(req->data, 0, req->datalen + 1);

	bytes = read(sock, req->data, req->datalen);
	if (req->datalen != bytes) {
		fprintf(stderr, "error reading data %s\n", strerror(errno));
		free_req(&req);
		return NULL;
	}
	show_req_packet(req);

	return req;
}

static int child_command_action(REQ *req, int *pipe2)
{
	int devnull;
	int i;

	devnull = open("/dev/null", O_RDWR);
	if (devnull < 0) {
		fprintf(stderr, "error open /dev/null %s\n", strerror(errno));
		return -1;
	}
	dup2(devnull, STDIN_FILENO);

	if (req->hdr.subtype == REQ_HDR_SUBTYPE_COMMAND_RET_RESULT) {
		if (close(pipe2[PIPE_RD]) != 0) {
			fprintf(stderr, "error close read pipe %s\n", strerror(errno));
			return -1;
		}
		if (dup2(pipe2[PIPE_WR], STDOUT_FILENO) == -1) {
			fprintf(stderr, "error dup stdout %s\n", strerror(errno));
			return -1;
		}
		close(STDERR_FILENO);
		if (dup2(pipe2[PIPE_WR], STDERR_FILENO) == -1) {
			fprintf(stderr, "error dup stderr %s\n", strerror(errno));
			return -1;
		}
	} else {
		dup2(devnull, STDOUT_FILENO);
		dup2(devnull, STDERR_FILENO);
	}
	close(devnull);

	for (i=3; i<256; i++) {
		close(i);
	}

	execl("/bin/sh", "sh", "-c", req->data, (char*)0);
	return 0;
}

static int parent_command_action(int sock, REQ *req, int *pipe2)
{


	if (req->hdr.subtype == REQ_HDR_SUBTYPE_COMMAND_RET_IMMEDIATELY) {
		send_ack_msg(sock,
				REQ_HDR_CODE_ACK_SUCCESS,
				req->)
	}
}

static void client_command_handler(int sock, REQ *req)
{
	int pipe2[2];
	pid_t pid;
	int rc;

	// response result
	if (req->hdr.subtype == REQ_HDR_SUBTYPE_COMMAND_RET_RESULT) {
		// create pipe
		if (pipe(pipe2) != 0) {
			send_ack_msg(sock,
					REQ_HDR_CODE_ACK_FAILURE,
					req->hdr.type,
					req->hdr.subtype,
					new_errmsg("pipe()"));
			return;
		}
	}

	pid = fork();
	if (pid == 0) {
		// child
		signal(SIGHUP,  SIG_DFL);
		signal(SIGALRM, SIG_DFL);
		signal(SIGINT,  SIG_DFL);
		signal(SIGTERM, SIG_DFL);
		signal(SIGABRT, SIG_DFL);

		rc = child_command_action(req, pipe2);

		exit(rc);
	} else if (pid > 0) {
		// parent
		free(req->data);
		req->data = NULL;

		rc = parent_command_action(sock, req, pipe2);

	} else {
		// fail fork
		send_ack_msg(sock,
				req->hdr.type,
				req->hdr.subtype,
				new_errmsg("fork()"));
	}
}

static void *client_request_handler(void *sock)
{
	int csock;
	REQ *req = NULL;

	pthread_detach(pthread_self());

	csock = *(int *)sock;
	free(sock);

	req = recv_req(csock);
	if (!req) {
		shutdown(csock, 2);
		close(csock);
		return NULL;
	}

	switch (req->hdr.type) {
	case REQ_HDR_TYPE_COMMAND:
		client_command_handler(csock, req);
		break;
	}

	free_req(&req);
	shutdown(csock, 2);
	close(csock);
}

static int exec_command(int ssock)
{
	struct sockaddr caddr;
	int *csock;
	int clen;
	int rc, flag;
	fd_set fdset;
	pthread_t pid;


	rc = listen(ssock, 5);
	if (rc < 0) {
		fprintf(stderr, "unable listen %s\n", strerror(errno));
		exit(-1);
	}

	flag = fcntl(ssock, F_GETFL, 0);
	fcntl(ssock, F_SETFL, flag|O_NONBLOCK);

	while (1) {
		FD_ZERO(&fdset);
		FD_SET(ssock, &fdset);

		rc = select(ssock+1, &fdset, NULL, NULL, NULL);
		if (rc < 0) {
			if (errno == EINTR) {
				continue;
			}
			fprintf(stderr, "error select %s\n", strerror(errno));
			exit(-1);
		} else if (rc == 0) {
			continue;
		}

		if (FD_ISSET(ssock, &fdset)) {
			clen = sizeof(caddr);
			csock = (int *)malloc(sizeof(csock));
			*csock = accept(ssock, (struct sockaddr *)&caddr, (socklen_t*)&clen);
			if (*csock == -1) {
				free(csock);
				fprintf(stderr, "error client accept %s\n", strerror(errno));
				continue;
			}

			rc = pthread_create(&pid, NULL, client_request_handler, (void*)csock);
			if (rc < 0) {
				fprintf(stderr, "error client request handler %s\n", strerror(errno));
				continue;
			}
		}
	}
	return 0;
}

static void init_daemon()
{
	char buf[128];

	write_lock_file(AIRCOMMANDER_PID, 1);
	if (ftruncate(AIRCOMMANDER_PID, 0)) {
		fprintf(stderr, "unable truncate to file %s %s\n", PID_FILE, strerror(errno));
	}
	lseek(AIRCOMMANDER_PID, 0, SEEK_SET);

	snprintf(buf, sizeof(buf), "%d", getpid());
	if (write(AIRCOMMANDER_PID, buf, strlen(buf)) <= 0) {
		fprintf(stderr, "unable write pid to file %s %s\n", PID_FILE, strerror(errno));
	}

	setsid();
}

int main(int argc, char **argv)
{
	const char *pname = basename(argv[0]);
	int oldpid = 0;
	int debug = 0;
	int aircommander_sock = -1;


	while (1) {
		int opt;
		int opt_index;
		static struct option long_opts[] = {
				{"debug",   0, 0, 'd'},
				{"version", 0, 0, 'v'},
				{"help",    0, 0, 'h'},
				{0,           0, 0, 0},
		};

		opt = getopt_long_only(argc, argv, "dvh", long_opts, &opt_index);
		if (opt == -1) {
			break;
		}

		switch (opt) {
		case 'd':
			debug = 1;
			break;
		case 'v':
			show_version(pname);
			break;
		case 'h':
		case '?':
		case '0':
		default:
			usage(pname);
			break;
		}
	}


	oldpid = check_and_pidfile();
	if (oldpid > 0) {
		fprintf(stderr, "warnning another %s is running\n", pname);
		exit(-1);
	}

	if (!debug) {
		if (daemon(0, 0)) {
			fprintf(stderr, "unable daemon - %s\n", strerror(errno));
			exit(-1);
		}
	}

	aircommander_sock = make_command_socket();
	if (aircommander_sock < 0) {
		fprintf(stderr, "unable create command socket\n");
		exit(-1);
	}

	exec_command(aircommander_sock);

	shutdown(aircommander_sock, 2);
	close(aircommander_sock);
	exit(-1);
}
