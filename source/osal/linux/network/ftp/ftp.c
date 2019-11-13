#include <stdlib.h>
#include <stdio.h>
#include "alumy/string.h"
#include "alumy/types.h"
#include "alumy/base.h"
#include "alumy/network/ftp.h"
#include "alumy/log.h"
#include "alumy/errno.h"
#include "curl/curl.h"
#include "ftp_curl.h"

__BEGIN_DECLS

const char *al_ftp_get_url(al_ftp_t *ftp,
                           char *dst, size_t dstsz, const char *path)
{
    ssize_t l = snprintf(dst, dstsz, "ftp://%s:%s@%s:%d/%s",
                         ftp->user, ftp->passwd, ftp->host, ftp->port, path);
    if (l <= 0 || l > dstsz - 1) {
        return NULL;
    }

    return (const char *)dst;
}

int32_t al_ftp_init(al_ftp_t *ftp, const char *host, uint16_t port,
                    const char *user, const char *passwd)
{
    strlcpy(ftp->host, host, sizeof(ftp->host));
    ftp->port = port;
    strlcpy(ftp->user, user, sizeof(ftp->user));
    strlcpy(ftp->passwd, passwd, sizeof(ftp->passwd));
	ftp->conn_timeout = 30;
    ftp->timeout = 300;

    ftp->handle = curl_easy_init();
    if (ftp->handle == NULL) {
        return -1;
    }

    return 0;
}

int32_t al_ftp_final(al_ftp_t *ftp)
{
    if (ftp->handle != NULL) {
        curl_easy_cleanup((CURL *)ftp->handle);
    }

    curl_global_cleanup();
    return 0;
}

int32_t al_ftp_set_timeout(al_ftp_t *ftp, 
						   int32_t conn_timeout, int32_t timeout)
{
	ftp->conn_timeout = conn_timeout;
	ftp->timeout = timeout;

	return 0;
}

size_t curl_write_dft(void *ptr, size_t size, size_t nmemb, void *stream)
{
    if (stream == NULL || ptr == NULL || size == 0) {
        return 0;
    }

    curl_write_data_t *wd = (curl_write_data_t *)stream;

    AL_ASSERT(wd->wp <= wd->size);

    size_t len = min(size * nmemb, wd->size - 1 - wd->wp);

    memcpy((void *)((intptr_t)wd->buf + wd->wp),  ptr, len);

    wd->wp += len;

    /* write EOF at the end of stream */
    *((char *)wd->buf + wd->wp) = 0;

    return len;
}

int32_t al_ftp_req_quote(al_ftp_t *ftp, const char *cmd,
                         curl_write_fp wr_cb, void *wr_data)
{
    CURL *curl = (CURL *)ftp->handle;

    curl_easy_reset(curl);

    char url[1024] = { 0 };
    char userpwd[256] = { 0 };

    snprintf(url, sizeof(url), "ftp://%s/", ftp->host);
    snprintf(userpwd, sizeof(userpwd), "%s:%s", ftp->user, ftp->passwd);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_PORT, ftp->port);
    curl_easy_setopt(curl, CURLOPT_USERPWD, userpwd);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);

    struct curl_slist *slist = NULL;

    slist = curl_slist_append(slist, cmd);
    curl_easy_setopt(curl, CURLOPT_QUOTE, slist);

    if (wr_cb != NULL) {
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, wr_cb);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, wr_data);
    }

    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, ftp->conn_timeout);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, ftp->timeout);
    curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1);

    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    CURLcode res = curl_easy_perform(curl);

    if (slist != NULL) {
        curl_slist_free_all(slist); //free the list again
    }

    if (res != CURLE_OK) {
        return -1;
    }

    return 0;
}

int32_t al_ftp_req_cmd(al_ftp_t *ftp, const char *cmd,
                       curl_write_fp wr_cb, void *wr_data)
{
    CURL *curl = (CURL *)ftp->handle;

    curl_easy_reset(curl);

    char url[1024] = { 0 };
    char userpwd[256] = { 0 };

    snprintf(url, sizeof(url), "ftp://%s/", ftp->host);
    snprintf(userpwd, sizeof(userpwd), "%s:%s", ftp->user, ftp->passwd);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_PORT, ftp->port);
    curl_easy_setopt(curl, CURLOPT_USERPWD, userpwd);
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);

    curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, cmd);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, wr_cb);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, wr_data);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, ftp->conn_timeout);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, ftp->timeout);
    curl_easy_setopt(curl, CURLOPT_FTP_CREATE_MISSING_DIRS, 1);

    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        set_errno(EPERM);

        if ((strstr(cmd, "MKD ") != NULL || strstr(cmd, "RMD ") != NULL) &&
            (res == CURLE_FTP_COULDNT_RETR_FILE)) {
            set_errno(ENOENT);
        }

        return -1;
    }

    return 0;
}

