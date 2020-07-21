#ifndef __BANK_CARD_H
#define __BANK_CARD_H

#include "alumy/types.h"
#include "alumy/base.h"

__BEGIN_DECLS

ssize_t al_bank_card_num_mask_pan(char *dst, size_t dstsz, const char *src);

__END_DECLS

#endif
