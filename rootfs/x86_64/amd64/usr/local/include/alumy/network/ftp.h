#ifndef __AL_FTP_H
#define __AL_FTP_H 1

#include "alumy/types.h"
#include "alumy/base.h"

__BEGIN_DECLS

typedef struct al_ftp {
    char host[63 + 1];
    uint16_t port;
    char user[32];
    char passwd[64];
    int32_t conn_timeout;
    int32_t timeout;
    void *handle;
} al_ftp_t;

int32_t al_ftp_init(al_ftp_t *ftp, const char *host, uint16_t port,
                    const char *user, const char *passwd);

int32_t al_ftp_final(al_ftp_t *ftp);

int32_t al_ftp_set_timeout(al_ftp_t *ftp, 
						   int32_t conn_timeout, int32_t timeout);

const char* al_ftp_get_url(al_ftp_t *ftp,
                           char *dst, size_t dstsz, const char *path);

int32_t al_ftpput(al_ftp_t *ftp,
                  const char *remote_path, const char *lo_path,
                  int32_t timeout, int32_t tries);

int32_t al_ftpget(al_ftp_t *ftp, const char *lo_path, const char *remote_path);

int32_t al_ftp_del_file(al_ftp_t *ftp, const char *remote_path);


__END_DECLS

#endif
