#ifndef __ALUMY_H
#define __ALUMY_H 1

#include "alumy/config.h"
#include "alumy/byteorder.h"
#include "alumy/types.h"
#include "alumy/base.h"
#include "alumy/bit.h"
#include "alumy/version.h"
#include "alumy/errno.h"
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
#include "alumy/filter.h"

__BEGIN_DECLS

/**
 * @brief alumy init
 *
 * @author jack (2021/9/9)
 *
 * @param void
 *
 * @return int32_t return 0 on first init success, return 1 on
 *         already initialized
 */
int32_t alumy_init(void);

/**
 * @brief alumy cleanup
 *
 * @author jack (2021/9/9)
 *
 * @param void
 *
 * @return int32_t return 0 on cleanup success, return 1 on need
 *         more cleanup, return -1 on not initialized
 */
int32_t alumy_cleanup(void);

__END_DECLS

#endif

