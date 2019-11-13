#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <errno.h>
#include <inttypes.h>
#include <unistd.h>
#include <fcntl.h>
#include "alumy/network/tcp.h"
#include "alumy/log.h"
#include "alumy/config.h"

__BEGIN_DECLS

static int32_t select_s(int nfds, fd_set *readfds, fd_set *writefds,
						fd_set *exceptfds, struct timeval *timeout)
{
	int32_t ret;

	do {
		ret = select(nfds, readfds, writefds, exceptfds, timeout);
	} while ((ret < 0) && (errno == EINTR));

	return ret;
}

static ssize_t send_s(int fd, const void *data, size_t len)
{
	AL_ASSERT((fd >= 0) && (data != NULL) && (len > 0));

	ssize_t n;

	/*lint --e{722} */
	while (((n = send(fd, data, len, 0)) < 0) && (errno == EINTR));

	return n;
}

static ssize_t recv_s(int fd, void *buf, size_t bufsz)
{
	AL_ASSERT((fd >= 0) && (buf != NULL) && (bufsz > 0));

	ssize_t n;

	/*lint --e{722} */
	while (((n = recv(fd, buf, bufsz, 0)) < 0) && (errno == EINTR));

	return n;
}

static int32_t connect_timeout(int32_t sockfd, const struct sockaddr *addr,
							   socklen_t addrlen, int32_t timeout)
{
	int old_opt;
	int32_t ret;

	/**
	 * If timeout is less than zero, blocking receive
	 */
	if (timeout < 0) {
		return (connect(sockfd, addr, addrlen) == 0) ? 0 : -1;
	}

	/**
	 * Set to non-blocking
	 */
	old_opt = fcntl(sockfd, F_GETFL);
	fcntl(sockfd, F_SETFL, old_opt | O_NONBLOCK);

	if (connect(sockfd, addr, addrlen) == 0) {
		fcntl(sockfd, F_SETFL, old_opt);
		return 0;
	}

	if (errno != EINPROGRESS) {
		AL_ERROR(AL_LOG, "unblock connect failed: %d(%s)",
				 errno, strerror(errno));
		return -1;
	}

	struct timeval tv;
	fd_set wset;

	/**
	 * If connect failed, block timeout ms
	 */
	tv.tv_sec = timeout / 1000;
	tv.tv_usec = timeout % 1000;
	FD_ZERO(&wset);
	FD_SET(sockfd, &wset);

	ret = select_s(sockfd + 1, NULL, &wset, NULL, &tv);
	if (ret < 0) {
		AL_ERROR(AL_LOG, "select socket failed: %d(%s)",
				 errno, strerror(errno));
		return -1;
	}
	if (ret == 0) {
		AL_ERROR(AL_LOG, "select socket timeout");
		return -1;
	}

	if (!FD_ISSET(sockfd, &wset)) {
		AL_ERROR(AL_LOG, "no events on sockfd found");
		return -1;
	}

	int err;
	socklen_t socklen = sizeof(err);

	if(getsockopt(sockfd, SOL_SOCKET, SO_ERROR, &err, &socklen) < 0){
		AL_ERROR(AL_LOG, "get socket option failed");
		return -1;
	}

	if (err != 0) {
		AL_ERROR(AL_LOG,
				 "connection failed after select with the error: %d", err);
		return -1;
	}

	fcntl(sockfd, F_SETFL, old_opt);

	return 0;
}

static ssize_t recv_timeout(int32_t fd, void *buf, size_t bufsz,
							int32_t timeout)
{
	int32_t ret;

	if ((buf == NULL) || (fd == -1)) {
		return -EINVAL;
	}

	/**
	 * If timeout less than zero, blocking receive
	 */
	if (timeout < 0) {
		return recv_s(fd, buf, bufsz);
	}

	fd_set rset;
	struct timeval tv;

	FD_ZERO(&rset);
	FD_SET(fd, &rset);
	tv.tv_sec = timeout / 1000;
	tv.tv_usec = timeout % 1000;

	ret = select_s(fd + 1, &rset, NULL, NULL, &tv);

	if (ret < 0) {
		AL_ERROR(AL_LOG, "%s, select, errno = %d", __func__, errno);
		return -1;
	}

	if (ret == 0) {
		AL_ERROR(AL_LOG, "%s, select timeout", __func__);
		return -1;
	}

	if (!FD_ISSET(fd, &rset)) {
		return -1;
	}

	return recv_s(fd, buf, bufsz);
}

ssize_t al_tcp_client_send(al_tcp_client_t *tc,
								const void *data, size_t len)
{
	if ((data == NULL) || (tc == NULL) || (tc->sockfd == -1)) {
		return -EINVAL;
	}

	return send_s(tc->sockfd, data, len);
}

ssize_t al_tcp_client_recv(al_tcp_client_t *tc, void *buf,
								size_t bufsz, int32_t timeout)
{
	if ((buf == NULL) || (tc == NULL) || (tc->sockfd == -1)) {
		return -EINVAL;
	}

	return recv_timeout(tc->sockfd, buf, bufsz, timeout);
}

int32_t al_tcp_client_connect(al_tcp_client_t *tc,
								   const char *host, const char *serv,
								   int32_t timeout)
{
	struct addrinfo hints, *result;

	bzero(&hints, sizeof(struct addrinfo));

	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	int32_t ret = getaddrinfo(host, serv, &hints, &result);
	if (ret != 0) {
		AL_ERROR(AL_LOG, "getaddrinfo for %s:%s failed: %d(%s)",
				 host, serv, ret, gai_strerror(ret));
		return -1;
	}

	struct addrinfo *curr = result;
	for (; curr != NULL; curr = curr->ai_next) {
		char addr[NI_MAXHOST] = "unknown";
		char port[NI_MAXSERV] = "unknown";

		getnameinfo(curr->ai_addr, curr->ai_addrlen, addr, sizeof(addr),
					port, sizeof(port), NI_NUMERICHOST | NI_NUMERICSERV);

		AL_INFO(AL_LOG, "addrinfo: %s:%s -> %s:%s", host, serv, addr, port);

		if ((tc->sockfd = socket(curr->ai_family, curr->ai_socktype,
								 curr->ai_protocol)) == -1) {
			AL_ERROR(AL_LOG, "socket error: %d(%s)", errno, strerror(errno));
			continue;
		}

		if (connect_timeout(tc->sockfd, curr->ai_addr, curr->ai_addrlen,
							timeout) == 0) {
			break;
		}

		close(tc->sockfd);
		tc->sockfd = -1;
	}

	freeaddrinfo(result);

	if (curr == NULL) {
		AL_ERROR(AL_LOG, "all addr info is invalid, %s:%s", host, serv);
		return -1;
	}

	return tc->sockfd;
}

int32_t al_tcp_client_destroy(al_tcp_client_t *tc)
{
	return (close(tc->sockfd) == 0) ? 0 : -1;
}

__END_DECLS


