#ifndef TYPE_ALIASES_H_INCLUDED
#define TYPE_ALIASES_H_INCLUDED

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