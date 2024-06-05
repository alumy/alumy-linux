#ifndef __AL_NET_SLIP_H
#define __AL_NET_SLIP_H 1

#include "alumy/config.h"
#include "alumy/types.h"
#include "alumy/base.h"
#include "alumy/errno.h"

__BEGIN_DECLS

struct al_slip;
typedef struct al_slip al_slip_t;

typedef struct al_slip_opt {
    int_t (*sl_putc)(al_slip_t *slip, int_t c);
    int_t (*sl_getc)(al_slip_t *slip);
    int_t (*sl_flush)(al_slip_t *slip);    /* let to NULL, if not need */
} al_slip_opt_t;

typedef struct al_slip_cb {
    void (*recv_done)(al_slip_t *slip, const void *data, size_t len);
} al_slip_cb_t;

typedef struct al_slip {
	void *recv_buf;
	size_t recv_size;

	uintptr_t recv_wp;
	size_t recv_len;
	int_t recv_state;

    const al_slip_opt_t * const opt;
    const al_slip_cb_t * const cb;

    void *user_data;
} al_slip_t;

int_t al_slip_init(al_slip_t *slip, void *recv_buf, size_t recv_size,
                   const al_slip_opt_t *opt, const al_slip_cb_t *cb,
                   void *user_data);

size_t al_slip_write(al_slip_t *slip, const void *data, size_t len);

int_t al_slip_flush(al_slip_t *slip);

size_t al_slip_recv(al_slip_t *slip);

void al_slip_recv_1(al_slip_t *slip, const void *data, size_t len);

void *al_slip_get_recv(al_slip_t *slip);

__END_DECLS

#endif

