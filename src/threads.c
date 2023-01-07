#include "threads.h"

void
initMutex(pthread_mutex_t * mutex)
{
    if (pthread_mutex_init(mutex, NULL) != 0) {
        WARNING("Error in mutex initializer\n");
        exit(1);
    }
}

void
initCond(pthread_cond_t * cond)
{
    if (pthread_cond_init(cond, NULL)) {
        WARNING("Error in cond initializer\n");
        exit(1);
    }
}

void
mutexLock(pthread_mutex_t * mutex)
{
    // printf("lock\n");
    if (pthread_mutex_lock(mutex) != 0) {                                          
        WARNING("Error at mutex lock (%s)\n", getLocalCurrentTimeInHttpFormat());    
        LOG_ERROR("Error at mutex lock (%s)\n", getLocalCurrentTimeInHttpFormat());                                                       
        exit(2);                                                                    
    }
}

void
mutexUnlock(pthread_mutex_t * mutex)
{
    // printf("unlock\n");
    if (pthread_mutex_unlock(mutex) != 0) {                                          
        WARNING("Error at mutex unlock (%s)\n", getLocalCurrentTimeInHttpFormat());     
        LOG_ERROR("Error at mutex unlock (%s)\n", getLocalCurrentTimeInHttpFormat());                                                       
        exit(2);                                                                    
    }
}

void
emitSignal(pthread_cond_t * cond)
{
    // printf("emit signal\n");
    if (pthread_cond_signal(cond) != 0) {                                          
        WARNING("Error at mutex unlock (%s)\n", getLocalCurrentTimeInHttpFormat());  
        LOG_ERROR("Error at mutex unlock (%s)\n", getLocalCurrentTimeInHttpFormat());                             
    }
}

void
emitBroadcast(pthread_cond_t * cond)
{
    // printf("emit broadcast\n");
    if (pthread_cond_broadcast(cond) != 0) {                                          
        WARNING("Error at mutex unlock (%s)\n", getLocalCurrentTimeInHttpFormat());  
        LOG_ERROR("Error at mutex unlock (%s)\n", getLocalCurrentTimeInHttpFormat());                             
    }
}

void
condWait(pthread_cond_t * cond, pthread_mutex_t * mutex)
{
    if (pthread_cond_wait(cond, mutex) != 0) {                                          
        WARNING("Error at mutex unlock (%s)\n", getLocalCurrentTimeInHttpFormat());      
        LOG_ERROR("Error at mutex unlock (%s)\n", getLocalCurrentTimeInHttpFormat());                             
    }
}