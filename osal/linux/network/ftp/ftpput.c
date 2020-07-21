#include <stdlib.h>
#include <stdio.h>
#include "alumy/types.h"
#include "alumy/base.h"
#include "alumy/network/ftp.h"
#include "alumy/errno.h"
#include "alumy/log.h"
#include "alumy/config.h"
#include "curl/curl.h"

__BEGIN_DECLS


/* parse headers for Content-Length */
static size_t getcontentlengthfunc(void *ptr, size_t size, size_t nmemb,
                                   void *stream)
{
    int r;
    long len = 0;

    r = sscanf(ptr, "Content-Length: %ld\n", &len);
    if (r)
        *((long *)stream) = len;

    return size * nmemb;
}

/* discard downloaded data */
static size_t discardfunc(void *ptr, size_t size, size_t nmemb, void *stream)
{
    (void)ptr;
    (void)stream;
    return size * nmemb;
}

/* read data to upload */
static size_t readfunc(void *ptr, size_t size, size_t nmemb, void *stream)
{
    FILE *f = stream;
    size_t n;

    if (ferror(f))
        return CURL_READFUNC_ABORT;

    n = fread(ptr, size, nmemb, f) * size;

    return n;
}


static int32_t upload(CURL *curlhandle, const char *remotepath,
                      const char *localpath, long timeout, long tries)
{
    FILE *f;
    long uploaded_len = 0;
    CURLcode r = CURLE_GOT_NOTHING;
    int c;

    f = fopen(localpath, "rb");
    if (!f) {
        perror(NULL);
        return -1;
    }

    curl_easy_reset(curlhandle);

    curl_easy_setopt(curlhandle, CURLOPT_UPLOAD, 1L);

    curl_easy_setopt(curlhandle, CURLOPT_URL, remotepath);

    if (timeout)
        curl_easy_setopt(curlhandle, CURLOPT_FTP_RESPONSE_TIMEOUT, timeout);

    curl_easy_setopt(curlhandle, CURLOPT_HEADERFUNCTION, getcontentlengthfunc);
    curl_easy_setopt(curlhandle, CURLOPT_HEADERDATA, &uploaded_len);

    curl_easy_setopt(curlhandle, CURLOPT_WRITEFUNCTION, discardfunc);

    curl_easy_setopt(curlhandle, CURLOPT_READFUNCTION, readfunc);
    curl_easy_setopt(curlhandle, CURLOPT_READDATA, f);

    /* uncomment this if you want disable passive mode */
    /* curl_easy_setopt(curlhandle, CURLOPT_FTPPORT, "-"); */
    curl_easy_setopt(curlhandle, CURLOPT_FTP_CREATE_MISSING_DIRS, 1L);

    curl_easy_setopt(curlhandle, CURLOPT_VERBOSE, 1L);

    for (c = 0; (r != CURLE_OK) && (c < tries); c++) {
        /* are we resuming? */
        if (c) { /* yes */
            /* determine the length of the file already written */

            /*
             * With NOBODY and NOHEADER, libcurl will issue a SIZE
             * command, but the only way to retrieve the result is
             * to parse the returned Content-Length header. Thus,
             * getcontentlengthfunc(). We need discardfunc() above
             * because HEADER will dump the headers to stdout
             * without it.
             */
            curl_easy_setopt(curlhandle, CURLOPT_NOBODY, 1L);
            curl_easy_setopt(curlhandle, CURLOPT_HEADER, 1L);

            r = curl_easy_perform(curlhandle);
            if (r != CURLE_OK)
                continue;

            curl_easy_setopt(curlhandle, CURLOPT_NOBODY, 0L);
            curl_easy_setopt(curlhandle, CURLOPT_HEADER, 0L);

            fseek(f, uploaded_len, SEEK_SET);

            curl_easy_setopt(curlhandle, CURLOPT_APPEND, 1L);
        } else { /* no */
            curl_easy_setopt(curlhandle, CURLOPT_APPEND, 0L);
        }

        r = curl_easy_perform(curlhandle);
    }

    fclose(f);

    if (r != CURLE_OK) {
        AL_ERROR(AL_LOG, "%s\n", curl_easy_strerror(r));
        return -1;
    }

    return 0;
}

int32_t al_ftpput(al_ftp_t *ftp,
                  const char *remote_path, const char *lo_path,
                  int32_t timeout, int32_t tries)
{
    char url[1024];

    if (al_ftp_get_url(ftp, url, sizeof(url), remote_path) == NULL) {
        set_errno(EINVAL);
        return -1;
    }

    return upload((CURL *)ftp->handle, url, lo_path, timeout, tries);
}

__END_DECLS

