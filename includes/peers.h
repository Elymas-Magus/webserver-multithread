#ifndef PEERS_H_INCLUDED
#define PEERS_H_INCLUDED

#include "client.h"
#include "server_def.h"

typedef struct peers {
    Server server;
    Client client;
} Peers;

#endif