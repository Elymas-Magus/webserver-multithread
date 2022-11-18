#ifndef TIME_H_INCLUDED
#define TIME_H_INCLUDED

#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#define DATE_MAX                30

typedef struct timespec TimeSpec;

float time_diff(TimeSpec * start, TimeSpec * end);

char * getCurrentTime();

char * getCurrentTimeInHttpFormat();

char * getTimeInHttpFormat(const time_t * time);

#endif // TIME_H_INCLUDED