#ifndef _TM_H
#define _TM_H
typedef long time_t;
struct tm { int tm_sec,tm_min,tm_hour,tm_mday,tm_mon,tm_year,tm_wday,tm_yday,tm_isdst; };
struct tm *gmtime(const time_t*);
time_t time(time_t*);
#endif
