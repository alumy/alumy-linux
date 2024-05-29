#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/nameser.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <linux/limits.h>
#include "alumy/log.h"
#include "alumy/errno.h"
#include "alumy/network/manager.h"
#include "alumy/config.h"
#include "alumy/network/ipv4.h"

#define RESOLV_CONF			"/etc/resolv.conf"
#define RESOLV_CONF_TMP		"/tmp/resolv.conf"


static int32_t al_write_resolv(const char *resolv,
							   const char *dns0, const char *dns1)
{
	AL_ASSERT(resolv != NULL);
	AL_ASSERT(dns0 != NULL);
	AL_ASSERT(dns1 != NULL);

	if ((dns0 == NULL) && (dns1 == NULL)) {
		return -1;
	}

	FILE *f = fopen("/etc/resolv.conf", "w");
	if (f == NULL) {
		AL_ERROR(AL_LOG, "%s, fopen failed, errno = %d", __func__, errno);
		return -1;
	}

	if (dns0 && al_ipaddr_is_valid(dns0)) {
		fprintf(f, "nameserver %s\n", dns0);
	}

	if (dns1 && al_ipaddr_is_valid(dns1)) {
		fprintf(f, "nameserver %s\n", dns1);
	}

	fflush(f);
	fsync(fileno(f));
	fclose(f);

	return 0;
}

int32_t al_set_dns(const char *dns0, const char *dns1)
{
	char cmd[PATH_MAX * 2 + 32];
	ssize_t l;

	if(al_write_resolv(RESOLV_CONF_TMP, dns0, dns1) != 0){
		AL_ERROR(AL_LOG, "set nameserver to %s failed", RESOLV_CONF_TMP);
		return -1;
	}

	l = snprintf(cmd, sizeof(cmd), "rm -f '%s'", RESOLV_CONF);
	if ((l <= 0) && (l > (sizeof(cmd) - 1))) {
		AL_ERROR(AL_LOG, "cmd %s is truncated", cmd);
        return -ENOBUFS;
	}

	if (system(cmd) != 0) {
		AL_ERROR(AL_LOG, "execute %s error", cmd);
		return -1;
	}

	l = snprintf(cmd, sizeof(cmd),
				 "ln -s %s %s", RESOLV_CONF_TMP, RESOLV_CONF);
	if ((l <= 0) && (l > (sizeof(cmd) - 1))) {
		AL_ERROR(AL_LOG, "cmd %s is truncated", cmd);
        return -ENOBUFS;
	}

	if(system(cmd) != 0) {
		AL_ERROR(AL_LOG, "execute %s error", cmd);
		return -1;
	}

	if (res_init() != 0) {
		return -1;
	}

	return 0;
}

