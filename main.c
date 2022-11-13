#include "includes/server.h"
#include "includes/connection.h"

#define DEFAUL_FILE_CONFIG "./src/server.conf"

int
main()
{
    ServerConfig * config = createServerConfigFromConfigFile(DEFAUL_FILE_CONFIG);
    Server server = createServer(config);

    initServer(server);
    listenConnection(server, config);

    return 0;
}
