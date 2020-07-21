#ifndef __AL_VERSION_H
#define __AL_VERSION_H

#include "alumy/types.h"
#include "alumy/base.h"

__BEGIN_DECLS

#define ALUMY_MAJOR		0
#define ALUMY_MINOR		0
#define ALUMY_PATCH		1
#define ALUMY_BUILD		10

#define AL_PREREQ(ma, mi)	\
	(ALUMY_MAJOR << 16 | ALUMY_MINOR >= (ma) << 16 | (mi))

#define AL_VERSION	\
	__MS(ALUMY_MAJOR.ALUMY_MINOR.ALUMY_PATCH.ALUMY_BUILD)

typedef struct al_version {
	uint16_t major;
	uint16_t minor;
	uint16_t patch;
	uint16_t build;
} al_version_t;

extern const char *al_get_build_time(void);
extern const char *al_get_version_s(void);
extern int32_t al_get_version(al_version_t *ver);

__END_DECLS

#endif
