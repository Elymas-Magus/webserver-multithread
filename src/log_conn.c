#include "log_conn.h"

void
setThreadFilename(String buffer, int threadId)
{
    String filename = (String) malloc(MAX_FOLDERNAME);

    time_t tmi;
    struct tm * info;

    time(&tmi);
    info = localtime(&tmi);

    sprintf(buffer, LOG_CONNECTION_THREAD_FOLDER, threadId);
    strftime(filename, DATE_MAX, LOG_CONNECTION_FILE, info);
    strcat(buffer, filename);
    strcat(buffer, LOG_EXTENSION); 
}

void
logConnectionStart(ThreadArg * args, Client * client, String currTime)
{
    setThreadFilename(args->logFilename, args->threadId);
    LOG_CONNECTTION_ON_FILE(
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

void
logMessage(ThreadArg * args, Client * client, String currTime, String path, String message)
{
    LOG_CONNECTTION_ON_FILE(
        args->logFilename,
        "CODE: %u - THREAD_ID: %u\nClientSocket: %d; ClientAddr: %s; TIME: %s;\n Path: %s; Message: %s\n\n",
        args->connectionId, args->threadId, client->socket, getIpv4(client->address), currTime, path, message
    );
}