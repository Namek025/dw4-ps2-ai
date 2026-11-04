/* Minimal libc declarations for the Lua units (the game links Sony's newlib-based libc). */
#ifndef _LIBC_STDDEF_H
#define _LIBC_STDDEF_H
#ifdef __cplusplus
extern "C" {
#endif
typedef unsigned int size_t;
typedef int ptrdiff_t;
#define NULL 0
#define offsetof(t, m) ((size_t)&((t *)0)->m)
#ifdef __cplusplus
}
#endif
#endif
