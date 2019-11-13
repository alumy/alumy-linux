#include <stdlib.h>
#include <stdio.h>
#include "alumy/types.h"
#include "alumy/base.h"
#include "alumy/network/ftp.h"
#include "alumy/errno.h"
#include "alumy/config.h"
#include "alumy/log.h"
#include "curl/curl.h"

__BEGIN_DECLS

/* <DESC>
 * Get a single file from an FTP server.
 * </DESC>
 */

struct ftpfile {
  const char *filename;
  FILE *stream;
};

static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
{
  struct ftpfile *out = (struct ftpfile *)stream;

  if(out && !out->stream) {
    /* open file for writing */
    out->stream = fopen(out->filename, "wb");
    if(!out->stream)
      return -1; /* failure, can't open file to write */
  }

  return fwrite(buffer, size, nmemb, out->stream);
}


int32_t al_ftpget(al_ftp_t *ftp, const char *lo_path, const char *remote_path)
{
    int32_t rv = 0;

    CURLcode res;
    char url[1024];
    struct ftpfile ftpfile = { 0 };

    if (al_ftp_get_url(ftp, url, sizeof(url), remote_path) == NULL) {
        return -1;
    }

    ftpfile.filename = lo_path;

    CURL *curl = (CURL *)ftp->handle;

    curl_easy_reset(curl);

    /*
     * You better replace the URL with one that works!
     */
    curl_easy_setopt(curl, CURLOPT_URL, url);
    /* Define our callback to get called when there's data to be written */
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite);
    /* Set a pointer to our struct to pass to the callback */
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ftpfile);

    /* Switch on full protocol/debug output */
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    res = curl_easy_perform(curl);

    if (ftpfile.stream)
        fclose(ftpfile.stream); /* close the local file */

    if (res != CURLE_OK) {
        /* we failed */
        AL_ERROR(AL_LOG, "curl told us %d\n", res);

        set_errno(EPERM);

        if (res == CURLE_REMOTE_FILE_NOT_FOUND) {
            set_errno(ENOENT);
        }

        rv = -1;
    }

    return rv;
}

__END_DECLS

