#include <sys/types.h>
#include <grp.h>
#include "alumy/base.h"
#include "alumy/log.h"
#include "alumy/config.h"

__BEGIN_DECLS


int32_t al_get_group_gid(const char *grp)
{

	char *endptr;

	long i = strtol(grp, &endptr, 0);

	if (*grp != '\0' && *endptr == '\0') {
		return i;
	}

	struct group *groupbuf;

	groupbuf = getgrnam(grp);
	if (groupbuf == NULL) {
		AL_ERROR(AL_LOG, "No such group: %s", grp);
		return -1;
	}

	return groupbuf->gr_gid;
}

__END_DECLS

