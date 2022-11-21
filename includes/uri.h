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

extern MimeType mimeTypes[];

/**
 * Resolve uri to make filename, path and query
 * @param String root
 * @param HttpRequest * request
 */
void parseUri(String root, HttpRequest * request);

/**
 * Get mime type from file extension
 * @param const String type
 */
const String getFileType(const String type);

/**
 * Get file extension from filename
 * @param const String filename
 */
const String getFileExtension(const String filename);

#endif // URI_H