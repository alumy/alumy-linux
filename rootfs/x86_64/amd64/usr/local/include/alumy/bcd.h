#ifndef __AL_BCD_H
#define __AL_BCD_H 1

#include "alumy/types.h"
#include "alumy/base.h"

__BEGIN_DECLS

uint8_t al_bin2bcd(uint_fast8_t bin);
uint8_t al_bcd2bin(uint_fast8_t bcd);
const char *al_bcd2str(char *dst, size_t dstsz, const uint8_t *bcd, size_t n);

__END_DECLS

#endif
