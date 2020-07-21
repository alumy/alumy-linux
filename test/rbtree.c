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

typedef struct rb_item {
	rb_node_t node;
	int32_t value;
} rb_item_t;

static int rbtree_suite_init(void)
{
	if(al_log_open("bcd", "zlog.conf", "test") != 0) {
		return -1;
	}

	return 0;
}

static int rbtree_suite_clean(void)
{
	al_log_close();
	return 0;
}

int32_t rb_compare(const void *na, const void *nb)
{
	const rb_item_t *__na = (const rb_item_t *)rb_entry(na, rb_item_t, node);
	const rb_item_t *__nb = (const rb_item_t *)rb_entry(nb, rb_item_t, node);

	return __na->value - __nb->value;
}

static void rbtree_test(void)
{
	rbtree_t rb = { { 0 } };

	CU_ASSERT(rbtree_init(&rb,
						  rb_compare,
						  rbtree_search_dft,
						  rbtree_remove_dft,
						  rbtree_insert_dft) == 0);

	rb_item_t item = {
		.value = 1
	};

	rb_item_t *key = NULL;

	CU_ASSERT(rb.insert(&rb, &item, offsetof(rb_item_t, node)) == 0);
	CU_ASSERT(((key = rb.search(&rb, &item,
								offsetof(rb_item_t, node))) != NULL) &&
			  (key->value == 1));
}

void add_rbtree_tests(void)
{
	CU_pSuite suite;

	suite = CU_add_suite("rbtree", rbtree_suite_init, rbtree_suite_clean);

	CU_add_test(suite, "rbtree_test", rbtree_test);
}

__END_DECLS
