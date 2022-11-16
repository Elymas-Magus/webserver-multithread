#ifndef SERVER_DEF_H_INCLUDED
#define SERVER_DEF_H_INCLUDED

#include <arpa/inet.h>
#include "type_aliases.h" 
#include "threadpool.h"
#include "queue.h"

#define MAX_SERVER_NAME 100
#define MAX_PATH_LENGTH 128

typedef struct server {
    char name[MAX_SERVER_NAME];
    char root[MAX_PATH_LENGTH];
    int socket;
    u_int port;
    SA_IN address;
    Threadpool * pools;
    void (* initPools)(Threadpool *);
} Server;

#endif // SERVER_DEF_H_INCLUDED
