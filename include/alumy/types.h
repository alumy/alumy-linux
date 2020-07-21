#ifndef __AL_TYPES_H
#define __AL_TYPES_H 1

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <sys/types.h>

#ifndef UNUSED
#define UNUSED(v)	(void)(v)
#endif

#ifndef container_of
#define container_of(ptr, type, member) ({		\
	const typeof(((type *)0)->member) * __mptr = (ptr);		\
	(type *)((uintptr_t)__mptr - offsetof(type, member)); })
#endif

#ifndef min
#define min(x,y) ({				\
	typeof(x) _x = (x);			\
	typeof(y) _y = (y);			\
	(void) (&_x == &_y);		\
	_x < _y ? _x : _y; })
#endif

#ifndef max
#define max(x,y) ({				\
	typeof(x) _x = (x);			\
	typeof(y) _y = (y);			\
	(void) (&_x == &_y);		\
	_x > _y ? _x : _y; })
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof((a)) / sizeof((a)[0]))
#endif

__inline__ void split_write_two(void *_addr, uint16_t two, bool le)
{
	uint8_t *addr = (uint8_t *)_addr;

	if (le) {
		addr[0] = two & 0xFF;
		addr[1] = (two & 0xFF00) >> 8;
	} else {
		addr[1] = two & 0xFF;
		addr[0] = (two & 0xFF00) >> 8;
	}
}

__inline__ uint16_t split_read_two(const void *_addr, bool le)
{
	uint16_t ret;
	const uint8_t *two = (const uint8_t *)_addr;

	if (le) {
		ret = (two[1] << 8) | two[0];
	} else {
		ret = (two[0] << 8) | two[1];
	}

	return ret;
}


__inline__ void split_write_four(void *_addr, uint32_t four, bool le)
{
	uint8_t *addr = (uint8_t *)_addr;

	if (le) {
		addr[0] = four & 0xFF;
		addr[1] = (four & 0x0000FF00) >> 8;
		addr[2] = (four & 0x00FF0000) >> 16;
		addr[3] = (four & 0xFF000000) >> 24;
	} else {
		addr[3] = four & 0xFF;
		addr[2] = (four & 0x0000FF00) >> 8;
		addr[1] = (four & 0x00FF0000) >> 16;
		addr[0] = (four & 0xFF000000) >> 24;
	}
}


__inline__ uint32_t split_read_four(const void *addr, bool le)
{
	uint32_t ret;
	const uint8_t *four = (const uint8_t *)addr;

	if (le) {
		ret = four[0] | (four[1] << 8) | (four[2] << 16) | (four[3] << 24);
	} else {
		ret = four[3] | (four[2] << 8) | (four[1] << 16) | (four[0] << 24);
	}

	return ret;
}

__inline__ void split_write_eight(void *_addr, uint64_t eight, bool le)
{
	uint8_t *addr = (uint8_t *)_addr;

	if (le) {
		addr[0] = eight & 0x00000000000000FF;
		addr[1] = (eight & 0x000000000000FF00) >> 8;
		addr[2] = (eight & 0x0000000000FF0000) >> 16;
		addr[3] = (eight & 0x00000000FF000000) >> 24;
		addr[4] = (eight & 0x000000FF00000000) >> 32;
		addr[5] = (eight & 0x0000FF0000000000) >> 40;
		addr[6] = (eight & 0x00FF000000000000) >> 48;
		addr[7] = (eight & 0xFF00000000000000) >> 56;
	} else {
		addr[7] = eight & 0x00000000000000FF;
		addr[6] = (eight & 0x000000000000FF00) >> 8;
		addr[5] = (eight & 0x0000000000FF0000) >> 16;
		addr[4] = (eight & 0x00000000FF000000) >> 24;
		addr[3] = (eight & 0x000000FF00000000) >> 32;
		addr[2] = (eight & 0x0000FF0000000000) >> 40;
		addr[1] = (eight & 0x00FF000000000000) >> 48;
		addr[0] = (eight & 0xFF00000000000000) >> 56;
	}
}

__inline__ uint64_t split_read_eight(const void *addr, bool le)
{
	uint64_t ret;
	const uint8_t *eight = (const uint8_t *)addr;

	if (le) {
		ret = (((uint64_t)eight[7] << 56) | ((uint64_t)eight[6] << 48) |
			   ((uint64_t)eight[5] << 40) | ((uint64_t)eight[4] << 32) |
			   ((uint64_t)eight[3] << 24) | ((uint64_t)eight[2] << 16) |
			   ((uint64_t)eight[1] << 8) | (uint64_t)eight[0]);
	} else {
		ret = (((uint64_t)eight[0] << 56) | ((uint64_t)eight[1] << 48) |
			   ((uint64_t)eight[2] << 40) | ((uint64_t)eight[3] << 32) |
			   ((uint64_t)eight[4] << 24) | ((uint64_t)eight[5] << 16) |
			   ((uint64_t)eight[6] << 8) | (uint64_t)eight[7]);
	}

	return ret;
}

#endif	/* end of _TYPES_H */

