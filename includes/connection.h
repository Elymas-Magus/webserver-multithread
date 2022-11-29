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
#include "stream.h"
#include "logs.h"
#include "client.h"

#define CONNECTION_BUFFER_SIZE      81920
#define CONNECTION_PATH_MAX         1000
#define MAX_CONTENT_LENGTH_STRING   10

/**
 * Receive server object and init
 * Listen Loop
 * @param Server server
 * @returns void
 */
void listenConnection(Server * server);

/**
 * Initialize threadpool and task
 * @param int serverSocket
 * @returns void
 */
void initServerPool(Server * server);

/**
 * Make a loop for listen new connections
 * @param int serverSocket
 * @returns void
 */
void connectionLoop(Server * server);

/**
 * Receive any new connection and log
 * @param int serverSocket
 * @param int clientSocket
 * @param int addrSize
 * @param struct sockaddr_in clientAddr
 * @returns void
 */
void connectionListener(Server * server);

/**
 * Handle thread connections
 * @param void * arg
 * @returns void *
 */
void * threadConnectionHandler(void * arg);

/**
 * Treat each connection
 * Implement the communication
 * @param Client * client
 * @param Server * server
 * @returns void
 */
void handleConnection(ThreadArg * args, Client * client, Server * server);

/**
 * Register connection start data in log file for debug
 * @param ThreadArg * argsdebug
 * @param Client * client
 * @param String currTime
 */
void logConnectionStart(ThreadArg * args, Client * client, String currTime);

/**
 * Register connection start data in log file for debug
 * @param ThreadArg * argsdebug
 * @param Client * client
 * @param String currTime
 * @param float duration
 * @param String path
 * @param bool error
 */
void logConnectionEnd(ThreadArg * args, Client * client, String currTime, float duration, String path, bool error);

#endif // CONNECTION_H_INCLUDED
