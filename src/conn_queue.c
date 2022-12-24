#include "conn_queue.h"

ConnQueue connections = {
    .length = 0
};

void
enqueueConn(Client client)
{
    connections.clients[connections.length++] = client;
}

Client
dequeueConn()
{
    Client client = connections.clients[0];

    connections.length--;
    for (int i = 0; i < connections.length; i++) {
        connections.clients[i] = connections.clients[i + 1];
    }

    return client;
}

bool
emptyConnectionList()
{
    return connections.length == 0;
}

int
getNumberOfConnections()
{
    return connections.length;
}

void
printConn()
{
    int i = 0;

    printf("connections-queue: [");
    for (; i < connections.length - 1; i++) {
        printf("%d,", connections.clients[i].socket);
    }
    printf("%d]\n", connections.clients[i].socket);
}