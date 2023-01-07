#ifndef THREADS_H
#define THREADS_H

#include <pthread.h>

#include "timer.h"
#include "logs.h"

/**
 * Receive a mutex and lock a thread
 * Register logs
 * @param pthread_mutex_t * mutex
 */
void mutexLock(pthread_mutex_t * mutex);

/**
 * Receive a mutex and unlock a thread
 * Register logs
 * @param pthread_mutex_t * mutex
 */
void mutexUnlock(pthread_mutex_t * mutex);

/**
 * Emit a signal to stop waiting
 * Register logs
 * @param pthread_cond_t * cond
 */
void emitSignal(pthread_cond_t * cond);

/**
 * Emit a broadcast to stop waiting
 * Register logs
 * @param pthread_cond_t * cond
 */
void emitBroadcast(pthread_cond_t * cond);

/**
 * Check a condition to stop while it's not satisfied
 * Register logs
 * @param pthread_cond_t * cond
 * @param pthread_mutex_t * mutex
 */
void condWait(pthread_cond_t * cond, pthread_mutex_t * mutex);

/**
 * initialize mutex
 * @param pthread_mutex_t * mutex
 */
void initMutex(pthread_mutex_t * mutex);

/**
 * initialize cond var
 * @param pthread_cond_t * cond
 */
void initCond(pthread_cond_t * cond);


#endif