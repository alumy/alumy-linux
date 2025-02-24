#ifndef __AL_TYPES_H
#define __AL_TYPES_H 1

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <linux/types.h>
#include <sys/types.h>

#include "alumy/config.h"
#include "alumy/typecheck.h"


#ifndef __inline
#define __inline		inline
#endif

#ifndef __inline__
#define __inline__		__inline
#endif

#ifndef __static_inline__
#define __static_inline__		static __inline
#endif

#ifndef __has_builtin
#define __has_builtin(x)	(0)
#endif

#ifndef UNUSED
#define UNUSED(v)	(void)(v)
#endif

#ifndef container_of
#define container_of(ptr, type, member) ({					\
    const typeof(((type *)0)->member) *__mptr = (ptr);		\
    (type *)((void *)__mptr - offsetof(type, member)); })
#endif

#ifndef min
#if defined(__GNUC__)
    #define min(x,y) ({             \
        typeof(x) _x = (x);         \
        typeof(y) _y = (y);         \
        (void) (&_x == &_y);        \
        _x < _y ? _x : _y; })
#elif defined(__CC_ARM)
    #define min(x,y)        ((x) < (y) ? (x) : (y))
#else
    #define min(x,y)        ((x) < (y) ? (x) : (y))
#endif
#endif

#ifndef max
#if defined(__GNUC__)
    #define max(x,y) ({             \
        typeof(x) _x = (x);         \
        typeof(y) _y = (y);         \
        (void) (&_x == &_y);        \
        _x > _y ? _x : _y; })
#elif defined(__CC_ARM)
    #define max(x,y)        ((x) > (y) ? (x) : (y))
#else
    #define max(x,y)        ((x) > (y) ? (x) : (y))
#endif
#endif

/**
 * min3 - return minimum of three values
 * @x: first value
 * @y: second value
 * @z: third value
 */
#ifndef min3
#if defined(__GNUC__)
    #define min3(x, y, z) min((typeof(x))min((x), (y)), (z))
#elif defined(__CC_ARM)
    #define min3(x, y, z) min(min((x), (y)), (z))
#else
    #define min3(x, y, z) min(min((x), (y)), (z))
#endif
#endif

/**
 * max3 - return maximum of three values
 * @x: first value
 * @y: second value
 * @z: third value
 */
#ifndef max3
#if defined(__GNUC__)
    #define max3(x, y, z) max((typeof(x))max((x), (y)), (z))
#elif defined(__CC_ARM)
    #define max3(x, y, z) max(max((x), (y)), (z))
#else
    #define max3(x, y, z) max(max((x), (y)), (z))
#endif
#endif

/*
 *  * ..and if you can't take the strict
 *   * types, you can specify one yourself.
 *    *
 *     * Or not use min/max/clamp at all, of course.
 *      */
#define min_t(type, x, y) ({              \
    type __min1 = (x);                    \
    type __min2 = (y);                    \
    __min1 < __min2 ? __min1: __min2; })

#define max_t(type, x, y) ({              \
    type __max1 = (x);                    \
    type __max2 = (y);                    \
    __max1 > __max2 ? __max1: __max2; })

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof((a)) / sizeof((a)[0]))
#endif

#define __KERNEL_DIV_ROUND_UP(n, d) (((n) + (d) - 1) / (d))

#define DECLARE_BITMAP(name,bits) \
    unsigned long name[BITS_TO_LONGS(bits)]

#if !(defined( __off_t_defined) || \
	  defined(__DEFINED_off_t))
typedef long int off_t;
#define __off_t_defined
#define __DEFINED_off_t
#endif

#if defined(__CC_ARM)
	#ifndef __ssize_t_defined
	typedef long ssize_t;
	#define __ssize_t_defined
	#endif

	#ifndef __int_t_defined
	typedef signed int int_t;
	#define __int_t_defined
	#endif

	#ifndef __uint_t_defined
	typedef unsigned int uint_t;
	#define __uint_t_defined
	#endif
#endif

#if defined(__ICCRL78__)
	#ifndef __ssize_t_defined
	typedef signed short ssize_t;
	#define __ssize_t_defined
	#endif

	#ifndef __int_t_defined
	typedef signed int int_t;
	#define __int_t_defined
	#endif

	#ifndef __uint_t_defined
	typedef unsigned int uint_t;
	#define __uint_t_defined
	#endif
#endif

#if defined(__GNUC__)
    #if !(defined(__ssize_t_defined) || \
          defined(_SSIZE_T_DECLARED) || \
          defined(_SSIZE_T_DEFINED) || \
          defined(__DEFINED_ssize_t))
        typedef long ssize_t;
        #define __ssize_t_defined
        #define __DEFINED_ssize_t
        #define _SSIZE_T_DECLARED
        #define _SSIZE_T_DEFINED
        
        #ifndef SSIZE_MAX
            #define SSIZE_MAX INT_MAX
        #endif
    #endif

	#ifndef __int_t_defined
	typedef signed int int_t;
	#define __int_t_defined
	#endif

	#ifndef __uint_t_defined
	typedef unsigned int uint_t;
	#define __uint_t_defined
	#endif
#endif

#ifndef __bool_t_defined
typedef uint_t bool_t;
#define __bool_t_defined
#endif

#ifndef __char_t_defined
typedef char char_t;
#define __char_t_defined
#endif

typedef __u16 u16;
typedef __u32 u32;
typedef __u64 u64;

/* bsd */
typedef unsigned char       u_char;
typedef unsigned short      u_short;
typedef unsigned int        u_int;
typedef unsigned long       u_long;

/* sysv */
typedef unsigned char       unchar;
typedef unsigned short      ushort;
typedef unsigned int        uint;
typedef unsigned long       ulong;

#endif	/* end of _TYPES_H */

