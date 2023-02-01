#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <sys/socket.h>

#ifndef SERVER_DEF_H_INCLUDED
#include "server_def.h"
#endif

#include "connection.h"
#include "validation.h"
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
 * init listen
 * And init threadpools
 * @param Server server 
 */
void initServer(Server * server);

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
 * Receive server object and init
 * Listen Loop
 * @param Server server
 * @returns void
 */
void listenConnections(Server * server);

/**
 * Initialize threadpool and task
 * @param int serverSocket
 * @returns void
 */
void initServerPool(Server * server);

/**
 * Destroys server and it's attributes
 * @param Server * server
 */
void serverDestroy(Server * server);

#endif // SERVER_H_INCLUDED
