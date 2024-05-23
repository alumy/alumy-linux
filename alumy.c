#include "curl/curl.h"
#include "alumy/types.h"
#include "alumy/base.h"
#include "alumy/errno.h"

__BEGIN_DECLS

static uint32_t initialized = 0;

int32_t alumy_init(void)
{
    if (initialized++) {
        return 0;
    }

    if(curl_global_init(CURL_GLOBAL_DEFAULT) != CURLE_OK) {
        errno = EPERM;
        return -1;
    }

    return 0;
}

int32_t alumy_cleanup(void)
{
    if (!initialized) {
        return 0;
    }

    if (--initialized) {
        return 0;
    }

    curl_global_cleanup();

    return 0;
}

__END_DECLS

