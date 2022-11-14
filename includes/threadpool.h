#ifndef THREADPOOL_H_INCLUDED
#define THREADPOOL_H_INCLUDED

#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
#include "type_aliases.h" 

pthread_t * thread_pool;

typedef struct thread {
    u_int index;
    pthread_t thread;
} Thread;

typedef struct threadTask {
    void (*func)(void *);
    void * arg;
    struct threadTask * next;
} ThreadTask;

typedef struct threadpool {
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    Thread * threads;
    ThreadTask * head;
    int length;
    int queueSize;
} Threadpool;

Threadpool * createThreadpool(u_int threadNumber);
Thread * initThreads(u_int threadNumber);

#endif // THREADPOOL_H_INCLUDED
