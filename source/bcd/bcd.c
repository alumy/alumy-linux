#include "alumy/types.h"
#include "alumy/base.h"
#include "alumy/log.h"
#include "alumy/errno.h"

__BEGIN_DECLS

__attribute_const__ uint8_t al_bin2bcd(uint_fast8_t bin)
{
	uint8_t high = ((bin / 10) % 10);
	uint8_t low = bin % 10;

	return ((high << 4) & 0xF0) | (low & 0x0F);
}

__attribute_const__ uint8_t al_bcd2bin(uint_fast8_t bcd)
{
	uint_fast8_t high = (bcd & 0xF0) >> 4;
	uint_fast8_t low = (bcd & 0x0F);

	AL_ASSERT(high < 10);
	AL_ASSERT(low < 10);

	return (uint_fast8_t)(high * 10 + low);
}

const char *al_bcd2str(char *dst, size_t dstsz, const uint8_t *bcd, size_t n)
{
	const char hex_tab[] = "0123456789ABCDEF";

	if(dstsz < (n * 2 + 1)) {
        set_errno(ENOBUFS);
		return NULL;
	}

	char *c = dst;
	const uint8_t *b = bcd;
	const uint8_t *bcd_end = bcd + n;

	for (; b < bcd_end; ++b) {
		*c++ = hex_tab[(*b >> 4) & 0x0F];
		*c++ = hex_tab[*b & 0x0F];
	}

	*c = 0;

	return dst;
}

__END_DECLS

