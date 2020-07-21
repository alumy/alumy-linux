#include <sys/types.h>
#include <pwd.h>
#include "alumy/base.h"
#include "alumy/log.h"
#include "alumy/config.h"

__BEGIN_DECLS

int32_t al_get_user_uid(const char *user)
{
	char *endptr;

	long i = strtol(user, &endptr, 0);

	if (*user != '\0' && *endptr == '\0') {
		return i;
	}

	struct passwd *passwdbuf;

	passwdbuf = getpwnam(user);
	if (passwdbuf == NULL) {
		AL_ERROR(AL_LOG, "No such user: %s", user);
		return -1;
	}

	return passwdbuf->pw_uid;
}

__END_DECLS

