#include "queue.h"

Queue *
createQueue()
{
    Queue * queue = (Queue *) malloc(sizeof(Queue));

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
dequeue(Queue * queue)
{
    printf("Cheguei! Dequeue!\n");
    return arrayPop(queue->items);
}

void
queueFree(Queue * queue)
{
    arrayFree(queue->items);
    free(queue);
}