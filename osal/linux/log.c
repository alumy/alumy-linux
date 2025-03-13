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
#include "zlog.h"

__BEGIN_DECLS

#define BIN_LINE_SIZE	16

static const char hex[] = "0123456789ABCDEF";

static int32_t logmask = 0xFF;
static zlog_category_t *zlog_cat = NULL;

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

	if (zlog_init(path) != 0) {
		fprintf(stderr, "zlog init failed\n");
		return -1;
	}

	zlog_cat = zlog_get_category(cat);
	if (zlog_cat == NULL) {
		fprintf(stderr, "zlog get cat fail\n");
		zlog_fini();
		return -1;
	}

	fprintf(stdout, "alumy log open succ\n");

	return 0;
}

int32_t al_log_close(void)
{
	zlog_fini();
	fprintf(stdout, "alumy log closed\n");

	return 0;
}

__weak void al_vlog(int32_t pri, const char *fmt, va_list ap)
{
	if (zlog_cat == NULL)
		return;

	switch (pri) {
		case AL_LOG_DEBUG:
			zlog_debug(zlog_cat, "%s ", al_log_timestamp());
			vzlog_debug(zlog_cat, fmt, ap);
			break;

		case AL_LOG_INFO:
			zlog_info(zlog_cat, "%s ", al_log_timestamp());
			vzlog_info(zlog_cat, fmt, ap);
			break;

		case AL_LOG_NOTICE:
			zlog_notice(zlog_cat, "%s ", al_log_timestamp());
			vzlog_notice(zlog_cat, fmt, ap);
			break;

		case AL_LOG_WARN:
			zlog_warn(zlog_cat, "%s ", al_log_timestamp());
			vzlog_warn(zlog_cat, fmt, ap);
			break;

		case AL_LOG_ERR:
			zlog_error(zlog_cat, "%s ", al_log_timestamp());
			vzlog_error(zlog_cat, fmt, ap);
			break;

		case AL_LOG_CRIT:
			zlog_fatal(zlog_cat, "%s ", al_log_timestamp());
			vzlog_fatal(zlog_cat, fmt, ap);
			break;

		case AL_LOG_ALERT:
			zlog_fatal(zlog_cat, "%s ", al_log_timestamp());
			vzlog_fatal(zlog_cat, fmt, ap);
			break;

		case AL_LOG_EMERG:
			zlog_fatal(zlog_cat, "%s ", al_log_timestamp());
			vzlog_fatal(zlog_cat, fmt, ap);
			break;

		default:
			break;
	}
}

__weak const char *al_log_timestamp(void)
{
	static char tmstr[64];
    struct tm tm;
    time_t now = time(NULL);
	
	localtime_r(&now, &tm);
    strftime(tmstr, sizeof(tmstr), "%F %T", &tm);

    /* snprintf(str, sizeof(str), "[%5ld.%03d]", (long)tv_sec, (int)tv_msec); */

    return tmstr;
}

void al_log(int32_t pri, const char *file, int32_t line, const char *func,
			const char *fmt, ...)
{
	UNUSED(file);
	UNUSED(line);
	UNUSED(func);
	
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
    size_t line_size = 8 + 2 + 16 * 3 + 2 + 16 + 1;
    size_t i;

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

	for (i = 0; i < len; ++i) {
        *h_wp++ = hex[(*rp >> 4) & 0x0F];
        *h_wp++ = hex[*rp & 0x0F];
        *h_wp++ = ' ';

#if defined (__GNUC__)
        *c_wp++ = isprint(*rp) ? (char)*rp : '.';
#elif defined (__CC_ARM)
        *c_wp++ = ((*rp <= 0x7F) && isprint(*rp)) ? *rp : '.';
#else
        *c_wp++ = ((*rp <= 0x7F) && isprint(*rp)) ? *rp : '.';
#endif

        ++rp;
    }

	for (i = len; i < BIN_LINE_SIZE; ++i) {
		*h_wp++ = ' ';
		*h_wp++ = ' ';
		*h_wp++ = ' ';
		*c_wp++ = ' ';
	}

    _buf[76] = 0;

	return (ssize_t)line_size - 1;
}

void al_log_bin(int32_t pri,
				const char *file, int32_t line, const char *func,
				const void *data, size_t len)
{
    static const char *fmt_tab[] = {
        [AL_LOG_DEBUG] = AL_LOG_FMT(D, "%s"),
        [AL_LOG_INFO] = AL_LOG_FMT(I, "%s"),
        [AL_LOG_NOTICE] = AL_LOG_FMT(N, "%s"),
        [AL_LOG_WARN] = AL_LOG_FMT(W, "%s"),
        [AL_LOG_ERR] = AL_LOG_FMT(E, "%s"),
        [AL_LOG_CRIT] = AL_LOG_FMT(C, "%s"),
        [AL_LOG_ALERT] = AL_LOG_FMT(A, "%s"),
        [AL_LOG_EMERG] = AL_LOG_FMT(E, "%s"),
    };

    char buf[80];
    const char *fmt = AL_LOG_FMT(D, "%s");
    int32_t nline = len >> 4;
    int32_t remain = len & 0x0F;
    intptr_t addr = 0;
    const uint8_t *rp = (const uint8_t *)data;

    if (pri < ARRAY_SIZE(fmt_tab)) {
        fmt = fmt_tab[pri];
    }

    while (nline--) {
        hex_raw_fmt_line(buf, sizeof(buf), addr, rp + addr, BIN_LINE_SIZE);
        addr += BIN_LINE_SIZE;

        al_log(pri, file, line, func, fmt, buf);
    }

    if (remain > 0) {
        hex_raw_fmt_line(buf, sizeof(buf), addr, rp + addr, (size_t)remain);

        al_log(pri, file, line, func, fmt, buf);
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
