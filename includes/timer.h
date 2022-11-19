#ifndef TIME_H_INCLUDED
#define TIME_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#include "type_aliases.h"

#define DATE_MAX                30

typedef struct timespec TimeSpec;

/**
 */
float time_diff(TimeSpec * start, TimeSpec * end);

/**
 */
time_t * getCurrentTime();

/**
 */
String getCurrentTimeString();

/**
 */
String getCurrentTimeInHttpFormat();

/**
 */
String getTimeInHttpFormat(const time_t * time);

/**
 */
String utcdate(const time_t * timeptr);

/**
 */
String getLocalCurrentTimeInHttpFormat();

/**
 */
String getLocalTimeInHttpFormat(const time_t * time);

#endif // TIME_H_INCLUDED