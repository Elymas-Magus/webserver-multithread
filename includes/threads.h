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
void semLock(sem_t *);

/**
 * Receive a mutex and unlock a thread
 * Register logs
 */
void semUnlock(sem_t *);

/**
 * initialize producer semaphore
 */
void initProducer();

/**
 * initialize consumer semaphore
 */
void initConsumer();

/**
 * destroy producer semaphore
 */
void destroyProducer();

/**
 * destroy consumer semaphore
 */
void destroyConsumer();

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
void blockConsumer(int);

/**
 * release producer, releasing cond variable
 */
void releaseProducer(int);

#endif