#include "threadpool.h"

Threadpool *
createThreadpool(u_int threadNumber)
{
    Threadpool * threadpool = (Threadpool *) malloc(sizeof(Threadpool));

    // threadpool->mutex = PTHREAD_MUTEX_INITIALIZER;
    threadpool->threads = initThreads(threadNumber);
    threadpool->length = 0;
    threadpool->queueSize = 0;
}

Thread *
initThreads(u_int threadNumber)
{
    return (Thread *) malloc(threadNumber * sizeof(Thread));
}