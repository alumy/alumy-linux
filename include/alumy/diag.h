#ifndef __AL_DIAG_H
#define __AL_DIAG_H

#include <time.h>
#include <sys/queue.h>
#include "alumy/types.h"
#include "alumy/base.h"

__BEGIN_DECLS

#define AL_DIAG_ITEM_NAME_MAX		32

typedef struct al_diag_elm {
	char name[AL_DIAG_ITEM_NAME_MAX];
	struct timespec start_time;
	struct timespec stop_time;

	struct {
		int_fast32_t (*init)(void *arg);
		int_fast32_t (*diag)(void *arg);
		int_fast32_t (*destroy)(void *arg);
	} opt;

	void *arg;

	TAILQ_ENTRY(al_diag_elm) link;
} al_diag_elm_t;

typedef struct al_diag {
	TAILQ_HEAD(, al_diag_elm) ls;
} al_diag_t;

int_fast32_t al_diag_init(al_diag_t *this);

int_fast32_t al_diag_add(al_diag_t *this, const char *name,
						 int_fast32_t(*init)(void *arg),
						 int_fast32_t(*diag)(void *arg),
						 int_fast32_t(*destroy)(void *arg),
						 void *arg);

int_fast32_t al_diag_dispatch(al_diag_t *this);


__END_DECLS


#endif
