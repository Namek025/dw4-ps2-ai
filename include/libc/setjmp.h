/* Minimal libc declarations for the Lua units (the game links Sony's newlib-based libc). */
#ifndef _LIBC_SETJMP_H
#define _LIBC_SETJMP_H
#ifdef __cplusplus
extern "C" {
#endif
/* 208 bytes: lua_longjmp.status sits at 212 (luaD_breakrun). */
typedef int jmp_buf[52];
int setjmp(jmp_buf);
void longjmp(jmp_buf, int);
#ifdef __cplusplus
}
#endif
#endif
