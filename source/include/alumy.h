#ifndef __ALUMY_H
#define __ALUMY_H 1

#include "alumy/types.h"
#include "alumy/base.h"
#include "alumy/version.h"
#include "alumy/log.h"
#include "alumy/crc.h"
#include "alumy/network.h"
#include "alumy/mem.h"
#include "alumy/bcd.h"
#include "alumy/string.h"
#include "alumy/diag.h"
#include "alumy/bank_card.h"
#include "alumy/errno.h"
#include "alumy/list.h"
#include "alumy/sys.h"
#include "alumy/rbtree.h"

__BEGIN_DECLS

int32_t alumy_init(void);

int32_t alumy_cleanup(void);

__END_DECLS

#endif

