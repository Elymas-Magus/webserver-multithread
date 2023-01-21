#ifndef CONNECTION_H_INCLUDED
#define CONNECTION_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <unistd.h>
#include <time.h>

#include "timer.h"
#include "server_def.h"
#include "validation.h"
#include "utils.h"
#include "request.h"
#include "type_aliases.h"
#include "throwable.h"
#include "uri.h"
#include "stream.h"
#include "log_conn.h"
#include "client.h"
#include "threads.h"

#define CONNECTION_BUFFER_SIZE      81920
#define CONNECTION_PATH_MAX         1000
#define MAX_CONTENT_LENGTH_STRING   10

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
 * Read message from TCP connection
 * And Validate
 * @returns String
 */
String getMessageFromConnection(SocketFD socket, int threadId);

#endif // CONNECTION_H_INCLUDED
