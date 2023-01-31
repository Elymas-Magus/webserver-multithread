#include "connection.h"

void
connectionLoop(Server * server)
{
    Client * client;
    socklen_t addrSize = (socklen_t) sizeof(SA_IN);

    initProducer();
    initConsumer();

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

        blockProducer();

        printf("[C] enqueue\n");
        enqueue(client);

        releaseConsumer();
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
        blockConsumer(threadArg->threadId);

        printf("[H:%d] Getting client\n", threadArg->threadId);
        client = dequeue();

        if (client == NULL) {
            releaseProducer(threadArg->threadId);
            continue;
        }

        releaseProducer(threadArg->threadId);

        printf("[H:%d] Desempilhando o cliente %d\n", threadArg->threadId, client->socket);
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

    releaseProducer(threadArg->threadId);
    pthread_exit(NULL);

    return NULL;  
}

void
handleConnection(ThreadArg * args, Client * client, Server * server)
{
    bool success = false;
    int messageCode;

    time_t start, end;
    ssize_t readed, size = 0;

    char absolutepath[CONNECTION_PATH_MAX];
    char buffer[MAX_HTTP_MESSAGE_LENGTH];
    char path[CONNECTION_BUFFER_SIZE + strlen(server->root)];

    struct stat htmlAttr;

    HttpRequest * request = newRequest();
    HttpRequest * response = newRequest();
    Stream * stream = initStream();

    start = getCurrentTime();

    printf("[HC:%d] Init...\n", args->threadId);

    String currentTime;     

    CONN_READER(client, buffer, readed, size);

    TRY {
        if (request == NULL || response == NULL) {
            strcpy(path, "");
            messageCode = HTTP_INTERNAL_SERVER_ERROR;
            THROW(INTERNAL_ERROR);
        }
        if (!validate(readed, "recv error")) {
            strcpy(path, "");
            messageCode = HTTP_INTERNAL_SERVER_ERROR;
            THROW(INTERNAL_ERROR);
        }

        buffer[size - 1] = 0;

        fflush(stdout);
        
        if (extractRequest(request, buffer, server->root, args->threadId) == false) {
            messageCode = HTTP_INTERNAL_SERVER_ERROR;
            THROW(INTERNAL_ERROR);
        }
        
        printf("[HC:%d] Path da requisição: %s\n", args->threadId, request->path);
        
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

        if (realpath(path, absolutepath) == NULL) {
            printf("[HC:%d] cheguei 8\n", args->threadId);
            messageCode = HTTP_NOT_FOUND;
            THROW(FILE_REALPATH_ERROR);
        }

        stat(absolutepath, &htmlAttr);
        strcpy(stream->path, absolutepath);
        
        addHeader(response, "Content-Length", toFstring("%lu", htmlAttr.st_size));
        addHeader(response, "Last-Modified", getTimeInHttpFormat(&htmlAttr.st_mtime));

        success = true;
        messageCode = HTTP_OK;
        
    } CATCHALL {
        WARNING("%s; PATH: %s\n", getCurrentThrowableMessage(), path);
    } FINALLY {
        end = getCurrentTime();
        currentTime = getCurrentTimeString();
        
        sendResponse(response, messageCode, client->socket, stream, args->threadId);
        printf("------------------------------------------------------------------\n");
        logConnectionEnd(args, client, currentTime, difftime(end, start), path, !success);

        requestFree(request);
        requestFree(response);

        printf("[HC:%d] closing connection\n", args->threadId);
        printf("------------------------------------------------------------------\n");
    }
}