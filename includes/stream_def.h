#ifndef STREAM_DEF_H_INCLUDED
#define STREAM_DEF_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <unistd.h>

#include "type_aliases.h"

#define MAX_STREAM_PATH 255
#define MAX_STREAM_MODE 5
#define MAX_STREAM_FILE_LENGTH      8192

#define STREAM_ERROR                -1
#define STREAM_SUCCESS              1

typedef int SocketFD;
typedef int FileDescriptor;

typedef struct buffer {
    String content;
    size_t size;
} Buffer;

typedef int FileDescriptor;

typedef struct stream {
    FileDescriptor file;
    String path;
    int mode;
    bool opened; 
    bool ended; 
    int (* open)(struct stream *);
    int (* close)(struct stream *);
    Buffer * (* get)(struct stream *, size_t);
} Stream;

#endif // STREAM_DEF_H_INCLUDED
