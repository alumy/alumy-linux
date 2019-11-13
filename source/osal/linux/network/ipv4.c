#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/nameser.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "alumy/types.h"
#include "alumy/base.h"
#include "alumy/log.h"
#include "alumy/config.h"

__BEGIN_DECLS

bool al_ipaddr_is_valid(const char *addr)
{
	uint8_t buf[sizeof(struct in6_addr)];

	return inet_pton(AF_INET, addr, buf) || inet_pton(AF_INET6, addr, buf);
}

static bool __ipv4_netmask_is_valid(uint32_t addr)
{
	addr = ~addr + 1;
	return ((addr & (addr - 1)) == 0);
}

bool al_ipv4_netmask_is_valid(const char *netmask)
{
	struct in_addr addr;

	if (inet_pton(AF_INET, netmask, &addr) != 1) {
		return false;
	}

	return __ipv4_netmask_is_valid(ntohl(addr.s_addr));
}

bool al_ipv4_config_is_valid(const char *ip, const char *mask, const char *gw)
{
	struct in_addr __ip, __mask, __gw;

	if (inet_pton(AF_INET, ip, &__ip) != 1) {
		AL_ERROR(AL_LOG, "inet_pton failed, src = %s, errno = %d(%s)",
				ip, errno, strerror(errno));

		return false;
	}

	if (inet_pton(AF_INET, mask, &__mask) != 1) {
		AL_ERROR(AL_LOG, "inet_pton failed, src = %s, errno = %d(%s)",
				mask, errno, strerror(errno));

		return false;
	}

	if (!__ipv4_netmask_is_valid(ntohl(__mask.s_addr))) {
		AL_ERROR(AL_LOG, "ipv4 netmask is invalid, netmask = %s", mask);
		return false;
	}

	if (inet_pton(AF_INET, gw, &__gw) != 1) {
		AL_ERROR(AL_LOG, "inet_pton failed, src = %s, errno = %d(%s)",
				 gw, errno, strerror(errno));

		return false;
	}

	return ((__ip.s_addr & __mask.s_addr) == (__gw.s_addr & __mask.s_addr));
}

__END_DECLS

