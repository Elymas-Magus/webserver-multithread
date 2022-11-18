#include "server.h"

Server *
createServer(ServerConfig * config)
{
    Server * server = (Server *) malloc(sizeof(Server));

    strcpy(server->name, "WebServerMT");
    strcpy(server->root, config->root);
    server->port = config->port;
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

void
saveSocketContext(int serverSocket)
{
    int nmemb = 1;
    FILE * file = fopen(LOG_CONTEXT_FILENAME, "w");

    TRY {
        if (file == NULL) {
            THROW(OPENING_FILE_ERROR);
        }
        if (fwrite(&serverSocket, sizeof(int), nmemb, file) != nmemb) {
            THROW(FILE_INSERTION_ERROR);
        }
        fclose(file);
    } CATCH (FILE_INSERTION_ERROR) {
        WARNING("%s\n", getCurrentThrowableMessage());
    } CATCH (OPENING_FILE_ERROR) {
        WARNING("%s\n", getCurrentThrowableMessage());
    }
}

int
getServerSocket()
{
    int serverSocket = check(
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
    serverAddr.sin_port = htons(port);

    return serverAddr;
}

int
initServer(Server * server)
{
    bindServerAddr(&server->socket, server->address);
    return initListen(server->socket);
}

int
initListen(int serverSocket)
{
    return check(listen(serverSocket, SERVER_BACKLOG), "Listen failed!");
}

int
bindServerAddr(int * serverSocket, SA_IN serverAddr)
{
    int bindStatus, socket = (* serverSocket);
    if ((bindStatus = bind(socket, (SA *) &serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)) {
        if ((socket = getServerSocketContext()) == SOCKET_ERROR) {
            bindStatus = SOCKET_ERROR;
        } else {
            (* serverSocket) = socket;
            bindStatus = setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));
        }
    }
    
    if (bindStatus != SOCKET_ERROR) {
        saveSocketContext(socket);
    }

    return check(bindStatus, "Bind Failed");
}
