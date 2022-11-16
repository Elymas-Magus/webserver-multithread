#ifndef THREADPOOL_H_INCLUDED
#define THREADPOOL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdint.h>
#include "socket_validation.h" 
#include "type_aliases.h" 
#include "threadpool_def.h"

#define ERROR_CODE -1
#define SUCCESS_CODE 1

/**
 * @param u_int threadNumber
 * @return Threadpool * pools
 */
Threadpool * createThreadpool(u_int threadNumber);

/**
 * @param Threadpool * pool
 * @param u_int threadNumber
 * @return void
 */
void makeThreads(Threadpool * pool, u_int threadNumber);

/**
 * @param Threadpool * pool
 * @return void
 */
void makeTask(Threadpool * pool);

/**
 * @param void * poolTmp
 * @return void
 */
void initThreadpools(Threadpool * pool);

/**
 * @param Threadpool * pool
 * @return void
 */
void makeMutex(Threadpool * pool);

/**
 * @param Threadpool * pool
 * @return void
 */
void makeCond(Threadpool * pool);

/**
 * @param Threadpool * pool
 * @return void
 */
void makeQueue(Threadpool * pool);

#endif // THREADPOOL_H_INCLUDED
