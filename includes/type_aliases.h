#ifndef TYPE_ALIASES_H_INCLUDED
#define TYPE_ALIASES_H_INCLUDED

#include <time.h>

#include "logs.h"

#define CONFIG_SUCCESS          0
#define CONFIG_ERROR            -1

#define OCTAL                   8
#define DECIMAL                 10
#define HEX                     16

#define typeof(var) _Generic( (var),\
    char: "Char",\
    int: "Integer",\
    float: "Float",\
    double: "Double",\
    char *: "String",\
    void *: "Pointer",\
    default: "Undefined")

#define DeclareSort(prefix, type, mode) \
    static int \
    _DeclareSort_ ## prefix ## _Compare_DESC(const void *a, const void *b) \
    { \
        const type *aa; const type *bb; \
        aa = a; bb = b; \
        if(aa < bb) return -1; \
        else if(bb < aa) return 1; \
        else return 0; \
    } \
    static int \
    _DeclareSort_ ## prefix ## _Compare_ASC(const void *a, const void *b) \
    { \
        const type *aa; const type *bb; \
        aa = a; bb = b; \
        if(aa < bb) return 1; \
        else if(bb < aa) return -1; \
        else return 0; \
    } \
    \
    void \
    prefix ## _sort(type *a, int n)\
    { \
        qsort(a, sizeof(type), n, _DeclareSort_ ## prefix ## _Compare_ ## mode); \
    }

typedef unsigned int u_int;
typedef unsigned long long int ull_int;
typedef char * String;

typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

#endif // TYPE_ALIASES_H_INCLUDED