#ifndef __AL_TLV
#define __AL_TLV 1

#include "alumy/types.h"
#include "alumy/base.h"
#include "alumy/list.h"

__BEGIN_DECLS

#if !defined(__packed)
	#error "tlv is need __packed, please check the macro __packed"
#endif

typedef struct tlv_item {
	list_head_t link;
	uint16_t tag;
	uint16_t len;
	void *value;
} tlv_item_t;

typedef struct tlv {
	list_head_t ls;
} tlv_t;

typedef struct tlv_hdr {
	uint16_t tag;
	uint16_t len;
} __packed tlv_hdr_t;

/**
 *
 *
 * @author chenyj (2019/4/28)
 *
 * @param void
 *
 * @return tlv_t*
 */
tlv_t *tlv_init(void);

/**
 *
 *
 * @author chenyj (2019/4/28)
 *
 * @param tlv
 *
 * @return int32_t
 */
int32_t tlv_free(tlv_t *tlv);

/**
 *
 *
 * @author chenyj (2019/4/28)
 *
 * @param tlv
 * @param data
 * @param len
 *
 * @return int32_t
 */
int32_t tlv_parse(tlv_t *tlv, const void *data, size_t len);

/**
 *
 *
 * @author chenyj (2019/4/28)
 *
 * @param tlv
 * @param buf
 * @param bufsz
 *
 * @return ssize_t
 */
ssize_t tlv_dump(tlv_t *tlv, void *buf, size_t bufsz);

/**
 *
 *
 * @author chenyj (2019/4/28)
 *
 * @param tlv
 * @param tag
 *
 * @return tlv_item_t*
 */
tlv_item_t *tlv_get_object(tlv_t *tlv, uint16_t tag);

/**
 *
 *
 * @author chenyj (2019/4/28)
 *
 * @param tlv
 * @param hdr
 *
 * @return int32_t
 */
int32_t tlv_add_object(tlv_t *tlv, const tlv_hdr_t *hdr);

__END_DECLS

#endif
