#ifndef THREADPOOL_H_INCLUDED
#define THREADPOOL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdint.h>

#include "validation.h" 
#include "type_aliases.h" 
#include "threadpool_def.h"
#include "server_def.h"

#define ERROR_CODE              -1
#define SUCCESS_CODE             1

/**
 * @param u_int threadNumber
 * @returns Threadpool * pools
 */
Threadpool * createThreadpool(u_int threadNumber);

/**
 * @param Threadpool * pool
 * @param u_int threadNumber
 * @returns void
 */
void makeThreads(Threadpool * pool, u_int threadNumber);

/**
 * @param Threadpool * pool
 * @returns void
 */
void makeTask(Threadpool * pool, u_int threadNumber);

/**
 * @param void * poolTmp
 * @returns void
 */
void initThreadpools(Threadpool * pool, Server * server);

/**
 * Destroys thread tasks and it's attributes
 * @param Threadpool * pool
 * @returns int
 */
void taskFree(ThreadTask * tasks);

/**
 * Destroys thread pool attributes
 * @param Threadpool * pool
 * @returns int
 */
int threadpoolFree(Threadpool * pool);

/**
 * Destroys thread pool and it's attributes
 * @param Threadpool * pool
 * @returns int
 */
int poolDestroy(Threadpool * pool);

#endif // THREADPOOL_H_INCLUDED
