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
 * Get the current time
 * @returns time_t
 */
time_t getCurrentTime();

/**
 * Get the current time formatted string 
 * @returns String
 */
String getCurrentTimeString();

/**
 * Get the current time formatted string
 * With Http format 
 * @returns String
 */
String getCurrentTimeInHttpFormat();

/**
 * Convert a time_t pointer to formatted string
 * With Http format 
 * @param const time_t * time
 * @returns String
 */
String getTimeInHttpFormat(const time_t * time);

/**
 * Get utc date string from time_t pointer 
 * @param const time_t * time
 * @returns String
 */
String utcdate(const time_t timeptr);

/**
 * Get current time local datetime string
 * @returns String
 */
String getLocalCurrentTimeInHttpFormat();

/**
 * Get local http date string from time_t pointer 
 * @param const time_t * time
 * @returns String
 */
String getLocalTimeInHttpFormat(const time_t * time);

#endif // TIME_H_INCLUDED