#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#include "queue_def.h"

extern node_t * head;
extern node_t * tail;

/**
 * Create new Queue
 * @returns Queue
 */
void enqueue(Client * client);


Client * dequeue();

#endif // QUEUE_H_INCLUDED
