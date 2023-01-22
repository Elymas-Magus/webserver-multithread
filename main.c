#include "includes/server.h"

#define MAX_FILENAME 20
#define DEFAUL_FILE_CONFIG "./server.conf"

void logServerConfigs(Server * server);

int
main(int argc, char * argv[])
{
    char filename[MAX_FILENAME];

    if (argc == 1) {
        strcpy(filename, DEFAUL_FILE_CONFIG);
    } else {
        strcpy(filename, argv[1]);
    }

    Server * server = createServer(
        getServerConfigFromConfigFile(filename)
    );

    logServerConfigs(server);
    initServer(server);
    listenConnections(server);
    serverDestroy(server);

    return 0;
}

void
logServerConfigs(Server * server)
{
    printf("Running at address: http://%s:%d/\n", getIpv4(server->address), server->port);
}