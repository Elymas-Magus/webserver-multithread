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

#define SERVER_BACKLOG          1

#define BUFFER_SIZE             4096
#define LOG_CONTEXT_FILENAME    "logs/socket_context.log"

/**
 * Create server object
 * @return Server server
 */
Server * createServer(ServerConfig * config);

/**
 * Create socket
 * @return int sockfd
 */
int getServerSocket();

/**
 * Reset last serverSocket context
 * @return int sockfd
 */
int getServerSocketContext();

/**
 * Save serverSocket context
 */
void saveSocketContext(int serverSocket);

/**
 * Create struct sockaddr_in instance
 * @return struct sockaddr_in socket
 */
SA_IN getServerAddr(u_int port);

/**
 * Bind the serverSocket with server address
 * And init listen
 * @param Server server 
 * @return struct sockaddr_in socket
 */
int initServer(Server * server);

/**
 * Initiate listener or Die
 * @param int serverSocket
 * @return int status
 */
int initListen(int serverSocket);

/**
 * Initiate listener or Die
 * @param int serverSocket
 * @param struct sockaddr_in serverAddr
 * @return int status
 */
int bindServerAddr(int * serverSocket, SA_IN serverAddr);

#endif // SERVER_H_INCLUDED
