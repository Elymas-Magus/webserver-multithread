#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#include "queue_def.h"

extern node_t * head;
extern node_t * tail;

/**
 * Insert a new client in the queue
 * @param Client *
 */
void enqueue(Client * client);

/**
 * Remove last client from queue
 * @returns Client *
 */
Client * dequeue();

/**
 * Check if queue is empty
 * @returns bool
 */
bool queueIsEmpty();

#endif // QUEUE_H_INCLUDED
