#include "threadpool.h"

Threadpool *
createThreadpool(u_int threadNumber)
{
    Threadpool * threadpool = (Threadpool *) malloc(sizeof(Threadpool));

    threadpool->shutdown = false;
    threadpool->started = 0;
    
    makeMutex(threadpool);
    // makeCond(threadpool);
    makeQueue(threadpool);
    makeTask(threadpool);
    makeThreads(threadpool, threadNumber);

    return threadpool;
}

void
makeMutex(Threadpool * pool)
{
    check(pthread_mutex_init(&(pool->mutex), NULL) != 0, "Error in mutex initializer");
}

// void
// makeCond(Threadpool * pool)
// {
//     check(pthread_cond_init(&(pool->cond), NULL), "Error in cond initializer");
// }

void
makeThreads(Threadpool * pool, u_int threadNumber)
{
    pool->threads = (pthread_t *) malloc(threadNumber * sizeof(pthread_t));

    if (pool->threads == NULL) {
        fprintf(stderr, "Threads couldn't be instantiated");
        exit(1);
    }

    pool->length = threadNumber;
}

void
makeQueue(Threadpool * pool)
{
    pool->queue = createQueue();
}

void
makeTask(Threadpool * pool)
{
    pool->tasks = (ThreadTask *) malloc(sizeof(ThreadTask));

    if (pool->tasks == NULL) {
        fprintf(stderr, "Tasks couldn't be instantiated");
        exit(1);
    }
}

int
threadpoolFree(Threadpool * pool)
{
    if (pool == NULL || pool->started > 0) {
        return ERROR_CODE;
    }
    if (pool->threads) {
        free(pool->threads);
    }
    if (pool->tasks) {
        free(pool->tasks);
    }
    if (pool->queue) {
        queueFree(pool->queue);
    }
    return 0;
}

int
selfDestroy(Threadpool * pool)
{
    for(int i = 0; i < pool->length; i++){
        if(pthread_join(pool->threads[i], NULL) != 0) {
            return ERROR_CODE;
        }
    }

    pthread_mutex_destroy(&(pool->mutex));
    pthread_cond_destroy(&(pool->cond));
    threadpoolFree(pool);

    return SUCCESS_CODE;
}

void
initThreadpools(Threadpool * pool)
{
    pthread_t * threads = (pthread_t *) pool->threads;
    ThreadTask * tasks = (ThreadTask *) pool->tasks;

    for (int i = 0; i < pool->length; i++) {
        if (pthread_create(&threads[i], NULL, tasks->func, (void *) tasks->args) != 0) {
            fprintf(stderr, "Thread couldn't be created");
            if (selfDestroy(pool) == ERROR_CODE) {
                exit(1);
            }
            return;
        }
        pool->started++;
    }
}