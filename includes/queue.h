#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#include "queue_def.h"

/**
 * Create new Queue
 * @return ArrayList
 */
Queue * createQueue();

/**
 * Insert an element into the array
 * @param Queue * queue
 * @param void ** content
 */
void enqueue(Queue * queue, void ** content);

/**
 * Remove an element from the array
 * @param Queue * queue
 * @return void **
 */
void ** dequeue(Queue * queue);

#endif // QUEUE_H_INCLUDED
