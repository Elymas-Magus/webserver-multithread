#include "queue.h"

Queue * createQueue()
{
    Queue * queue = (Queue *) malloc(sizeof(Queue));

    queue->items = arrayInit();
    queue->enqueue = enqueue;
    queue->dequeue = dequeue;

    return queue;
}

void enqueue(Queue * queue, void ** content)
{
    arrayUnshift(queue->items, content);
}

void ** dequeue(Queue * queue)
{
    return arrayPop(queue->items);
}