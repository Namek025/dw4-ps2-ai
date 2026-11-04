/* Minimal libc declarations for the Lua units (the game links Sony's newlib-based libc). */
#ifndef _LIBC_MATH_H
#define _LIBC_MATH_H
#ifdef __cplusplus
extern "C" {
#endif
double fabs(double);
double sin(double);
double cos(double);
double tan(double);
double asin(double);
double acos(double);
double atan(double);
double atan2(double, double);
double ceil(double);
double floor(double);
double fmod(double, double);
double sqrt(double);
double pow(double, double);
double log(double);
double log10(double);
double exp(double);
double frexp(double, int *);
double ldexp(double, int);
#define HUGE_VAL 1e300
#ifdef __cplusplus
}
#endif
#endif
