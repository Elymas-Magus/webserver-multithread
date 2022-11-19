#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "type_aliases.h"

#define CONFIG_SUCCESS              0
#define CONFIG_ERROR                -1

#define MAX_CONFIG_PATH_LENGTH      128
#define MAX_CONFIG_FILE_LENGTH      8192
#define MAX_CONFIG_KEY_LENGTH       50
#define MAX_CONFIG_VALUE_LENGTH     255
#define CONFIG_LINE_MODEL           "%[^=]=%[^;]; "

#define CONFIG_ROOT_PATH            "root"
#define CONFIG_PORT                 "port"
#define CONFIG_THREAD_MAX           "threadMax"

#define CONFIG_ROOT_PATH_LENGTH     4
#define CONFIG_PORT_LENGTH          4
#define CONFIG_THREAD_MAX_LENGTH    9

#define SERVER_PORT                 8090
#define THREAD_MAX_DEFAULT          4

typedef struct serverConfig {
    char root[MAX_CONFIG_PATH_LENGTH];
    u_int port;
    u_int threadMax;
} ServerConfig;

/**
 * Receive a filename, find the respective file
 * And return all content 
 * @param String filename
 * @returns string content
 */
String fileGetContent(String filename);

/**
 * Receive a start position and end position
 * And get a subtring from a string
 * @param String str
 * @param int start
 * @param int end
 * @param String buffer 
 */
void substring(String str, u_int start, u_int end, String buffer);

/**
 * Read an Server config file
 * And populate a ServerConfig object
 * @param String filename
 * @param ServerConfig * config
 * @returns int status
 */
int readConfigFile(String filename, ServerConfig * config);

/**
 * Receive a filename and instance a serverConfig
 * If initiantion fail, Die
 * @param String filename
 * @returns ServerConfig config
 */
ServerConfig * getServerConfigFromConfigFile(String filename);

#endif // UTILS_H_INCLUDED
