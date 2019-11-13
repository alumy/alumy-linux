#ifndef __AL_CRC_H
#define __AL_CRC_H 1

#include "alumy/types.h"
#include "alumy/base.h"

__BEGIN_DECLS

/**
 * @brief Calculate crc24 value
 *
 * @author jackchen (2017/8/22)
 *
 * @param data The data to calculate crc24
 * @param len The length of data
 *
 * @return uint32_t CRC24 value after calculate
 */
uint32_t get_crc24(const void *data, size_t len) __weak __nonnull((1));

/**
 * @brief Get crc32 value
 *
 * @author jackchen (2017/8/22)
 *
 * @param data The data to calculate crc32
 * @param len The length of data
 *
 * @return uint32_t CRC32 value after calculate
 */
uint32_t get_crc32(const void *data, size_t len) __weak __nonnull((1));

__END_DECLS

#endif




