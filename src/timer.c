#include "timer.h"

float
time_diff(TimeSpec * start, TimeSpec * end)
{
    return (end->tv_sec - start->tv_sec) + 1e-6 * (end->tv_nsec - start->tv_nsec);
}

time_t *
getCurrentTime()
{
    time_t * rawtime = (time_t *) malloc(sizeof(time_t));
    time(rawtime);
    return rawtime;
}

String
getCurrentTimeString()
{
    time_t * rawtime = getCurrentTime();
    return utcdate(rawtime);
}

String
getCurrentTimeInHttpFormat()
{
    time_t tmi;
    struct tm * info;
    String date = (String) malloc(DATE_MAX);
    
    time(&tmi);
    info = gmtime(&tmi);
    
    strftime(date, DATE_MAX, "%a, %d %b %Y %X %Z", info);

    return date;
}

String
getTimeInHttpFormat(const time_t * time)
{
    struct tm * info;
    String date = (String) malloc(DATE_MAX);
    
    info = gmtime(time);
    
    strftime(date, DATE_MAX, "%a, %d %b %Y %X %Z", info);

    return date;
}

String
getLocalCurrentTimeInHttpFormat()
{
    time_t tmi;
    struct tm * info;
    String date = (String) malloc(DATE_MAX);
    
    time(&tmi);
    info = localtime(&tmi);
    
    strftime(date, DATE_MAX, "%a, %d %b %Y %X %Z", info);

    return date;
}

String
getLocalTimeInHttpFormat(const time_t * time)
{
    struct tm * info;
    String date = (String) malloc(DATE_MAX);
    
    info = localtime(time);
    
    strftime(date, DATE_MAX, "%a, %d %b %Y %X %Z", info);

    return date;
}

String
utcdate(const time_t * timeptr)
{
    struct tm * info = localtime(timeptr);
    static char wday_name[7][3] = {
        "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
    };
    static char mon_name[12][3] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };
    static char result[26];


    sprintf(result, "%.3s %.3s%3d %.2d:%.2d:%.2d %d",
        wday_name[info->tm_wday],
        mon_name[info->tm_mon],
        info->tm_mday, info->tm_hour,
        info->tm_min, info->tm_sec,
        1900 + info->tm_year);
    return result;
}