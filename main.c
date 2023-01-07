#include "includes/server.h"

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
    listenConnections(server);
    serverDestroy(server);

    return 0;
}

void
logServerConfis(Server * server)
{
    printf("Running at address: http://%s:%d/\n", getIpv4(server->address), server->port);
}