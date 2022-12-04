#include "queue.h"

Queue *
createQueue()
{
    Queue * queue = (Queue *) mallocOrDie(sizeof(Queue), "queue");

    queue->items = arrayInit();
    queue->enqueue = enqueue;
    queue->dequeue = dequeue;
    queue->isEmpty = isEmpty;

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

bool
isEmpty(Queue * queue)
{
    return queue->items->length == 0 || queue->items->first == NULL;
}

void
queueFree(Queue * queue)
{
    arrayFree(queue->items);
    free(queue);
}