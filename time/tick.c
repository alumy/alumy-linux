#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "alumy/config.h"
#include "alumy/byteorder.h"
#include "alumy/types.h"
#include "alumy/base.h"
#include "alumy/bug.h"
#include "alumy/errno.h"
#include "alumy/time/tick.h"

__BEGIN_DECLS

void al_tick_init(void)
{
}

void al_tick_inc(void)
{
}

uint32_t al_tick_get_tick(void)
{
    struct timespec t;

    clock_gettime(CLOCK_MONOTONIC, &t);

    return t.tv_sec * 1000 + t.tv_nsec / 1000;
}

uint32_t al_tick_get_sec(void)
{
    struct timespec t;

    clock_gettime(CLOCK_MONOTONIC, &t);

    return t.tv_sec;
}

uint32_t al_tick_get_msec(void)
{
    struct timespec t;

    clock_gettime(CLOCK_MONOTONIC, &t);

    return t.tv_sec * 1000 + t.tv_nsec / 1000;
}

uint32_t al_tick_elapsed_tick(uint32_t last)
{
    return al_tick_get_tick() - last;
}

uint32_t al_tick_elapsed_sec(uint32_t last)
{
    return al_tick_get_sec() - last;
}

__END_DECLS

