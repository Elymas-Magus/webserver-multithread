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
    server->initPools(server->pools, server);
}

void
connectionLoop(Server * server)
{
    printf("................. Initing connection loop .................\n\n");
    while (true) {
        connectionListener(server);
    }
    
    shutdown(server->socket, SHUT_RDWR);
}

void
mutexLock(pthread_mutex_t * mutex)
{
    printf("lock\n");
    if (pthread_mutex_lock(mutex) != 0) {                                          
        WARNING("Error at mutex lock (%s)\n", getLocalCurrentTimeInHttpFormat());    
        LOG_ERROR("Error at mutex lock (%s)\n", getLocalCurrentTimeInHttpFormat());                                                       
        exit(2);                                                                    
    }
}

void
mutexUnlock(pthread_mutex_t * mutex)
{
    printf("unlock\n");
    if (pthread_mutex_unlock(mutex) != 0) {                                          
        WARNING("Error at mutex unlock (%s)\n", getLocalCurrentTimeInHttpFormat());     
        LOG_ERROR("Error at mutex unlock (%s)\n", getLocalCurrentTimeInHttpFormat());                                                       
        exit(2);                                                                    
    }
}

void
emitSignal(pthread_cond_t * cond)
{
    printf("emit signal\n");
    if (pthread_cond_signal(cond) != 0) {                                          
        WARNING("Error at mutex unlock (%s)\n", getLocalCurrentTimeInHttpFormat());  
        LOG_ERROR("Error at mutex unlock (%s)\n", getLocalCurrentTimeInHttpFormat());                             
    }
}

void
condWait(pthread_cond_t * cond, pthread_mutex_t * mutex)
{
    if (pthread_cond_wait(cond, mutex) != 0) {                                          
        WARNING("Error at mutex unlock (%s)\n", getLocalCurrentTimeInHttpFormat());      
        LOG_ERROR("Error at mutex unlock (%s)\n", getLocalCurrentTimeInHttpFormat());                             
    }
}

void
connectionListener(Server * server)
{
    socklen_t addrSize = (socklen_t) sizeof(SA_IN);

    Client * client = (Client *) malloc(sizeof(Client));
    printf("------------------- Waiting for connections --------------------\n");

    if (!validate(
        client->socket = accept(server->socket, (SA *) &(client->address), &addrSize),
        "Accept Failed"
    )) { return; }

    printf("------------------- Connected (%d) --------------------\n", server->socket);

    mutexLock(&(server->pools->mutex));

    server->pools->queue->enqueue(server->pools->queue, (void **) client, sizeof(Client *));

    emitSignal(&(server->pools->cond));
    mutexUnlock(&(server->pools->mutex));

    free(client);
}

void *
threadConnectionHandler(void * arg)
{
    if (arg == NULL) {
        return NULL;
    }

    void ** client;
    ThreadArg * threadArg = (ThreadArg *) arg;
    Server * server = (Server *) threadArg->content;

    printf("---------------------- Start thread loop -----------------------\n");
    while (true) {
        mutexLock(&(server->pools->mutex));

        if (server->pools->queue->items->length == 0 && !server->pools->shutdown) {
            condWait(&(server->pools->cond), &(server->pools->mutex));
        }
        if (server->pools->shutdown) {
            break;
        }

        client = server->pools->queue->dequeue(server->pools->queue);

        if (client == NULL) {
            continue;
        }

        logConnectionStart(threadArg, ((Client *) client), getCurrentTimeString());

        mutexUnlock(&(server->pools->mutex));
        handleConnection(threadArg, ((Client *) client), server);

        free(client);

        threadArg->connectionId++;
    }

    printf("----------------------- End thread loop ------------------------\n");
    free(server);
    free(threadArg);
    free(client);

    mutexUnlock(&(server->pools->mutex));
    pthread_exit(NULL);

    return NULL;  
}

void
handleConnection(ThreadArg * args, Client * client, Server * server)
{
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

    char absolutepath[CONNECTION_PATH_MAX + slack];
    char IBuffer[MAX_HTTP_MESSAGE_LENGTH];
    char path[CONNECTION_BUFFER_SIZE + rootPathSize + slack];

    struct stat htmlAttr;

    Stream * stream = initStream();

    HttpRequest * request = newRequest();
    HttpRequest * response = newRequest();

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
            IBuffer[messageSize - slack] == '\n'
        ) {
            break;
        }
    }

    validateOrDie(bytesRead, "recv error");
    IBuffer[messageSize - 1] = 0;

    TRY {
        if (extractRequest(request, IBuffer, server->root) == false) {
            messageCode = HTTP_INTERNAL_SERVER_ERROR;
            THROW(INTERNAL_ERROR);
        }

        fflush(stdout);
        
        printf("Path da requisição:\n%s\n", request->path);
        
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

void
logConnectionStart(ThreadArg * args, Client * client, String currTime)
{
    char ipAddress[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(client->address.sin_addr), ipAddress, INET_ADDRSTRLEN);
    LOG_CONNECTTION(
        args->logFilename,
        "CODE: %u - THREAD_ID: %u\nClientSocket: %d; ClientAddr: %s; Start: %s\n\n",
        args->connectionId, args->threadId, client->socket, ipAddress, currTime
    );
}

void
logConnectionEnd(ThreadArg * args, Client * client, String currTime, float duration, String path, bool error)
{
    char ipAddress[INET_ADDRSTRLEN];
    char errorStatus[][8] = {
        "SUCCESS",
        "ERROR",
    };
    inet_ntop(AF_INET, &(client->address.sin_addr), ipAddress, INET_ADDRSTRLEN);
    LOG_CONNECTTION_ON_FILE(
        args->logFilename,
        "CODE: %u - THREAD_ID: %u\nClientSocket: %d; ClientAddr: %s; End: %s;\n Path: %s; Duration: %0.8f; Status: %s\n\n",
        args->connectionId, args->threadId, client->socket, ipAddress, currTime, path, duration, errorStatus[error]
    );
}