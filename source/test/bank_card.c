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

static int bank_card_suite_init(void)
{
	return 0;
}

static int bank_card_suite_clean(void)
{
	return 0;
}

static void al_bank_card_num_mask_pan_test(void)
{
	char dst[128];
	const char *src;

	src = "1234567890123456";
	CU_ASSERT((al_bank_card_num_mask_pan(dst, sizeof(dst), src) == 16) &&
			  (strcmp(dst, "123456******3456")== 0));

	src = "1234567890";
    CU_ASSERT(
        (al_bank_card_num_mask_pan(dst, sizeof(dst), src) == -1) &&
        (errno == EINVAL));

	src = "1234567890";
    CU_ASSERT(
        (al_bank_card_num_mask_pan(dst, 10, src) == -1) && (errno == ENOBUFS));
}

void add_bank_card_tests(void)
{
	CU_pSuite suite;

	suite = CU_add_suite("bank_card",
						 bank_card_suite_init,
						 bank_card_suite_clean);

	CU_add_test(suite, "al_bank_card_num_mask_pan",
				al_bank_card_num_mask_pan_test);
}

__END_DECLS

