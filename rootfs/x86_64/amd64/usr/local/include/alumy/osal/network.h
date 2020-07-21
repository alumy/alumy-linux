#ifndef __AL_OSAL_NETWORK_H
#define __AL_OSAL_NETWORK_H 1

#include "alumy/types.h"
#include "alumy/base.h"

__BEGIN_DECLS

/**
 *
 *
 * @author chenyj (2018/7/21)
 *
 * @param dns0
 * @param dns1
 *
 * @return int_fast32_t
 */
int_fast32_t al_set_dns(const char *dns0, const char *dns1) __nonnull((1, 2));

__END_DECLS

#endif

