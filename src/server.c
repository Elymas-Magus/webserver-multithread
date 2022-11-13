#include "server.h"

Server
createServer(ServerConfig * config)
{
    Server server;

    strcpy(server.name, "WebServerMT");
    server.socket = getServerSocket();
    server.address = getServerAddr(config->port);

    return server;
}

int
getServerSocket()
{
    return check(socket(AF_INET, SOCK_STREAM, 0), "Failed to create socket");
}

SA_IN
getServerAddr(u_int port)
{
    SA_IN serverAddr;

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddr.sin_port = htons(port);

    return serverAddr;
}

int
initServer(Server server)
{
    bindServerAddr(server.socket, server.address);
    return initListen(server.socket);
}

int
initListen(int serverSocket)
{
    return check(listen(serverSocket, SERVER_BACKLOG), "Listen failed!");
}

int
bindServerAddr(int serverSocket, SA_IN serverAddr)
{
    return check(bind(serverSocket, (SA *) &serverAddr, sizeof(serverAddr)), "Bind Failed");
}
