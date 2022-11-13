#include "connection.h"

void
listenConnection(Server server, ServerConfig * config)
{
    connectionLoop(server.socket, config);
}

void
connectionLoop(int serverSocket, ServerConfig * config)
{
    int * clientSocket = (int *) malloc(sizeof(int));
    SA_IN clientAddr;

    while (true) {
        connectionListener(serverSocket, clientSocket, sizeof(SA_IN), clientAddr, config);
    }

    // // closing the connected socket
    // close(clientSocket);
    // // closing the listening socket
    // shutdown(serverSocket, SHUT_RDWR);
}

void
connectionListener(int serverSocket, int * clientSocket, int addrSize, SA_IN clientAddr, ServerConfig * config)
{
    printf("Waiting for connections ...\n");

    createConnectionThread(config);

    // makeSocketNonBlocking
    // Create HttpRequest Object
    // Add To EPoll
    // Add Timer

    // check(*clientSocket = accept(serverSocket, (SA *) &clientAddr, (socklen_t *) &addrSize), "Accept Failed");

    // printf("Connected!\n");
}

void
createConnectionThread(ServerConfig * config)
{
    thread_pool = (pthread_t *) malloc(config->threadMax * sizeof(pthread_t));

    for (int i = 0; i < config->threadMax; i++) {
        pthread_create(&thread_pool[i], NULL, threadPoolHander, config);
    }
}

void * threadPoolHander(void * pServerConfig)
{
    ServerConfig * config = ((ServerConfig *) pServerConfig);

    
}

void * handleConnection(void * pClientSocket)
{
    int clientSocket = *((int *) pClientSocket);

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
        return NULL;
    }

    fp = fopen(actualpath, "r");
    if (fp == NULL) {
        fprintf(stderr, "ERROR(open): %s\n", buffer);
        close(clientSocket);
        return NULL;
    }

    while((bytesRead = fread(buffer, 1, CONNECTION_BUFFER_SIZE, fp)) > 0) {
        printf("sending %zu bytes\n", bytesRead);
        write(clientSocket, buffer, bytesRead);
    }

    close(clientSocket);
    fclose(fp);
    printf("closing connection\n");

    return NULL;
}