#include "queue.h"

Queue *
createQueue()
{
    Queue * queue = (Queue *) mallocOrDie(sizeof(Queue), "queue");

    queue->items = arrayInit();
    queue->enqueue = enqueue;
    queue->dequeue = dequeue;

    return queue;
}

void
enqueue(Queue * queue, void ** content, size_t length)
{
    arrayUnshift(queue->items, content, length);
}

void **
dequeue(Queue * queue, size_t length)
{
    return arrayPop(queue->items, length);
}

void
queueFree(Queue * queue)
{
    arrayFree(queue->items);
    free(queue);
}