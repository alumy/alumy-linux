#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "CUnit/CUnit.h"
#include "CUnit/Automated.h"
#include "CUnit/Basic.h"
#include "alumy/types.h"
#include "alumy/base.h"
#include "alumy/config.h"
#include "alumy/log.h"
#include "test.h"

__BEGIN_DECLS

static int network_suite_init(void)
{
	if(al_log_open("network", "zlog.conf", "test") != 0) {
		return -1;
	}

	return 0;
}

static int network_suite_clean(void)
{
	al_log_close();
	return 0;
}

static void al_ipaddr_is_valid_test(void)
{
	CU_ASSERT(al_ipaddr_is_valid("192.168.1.1"));
	CU_ASSERT(!al_ipaddr_is_valid("192.168.1.x"));
	CU_ASSERT(al_ipaddr_is_valid("::1"));
}

static void al_ipv4_netmask_is_valid_test(void)
{
	CU_ASSERT(al_ipv4_netmask_is_valid("0.0.0.0"));
	CU_ASSERT(al_ipv4_netmask_is_valid("255.0.0.0"));
	CU_ASSERT(al_ipv4_netmask_is_valid("255.255.0.0"));
	CU_ASSERT(al_ipv4_netmask_is_valid("255.255.255.0"));
	CU_ASSERT(al_ipv4_netmask_is_valid("255.255.128.0"));
	CU_ASSERT(!al_ipv4_netmask_is_valid("255.255.0.1"));
	CU_ASSERT(!al_ipv4_netmask_is_valid("255.255.0.x"));
}

static void al_ipv4_config_is_valid_test(void)
{
	CU_ASSERT(al_ipv4_config_is_valid("192.168.1.2",
									  "255.255.255.0",
									  "192.168.1.1"));
	CU_ASSERT(!al_ipv4_config_is_valid("192.168.1.2",
									   "255.255.255.0",
									   "192.168.2.1"));
	CU_ASSERT(!al_ipv4_config_is_valid("192.168.1.x",
									   "255.255.255.0",
									   "192.168.1.1"));
	CU_ASSERT(!al_ipv4_config_is_valid("192.168.1.1",
									   "255.255.255.x",
									   "192.168.1.1"));
	CU_ASSERT(!al_ipv4_config_is_valid("192.168.1.2",
									   "255.255.255.0",
									   "192.168.1.x"));
	CU_ASSERT(!al_ipv4_config_is_valid("192.168.1.2",
									   "0.0.0.1",
									   "192.168.1.1"));
}


void add_network_tests(void)
{
	CU_pSuite suite;

	suite = CU_add_suite("network", network_suite_init, network_suite_clean);

	CU_add_test(suite, "al_ipaddr_is_valid",
				al_ipaddr_is_valid_test);
	CU_add_test(suite, "al_ipv4_netmask_is_valid",
				al_ipv4_netmask_is_valid_test);
	CU_add_test(suite, "al_ipv4_config_is_valid",
				al_ipv4_config_is_valid_test);
}

__END_DECLS

