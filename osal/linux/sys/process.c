#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include "alumy/types.h"
#include "alumy/base.h"
#include "alumy/log.h"
#include "alumy/config.h"

__BEGIN_DECLS

int_fast32_t al_write_pid(const char *path)
{
	if (path == NULL) {
		return -EINVAL;
	}

	FILE *f = fopen(path, "w");
	if (f == NULL) {
		AL_ERROR(AL_LOG, "fopen failed, path = %s, errno = %d", path, errno);
		return -EPERM;
	}

	fprintf(f, "%"PRIdFAST32"\n", (int_fast32_t)getpid());

	fclose(f);

	return 0;
}

__END_DECLS
