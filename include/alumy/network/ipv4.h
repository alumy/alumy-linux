#ifndef __AL_IPV4_H
#define __AL_IPV4_H

#include "alumy/types.h"
#include "alumy/base.h"

__BEGIN_DECLS

bool al_ipaddr_is_valid(const char *addr);

bool al_ipv4_netmask_is_valid(const char *netmask);

bool al_ipv4_config_is_valid(const char *ip, const char *mask, const char *gw);

__END_DECLS

#endif
