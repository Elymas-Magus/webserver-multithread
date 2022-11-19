#ifndef CONNECTION_H_INCLUDED
#define CONNECTION_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>

#include "timer.h"
#include "server_def.h"
#include "socket_validation.h"
#include "utils.h"
#include "request.h"
#include "type_aliases.h"
#include "throwable.h"
#include "uri.h"

#define CONNECTION_BUFFER_SIZE      81920
#define CONNECTION_PATH_MAX         1000
#define MAX_CONTENT_LENGTH_STRING   10


/**
 * Receive server object and init
 * Listen Loop
 * @param Server server
 * @return void
 */
void listenConnection(Server * server);

/**
 * Initialize threadpool and task
 * @param int serverSocket
 * @return void
 */
void initServerPool(Server * server);

/**
 * Make a loop for listen new connections
 * @param int serverSocket
 * @return void
 */
void connectionLoop(Server * server);

/**
 * Receive any new connection and log
 * @param int serverSocket
 * @param int clientSocket
 * @param int addrSize
 * @param struct sockaddr_in clientAddr
 * @return void
 */
void connectionListener(Server * server, socklen_t * addrSize, SA_IN clientAddr);

/**
 * Handle thread connections
 * @param void * arg
 * @return void *
 */
void * threadConnectionHandler(void * arg);

/**
 * Treat each connection
 * Implement the communication
 * @param int pClientSocket
 * @param Server * server
 * @return void
 */
void handleConnection(ThreadArg * args, int clientSocket, Server * server);

/**
 * Register connection start data in log file for debug
 * @param ThreadArg * argsdebug
 * @param int clientSocket
 * @param String currTime
 */
void logConnectionStart(ThreadArg * args, int clientSocket, String currTime);

/**
 * Register connection start data in log file for debug
 * @param ThreadArg * argsdebug
 * @param int clientSocket
 * @param String currTime
 * @param float duration
 * @param String path
 * @param bool error
 */
void logConnectionEnd(ThreadArg * args, int clientSocket, String currTime, float duration, String path, bool error);

#endif // CONNECTION_H_INCLUDED
