#include "connection.h"

void
connectionLoop(Server * server)
{
    Client * client;
    socklen_t addrSize = (socklen_t) sizeof(SA_IN);

    initMutex(&(mutex));
    initCond(&(cond));

    printf("................. Initing connection loop .................\n\n");
    while (true) {

        client = (Client *) mallocOrDie(sizeof(Client), "Client");
        printf("[C] Waiting for connections\n");

        if (!validate(
            client->socket = accept(server->socket, (SA *) &(client->address), &addrSize),
            "Accept Failed"
        )) { break; }

        printf("[C] Connected (%d);\t", client->socket);
        printf("Address: (%s)\n\n", getIpv4(client->address));

        printf("[C] lock\n");
        mutexLock();

        printf("[C] enqueue\n");
        enqueue(client);

        // // usleep(150000);
        // printf("[C] emit broadcast\n");
        // // if (queueIsEmpty()) {
        //     emitBroadcast();
        // // }

        printf("[C] unlock\n");
        mutexUnlock();

        // usleep(150000);
        emitSignal();
    }

    free(client);
    
    printf("Poweroff\n");
    shutdown(server->socket, SHUT_RDWR);
}

void *
threadConnectionHandler(void * arg)
{
    if (arg == NULL) {
        return NULL;
    }

    Client * client;
    ThreadArg * threadArg = (ThreadArg *) arg;
    Server * server = (Server *) threadArg->content;

    printf("[H:%d] Start thread loop\n", threadArg->threadId);
    while (true) {
        printf("[H:%d] lock\n", threadArg->threadId);
        mutexLock();

        printf("[H:%d] Getting client\n", threadArg->threadId);
        client = dequeue();

        while (client == NULL) {
            condWait(threadArg->threadId);
            client = dequeue();
        }

        printf("[H:%d] Desempilhando o cliente %d\n", threadArg->threadId, client->socket);

        printf("[H:%d] unlock\n", threadArg->threadId);
        mutexUnlock();

        printf("[H:%d] Logging\n", threadArg->threadId);
        logConnectionStart(threadArg, client, getCurrentTimeString());

        printf("[H:%d] Tratando conexão\n", threadArg->threadId);
        handleConnection(threadArg, client, server);
        printf("[H:%d] Finalizando conexão\n", threadArg->threadId);

        threadArg->connectionId++;
        free(client);
    }

    printf("[H:%d] End thread loop\n", threadArg->threadId);
    free(server);
    free(threadArg);

    mutexUnlock();
    pthread_exit(NULL);

    return NULL;  
}

void
handleConnection(ThreadArg * args, Client * client, Server * server)
{
    printf("[HC:%d] Tratando cliente %d\n", args->threadId, client->socket);
    bool error = true;

    String currentTime;
    time_t start;
    time_t end;

    start = getCurrentTime();
    
    size_t bytesRead;

    int messageCode;
    int messageSize = 0;
    int slack = 1;
    int rootPathSize = strlen(server->root);

    printf("[HC:%d] Init...\n", args->threadId);

    char absolutepath[CONNECTION_PATH_MAX + slack];
    char IBuffer[MAX_HTTP_MESSAGE_LENGTH];
    char path[CONNECTION_BUFFER_SIZE + rootPathSize + slack];

    struct stat htmlAttr;

    printf("[HC:%d] Init stream request\n", args->threadId);

    Stream * stream = initStream();

    printf("[HC:%d] Alocate memory for request\n", args->threadId);

    HttpRequest * request = newRequest();
    HttpRequest * response = newRequest();

    printf("[HC:%d] read request\n", args->threadId);

    while (
        (bytesRead = read(
            client->socket,
            IBuffer + messageSize,
            sizeof(IBuffer) - messageSize - slack
        )) > 0
    ) {
        messageSize += bytesRead;
        if (
            messageSize > MAX_HTTP_MESSAGE_LENGTH - slack ||
            (IBuffer[messageSize - slack - 1] == '\r' &&
            IBuffer[messageSize - slack] == '\n')
        ) {
            break;
        }
    }

    printf("[HC:%d] Validate request\n", args->threadId);

    validateOrDie(bytesRead, "recv error");
    IBuffer[messageSize - 1] = 0;

    TRY {
        if (extractRequest(request, IBuffer, server->root) == false) {
            messageCode = HTTP_INTERNAL_SERVER_ERROR;
            THROW(INTERNAL_ERROR);
        }

        fflush(stdout);
        
        printf("[HC:%d] Path da requisição:\n%s\n", args->threadId, request->path);
        
        strcpy(path, request->path);
        strcpy(response->mimeType, request->mimeType);
        strcpy(response->httpVersion, HTTP_VERSIONS[HTTP_VERSION_1s1]);

        addHeader(response, "Accept-Ranges", "bytes");
        addHeader(response, "Keep-Alive", "timeout=5, max=100");
        addHeader(response, "Date", getCurrentTimeInHttpFormat());
        addHeader(response, "Accept-Language", "pt-BR,pt;q=0.9,en-US;q=0.8,en;q=0.7");
        addHeader(response, "Content-Type", getMimeTypeFormatted(request->mimeType));
        addHeader(response, "Server", server->name);
        addHeader(response, "Connection", "close");

        fflush(stdout);
        
        if (realpath(path, absolutepath) == NULL) {
            messageCode = HTTP_NOT_FOUND;
            THROW(FILE_REALPATH_ERROR);
        }

        stat(absolutepath, &htmlAttr);
        strcpy(stream->path, absolutepath);
        
        addHeader(response, "Content-Length", toFstring("%lu", htmlAttr.st_size));
        addHeader(response, "Last-Modified", getTimeInHttpFormat(&htmlAttr.st_mtime));

        error = false;
        messageCode = HTTP_OK;
        
    } CATCHALL {
        WARNING("%s; PATH: %s\n", getCurrentThrowableMessage(), path);
    } FINALLY {
        end = getCurrentTime();
        currentTime = getCurrentTimeString();
        
        sendResponse(response, messageCode, client->socket, stream);
        logConnectionEnd(args, client, currentTime, difftime(end, start), path, error);

        requestFree(request);
        requestFree(response);

        printf("\n----- closing connection -----\n");
    }
}