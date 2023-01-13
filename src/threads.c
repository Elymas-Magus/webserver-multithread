#include "threads.h"

pthread_mutex_t mutex;
pthread_cond_t cond;

void
initMutex()
{
    if (pthread_mutex_init(&mutex, NULL) != 0) {
        WARNING("Error in mutex initializer\n");
        exit(1);
    }
}

void
initCond()
{
    if (pthread_cond_init(&cond, NULL)) {
        WARNING("Error in cond initializer\n");
        exit(1);
    }
}

void
destroyMutex()
{
    if (pthread_mutex_destroy(&mutex) != 0) {
        WARNING("Error in mutex initializer\n");
        exit(1);
    }
}

void
destroyCond()
{
    if (pthread_cond_destroy(&cond)) {
        WARNING("Error in cond initializer\n");
        exit(1);
    }
}

void
mutexLock()
{
    // printf("lock\n");
    if (pthread_mutex_lock(&mutex) != 0) {                                          
        WARNING("Error at mutex lock (%s)\n", getLocalCurrentTimeInHttpFormat());    
        LOG_ERROR("Error at mutex lock (%s)\n", getLocalCurrentTimeInHttpFormat());                                                       
        exit(2);                                                                    
    }
}

void
mutexUnlock()
{
    // printf("unlock\n");
    if (pthread_mutex_unlock(&mutex) != 0) {                                          
        WARNING("Error at mutex unlock (%s)\n", getLocalCurrentTimeInHttpFormat());     
        LOG_ERROR("Error at mutex unlock (%s)\n", getLocalCurrentTimeInHttpFormat());                                                       
        exit(2);                                                                    
    }
}

void
emitSignal()
{
    printf("[C] emit signal\n");
    if (pthread_cond_signal(&cond) != 0) {                                          
        WARNING("Error at mutex unlock (%s)\n", getLocalCurrentTimeInHttpFormat());  
        LOG_ERROR("Error at mutex unlock (%s)\n", getLocalCurrentTimeInHttpFormat());                             
    }
}

void
emitBroadcast()
{
    // printf("emit broadcast\n");
    if (pthread_cond_broadcast(&cond) != 0) {                                          
        WARNING("Error at mutex unlock (%s)\n", getLocalCurrentTimeInHttpFormat());  
        LOG_ERROR("Error at mutex unlock (%s)\n", getLocalCurrentTimeInHttpFormat());                             
    }
}

void
condWait(int identifier)
{
    printf("[H:%d] Waiting for clients\n", identifier);
    if (pthread_cond_wait(&cond, &mutex) != 0) {                                          
        WARNING("Error at mutex unlock (%s)\n", getLocalCurrentTimeInHttpFormat());      
        LOG_ERROR("Error at mutex unlock (%s)\n", getLocalCurrentTimeInHttpFormat());                             
    }
    printf("[H:%d] Getting client\n", identifier);
}