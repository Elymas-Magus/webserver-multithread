#include "connection.h"

typedef struct connQueue {
    Client clients[5000];
    int length;
} ConnQueue;

ConnQueue queue = {
    .length = 0
};

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
connectionListener(Server * server)
{
    socklen_t addrSize = (socklen_t) sizeof(SA_IN);

    Client * client = (Client *) mallocOrDie(sizeof(Client), "Client");
    printf("------------------- Waiting for connections --------------------\n");

    if (!validate(
        client->socket = accept(server->socket, (SA *) &(client->address), &addrSize),
        "Accept Failed"
    )) { return; }

    printf("------------------- Connected (%d) ------------------------------\n", client->socket);
    printf("Address: (%s) -------------------------------------------\n", getIpv4(client->address));

    printf("Listener locking\n");
    mutexLock(&(server->pools->mutex));

    queue.clients[queue.length++] = * client;
    // server->pools->queue->enqueue(server->pools->queue, (void **) client, sizeof(Client *));

    printf("Fila\tLength: %d\n", queue.length);

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

    Client client;
    ThreadArg * threadArg = (ThreadArg *) arg;
    Server * server = (Server *) threadArg->content;

    printf("---------------------- Start thread loop -----------------------\n");
    while (true) {
        mutexLock(&(server->pools->mutex));

        if (queue.length <= 0) {
            condWait(&(server->pools->cond), &(server->pools->mutex));
        }
        // if (server->pools->shutdown) {
        //     break;
        // }

        printf("Fila_Length: %d\n", queue.length);
        printf("client: %d\n", queue.clients[queue.length].socket);
        if (queue.length <= 0) {
            printf("------------------------ invalid client\n");
            printf("Fila_Length: %d\n\n\n\n\n\n", queue.length);
            continue;
        }

        client = queue.clients[--queue.length];
        printf("------------------- Desempilhando o cliente %d\n", client.socket);

        logConnectionStart(threadArg, &client, getCurrentTimeString());

        mutexUnlock(&(server->pools->mutex));
        handleConnection(threadArg, &client, server);

        threadArg->connectionId++;
    }

    printf("----------------------- End thread loop ------------------------\n");
    free(server);
    free(threadArg);

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