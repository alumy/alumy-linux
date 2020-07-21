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

static int version_suite_init(void)
{
	return 0;
}

static int version_suite_clean(void)
{
	return 0;
}

static void al_get_version_test(void)
{
	CU_ASSERT(strlen(al_get_version_s()) > 0);
}

void add_version_tests(void)
{
	CU_pSuite suite;

	suite = CU_add_suite("version",
						 version_suite_init,
						 version_suite_clean);

	CU_add_test(suite, "al_get_version", al_get_version_test);
}

__END_DECLS
