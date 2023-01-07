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
checkThreadLogDirectory(int threadId)
{
    int check;
    String dirname = (String) malloc(MAX_FOLDERNAME);

    sprintf(dirname, LOG_CONNECTION_THREAD_FOLDER, threadId);
    check = mkdir(dirname, LOG_DIRECTORY_PERMS);

    if (!check) {
        printf("Directory created\n");
    } else {
        perror("Unable to create directory\n");
    }
}

void
logConnectionStart(ThreadArg * args, Client * client, String currTime)
{
    checkThreadLogDirectory(args->threadId);
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