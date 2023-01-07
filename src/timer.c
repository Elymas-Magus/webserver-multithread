#include "timer.h"

time_t
getCurrentTime()
{
    time_t rawtime;
    time(&rawtime);
    return rawtime;
}

String
getCurrentTimeString()
{
    return utcdate(getCurrentTime());
}

String
getCurrentTimeInHttpFormat()
{
    time_t tmi;
    struct tm * info;
    String date = (String) malloc(DATE_MAX);
    
    time(&tmi);
    info = gmtime(&tmi);
    validatePointerOrDie(info, "Time info failed\n");
    
    strftime(date, DATE_MAX, "%a, %d %b %Y %X %Z", info);

    return date;
}

String
getTimeInHttpFormat(const time_t * time)
{
    struct tm * info;
    String date = (String) malloc(DATE_MAX);
    
    info = gmtime(time);
    validatePointerOrDie(info, "Time info failed\n");
    
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
    validatePointerOrDie(info, "Time info failed\n");
    
    strftime(date, DATE_MAX, "%a, %d %b %Y %X %Z", info);

    return date;
}

String
getLocalTimeInHttpFormat(const time_t * time)
{
    struct tm * info;
    String date = (String) malloc(DATE_MAX);
    
    info = localtime(time);
    validatePointerOrDie(info, "Time info failed\n");
    
    strftime(date, DATE_MAX, "%a, %d %b %Y %X %Z", info);

    return date;
}

String
utcdate(const time_t timeptr)
{
    struct tm * info = localtime(&timeptr);
    validatePointerOrDie(info, "Time info failed\n");
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