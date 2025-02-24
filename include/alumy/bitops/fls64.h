/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __AL_BITOPS_FLS64_H
#define __AL_BITOPS_FLS64_H 1

#include "alumy/config.h"
#include "alumy/types.h"
#include "alumy/base.h"
#include "alumy/bitops/bitsperlong.h"
#include "alumy/bitops/fls.h"

/**
 * fls64 - find last set bit in a 64-bit word
 * @x: the word to search
 *
 * This is defined in a similar way as the libc and compiler builtin
 * ffsll, but returns the position of the most significant set bit.
 *
 * fls64(value) returns 0 if value is 0 or the position of the last
 * set bit if value is nonzero. The last (most significant) bit is
 * at position 64.
 */
#if BITS_PER_LONG == 32
static __always_inline int fls64(uint64_t x)
{
    uint32_t h = x >> 32;
    if (h)
        return fls(h) + 32;
    return fls(x);
}
#elif BITS_PER_LONG == 64
static __always_inline int fls64(uint64_t x)
{
    if (x == 0)
        return 0;
    return __fls(x) + 1;
}
#else
#error BITS_PER_LONG not 32 or 64
#endif

#endif /* _ASM_GENERIC_BITOPS_FLS64_H_ */
