#ifndef QUEUE_DEF_H_INCLUDED
#define QUEUE_DEF_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "list.h"

typedef struct queue {
    ArrayList items;   
    void (* enqueue)(struct queue *, void **, size_t);
    void ** (* dequeue)(struct queue *);
} Queue;

#endif // QUEUE_DEF_H_INCLUDED
