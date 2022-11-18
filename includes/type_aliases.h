#ifndef TYPE_ALIASES_H_INCLUDED
#define TYPE_ALIASES_H_INCLUDED

#include "timer.h"

#define CONFIG_SUCCESS          0
#define CONFIG_ERROR            -1

#define WARNING(...) fprintf(stderr, __VA_ARGS__)
#define PRINT_EXPRESSION(x) (printf("%s = %d\n", #x, (x)))

#define OCTAL                   8
#define DECIMAL                 10
#define HEX                     16

#define LOG(fmt , ...)    \
    do { \
        time_t tmi; \
        struct tm * info; \
        char filename[DATE_MAX]; \
        \
        time(&tmi); \
        info = localtime(&tmi); \
        \
        strftime(filename, DATE_MAX, "logs/request-%Y-%m-%d", info); \
        strcat(filename, ".log"); \
        FILE* f = fopen(filename, "a") ; \
        if (!f) break ; \
        fprintf(f, fmt"\n",__VA_ARGS__);    \
        fclose(f);   \
    } while (0)

#define LOG_CONNECTTION(fmt , ...)    \
    do { \
        time_t tmi; \
        struct tm * info; \
        char filename[DATE_MAX]; \
        \
        time(&tmi); \
        info = localtime(&tmi); \
        \
        strftime(filename, DATE_MAX, "logs/connection-%Y-%m-%d", info); \
        strcat(filename, ".log"); \
        FILE* f = fopen(filename, "a") ; \
        if (!f) break ; \
        fprintf(f, fmt"\n",__VA_ARGS__);    \
        fclose(f);   \
    } while (0)

#define LOG_ERROR(fmt , ...)    \
    do { \
        FILE* f = fopen("logs/errors.log" , "a") ; \
        if (!f) break ; \
        fprintf(f, fmt"\n",__VA_ARGS__);    \
        fclose(f);   \
    } while (0)

#define LOG_WARNING(fmt , ...)    \
    do { \
        FILE* f = fopen("logs/errors.log" , "a") ; \
        if (!f) break ; \
        fprintf(f, fmt"\n",__VA_ARGS__);    \
        fprintf(stderr, fmt, __VA_ARGS__);    \
        fclose(f);   \
    } while (0)

#define DeclareSort(prefix, type) \
static int \
_DeclareSort_ ## prefix ## _Compare(const void *a, const void *b) \
{ \
    const type *aa; const type *bb; \
    aa = a; bb = b; \
    if(aa < bb) return -1; \
    else if(bb < aa) return 1; \
    else return 0; \
} \
\
void \
prefix ## _sort(type *a, int n)\
{ \
    qsort(a, sizeof(type), n, _DeclareSort_ ## prefix ## _Compare); \
}

typedef unsigned int u_int;
typedef unsigned long long int ull_int;
typedef char * String;

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

#endif // TYPE_ALIASES_H_INCLUDED