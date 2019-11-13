#include <string.h>
#include <errno.h>
#include <inttypes.h>
#include "alumy/types.h"
#include "alumy/base.h"
#include "alumy/config.h"
#include "alumy/log.h"
#include "alumy/errno.h"

__BEGIN_DECLS

ssize_t al_bank_card_num_mask_pan(char *dst, size_t dstsz, const char *src)
{
	size_t len = strlen(src);

	if ((len + 1) > dstsz) {
        set_errno(ENOBUFS);
        return -1;
	}

	if (len <= 10) {
        set_errno(EINVAL);
        return -1;
	}

	char *d = dst;
	const char *s = src;
	size_t pan_len = len - 10;

	/**
	 * head 6 characters
	 */
	*d++ = *s++;
	*d++ = *s++;
	*d++ = *s++;
	*d++ = *s++;
	*d++ = *s++;
	*d++ = *s++;

	/**
	 * Mark as '*'
	 */
	s += pan_len;
	while (pan_len--) {
		*d++ = '*';
	}

	/**
	 * tail 4 characters
	 */
	*d++ = *s++;
	*d++ = *s++;
	*d++ = *s++;
	*d++ = *s++;

	*d = '\0';

	return len;
}

__END_DECLS
