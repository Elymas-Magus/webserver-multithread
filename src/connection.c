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
connectionListener(Server * server, socklen_t * addrSize, SA_IN clientAddr)
{
    int * clientSocket = (int *) malloc(sizeof(int));
    printf("Waiting for connections ...\n");

    check((* clientSocket) = accept(server->socket, (SA *) &clientAddr, addrSize), "Accept Failed");

    printf("new client: %d\n", (* clientSocket));
    mutexLock(&(server->pools->mutex));

    printf("add to queue\n");
    server->pools->queue->enqueue(server->pools->queue, (void **) clientSocket, sizeof(int *));
    printf("length: %d\n\n", server->pools->queue->items->length);
    
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

        if (server->pools->queue->items->length == 0) {
            continue;
        }
        if (server->pools->shutdown) {
            break;
        }

        printf("remove from queue\n");
        clientSocket = server->pools->queue->dequeue(server->pools->queue);

        if (clientSocket == NULL) {
            continue;
        }

        printf("dequeue: %d\n", *((int *) clientSocket));
        mutexUnlock(&server->pools->mutex);

        handleConnection(clientSocket);
        free(clientSocket);
    }
    mutexUnlock(&server->pools->mutex);
    pthread_exit(NULL);

    return NULL;  
}

void handleConnection(void ** pClientSocket)
{
    int clientSocket = *((int *) pClientSocket);

    printf("%d\n", clientSocket);
    int messageSize = 0;
    char buffer[CONNECTION_BUFFER_SIZE];
    char actualpath[CONNECTION_PATH_MAX + 1];
    size_t bytesRead;

    FILE * fp = NULL;

    while ((bytesRead = read(clientSocket, buffer + messageSize, sizeof(buffer) - messageSize - 1)) > 0) {
        messageSize += bytesRead;
        if (messageSize > CONNECTION_BUFFER_SIZE - 1 || buffer[messageSize - 1] == '\n') {
            break;
        }
    }

    check(bytesRead, "recv error");
    buffer[messageSize - 1] = 0;

    printf("REQUEST: %s\n", buffer);
    fflush(stdout);

    if (realpath(buffer, actualpath) == NULL) {
        fprintf(stderr, "ERROR(bad path): %s\n", buffer);
        close(clientSocket);
        return;
    }

    fp = fopen(actualpath, "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR(open): %s\n", buffer);
        close(clientSocket);
        return;
    }

    while((bytesRead = fread(buffer, 1, CONNECTION_BUFFER_SIZE, fp)) > 0) {
        printf("sending %zu bytes\n", bytesRead);
        write(clientSocket, buffer, bytesRead);
    }

    close(clientSocket);
    fclose(fp);
    printf("closing connection\n");
}