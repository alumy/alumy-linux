
#ifndef __RESTFUL_H
#define __RESTFUL_H 1

#include <sys/queue.h>
#include <evhttp.h>
#include "alumy/types.h"
#include "alumy/base.h"

__BEGIN_DECLS

enum {
	RESTFUL_GET = 0,
	RESTFUL_PUT,
	RESTFUL_POST,
	RESTFUL_PATCH,
	RESTFUL_DEL,
};

typedef void (*evhttp_req_cb_fp)(struct evhttp_request *req, void *arg);

typedef struct restful_item {
	TAILQ_ENTRY(restful_item) entry;

	int32_t cmd;
	char *endpoint;
	evhttp_req_cb_fp req_cb;
} restful_item_t;

typedef struct restful {
	TAILQ_HEAD(, restful_item) head;
} restful_t;



__END_DECLS


#endif
