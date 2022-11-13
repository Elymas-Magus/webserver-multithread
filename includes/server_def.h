#ifndef SERVER_DEF_H_INCLUDED
#define SERVER_DEF_H_INCLUDED

#include <arpa/inet.h>

#define MAX_SERVER_NAME 100

// aliases
typedef struct sockaddr_in SA_IN;
typedef struct sockaddr SA;

typedef struct server {
    char name[MAX_SERVER_NAME];
    int socket;
    SA_IN address;
} Server;

#endif // SERVER_DEF_H_INCLUDED
