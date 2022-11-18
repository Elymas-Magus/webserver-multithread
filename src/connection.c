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
        printf("Process new request -> %d\n", *((int *) clientSocket));

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
    size_t bytesRead;

    int messageCode;
    int messageSize = 0;

    char contentLength[MAX_CONTENT_LENGTH_STRING];
    char absolutepath[CONNECTION_PATH_MAX + 1];
    char buffer[MAX_HTTP_MESSAGE_LENGTH];
    char path[CONNECTION_BUFFER_SIZE + strlen(server->root) + 1];

    struct stat htmlAttr;

    TimeSpec start;
    TimeSpec end;

    FILE * fp = NULL;
    HttpRequest * request = (HttpRequest *) malloc(sizeof(HttpRequest));
    HttpRequest * response = (HttpRequest *) malloc(sizeof(HttpRequest));

    clock_gettime(CLOCK_REALTIME, &start);
    logConnectionStart(clientSocket, getCurrentTime());
    while ((bytesRead = read(clientSocket, buffer + messageSize, sizeof(buffer) - messageSize - 1)) > 0) {
        messageSize += bytesRead;
        if (messageSize > MAX_HTTP_MESSAGE_LENGTH - 1 || buffer[messageSize - 1] == '\n') {
            break;
        }
    }

    check(bytesRead, "recv error");
    buffer[messageSize - 1] = 0;

    TRY {
        if (extractRequest(request, buffer) == false) {
            THROW(INTERNAL_ERROR);
        }

        strcpy(path, server->root);
        strcat(path, request->path);

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
        addHeader(response, "Content-Type", (String) httpHeaders[HTTP_HEADER_CONTENT_TYPE].value);
        addHeader(response, "Content-Length", contentLength);

        // printf("\n%s 200 OK\n", response->httpVersion);
        // for (Node * no = response->headers->first; no; no = no->next) {
        //     printf("%s: %s\n", ((HttpHeaders *) no->content)->key, ((HttpHeaders *) no->content)->value);
        // }
        // printf("\n%s\n\n", response->body);

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
        clock_gettime(CLOCK_REALTIME, &end);

        mutexLock(&server->pools->mutex);
        logConnectionEnd(clientSocket, getCurrentTime(), time_diff(&start, &end));
        mutexUnlock(&server->pools->mutex);

        sendResponse(response, messageCode, clientSocket);

        printf("\n----- closing connection -----\n");
    }
}

void
logConnectionStart(int clientSocket, String currTime)
{
    LOG_CONNECTTION("ClientSocket: %d; Start: %s", clientSocket, currTime);
}

void
logConnectionEnd(int clientSocket, String currTime, float duration)
{
    LOG_CONNECTTION("ClientSocket: %d; End: %s; Duration: %0.8f\n", clientSocket, currTime, duration);
}