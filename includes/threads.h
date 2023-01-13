#ifndef THREADS_H
#define THREADS_H

#include <pthread.h>

#include "timer.h"
#include "logs.h"

extern pthread_mutex_t mutex;
extern pthread_cond_t cond;

/**
 * Receive a mutex and lock a thread
 * Register logs
 */
void mutexLock();

/**
 * Receive a mutex and unlock a thread
 * Register logs
 */
void mutexUnlock();

/**
 * Emit a signal to stop waiting
 * Register logs
 */
void emitSignal();

/**
 * Emit a broadcast to stop waiting
 * Register logs
 */
void emitBroadcast();

/**
 * Check a condition to stop while it's not satisfied
 * Register logs
 */
void condWait(int identifier);

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


#endif