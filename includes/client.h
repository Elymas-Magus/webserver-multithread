#ifndef CLIENT_H_INCLUDED
#define CLIENT_H_INCLUDED

#include "stream_def.h"
#include "type_aliases.h"

typedef struct client {
    SocketFD socket;
    SA_IN address;
} Client;

#endif // CLIENT_H_INCLUDED