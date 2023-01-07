#ifndef QUEUE_DEF_H_INCLUDED
#define QUEUE_DEF_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "list.h"
#include "client.h"

typedef struct node_t {
    struct node_t * next;
    Client * client;
} node_t;

#endif // QUEUE_DEF_H_INCLUDED
