#include "timer.h"

float
time_diff(TimeSpec * start, TimeSpec * end)
{
    return (end->tv_sec - start->tv_sec) + 1e-6 * (end->tv_nsec - start->tv_nsec);
}

char *
getCurrentTime()
{
    time_t rawtime;

    time(&rawtime);
    
    return asctime(localtime(&rawtime));
}

char *
getCurrentTimeInHttpFormat()
{
    time_t tmi;
    struct tm * info;
    char * date = (char *) malloc(DATE_MAX);
    
    time(&tmi);
    info = localtime(&tmi);
    
    strftime(date, DATE_MAX, "%a, %d %b %Y %X %Z", info);

    return date;
}

char *
getTimeInHttpFormat(const time_t * time)
{
    struct tm * info;
    char * date = (char *) malloc(DATE_MAX);
    
    info = localtime(time);
    
    strftime(date, DATE_MAX, "%a, %d %b %Y %X %Z", info);

    return date;
}