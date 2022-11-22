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
        WARNING("Error at mutex lock\n");                                                       
        exit(2);                                                                    
    }
}

void
mutexUnlock(pthread_mutex_t * mutex)
{
    if (pthread_mutex_unlock(mutex) != 0) {                                          
        WARNING("Error at mutex unlock\n");                                                       
        exit(2);                                                                    
    }
}

void
emitSignal(pthread_cond_t * cond)
{
    if (pthread_cond_signal(cond) != 0) {                                          
        WARNING("Error at mutex unlock\n");                             
    }
}

void
condWait(pthread_cond_t * cond, pthread_mutex_t * mutex)
{
    if (pthread_cond_wait(cond, mutex) != 0) {                                          
        WARNING("Error at mutex unlock\n");                             
    }
}

void
connectionListener(Server * server, socklen_t * addrSize, SA_IN clientAddr)
{
    int * clientSocket = (int *) malloc(sizeof(int));
    printf("Waiting for connections ...\n");

    check((* clientSocket) = accept(server->socket, (SA *) &clientAddr, addrSize), "Accept Failed");

    mutexLock(&(server->pools->mutex));

    printf("New request -> %d\n", (* clientSocket));
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
    ThreadArg * threadArg = (ThreadArg *) arg;
    Server * server = (Server *) threadArg->content;

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

        logConnectionStart(threadArg, *((SocketFD *) clientSocket), getCurrentTimeString());

        mutexUnlock(&server->pools->mutex);
        handleConnection(threadArg, *((SocketFD *) clientSocket), server);

        threadArg->connectionId++;
    }

    free(server);
    free(threadArg);
    free(clientSocket);

    mutexUnlock(&server->pools->mutex);
    pthread_exit(NULL);

    return NULL;  
}

void
handleConnection(ThreadArg * args, SocketFD clientSocket, Server * server)
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
            clientSocket,
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

    check(bytesRead, "recv error");
    IBuffer[messageSize - 1] = 0;

    printf("%s\n", IBuffer);
    TRY {
        if (extractRequest(request, IBuffer, server->root) == false) {
            THROW(INTERNAL_ERROR);
        }

        strcpy(path, request->path);
        strcpy(response->mimeType, request->mimeType);
        strcpy(response->httpVersion, HTTP_VERSIONS[HTTP_VERSION_1s1]);

        setHeader(response, "Accept-Ranges", "bytes");
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
        
        printf("%s\n", getStringFileSize(absolutepath));
        addHeader(response, "Content-Length", getStringFileSize(absolutepath));
        addHeader(response, "Last-Modified", getTimeInHttpFormat(&htmlAttr.st_mtime));

        error = false;
        messageCode = HTTP_OK;
        
    } CATCH (INTERNAL_ERROR) {
        WARNING("%s; PATH: %s\n", getCurrentThrowableMessage(), path);
        close(clientSocket);
        return;
    } CATCH (FILE_REALPATH_ERROR) {
        WARNING("%s; PATH: %s\n", getCurrentThrowableMessage(), path);
    } CATCH (FILE_READING_ERROR) {
        WARNING("%s; PATH: %s\n", getCurrentThrowableMessage(), path);
    } FINALLY {
        end = getCurrentTime();
        currentTime = getCurrentTimeString();
        
        sendResponse(response, messageCode, clientSocket, stream);
        logConnectionEnd(args, clientSocket, currentTime, difftime(end, start), path, error);

        requestFree(request);
        requestFree(response);

        printf("\n----- closing connection -----\n");
    }
}

void
logConnectionStart(ThreadArg * args, int clientSocket, String currTime)
{
    LOG_CONNECTTION(args->logFilename, "CODE: %u - THREAD_ID: %u\nClientSocket: %d; Start: %s\n\n", args->connectionId, args->threadId, clientSocket, currTime);
}

void
logConnectionEnd(ThreadArg * args, int clientSocket, String currTime, float duration, String path, bool error)
{
    char errorStatus[][8] = {
        "SUCCESS",
        "ERROR",
    };
    LOG_CONNECTTION_ON_FILE(
        args->logFilename,
        "CODE: %u - THREAD_ID: %u\nClientSocket: %d; End: %s;\n Path: %s; Duration: %0.8f; Status: %s\n\n",
        args->connectionId, args->threadId, clientSocket, path, currTime, duration, errorStatus[error]
    );
}