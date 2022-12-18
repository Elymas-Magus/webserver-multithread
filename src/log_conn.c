#include "log_conn.h"

void
logConnectionStart(ThreadArg * args, Client * client, String currTime)
{
    char ipAddress[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(client->address.sin_addr), ipAddress, INET_ADDRSTRLEN);
    LOG_CONNECTTION(
        args->logFilename,
        "CODE: %u - THREAD_ID: %u\nClientSocket: %d; ClientAddr: %s; Start: %s\n\n",
        args->connectionId, args->threadId, client->socket, ipAddress, currTime
    );
}

void
logConnectionEnd(ThreadArg * args, Client * client, String currTime, float duration, String path, bool error)
{
    char ipAddress[INET_ADDRSTRLEN];
    char errorStatus[][8] = {
        "SUCCESS",
        "ERROR",
    };
    inet_ntop(AF_INET, &(client->address.sin_addr), ipAddress, INET_ADDRSTRLEN);
    LOG_CONNECTTION_ON_FILE(
        args->logFilename,
        "CODE: %u - THREAD_ID: %u\nClientSocket: %d; ClientAddr: %s; End: %s;\n Path: %s; Duration: %0.8f; Status: %s\n\n",
        args->connectionId, args->threadId, client->socket, ipAddress, currTime, path, duration, errorStatus[error]
    );
}