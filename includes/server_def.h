#ifndef SERVER_DEF_H_INCLUDED
#define SERVER_DEF_H_INCLUDED

#include <arpa/inet.h>

#include "stream_def.h"
#include "type_aliases.h" 
#include "threadpool_def.h"
#include "queue.h"

#define MAX_SERVER_NAME         100
#define MAX_SERVER_PATH_LENGTH  512

typedef struct server {
    char name[MAX_SERVER_NAME];
    char root[MAX_SERVER_PATH_LENGTH];
    SocketFD socket;
    u_int backlog;
    u_int port;
    SA_IN address;
    Threadpool * pools;
    void (* initPools)(Threadpool *, struct server *);
} Server;

#endif // SERVER_DEF_H_INCLUDED
