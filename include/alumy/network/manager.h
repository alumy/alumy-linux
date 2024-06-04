#ifndef __NETWORK_MANAGER_H
#define __NETWORK_MANAGER_H

#include "alumy/types.h"
#include "alumy/base.h"

__BEGIN_DECLS

/**
 * Network manager set ipv4
 *
 * @author chenyj (2018/7/21)
 *
 * @param ifname Interface name
 * @param ip Ip address
 * @param mask Net mask
 * @param gw Gateway
 *
 * @return int_fast32_t Return 0 on success, otherwise return
 *  	   negative
 */
int_fast32_t al_nm_set_ip(const char *ifname, bool dhcp,
						  const char *ip, const char *mask, const char *gw)
				__nonnull((1, 3, 4, 5));

/**
 * Setup ip via ifconfig
 */
int_t al_nm_set_ip_ifconfig(const char *ifname, bool dhcp,
                            const char *ip, const char *mask, const char *gw);

/**
 * Check if the ip address is valid, support for ipv4 and ipv6
 *
 * @author chenyj (2018/7/30)
 *
 * @param addr The address to check
 *
 * @return bool Return true on success, otherwise return false
 */
bool al_ipaddr_is_valid(const char *addr)__nonnull((1));

/**
 * Check if the ipv4 netmaks is valid
 *
 * @author chenyj (2018/07/30)
 *
 * @param netmask The netmask to check
 *
 * @return bool Return true on success, otherwise return false
 */
bool al_ipv4_netmask_is_valid(const char *netmask) __nonnull((1));

/**
 * @brief Check ipv4 configuration
 *
 * @author chenyj (2019/2/28)
 *
 * @param ip ip address
 * @param mask net mask
 * @param gw gateway
 *
 * @return bool true on success, otherwise return false
 */
bool al_ipv4_config_is_valid(const char *ip, const char *mask, const char *gw)
		__nonnull((1, 2, 3));

/**
 * Check if the ipv4 group is valid, must call before set local
 * ip address
 *
 * @author chenyj (2018/07/30)
 *
 * @param ip IP to check
 * @param mask Netmask to check
 * @param gw Gateway to check
 *
 * @return bool Return true on success, otherwise return false
 */
bool al_nm_ipv4_is_valid(const char *ip, const char *mask, const char *gw)
		__nonnull((1, 2, 3));;

__END_DECLS


#endif
