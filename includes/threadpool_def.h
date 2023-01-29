#ifndef THREADPOOL_DEF_H_INCLUDED
#define THREADPOOL_DEF_H_INCLUDED

#include <pthread.h>

#include "queue.h"
#include "type_aliases.h"

#define MAX_LOG_FILENAME            128

typedef struct threadArg {
    u_int threadId;
    u_int connectionId;
    void * content;
    struct tm * start;
    char logFilename[MAX_LOG_FILENAME];
} ThreadArg;

typedef struct threadTask {
    ThreadArg * args;
    void * (* func)(void *);
} ThreadTask;

typedef struct threadpool {
    pthread_t * threads;
    ThreadTask * tasks;
    u_int length;
    u_int started;
    bool shutdown;
} Threadpool;

#endif // THREADPOOL_DEF_H_INCLUDED
