/* Minimal libc declarations for the Lua units (the game links Sony's newlib-based libc). */
#ifndef _LIBC_ASSERT_H
#define _LIBC_ASSERT_H
#ifdef __cplusplus
extern "C" {
#endif
#define assert(x) ((void)0)
#ifdef __cplusplus
}
#endif
#endif
