#include "alumy/types.h"
#include "alumy/base.h"

__BEGIN_DECLS

uint8_t get_bcc(const void *data, size_t len)
{
	uint8_t bcc = 0;
	const uint8_t *p = (const uint8_t *)data;

	while (len--) {
		bcc ^= *p++;
	}

	return bcc;
}

__END_DECLS

