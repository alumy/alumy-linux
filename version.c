#include "alumy/version.h"
#include "alumy/types.h"
#include "alumy/base.h"
#include "alumy/errno.h"

__BEGIN_DECLS

const char *al_get_build_time(void)
{
	return __DATE__" "__TIME__;
}

const char *al_get_version_s(void)
{
	return AL_VERSION;
}

int32_t al_get_version(al_version_t *ver)
{
	if(ver == NULL) {
        set_errno(EINVAL);
        return -1;
	}

	ver->major = ALUMY_MAJOR;
	ver->minor = ALUMY_MINOR;
	ver->patch = ALUMY_PATCH;
	ver->build = ALUMY_BUILD;

	return 0;
}

__END_DECLS

