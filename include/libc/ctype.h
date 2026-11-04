/* Minimal libc declarations for the Lua units (the game links Sony's newlib-based libc). */
#ifndef _LIBC_CTYPE_H
#define _LIBC_CTYPE_H
#ifdef __cplusplus
extern "C" {
#endif
/* newlib: the is* tests are macros over a table (0x4D3D10). */
#define _U 01
#define _L 02
#define _N 04
#define _S 010
#define _P 020
#define _C 040
#define _X 0100
#define _B 0200
extern const char _ctype_[];
#define isalpha(c) ((_ctype_ + 1)[c] & (_U | _L))
#define isupper(c) ((_ctype_ + 1)[c] & _U)
#define islower(c) ((_ctype_ + 1)[c] & _L)
#define isdigit(c) ((_ctype_ + 1)[c] & _N)
#define isxdigit(c) ((_ctype_ + 1)[c] & (_X | _N))
#define isspace(c) ((_ctype_ + 1)[c] & _S)
#define ispunct(c) ((_ctype_ + 1)[c] & _P)
#define isalnum(c) ((_ctype_ + 1)[c] & (_U | _L | _N))
#define isprint(c) ((_ctype_ + 1)[c] & (_P | _U | _L | _N | _B))
#define iscntrl(c) ((_ctype_ + 1)[c] & _C)
int tolower(int);
int toupper(int);
#ifdef __cplusplus
}
#endif
#endif
