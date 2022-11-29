#include "includes/server.h"
#include "includes/connection.h"

#define DEFAUL_FILE_CONFIG "./server.conf"

void logServerConfis(Server * server);

int
main()
{
    Server * server = createServer(
        getServerConfigFromConfigFile(DEFAUL_FILE_CONFIG)
    );

    logServerConfis(server);
    initServer(server);
    listenConnection(server);
    serverDestroy(server);

    return 0;
}

void
logServerConfis(Server * server)
{
    char ipAddress[INET_ADDRSTRLEN];

    inet_ntop(AF_INET, &(server->address.sin_addr), ipAddress, INET_ADDRSTRLEN);

    printf("Running at address: %s:%d\n", ipAddress, server->port);
}