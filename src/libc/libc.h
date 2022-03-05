#ifndef LIBC_H
#define LIBC_H
#include <stddef.h>
#include <stdarg.h>

#if defined(__clang__)
#	define memcpy(dst, src, size) __builtin_memcpy((dst, (src), (size)))
#	define memmove(dst, src, size) __builtin_memmove((dst), (src), (size))
#	define memset(dst, val, size) __builtin_memset((dst), (val), (size))
#	define memcmp(p1, p2, size) __builtin_memcmp((p1), (p2), (size))
#	define strlen(str) __builtin_strlen((str))
#endif

#endif
