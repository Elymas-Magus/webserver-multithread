#include "log_conn.h"

void
logConnectionStart(ThreadArg * args, Client * client, String currTime)
{
    LOG_CONNECTTION(
        args->logFilename,
        "CODE: %u - THREAD_ID: %u\nClientSocket: %d; ClientAddr: %s; Start: %s\n\n",
        args->connectionId, args->threadId, client->socket, getIpv4(client->address), currTime
    );
}

void
logConnectionEnd(ThreadArg * args, Client * client, String currTime, float duration, String path, bool error)
{
    char status[][8] = { "SUCCESS", "ERROR" };
    LOG_CONNECTTION_ON_FILE(
        args->logFilename,
        "CODE: %u - THREAD_ID: %u\nClientSocket: %d; ClientAddr: %s; End: %s;\n Path: %s; Duration: %0.8f; Status: %s\n\n",
        args->connectionId, args->threadId, client->socket, getIpv4(client->address), currTime, path, duration, status[error]
    );
}