#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <sys/socket.h>

#include "server_def.h"
#include "socket_validation.h"
#include "utils.h"
#include "type_aliases.h"
#include "throwable.h"
#include "threadpool.h"

#define BUFFER_SIZE             4096
#define LOG_CONTEXT_FILENAME    "logs/socket_context.log"

/**
 * Create server object
 * @returns Server server
 */
Server * createServer(ServerConfig * config);

/**
 * Create socket
 * @returns int sockfd
 */
int getServerSocket();

/**
 * Reset last serverSocket context
 * @returns int sockfd
 */
int getServerSocketContext();

/**
 * Create struct sockaddr_in instance
 * @returns struct sockaddr_in socket
 */
SA_IN getServerAddr(u_int port);

/**
 * Bind the serverSocket with server address
 * And init listen
 * @param Server server 
 * @returns struct sockaddr_in socket
 */
int initServer(Server * server);

/**
 * Initiate listener or Die
 * @param int serverSocket
 * @returns int status
 */
int initListen(SocketFD serverSocket, u_int backlog);

/**
 * Initiate listener or Die
 * @param int serverSocket
 * @param struct sockaddr_in serverAddr
 * @returns int status
 */
int bindServerAddr(SocketFD serverSocket, SA_IN serverAddr);

/**
 * Destroys server and it's attributes
 * @param Server * server
 */
void serverDestroy(Server * server);

#endif // SERVER_H_INCLUDED
