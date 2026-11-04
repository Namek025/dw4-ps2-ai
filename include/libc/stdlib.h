/* Minimal libc declarations for the Lua units (the game links Sony's newlib-based libc). */
#ifndef _LIBC_STDLIB_H
#define _LIBC_STDLIB_H
#include <stddef.h>
#ifdef __cplusplus
extern "C" {
#endif
void *malloc(size_t);
void *realloc(void *, size_t);
void free(void *);
void exit(int);
double strtod(const char *, char **);
unsigned long strtoul(const char *, char **, int);
int abs(int);
int rand(void);
void srand(unsigned);
char *getenv(const char *);
#define RAND_MAX 0x7FFFFFFF
#define EXIT_FAILURE 1
#ifdef __cplusplus
}
#endif
#endif
