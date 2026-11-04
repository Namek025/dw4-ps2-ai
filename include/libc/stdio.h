/* Minimal libc declarations for the Lua units (the game links Sony's newlib-based libc). */
#ifndef _LIBC_STDIO_H
#define _LIBC_STDIO_H
#include <stddef.h>
#include <stdarg.h>
#ifdef __cplusplus
extern "C" {
#endif
typedef struct _FILE FILE;
/* newlib: the standard streams live in the reentrancy struct. */
struct _reent {
    int _errno;
    FILE *_stdin, *_stdout, *_stderr;
};
extern struct _reent *_impure_ptr;
#define stdin (_impure_ptr->_stdin)
#define stdout (_impure_ptr->_stdout)
#define stderr (_impure_ptr->_stderr)
#define EOF (-1)
#define BUFSIZ 1024
int printf(const char *, ...);
int fprintf(FILE *, const char *, ...);
int sprintf(char *, const char *, ...);
int vsprintf(char *, const char *, va_list);
int fputs(const char *, FILE *);
int fflush(FILE *);
FILE *fopen(const char *, const char *);
int fclose(FILE *);
FILE *freopen(const char *, const char *, FILE *);
int getc(FILE *);
int ungetc(int, FILE *);
size_t fread(void *, size_t, size_t, FILE *);
int ferror(FILE *);
int feof(FILE *);
int fgetc(FILE *);
#ifdef __cplusplus
}
#endif
#endif
