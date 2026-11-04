/* Minimal libc declarations for the Lua units (the game links Sony's newlib-based libc). */
#ifndef _LIBC_STDARG_H
#define _LIBC_STDARG_H
#ifdef __cplusplus
extern "C" {
#endif
typedef char *va_list;
/* MWCC (EE): named args that came in registers are spilled under the variadic
 * ones; __builtin_args_info(2) is how many, __builtin_next_arg the area's end. */
#define va_start(ap, last) \
    ((ap) = (char *)__builtin_next_arg(last) - \
            (__builtin_args_info(2) >= 8 ? 0 : (8 - __builtin_args_info(2)) * 8))
#define va_arg(ap, t) (*(t *)(((ap) += sizeof(t)) - sizeof(t)))
#define va_end(ap) ((void)0)
#ifdef __cplusplus
}
#endif
#endif
