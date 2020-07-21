#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include "alumy/base.h"
#include "alumy/log.h"
#include "alumy/config.h"

__BEGIN_DECLS

int32_t al_set_user_grp(int32_t uid, int32_t gid)
{
	if (getuid() != 0) {
		AL_INFO(AL_LOG, "only root can give away our privs");
		return 0;
	}

	struct passwd *passwdbuf;

	passwdbuf = getpwuid(uid);
	if (passwdbuf == NULL) {
		AL_CRIT(AL_LOG, "getpwuid");
		return -1;
	}

	if (initgroups(passwdbuf->pw_name, passwdbuf->pw_gid) == -1) {
		AL_CRIT(AL_LOG, "initgroups");
		return -1;
	}

	if (setgid(gid) == -1) {
		AL_CRIT(AL_LOG, "setgid");
		return -1;
	}

	if (setuid(uid) == -1) {
		AL_CRIT(AL_LOG, "setuid");
		return -1;
	}

	/* test for failed-but-return-was-successful setuid
	 * http://www.securityportal.com/list-archive/bugtraq/2000/Jun/0101.html
	 */
	if (setuid(0) != -1) {
		AL_CRIT(AL_LOG, "icky Linux kernel bug!");
		return -1;
	}

	return 0;
}

__END_DECLS


