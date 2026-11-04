/* Minimal libc declarations for the Lua units (the game links Sony's newlib-based libc). */
#ifndef _LIBC_LIMITS_H
#define _LIBC_LIMITS_H
#ifdef __cplusplus
extern "C" {
#endif
#define CHAR_BIT 8
#define UCHAR_MAX 255
#define SHRT_MAX 32767
#define INT_MAX 2147483647
#define INT_MIN (-INT_MAX - 1)
#define UINT_MAX 4294967295U
#define LONG_MAX 2147483647L
#define ULONG_MAX 4294967295UL
#ifdef __cplusplus
}
#endif
#endif
