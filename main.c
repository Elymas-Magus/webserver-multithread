#include "includes/server.h"

#define MAX_FILENAME 20
#define DEFAUL_FILE_CONFIG "./server.conf"

void logServerConfis(Server * server);

int
main(int argc, char * argv[])
{
    char filename[MAX_FILENAME];

    if (argc == 1) {
        strcpy(filename, DEFAUL_FILE_CONFIG);
    } else {
        strcpy(filename, argv[1]);
    }

    printf("%s\n", filename);

    Server * server = createServer(
        getServerConfigFromConfigFile(filename)
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