#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#include "queue_def.h"

/**
 * Create new Queue
 * @returns Queue
 */
Queue * createQueue();

/**
 * Insert an element into the array
 * @param Queue * queue
 * @param size_t length
 * @param void ** content
 */
void enqueue(Queue * queue, void ** content, size_t length);

/**
 * Remove an element from the array
 * @param Queue * queue
 * @param size_t length
 * @returns void **
 */
void ** dequeue(Queue * queue, size_t length);

/**
 * Check if queue is empty
 * @param Queue * queue
 * @returns bool
 */
bool isEmpty(Queue * queue);

/**
 * Unallocate memory for Queue
 * @param Queue * queue
 */
void queueFree(Queue * queue);

#endif // QUEUE_H_INCLUDED
