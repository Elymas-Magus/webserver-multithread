#include "utils.h"

String
fileGetContent(String filename)
{
    if (filename == NULL) {
        fprintf(stderr, "Filename is required");
        return NULL;
    }

    int i;
    char character;
    char * content = (char *) malloc(MAX_FILE_LENGTH);

    FILE * file = fopen(filename, "r");

    if (file == NULL) {
        fprintf(stderr, "File couldn't be opened. Filename %s", filename);
    }

    for (i = 0; (character = fgetc(file)) != EOF; i++) {
        content[i] = character;        
    }
    content[i] = 0;

    return content;
}

void
substring(String str, u_int start, u_int end, String buffer)
{
    if (str == NULL) {
        fprintf(stderr, "String is required");
        return;
    }
    if (buffer == NULL) {
        fprintf(stderr, "Buffer reference is required");
        return;
    }

    u_int i, j;
    for (i = start, j = 0; i < end; i++, j++) {
        buffer[j] = str[i];
    }
    buffer[j] = 0;
}

ServerConfig *
createServerConfigFromConfigFile(String filename)
{
    ServerConfig * config;

    config->port = 0;
    config->threadMax = 0;

    if (readConfigFile(filename, config) == CONFIG_ERROR) {
        fprintf(stderr, "The object couldn't be made by this config file. Filename %s", filename);
        exit(1);
    }

    return config;
}

int
readConfigFile(String filename, ServerConfig * config)
{
    if (filename == NULL) {
        fprintf(stderr, "Filename is required");
        return CONFIG_ERROR;
    }
    if (config == NULL) {
        fprintf(stderr, "ServerConfig reference is required");
        return CONFIG_ERROR;
    }

    String delimiterPosition;
    char line[MAX_LINE_LENGTH];

    FILE * file = fopen(filename, "r");

    if (file == NULL) {
        fprintf(stderr, "File couldn't be opened. Filename %s", filename);
    }

    while (fscanf(file, "%[^\n]", line)) {
        delimiterPosition = strstr(line, DELIM);

        if (delimiterPosition == NULL) {
            return CONFIG_ERROR;
        }

        if (!strncmp(CONFIG_ROOT_PATH, line, CONFIG_ROOT_PATH_LENGTH)) {
            strcpy(config->root, delimiterPosition + 1);
        }

        if (!strncmp(CONFIG_PORT, line, CONFIG_PORT_LENGTH)) {
            config->port = atoi(delimiterPosition + 1);
        }

        if (!strncmp(CONFIG_THREAD_MAX, line, CONFIG_THREAD_MAX_LENGTH)) {
            config->threadMax = atoi(delimiterPosition + 1);
        }
    }

    if (config->port < 1) {
        config->port = SERVER_PORT;
    }

    if (config->threadMax < 1) {
        config->threadMax = THREAD_MAX_DEFAULT;
    }

    return CONFIG_SUCCESS;
}