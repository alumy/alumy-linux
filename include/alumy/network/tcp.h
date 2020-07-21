#ifndef __AL_TCP_H
#define __AL_TCP_H

#include "alumy/types.h"
#include "alumy/base.h"

__BEGIN_DECLS

#define AL_TCP_CLIENT_FD(ptr)		((ptr)->sockfd)

typedef struct al_tcp_client {
	int sockfd;
} al_tcp_client_t;


/**
 * @brief Tcp client connect, support for ipv6
 *
 * @author jack (6/21/2018)
 *
 * @param tc The handle to tcp client
 * @param host The host name, host name or ip address
 * @param serv The service name or port
 * @param timeout Connect timeout
 *
 * @return int_fast32_t On success, 0 is returned. On error, -1
 *  	   is returned
 */
int32_t al_tcp_client_connect(al_tcp_client_t *tc, const char *host,
								   const char *serv, int32_t timeout)
		__nonnull((1, 2, 3));

__END_DECLS


#endif
