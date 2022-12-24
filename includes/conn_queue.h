#ifndef CONN_QUEUE_H_INCLUDED
#define CONN_QUEUE_H_INCLUDED

#include <stdbool.h>
#include "client.h"

typedef struct connQueue {
    Client clients[5000];
    int length;
} ConnQueue;

extern ConnQueue connections;

void enqueueConn(Client client);

Client dequeueConn();

bool emptyConnectionList();

int getNumberOfConnections();

void printConn();

#endif