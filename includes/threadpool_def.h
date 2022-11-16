#ifndef THREADPOOL_DEF_H_INCLUDED
#define THREADPOOL_DEF_H_INCLUDED

#include <pthread.h>
#include "queue.h"
#include "type_aliases.h"

#define MAX_PATH_LENGTH 128

typedef struct threadTask {
    void * (* func)(void *);
    void * args;
    struct threadTask * next;
} ThreadTask;

typedef struct threadpool {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    pthread_t * threads;
    ThreadTask * tasks;
    u_int length;
    u_int started;
    Queue * queue;
    bool shutdown;
} Threadpool;

#endif // THREADPOOL_DEF_H_INCLUDED
