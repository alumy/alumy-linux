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

static int process_suite_init(void)
{
	if(al_log_open("proc", "zlog.conf", "test") != 0) {
		return -1;
	}

	return 0;
}

static int process_suite_clean(void)
{
	al_log_close();
	return 0;
}

static void al_write_pid_test(void)
{
	CU_ASSERT(al_write_pid("test.pid") == 0);
}

void add_process_tests(void)
{
	CU_pSuite suite;

	suite = CU_add_suite("process", process_suite_init, process_suite_clean);

	CU_add_test(suite, "al_write_pid", al_write_pid_test);
}

__END_DECLS
