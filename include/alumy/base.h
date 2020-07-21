#ifndef __AL_BASE__
#define __AL_BASE__ 1

#include <sys/cdefs.h>

#if __GNUC__

#ifndef __inline__
#define __inline__		static inline
#endif

#ifndef __attribute_const__
#define __attribute_const__		__attribute__((__const__))
#endif

#ifndef __deprecated
#define __deprecated		__attribute__((deprecated))
#endif

#ifndef __packed
#define __packed			__attribute__((packed))
#endif

#define __weak				__attribute__((weak))
#define __naked				__attribute__((naked)) noinline __noclone notrace
#define __noreturn			__attribute__((noreturn))

#ifndef __aligned
#define __aligned(x)		__attribute_aligned__(x)
#endif

#ifndef __used
#define __used				__attribute_used__
#endif

#ifndef __printf
#define __printf(a,b)		__attribute__((format(printf,a,b)))
#endif

#ifndef __scanf
#define __scanf(a,b)		__attribute__((format(scanf,a,b)))
#endif

#ifndef __maybe_unused
#define __maybe_unused		__attribute__((unused))
#endif

#ifndef __always_unused
#define __always_unused		__attribute__((unused))
#endif

#ifndef __section
#define __section(sec)		__attribute_used__ __attribute__((section(sec)))
#endif

#ifndef __unlikely
#define __unlikely(cond)	__glibc_unlikely(cond)
#endif

#ifndef __likely
#define __likely(cond)		__glibc_likely(cond)
#endif

#else

#define __inline__
#define __deprecated
#define __packed
#define __weak
#define __naked

#define __noreturn
#define __aligned(x)
#define __printf(a,b)
#define __scanf(a,b)
#define __maybe_unused
#define __always_unused

#endif

#ifndef __STRING
#define __STRING(x) #x
#endif

#ifndef __MS
#define __MS(x)		__STRING(x)
#endif

#ifndef __ES
#define __ES(v)		[v] = __STRING(v)
#endif

#endif

