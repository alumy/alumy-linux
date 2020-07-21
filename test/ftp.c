#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "CUnit/CUnit.h"
#include "CUnit/Automated.h"
#include "CUnit/Basic.h"
#include "alumy.h"
#include "test.h"
#include "curl/curl.h"

__BEGIN_DECLS

#define FTP_HOST        "ftp.example.com"
#define FTP_PORT        21
#define FTP_USER        "user"
#define FTP_PASSWD      "passwd"
#define FTP_REMOTE_PATH     "path/to/file"
#define FTP_LO_PATH         "path/to/file"

static int ftp_suite_init(void)
{
    if(al_log_open("bcd", "zlog.conf", "test") != 0) {
        return -1;
    }

    alumy_init();

    return 0;
}

static int ftp_suite_clean(void)
{
    alumy_cleanup();
    al_log_close();

    return 0;
}

static void ftpput_test(void)
{
    al_ftp_t ftp;

    CU_ASSERT(
        al_ftp_init(&ftp, FTP_HOST, FTP_PORT, FTP_USER, FTP_PASSWD) == 0);

    CU_ASSERT(
        al_ftpput(&ftp, FTP_REMOTE_PATH, FTP_LO_PATH, 0, 10) == 0);

    CU_ASSERT(al_ftp_final(&ftp) == 0);
}

static void ftpget_test(void)
{
    al_ftp_t ftp;

    CU_ASSERT(
        al_ftp_init(&ftp, FTP_HOST, FTP_PORT, FTP_USER, FTP_PASSWD) == 0);

    CU_ASSERT(
        al_ftpget(&ftp, FTP_LO_PATH, FTP_REMOTE_PATH) == 0);

    CU_ASSERT(al_ftp_final(&ftp) == 0);
}

static void ftp_del_file_test(void)
{
    al_ftp_t ftp;

    CU_ASSERT(
        al_ftp_init(&ftp, FTP_HOST, FTP_PORT, FTP_USER, FTP_PASSWD) == 0);

    CU_ASSERT(al_ftp_del_file(&ftp, FTP_REMOTE_PATH) == 0);
    CU_ASSERT(al_ftp_final(&ftp) == 0);
}

void add_ftp_tests(void)
{
    CU_pSuite suite;

    suite = CU_add_suite("ftp", ftp_suite_init, ftp_suite_clean);

    CU_add_test(suite, "ftpget_test", ftpget_test);
    CU_add_test(suite, "ftpput_test", ftpput_test);
    CU_add_test(suite, "ftp_del_file_test", ftp_del_file_test);
}

__END_DECLS

