#ifndef __AL_FIFO_H
#define __AL_FIFO_H 1

#include "alumy/types.h"
#include "alumy/base.h"

__BEGIN_DECLS

/**
 * @brief The data struct of fifo
 *
 * @param buf The buffer which is used by fifo
 * @param size The size of buffer
 * @param in The write in pointer
 * @param out The read out pointer
 * @param put The put function pointer
 * @param get The get function pointer
 */
typedef struct fifo{
	uint8_t *buf;
	size_t size;
	uintptr_t in;
	uintptr_t out;
}fifo_t;

/**
 * @brief Initialize a fifo
 *
 * @author jackchen (2017/8/22)
 *
 * @param fifo The fifo handle
 * @param buf The buffer which is used by fifo
 * @param size The size of buffer
 */
void fifo_init(fifo_t *fifo, uint8_t *buf, size_t size);
size_t fifo_get(fifo_t *fifo, uint8_t *data, size_t len);
size_t fifo_put(fifo_t *fifo, const uint8_t *data, size_t len);

__END_DECLS

#endif // FIFO_H

