#ifndef STREAM_H_INCLUDED
#define STREAM_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "type_aliases.h"

#define MAX_STREAM_PATH 255
#define MAX_STREAM_MODE 5
#define MAX_STREAM_FILE_LENGTH      8192

#define STREAM_ERROR                -1
#define STREAM_SUCCESS              1

typedef int IMAGE;

typedef struct stream {
    FILE * file;
    IMAGE imageFile;
    String path;
    String mode;
    int * (* open)(struct stream *);
    int (* close)(struct stream *);
    String (* getContent)(struct stream *);
    // 
} Stream;

Stream * initStream();

String getContent(Stream * stream);

int streamClose(Stream * stream);

int streamOpen(Stream * stream);

/**
 * Receive a filename, find the respective file
 * And return all content 
 * @param String filename
 * @returns string content
 */
String fileGetContent(String filename);

#endif // STREAM_H_INCLUDED
