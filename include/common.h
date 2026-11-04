#ifndef COMMON_H
#define COMMON_H

#include "include_asm.h"

/* Fixed-width integer types (EE-GCC 2.96, EABI64: int = 32, long = 64 bits). */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long s64;
typedef unsigned long u64;
typedef float f32;
typedef double f64;

#ifndef NULL
#define NULL 0
#endif

#endif /* COMMON_H */
