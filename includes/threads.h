#ifndef THREADS_H
#define THREADS_H

#include <pthread.h>
#include <semaphore.h>

#include "timer.h"
#include "logs.h"

#define THREAD_SHARING 0
#define PSHARED THREAD_SHARING

#define GREEN 1
#define RED 0

extern sem_t mutex;
extern sem_t cond;

/**
 * Receive a mutex and lock a thread
 * Register logs
 */
void mutexLock(sem_t * mutex);

/**
 * Receive a mutex and unlock a thread
 * Register logs
 */
void mutexUnlock(sem_t * mutex);

/**
 * initialize mutex
 */
void initMutex();

/**
 * initialize cond var
 */
void initCond();

/**
 * destroy mutex
 */
void destroyMutex();

/**
 * destroy cond var
 */
void destroyCond();

/**
 * destroy all semaphores (mutex and cond)
 */
void destroySemaphores();

/**
 * block producer, blocking cond variable
 */
void blockProducer();

/**
 * release consumer, realising mutex variable
 */
void releaseConsumer();

/**
 * block consumer, blocking mutex variable
 */
void blockConsumer(int threadId);

/**
 * release producer, releasing cond variable
 */
void releaseProducer(int threadId);

#endif