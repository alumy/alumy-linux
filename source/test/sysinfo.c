#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "CUnit/CUnit.h"
#include "CUnit/Automated.h"
#include "CUnit/Basic.h"
#include "alumy.h"
#include "test.h"

__BEGIN_DECLS

static int sysinfo_suite_init(void)
{
	if(al_log_open("sysinfo", "zlog.conf", "test") != 0) {
		return -1;
	}

	return 0;
}

static int sysinfo_suite_clean(void)
{
	al_log_close();
	return 0;
}

static void al_get_os_test(void)
{
#if defined(__linux__)
	CU_ASSERT_STRING_EQUAL(al_get_os(), "linux");
#elif defined(__unix__)
	CU_ASSERT_STRING_EQUAL(al_get_os(), "unix");
#else
	CU_ASSERT_STRING_EQUAL(al_get_os(), "none");
#endif
}

static void al_get_arch_test(void)
{
#if defined(__arm__)
	CU_ASSERT_STRING_EQUAL(al_get_arch(), "arm");
#elif defined(__amd64__)
	CU_ASSERT_STRING_EQUAL(al_get_arch(), "amd64");
#elif defined(__i386__)
	CU_ASSERT_STRING_EQUAL(al_get_arch(), "i386");
#else
	CU_ASSERT_STRING_EQUAL(al_get_arch(), "noarch");
#endif
}

void add_sysinfo_tests(void)
{
	CU_pSuite suite;

	suite = CU_add_suite("sysinfo", sysinfo_suite_init, sysinfo_suite_clean);

	CU_add_test(suite, "al_get_arch", al_get_arch_test);
	CU_add_test(suite, "al_get_os", al_get_os_test);
}

__END_DECLS
