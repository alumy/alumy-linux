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

static int mem_suite_init(void)
{
	return 0;
}

static int mem_suite_clean(void)
{
	return 0;
}

static void mem_is_filled_test(void)
{
	uint8_t buf[128];

	memset(buf, 0xAA, sizeof(buf));
	CU_ASSERT(al_mem_is_filled(buf, 0xAA, sizeof(buf)));

	memset(buf, 0xAA, sizeof(buf));
	CU_ASSERT(al_mem_is_filled(buf, 0xAA, sizeof(buf)));

	memset(buf, 0xAA, sizeof(buf));
	buf[0] = 0x55;
	CU_ASSERT(!al_mem_is_filled(buf, 0xAA, sizeof(buf)));

	memset(buf, 0xAA, sizeof(buf));
	buf[sizeof(buf) - 1] = 0x55;
	CU_ASSERT(!al_mem_is_filled(buf, 0xAA, sizeof(buf)));

	memset(buf, 0xAA, sizeof(buf));
	buf[63] = 0x55;
	CU_ASSERT(!al_mem_is_filled(buf, 0xAA, sizeof(buf)));

	memset(buf, 0xAA, sizeof(buf));
	CU_ASSERT(al_mem_is_filled(buf + 1, 0xAA, sizeof(buf) - 1));

	memset(buf, 0xAA, sizeof(buf));
	CU_ASSERT(!al_mem_is_filled(buf + 1, 0x55, sizeof(buf) - 1));

	memset(buf, 0xAA, sizeof(buf));
	CU_ASSERT(al_mem_is_filled(buf, 0xAA, sizeof(buf) - 3));

	memset(buf, 0xAA, sizeof(buf));
	buf[sizeof(buf) - 4] = 0x55;
	CU_ASSERT(!al_mem_is_filled(buf, 0xAA, sizeof(buf) - 3));
}


void add_mem_tests(void)
{
	CU_pSuite suite;

	suite = CU_add_suite("mem", mem_suite_init, mem_suite_clean);

	CU_add_test(suite, "mem_is_filled", mem_is_filled_test);
}

__END_DECLS

