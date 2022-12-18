#include "threads.h"

void
mutexLock(pthread_mutex_t * mutex)
{
    printf("lock\n");
    if (pthread_mutex_lock(mutex) != 0) {                                          
        WARNING("Error at mutex lock (%s)\n", getLocalCurrentTimeInHttpFormat());    
        LOG_ERROR("Error at mutex lock (%s)\n", getLocalCurrentTimeInHttpFormat());                                                       
        exit(2);                                                                    
    }
}

void
mutexUnlock(pthread_mutex_t * mutex)
{
    printf("unlock\n");
    if (pthread_mutex_unlock(mutex) != 0) {                                          
        WARNING("Error at mutex unlock (%s)\n", getLocalCurrentTimeInHttpFormat());     
        LOG_ERROR("Error at mutex unlock (%s)\n", getLocalCurrentTimeInHttpFormat());                                                       
        exit(2);                                                                    
    }
}

void
emitSignal(pthread_cond_t * cond)
{
    printf("emit signal\n");
    if (pthread_cond_signal(cond) != 0) {                                          
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