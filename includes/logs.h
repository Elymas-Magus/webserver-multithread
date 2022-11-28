#ifndef LOGS_H_INCLUDED
#define LOGS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include "type_aliases.h"

#define DATE_MAX                     30
#define LOG_PATH                     "logs/"
#define LOG_ERROR_PATH               "logs/errors"
#define LOG_CONNECTION_PATH          "logs/connection-%Y-%m-%d"
#define LOG_REQUEST_PATH             "logs/request-%Y-%m-%d"
#define LOG_EXTENSION                ".log"
#define APPEND                       "a"

#define FULL_LOG_ERROR_PATH          "logs/errors"LOG_EXTENSION
#define FULL_LOG_WARNING_PATH        "logs/warning"LOG_EXTENSION
#define FULL_LOG_CONNECTION_PATH     "logs/connection-%Y-%m-%d"LOG_EXTENSION
#define FULL_LOG_REQUEST_PATH        "logs/request-%Y-%m-%d"LOG_EXTENSION
#define FULL_LOG_EXTENSION           ".log"LOG_EXTENSION


#define WARNING(...) fprintf(stderr, __VA_ARGS__)
#define PRINT_EXPRESSION(x) (printf("%s = %d\n", #x, (x)))
#define __LOG(file, fmt, ...) (fprintf(file, fmt, __VA_ARGS__))


#define LOG(fmt, ...)                                                       \
    do {                                                                    \
        time_t tmi;                                                         \
        struct tm * info;                                                   \
        char filename[DATE_MAX];                                            \
                                                                            \
        FILE * file = NULL;                                                 \
                                                                            \
        time(&tmi);                                                         \
        info = localtime(&tmi);                                             \
                                                                            \
        strftime(filename, DATE_MAX, FULL_LOG_REQUEST_PATH, info);          \
        strcat(filename, LOG_EXTENSION);                                    \
        file = fopen(filename, APPEND);                                     \
        if (!file) break;                                                   \
                                                                            \
        __LOG(file, fmt, __VA_ARGS__);                                      \
        __LOG(stderr, fmt, __VA_ARGS__);                                    \
                                                                            \
        fclose(file);                                                       \
    } while (0);


#define LOG_CONNECTTION(filename, fmt, ...)                                 \
    do {                                                                    \
        time_t tmi;                                                         \
        struct tm * info;                                                   \
                                                                            \
        FILE * file = NULL;                                                 \
                                                                            \
        time(&tmi);                                                         \
        info = localtime(&tmi);                                             \
                                                                            \
        strftime(filename, DATE_MAX, LOG_CONNECTION_PATH, info);            \
        strcat(filename, LOG_EXTENSION);                                    \
        file = fopen(filename, APPEND);                                     \
        if (!file) break;                                                   \
                                                                            \
        __LOG(file, fmt, __VA_ARGS__);                                      \
        __LOG(stderr, fmt, __VA_ARGS__);                                    \
                                                                            \
        fclose(file);                                                       \
    } while (0);
    

#define LOG_CONNECTTION_ON_FILE(filename, fmt , ...)                        \
    do {                                                                    \
        FILE * file = NULL;                                                 \
                                                                            \
        file = fopen(filename, APPEND);                                     \
        if (!file) break;                                                   \
                                                                            \
        __LOG(file, fmt, __VA_ARGS__);                                      \
        __LOG(stderr, fmt, __VA_ARGS__);                                    \
                                                                            \
        fclose(file);                                                       \
    } while (0);


#define LOG_ERROR(fmt, ...)                                                 \
    do {                                                                    \
        FILE * file = NULL;                                                 \
                                                                            \
        __LOG(stderr, fmt, __VA_ARGS__);                                    \
                                                                            \
        file = fopen(FULL_LOG_ERROR_PATH, APPEND);                          \
        if (!file) break;                                                   \
                                                                            \
        __LOG(file, fmt, __VA_ARGS__);                                      \
                                                                            \
        fclose(file);                                                       \
    } while (0);


#define LOG_WARNING(fmt , ...)                                              \
    do {                                                                    \
        FILE * file = NULL;                                                 \
                                                                            \
        __LOG(stderr, fmt, __VA_ARGS__);                                    \
                                                                            \
        file = fopen(FULL_LOG_WARNING_PATH, APPEND);                        \
        if (!file) break;                                                   \
                                                                            \
        __LOG(file, fmt, __VA_ARGS__);                                      \
                                                                            \
        fclose(file);                                                       \
    } while (0);


#endif // LOGS_H_INCLUDED