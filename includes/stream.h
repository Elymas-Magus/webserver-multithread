#ifndef STREAM_H_INCLUDED
#define STREAM_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>

#include "stream_def.h"
#include "type_aliases.h"
#include "validation.h"

#define MAX_STREAM_PATH 255
#define MAX_STREAM_MODE 5
#define MAX_STREAM_FILE_LENGTH      8192

#define STREAM_ERROR                -1
#define STREAM_SUCCESS              1

/**
 * Create a stream object and init them 
 * With default config
 * @returns Stream *
*/
Stream * initStream();

/**
 * Get file content
 * @param Stream * stream
 * @param size_t size
 * @returns Buffer *
*/
Buffer * getContent(Stream * stream, size_t size);

/**
 * Close a stream object
 * Returns STREAM_ERROR in error case
 * OR STREAM_SUCCESS otherwise
 * @param Stream * stream
 * @returns int
*/
int streamClose(Stream * stream);

/**
 * Open a stream object
 * Returns STREAM_ERROR in error case
 * OR STREAM_SUCCESS otherwise
 * @param Stream * stream
 * @returns int
*/
int streamOpen(Stream * stream);

/**
 * Receive a filename and get file size
 * @param String filename
 * @returns size_t
*/
size_t getFileSize(String filename);

/**
 * Receive a filename, get file size
 * And converts to string
 * @param String filename
 * @returns String
*/
String getStringFileSize(String filename);

#endif // STREAM_H_INCLUDED
