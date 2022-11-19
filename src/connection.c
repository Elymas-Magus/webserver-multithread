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

        printf("Dequeue\n");
        clientSocket = server->pools->queue->dequeue(server->pools->queue);

        if (clientSocket == NULL) {
            continue;
        }

        logConnectionStart(threadArg, *((int *) clientSocket), getCurrentTimeString());

        printf("Handling\n");
        handleConnection(threadArg, *((int *) clientSocket), server);
        mutexUnlock(&server->pools->mutex);

        threadArg->connectionId++;
    }
    free(clientSocket);
    mutexUnlock(&server->pools->mutex);
    pthread_exit(NULL);

    return NULL;  
}

void
handleConnection(ThreadArg * args, int clientSocket, Server * server)
{
    bool error = true;

    time_t * start;
    time_t * end;

    start = getCurrentTime();
    
    size_t bytesRead;

    int messageCode;
    int messageSize = 0;

    char contentLength[MAX_CONTENT_LENGTH_STRING];
    char absolutepath[CONNECTION_PATH_MAX + 1];
    char buffer[MAX_HTTP_MESSAGE_LENGTH];
    char path[CONNECTION_BUFFER_SIZE + strlen(server->root) + 1];

    struct stat htmlAttr;

    String contentType = (String) malloc(MAX_CONTENT_TYPE_LEN);

    FILE * fp = NULL;
    HttpRequest * request = (HttpRequest *) malloc(sizeof(HttpRequest));
    HttpRequest * response = (HttpRequest *) malloc(sizeof(HttpRequest));

    while ((bytesRead = read(clientSocket, buffer + messageSize, sizeof(buffer) - messageSize - 1)) > 0) {
        messageSize += bytesRead;
        if (messageSize > MAX_HTTP_MESSAGE_LENGTH - 1 || buffer[messageSize - 1] == '\n') {
            break;
        }
    }

    check(bytesRead, "recv error");
    buffer[messageSize - 1] = 0;

    printf("%s\n", buffer);
    TRY {
        if (extractRequest(request, buffer, server->root) == false) {
            THROW(INTERNAL_ERROR);
        }

        strcpy(path, request->path);

        printf("\n\nfilename: %s\n", request->filename);
        printf("mime_type: %s\n", request->mimeType);
        printf("extension: %s\n", request->extension);
        printf("path: %s\n", request->path);
        printf("query: %s\n\n\n", request->query);

        strcpy(contentType, request->mimeType);
        strcat(contentType, "; charset=utf-8");
        strcpy(response->mimeType, request->mimeType);
        strcpy(response->httpVersion, HTTP_VERSIONS[HTTP_VERSION_1s1]);
        strcpy(response->body, "");

        setHeader(response, "Date", getCurrentTimeInHttpFormat());

        printf("REQUEST: %s\n", path);
        fflush(stdout);
        
        if (realpath(path, absolutepath) == NULL) {
            messageCode = HTTP_NOT_FOUND;
            THROW(FILE_REALPATH_ERROR);
        }

        fp = fopen(absolutepath, "r");
        if (fp == NULL) {
            messageCode = HTTP_BAD_REQUEST;
            THROW(FILE_READING_ERROR);
        }

        if ((bytesRead = fread(buffer, 1, MAX_HTTP_MESSAGE_LENGTH, fp)) > 0) {
            printf("Sending %zu bytesRead\n", bytesRead);
            strcpy(response->body, buffer);
        }
        fclose(fp);

        stat(absolutepath, &htmlAttr);
        sprintf(contentLength, "%zu", bytesRead);
        addHeader(response, "Accept-Language", "pt-BR,pt;q=0.9,en-US;q=0.8,en;q=0.7");
        addHeader(response, "Last-Modified", getTimeInHttpFormat(&htmlAttr.st_mtime));
        addHeader(response, "Content-Type", contentType);
        addHeader(response, "Content-Length", contentLength);

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

        // mutexLock(&server->pools->mutex);
        logConnectionEnd(args, clientSocket, getCurrentTimeString(), difftime(*end, *start), path, error);
        // mutexUnlock(&server->pools->mutex);

        sendResponse(response, messageCode, clientSocket);

        printf("\n----- closing connection -----\n");
    }
}

void
logConnectionStart(ThreadArg * args, int clientSocket, String currTime)
{
    LOG_CONNECTTION(args->logFilename, "CODE: %u - THREAD_ID: %u\nClientSocket: %d; Start: %s", args->connectionId, args->threadId, clientSocket, currTime);
}

void
logConnectionEnd(ThreadArg * args, int clientSocket, String currTime, float duration, String path, bool error)
{
    LOG_CONNECTTION_ON_FILE(args->logFilename, "CODE: %u - THREAD_ID: %u\nClientSocket: %d; Path: %s\n End: %s; Duration: %0.8f; Status: %d\n", args->connectionId, args->threadId, clientSocket, path, currTime, duration, error);
}