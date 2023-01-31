#include "threads.h"

sem_t sem_producer;
sem_t sem_consumer;

void
initProducer()
{
    if (sem_init(&sem_producer, PSHARED, GREEN)) {
        WARNING("Error in sem_producer initializer\n");
        exit(1);
    }
}

void
initConsumer()
{
    if (sem_init(&sem_consumer, PSHARED, RED) != 0) {
        WARNING("Error in sem_consumer initializer\n");
        exit(1);
    }
}

void
destroyConsumer()
{
    if (sem_destroy(&sem_consumer) != 0) {
        WARNING("Error in sem_consumer destroyer\n");
        exit(1);
    }
}

void
destroyProducer()
{
    if (sem_destroy(&sem_producer)) {
        WARNING("Error in sem_producer destroyer\n");
        exit(1);
    }
}

void
destroySemaphores()
{
    destroyConsumer();
    destroyProducer();
}

void
semLock(sem_t * sem_consumer)
{
    // printf("lock\n");
    if (sem_wait(sem_consumer) != 0) {                                          
        WARNING("Error at sem_consumer lock (%s)\n", getLocalCurrentTimeInHttpFormat());    
        LOG_ERROR("Error at sem_consumer lock (%s)\n", getLocalCurrentTimeInHttpFormat());                                                       
        exit(2);                                                                    
    }
}

void
semUnlock(sem_t * sem_consumer)
{
    // printf("unlock\n");
    if (sem_post(sem_consumer) != 0) {                                          
        WARNING("Error at sem_consumer unlock (%s)\n", getLocalCurrentTimeInHttpFormat());     
        LOG_ERROR("Error at sem_consumer unlock (%s)\n", getLocalCurrentTimeInHttpFormat());                                                       
        exit(2);                                                                    
    }
}

void
blockProducer()
{
    // printf("[C] lock\n");
    semLock(&sem_producer);
}

void
releaseConsumer()
{
    // printf("[C] unlock\n");
    semUnlock(&sem_consumer);
}

void
blockConsumer(int threadId)
{
    // printf("[H:%d] lock\n", threadId);
    semLock(&sem_consumer);
}

void
releaseProducer(int threadId)
{
    // printf("[H:%d] unlock\n", threadId);
    semUnlock(&sem_producer);
}