#ifndef __AL_PROCESS_H
#define __AL_PROCESS_H 1

#include "alumy/types.h"
#include "alumy/base.h"

__BEGIN_DECLS


/**
 * write pid file to the path
 *
 * @author chenyj (2018/09/05)
 *
 * @param path The path of pid file
 *
 * @return int_fast32_t Return 0 on success, otherwise return
 *  	   the errno
 */
int_fast32_t al_write_pid(const char *path);

__END_DECLS

#endif

