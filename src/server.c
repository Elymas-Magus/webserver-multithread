#include "server.h"

Server *
createServer(ServerConfig * config)
{
    Server * server = (Server *) mallocOrDie(
        sizeof(Server), "server"
    );

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
listenConnections(Server * server)
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
serverDestroy(Server * server)
{
    poolDestroy(server->pools);
    free(server->initPools);
    free(server);

}