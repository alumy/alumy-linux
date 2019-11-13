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

static int bcd_suite_init(void)
{
	if(al_log_open("bcd", "zlog.conf", "test") != 0) {
		return -1;
	}

	return 0;
}

static int bcd_suite_clean(void)
{
	al_log_close();
	return 0;
}

static void al_bin2bcd_test(void)
{
	CU_ASSERT(al_bin2bcd(12) == 0x12);
	CU_ASSERT(al_bin2bcd(0) == 0x00);
	CU_ASSERT(al_bin2bcd(99) == 0x99);
}

static void al_bcd2bin_test(void)
{
	CU_ASSERT(al_bcd2bin(0x00) == 0);
	CU_ASSERT(al_bcd2bin(0x99) == 99);
}

static void al_bcd2str_test(void)
{
	char str[128];

	uint8_t bcd[] = { 0x01, 0x03, 0x05, 0x07 };

	CU_ASSERT_STRING_EQUAL(
		al_bcd2str(str, sizeof(str), bcd, sizeof(bcd)), "01030507");

	CU_ASSERT(al_bcd2str(str, 8, bcd, sizeof(bcd)) == NULL);
}

void add_bcd_tests(void)
{
	CU_pSuite suite;

	suite = CU_add_suite("bcd", bcd_suite_init, bcd_suite_clean);

	CU_add_test(suite, "al_bin2bcd", al_bin2bcd_test);
	CU_add_test(suite, "al_bcd2bin", al_bcd2bin_test);
	CU_add_test(suite, "al_bcd2str", al_bcd2str_test);
}

__END_DECLS
