#include "stream.h"


Stream *
initStream()
{
    Stream * stream = (Stream *) malloc(sizeof(Stream));

    stream->file = STREAM_ERROR;
    stream->path = (String) malloc(MAX_STREAM_PATH);
    stream->open = streamOpen;
    stream->close = streamClose;
    stream->get = getContent;
    stream->mode = O_RDONLY; 
    stream->opened = false; 
    stream->ended = false;

    return stream;
}

int
streamOpen(Stream * stream)
{
    if (stream->path == NULL) {
        WARNING("Stream path cannot be NULL\n");
        return STREAM_ERROR;
    }

    stream->file = open(stream->path, stream->mode);

    if (stream->file == STREAM_ERROR) {
        WARNING("File couldn't be open!\n");
        return STREAM_ERROR;
    }

    stream->opened = true;
    stream->ended = false;

    return STREAM_SUCCESS;
}

int
streamClose(Stream * stream)
{
    if (stream->file == STREAM_ERROR) {
        WARNING("Not opened file!\n");
        return STREAM_ERROR;
    }

    stream->opened = false;
    stream->ended = false;

    if (close(stream->file) == STREAM_ERROR) {
        WARNING("File couldn't be closed!\n");
        return STREAM_ERROR;
    }

    return STREAM_SUCCESS;
}

Buffer *
getContent(Stream * stream, size_t bufferSize)
{
    if (stream->file == STREAM_ERROR && stream->path == NULL) {
        WARNING("Empty stream!\n");
        return NULL;
    }

    size_t bytesRead;
    Buffer * buffer = (Buffer *) malloc(sizeof(Buffer));

    buffer->content = (String) malloc(bufferSize);
    buffer->size = bufferSize;

    if (stream->file != STREAM_ERROR) {
        stream->file = open(stream->path, O_RDONLY);
    }

    if (stream->file == STREAM_ERROR) {
        WARNING("File couldn't be opened. Filename %s\n", stream->path);
        return NULL;
    }
    
    stream->opened = true;
    stream->ended = false;

    bytesRead = read(stream->file, buffer->content, bufferSize - 1);
    buffer->content[buffer->size - 1] = 0;

    buffer->size = bytesRead;

    if (bytesRead < bufferSize) {
        stream->ended = true;
    }

    return buffer;
}

size_t
getFileSize(String filename)
{
    size_t size = 0;
    FILE * file = fopen(filename, "r");

    if (file == NULL) {
        WARNING("File not found!");
        return size;
    }
    
    fseek(file, 0, SEEK_END);
    size = ftell(file);

    return size;
}

String
getStringFileSize(String filename)
{
    String buffer = (String) malloc(MAX_STREAM_FILE_LENGTH);
    sprintf(buffer, "%lu", getFileSize(filename));

    return buffer;
}