#include "utils.h"

void
substring(String str, u_int start, u_int end, String buffer)
{
    if (str == NULL) {
        WARNING("String is required\n");
        return;
    }
    if (buffer == NULL) {
        WARNING("Buffer reference is required\n");
        return;
    }

    u_int i, j;
    for (i = start, j = 0; i < end; i++, j++) {
        buffer[j] = str[i];
    }
    buffer[j] = 0;
}

ServerConfig *
getServerConfigFromConfigFile(String filename)
{
    ServerConfig * config = (ServerConfig *) mallocOrDie(
        sizeof(ServerConfig), "Server config"
    );

    config->port = 0;
    config->backlog = 0;
    config->threadMax = 0;

    if (readConfigFile(filename, config) == CONFIG_ERROR) {
        WARNING("The object couldn't be made by this config file. Filename %s\n", filename);
        exit(1);
    }

    return config;
}

int
readConfigFile(String filename, ServerConfig * config)
{
    if (filename == NULL) {
        WARNING("Filename is required\n");
        return CONFIG_ERROR;
    }
    if (config == NULL) {
        WARNING("ServerConfig reference is required\n");
        return CONFIG_ERROR;
    }

    char key[MAX_CONFIG_KEY_LENGTH], value[MAX_CONFIG_VALUE_LENGTH];

    FILE * file = fopen(filename, "r");

    if (file == NULL) {
        WARNING("File couldn't be opened. Filename %s\n", filename);
    }

    while (fscanf(file, CONFIG_LINE_MODEL, key, value) != EOF) {
        if (!strcmp(CONFIG_ROOT_PATH, key)) {
            realpath(value, config->root);
        }

        if (!strcmp(CONFIG_PORT, key)) {
            config->port = atoi(value);
        }

        if (!strcmp(CONFIG_BACKLOG, key)) {
            config->backlog = atoi(value);
        }

        if (!strcmp(CONFIG_THREAD_MAX, key)) {
            config->threadMax = atoi(value);
        }
    }

    if (config->port < 1) {
        config->port = SERVER_PORT;
    }

    if (config->backlog < 1) {
        config->backlog = SERVER_BACKLOG;
    }

    if (config->threadMax < 1) {
        config->threadMax = THREAD_MAX_DEFAULT;
    }

    return CONFIG_SUCCESS;
}

String
toFstring(const char * fmt, ...)
{
    va_list args;
    String buffer = (String) mallocOrDie(
        MAX_STRING_BUFFER, "Fstring buffer"
    );

    va_start(args, fmt);
    vsprintf(buffer, fmt, args);
    va_end(args);

    return buffer;
}

String
newString(size_t length)
{
    return (String) malloc(length);
}

String
getIpv4(SA_IN address)
{
    String ipv4 = newString(INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &(address.sin_addr), ipv4, INET_ADDRSTRLEN);

    return ipv4;
}