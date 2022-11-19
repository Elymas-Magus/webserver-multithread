#ifndef URI_H
#define URI_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#include "type_aliases.h"
#include "server_def.h"
#include "request_def.h"

#define DOT                 '.'
#define INTERROGATION       '?'
#define SLASH_CHAR          '/'
#define SLASH               "/"
#define DEFAULT_FILENAME    "index.html"
#define DEFAULT_FILETYPE    "text/plain"

typedef struct mimeType{
    const String type;
    const String value;
} MimeType;

extern MimeType tkeed_mime[];

/**
 */
void parseUri(String root, HttpRequest * request);

/**
 */
const String getFileType(const String type);

/**
 */
const String getFileExtension(const String filename);

#endif // URI_H