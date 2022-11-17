#include "connection.h"

void
listenConnection(Server * server)
{
    initServerPool(server);
    connectionLoop(server);
}

void
initServerPool(Server * server)
{
    server->pools->tasks->func = threadConnectionHandler;
    server->pools->tasks->args = (void *) server;
    server->initPools(server->pools);
}

void
connectionLoop(Server * server)
{
    socklen_t addrSize = (socklen_t) sizeof(SA_IN);
    SA_IN clientAddr;

    while (true) {
        connectionListener(server, &addrSize, clientAddr);
    }
    
    shutdown(server->socket, SHUT_RDWR);
}

void
mutexLock(pthread_mutex_t * mutex)
{
    if (pthread_mutex_lock(mutex) != 0) {                                          
        fprintf(stderr, "Error at mutex lock");                                                       
        exit(2);                                                                    
    }
}

void
mutexUnlock(pthread_mutex_t * mutex)
{
    if (pthread_mutex_unlock(mutex) != 0) {                                          
        fprintf(stderr, "Error at mutex unlock");                                                       
        exit(2);                                                                    
    }
}

void
emitSignal(pthread_cond_t * cond)
{
    if (pthread_cond_signal(cond) != 0) {                                          
        fprintf(stderr, "Error at mutex unlock");                             
    }
}

void
condWait(pthread_cond_t * cond, pthread_mutex_t * mutex)
{
    if (pthread_cond_wait(cond, mutex) != 0) {                                          
        fprintf(stderr, "Error at mutex unlock");                             
    }
}

void
connectionListener(Server * server, socklen_t * addrSize, SA_IN clientAddr)
{
    int * clientSocket = (int *) malloc(sizeof(int));
    printf("Waiting for connections ...\n");

    check((* clientSocket) = accept(server->socket, (SA *) &clientAddr, addrSize), "Accept Failed");

    mutexLock(&(server->pools->mutex));

    server->pools->queue->enqueue(server->pools->queue, (void **) clientSocket, sizeof(int *));

    emitSignal(&(server->pools->cond));
    mutexUnlock(&(server->pools->mutex));
}

void *
threadConnectionHandler(void * arg)
{
    if (arg == NULL) {
        return NULL;
    }

    void ** clientSocket;
    Server * server = (Server *) arg;

    printf("-------------------- Init thread loop ---------------------\n");
    while (true) {
        mutexLock(&server->pools->mutex);

        if (server->pools->queue->items->length == 0 && !server->pools->shutdown) {
            condWait(&(server->pools->cond), &(server->pools->mutex));
        }
        if (server->pools->shutdown) {
            break;
        }

        clientSocket = server->pools->queue->dequeue(server->pools->queue);

        if (clientSocket == NULL) {
            continue;
        }

        handleConnection(*((int *) clientSocket), server);
        mutexUnlock(&server->pools->mutex);
        free(clientSocket);
    }
    mutexUnlock(&server->pools->mutex);
    pthread_exit(NULL);

    return NULL;  
}

void
handleConnection(int clientSocket, Server * server)
{
    int messageSize = 0;
    char absolutepath[CONNECTION_PATH_MAX + 1];
    char buffer[MAX_HTTP_MESSAGE_LENGTH];
    char path[CONNECTION_BUFFER_SIZE + strlen(server->root) + 1];
    size_t bytesRead;

    FILE * fp = NULL;
    HttpRequest * request;

    readConnectionMessage(&bytesRead, clientSocket, buffer, &messageSize);

    request = extractRequest(buffer);
    printf("cheguei ...\n");
    strcpy(path, server->root);
    strcat(path, request->path);

    printf("REQUEST: %s\n", path);
    fflush(stdout);

    if (realpath(path, absolutepath) == NULL) {
        fprintf(stderr, "ERROR(bad path): %s\n", path);
        sendResponse(request, HTTP_NOT_FOUND, clientSocket);
        return;
    }

    fp = fopen(absolutepath, "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR(open): %s\n", path);
        sendResponse(request, HTTP_BAD_REQUEST, clientSocket);
        return;
    }

    if ((bytesRead = fread(buffer, 1, MAX_HTTP_MESSAGE_LENGTH, fp)) > 0) {
        printf("Sending %zu bytes\n", bytesRead);
        strcpy(request->body, buffer);
    } else {
        strcpy(request->body, "");
    }

    sendResponse(request, HTTP_OK, clientSocket);
    fclose(fp);
    printf("\n----- closing connection -----\n");
}

void
readConnectionMessage(size_t * bytesRead, int clientSocket, char * message, int * messageSize)
{
    size_t bytes = (* bytesRead);
    int size = (* messageSize);
    while ((bytes = read(clientSocket, message + size, sizeof(message) - size - 1)) > 0) {
        size += bytes;
        if (size > MAX_HTTP_MESSAGE_LENGTH - 1 || message[size - 1] == '\n') {
            break;
        }
    }

    check(bytes, "recv error");
    message[size - 1] = 0;

    (* bytesRead) = bytes;
    (* messageSize) = size;
}