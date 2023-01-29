#include "threads.h"

sem_t mutex;
sem_t cond;

void
initMutex()
{
    if (sem_init(&mutex, PSHARED, RED) != 0) {
        WARNING("Error in mutex initializer\n");
        exit(1);
    }
}

void
initCond()
{
    if (sem_init(&cond, PSHARED, GREEN)) {
        WARNING("Error in cond initializer\n");
        exit(1);
    }
}

void
destroyMutex()
{
    if (sem_destroy(&mutex) != 0) {
        WARNING("Error in mutex initializer\n");
        exit(1);
    }
}

void
destroyCond()
{
    if (sem_destroy(&cond)) {
        WARNING("Error in cond initializer\n");
        exit(1);
    }
}

void
destroySemaphores()
{
    destroyCond();
    destroyMutex();
}

void
mutexLock(sem_t * mutex)
{
    // printf("lock\n");
    if (sem_wait(mutex) != 0) {                                          
        WARNING("Error at mutex lock (%s)\n", getLocalCurrentTimeInHttpFormat());    
        LOG_ERROR("Error at mutex lock (%s)\n", getLocalCurrentTimeInHttpFormat());                                                       
        exit(2);                                                                    
    }
}

void
mutexUnlock(sem_t * mutex)
{
    // printf("unlock\n");
    if (sem_post(mutex) != 0) {                                          
        WARNING("Error at mutex unlock (%s)\n", getLocalCurrentTimeInHttpFormat());     
        LOG_ERROR("Error at mutex unlock (%s)\n", getLocalCurrentTimeInHttpFormat());                                                       
        exit(2);                                                                    
    }
}

void
blockProducer()
{
    printf("[C] lock\n");
    mutexLock(&cond);
}

void
releaseConsumer()
{
    printf("[C] unlock\n");
    mutexUnlock(&mutex);
}

void
blockConsumer(int threadId)
{
    printf("[H:%d] lock\n", threadId);
    mutexLock(&mutex);
}

void
releaseProducer(int threadId)
{
    printf("[H:%d] unlock\n", threadId);
    mutexUnlock(&cond);
}