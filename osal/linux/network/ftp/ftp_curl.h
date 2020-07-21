#ifndef __AL_FTP_CURL_H
#define __AL_FTP_CURL_H 1

#include "alumy/types.h"
#include "alumy/base.h"

__BEGIN_DECLS

typedef size_t curl_write_fp(
    void *ptr, size_t size, size_t nmemb, void *stream);

typedef struct curl_write_data {
    void *buf;
    size_t size;
    intptr_t wp;
} curl_write_data_t;

size_t curl_write_dft(void *ptr, size_t size, size_t nmemb, void *stream);

int32_t al_ftp_req_quote(al_ftp_t *ftp, const char *cmd,
                         curl_write_fp wr_cb, void *wr_data);

int32_t al_ftp_req_cmd(al_ftp_t *ftp, const char *cmd,
                       curl_write_fp wr_cb, void *wr_data);

__END_DECLS

#endif

