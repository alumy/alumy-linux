#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/nameser.h>
#include <resolv.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "alumy/log.h"
#include "alumy/network/manager.h"
#include "alumy/config.h"
#include "alumy/network/ipv4.h"

__BEGIN_DECLS

#define INTERFACE_FILE		"/etc/network/interfaces"
#define RESOLV_FILE			"/etc/resolv.conf"


int_fast32_t nm_set_ip(const char *ifname, bool dhcp,
					   const char *ip, const char *mask, const char *gw)
{
	AL_ASSERT(ifname);
	AL_ASSERT((!dhcp) && ip && mask && gw);

	char buf[512];
	ssize_t l;

	if (dhcp) {
		l = snprintf(buf, sizeof(buf),
					 "auto lo\n"
					 "iface lo inet loopback\n"
					 "\n"
					 "auto %s\n"
					 "iface %s inet dhcp\n"
					 "\n",
					 ifname, ifname);
	} else {
		l = snprintf(buf, sizeof(buf),
					 "auto lo\n"
					 "iface lo inet loopback\n"
					 "\n"
					 "auto %s\n"
					 "iface %s inet static\n"
					 "address %s\n"
					 "netmask %s\n"
					 "gateway %s\n"
					 "\n",
					 ifname, ifname, ip, mask, gw);
	}

	if (l <= 0) {
		return -1;
	}

	FILE *f = fopen(INTERFACE_FILE, "w");
	if (f == NULL) {
		return -1;
	}

	if(fwrite(buf, 1, l, f) != l) {
		fclose(f);
		return -1;
	}

	fflush(f);
	fsync(fileno(f));
	fclose(f);

	system("/etc/init.d/networking restart");

	return 0;
}

__END_DECLS

