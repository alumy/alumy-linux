#include <inttypes.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include "alumy/types.h"
#include "alumy/base.h"
#include "alumy/log.h"

__BEGIN_DECLS

#define BIN_LINE_SIZE	16

static const char hex[] __used = "0123456789ABCDEF";

static int32_t logmask = 0xFF;

int32_t al_log_open(const char *ident, ...)
{
	const char *path = NULL;
	const char *cat = NULL;

	va_list ap;

	va_start(ap, ident);

	path = va_arg(ap, const char *);
	cat = va_arg(ap, const char *);

	va_end(ap);

	if ((path == NULL) || (cat == NULL)) {
		return -EINVAL;
	}

	fprintf(stdout, "alumy log open succ\n");

	return 0;
}

int32_t al_log_close(void)
{
    fprintf(stdout, "alumy log closed\n");
	return 0;
}

static void al_vlog(int32_t pri, const char *fmt, va_list ap)
{
    char tmstr[64];
    struct tm tm;
    time_t now = time(NULL);
    FILE *stream = stdout;

    if (pri == AL_LOG_EMERG || pri == AL_LOG_ERR ||
        pri == AL_LOG_WARN || pri == AL_LOG_CRIT ||
        pri == AL_LOG_ALERT) {
        stream = stderr;
    }

    localtime_r(& now, &tm);
    strftime(tmstr, sizeof(tmstr), "%F %T", &tm);

    fprintf(stream, "%s ", tmstr);
    vfprintf(stream, fmt, ap);
    fprintf(stream, "\n");
}

void al_log(int32_t pri, const char *file, int32_t line, const char *func,
			const char *fmt, ...)
{
	if (pri & ~(AL_LOG_PRIMASK)) {
		/* unknown prio mask */
		pri &= AL_LOG_PRIMASK;
	}

	if ((AL_LOG_MASK(AL_LOG_PRI(pri)) & logmask) == 0) {
		return;
	}

	va_list ap;

	va_start(ap, fmt);

    al_vlog(pri, fmt, ap);

	va_end(ap);
}

/**
 * @brief Format address as 00000000, the result exclude EOF
 *
 * @author jack (4/26/2018)
 *
 * @param buf
 * @param addr
 *
 * @return String length
 */
static size_t hex_addr_fmt(char buf[8], intptr_t addr)
{
	char *wp = buf;

	*wp++ = hex[addr >> 28 & 0x0F];
	*wp++ = hex[addr >> 24 & 0x0F];
	*wp++ = hex[addr >> 20 & 0x0F];
	*wp++ = hex[addr >> 16 & 0x0F];
	*wp++ = hex[addr >> 12 & 0x0F];
	*wp++ = hex[addr >> 8 & 0x0F];
	*wp++ = hex[addr >> 4 & 0x0F];
	*wp = hex[addr & 0x0F];

	return 8;
}

/**
 * @brief Format a line
 *  A line format:
 * 00000000: 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F ;
 * 000000000000000000\n
 *
 * @author jack (4/27/2018)
 *
 * @param buf
 * @param data
 * @param len
 *
 * @return size_t
 */
static ssize_t hex_raw_fmt_line(char *buf, size_t bufsz, intptr_t addr,
								const void *data, size_t len)
{
	size_t line_size = 8 + 2 + 16 * 3 + 2 + 16 + 1 + 1;

	if ((len > BIN_LINE_SIZE) || (bufsz < line_size)) {
		return -1;
	}

	char *_buf = buf;

	/**
	 * Add address and other fixed characters
	 */
	hex_addr_fmt(_buf, addr);
	_buf[8] = ':';
	_buf[9] = ' ';
	_buf[58] = ';';
	_buf[59] = ' ';

	char *h_wp = &_buf[10];
	char *c_wp = &_buf[60];

	const uint8_t *rp = (const uint8_t *)data;

	for (int_fast32_t i = 0; i < len; ++i) {
		*h_wp++ = hex[*rp >> 4 & 0x0F];
		*h_wp++ = hex[*rp & 0x0F];
		*h_wp++ = ' ';

		*c_wp++ = isprint(*rp) ? *rp : '.';

		++rp;
	}

	for (int_fast32_t i = len; i < BIN_LINE_SIZE; ++i) {
		*h_wp++ = ' ';
		*h_wp++ = ' ';
		*h_wp++ = ' ';
		*c_wp++ = ' ';
	}

	_buf[77] = 0;

	return line_size - 1;
}

void al_log_bin(int32_t pri,
				const char *file, int32_t line, const char *func,
				const void *data, size_t len)
{
	int_fast32_t nline = len >> 4;
	int_fast32_t remain = len & 0x0F;

	intptr_t addr = 0;
	const uint8_t *rp = (const uint8_t *)data;

	while (nline--) {
		char buf[128];
		hex_raw_fmt_line(buf, sizeof(buf), addr, rp + addr, BIN_LINE_SIZE);
		addr += BIN_LINE_SIZE;

		al_log(pri, file, line, func, "%s", buf);
	}

	if (remain > 0) {
		char buf[128];
		hex_raw_fmt_line(buf, sizeof(buf), addr, rp + addr, remain);

		al_log(pri, file, line, func, "%s", buf);
	}

}

int32_t al_log_set_mask(int32_t mask)
{
	int32_t omask = logmask;

	if (mask != 0) {
		logmask = mask;
	}

	return omask;
}

__END_DECLS

