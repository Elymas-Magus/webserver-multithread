#include "server.h"

Server *
createServer(ServerConfig * config)
{
    Server * server = (Server *) malloc(sizeof(Server));

    strcpy(server->name, "WebServerMT");
    strcpy(server->root, config->root);
    server->port = config->port;
    server->backlog = config->backlog;
    server->socket = getServerSocket();
    server->address = getServerAddr(config->port);
    server->pools = createThreadpool(config->threadMax);
    server->initPools = initThreadpools;

    return server;
}

int
getServerSocketContext()
{
    int nmemb = 1;
    int serverSocket, socket = SOCKET_ERROR;
    FILE * file = fopen(LOG_CONTEXT_FILENAME, "r");

    TRY {
        if (file == NULL) {
            THROW(OPENING_FILE_ERROR);
        }
        if (fread(&serverSocket, sizeof(int), nmemb, file) != nmemb) {
            THROW(FILE_READING_ERROR);
        }
        printf("Last Socket: %d\n", serverSocket);
        fclose(file);
        close(serverSocket);

        socket = serverSocket;
    } CATCH (FILE_READING_ERROR) {
        WARNING("%s\n", getCurrentThrowableMessage());
    } CATCH (OPENING_FILE_ERROR) {
        WARNING("%s\n", getCurrentThrowableMessage());
    } FINALLY {
        return socket;
    }
}

int
getServerSocket()
{
    SocketFD serverSocket = validateOrDie(
        socket(AF_INET, SOCK_STREAM, 0),
        "Failed to create socket"
    );

    return serverSocket;
}

SA_IN
getServerAddr(u_int port)
{
    SA_IN serverAddr;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    // serverAddr.sin_addr.s_addr = inet_addr("192.168.250.25");
    serverAddr.sin_port = htons(port);

    return serverAddr;
}

int
initServer(Server * server)
{
    int optval = 1;
    validateOrDie(
        setsockopt(server->socket, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int)), 
        "Error reusing address"
    );

    bindServerAddr(server->socket, server->address);
    return initListen(server->socket, server->backlog);
}

int
initListen(SocketFD serverSocket, u_int serverBacklog)
{
    return validateOrDie(listen(serverSocket, serverBacklog), "Listen failed!");
}

int
bindServerAddr(SocketFD serverSocket, SA_IN serverAddr)
{
    return validateOrDie(bind(serverSocket, (SA *) &serverAddr, sizeof(serverAddr)), "Bind Failed");
}

void
serverDestroy(Server * server)
{
    poolDestroy(server->pools);
    free(server->initPools);
    free(server);

}