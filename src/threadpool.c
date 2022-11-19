#include "threadpool.h"

Threadpool *
createThreadpool(u_int threadNumber)
{
    Threadpool * threadpool = (Threadpool *) malloc(sizeof(Threadpool));

    threadpool->shutdown = false;
    threadpool->started = 0;
    
    makeMutex(threadpool);
    makeCond(threadpool);
    makeQueue(threadpool);
    makeTask(threadpool, threadNumber);
    makeThreads(threadpool, threadNumber);

    return threadpool;
}

void
makeMutex(Threadpool * pool)
{
    if (pthread_mutex_init(&(pool->mutex), NULL) != 0) {
        WARNING("Error in mutex initializer\n");
        exit(1);
    }
}

void
makeCond(Threadpool * pool)
{
    if (pthread_cond_init(&(pool->cond), NULL)) {
        WARNING("Error in cond initializer\n");
        exit(1);
    }
}

void
makeThreads(Threadpool * pool, u_int threadNumber)
{
    pool->threads = (pthread_t *) malloc(threadNumber * sizeof(pthread_t));

    if (pool->threads == NULL) {
        WARNING("Threads couldn't be instantiated\n");
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
makeTask(Threadpool * pool, u_int threadNumber)
{
    pool->tasks = (ThreadTask *) malloc(sizeof(ThreadTask));
    pool->tasks->args = (ThreadArg *) malloc(threadNumber * sizeof(ThreadArg));

    if (pool->tasks == NULL) {
        WARNING("Tasks couldn't be instantiated\n");
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
initThreadpools(Threadpool * pool, Server * server)
{
    time_t now;
    pthread_t * threads = (pthread_t *) pool->threads;
    
    for (int i = 0; i < pool->length; i++) {
        pool->tasks->args[i].threadId = i;
        pool->tasks->args[i].connectionId = 0;
        pool->tasks->args[i].content = server;
        pool->tasks->args[i].start = localtime(&now);
        if (pthread_create(&threads[i], NULL, pool->tasks->func, (void *) &pool->tasks->args[i]) != 0) {
            WARNING("Thread couldn't be created\n");
            if (selfDestroy(pool) == ERROR_CODE) {
                exit(1);
            }
            return;
        }
        pool->started++;
    }
}