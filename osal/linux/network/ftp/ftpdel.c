#include <stdlib.h>
#include <stdio.h>
#include "alumy/types.h"
#include "alumy/base.h"
#include "alumy/network/ftp.h"
#include "alumy/errno.h"
#include "alumy/config.h"
#include "alumy/log.h"
#include "curl/curl.h"
#include "ftp_curl.h"

__BEGIN_DECLS

int32_t al_ftp_del_file(al_ftp_t *ftp, const char *remote_path)
{
    char cmd[1024];

    snprintf(cmd, sizeof(cmd), "DELE %s", remote_path);

    return al_ftp_req_quote(ftp, cmd, NULL, NULL);
}

__END_DECLS

