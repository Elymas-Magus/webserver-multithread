#ifndef CONNECTION_H_INCLUDED
#define CONNECTION_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

#include "server_def.h"
#include "socket_validation.h"
#include "utils.h"

#define CONNECTION_BUFFER_SIZE 4096
#define CONNECTION_PATH_MAX 1000

/**
 * Receive server object and init
 * Listen Loop
 * @param Server server
 * @return void
 */
void listenConnection(Server server, ServerConfig * config);

/**
 * Make a loop for listen new connections
 * @param int serverSocket
 * @return void
 */
void connectionLoop(int serverSocket, ServerConfig * config);

/**
 * Receive any new connection and log
 * @param int serverSocket
 * @param int clientSocket
 * @param int addrSize
 * @param struct sockaddr_in clientAddr
 * @return void
 */
void connectionListener(int serverSocket, int * clientSocket, int addrSize, SA_IN clientAddr, ServerConfig * config);

/**
 * Create a new thread for new connections
 * @param int serverSocket
 * @return void
 */
void createConnectionThread(ServerConfig * config);

/**
 * Handler for new threads
 * @param void * pServerConfig
 * @return void *
 */
void * threadPoolHander(void * pServerConfig);

/**
 * Treat each connection
 * Implement the communication
 * @param void * pClientSocket
 * @return void *
 */
void * handleConnection(void * pClientSocket);

#endif // CONNECTION_H_INCLUDED
