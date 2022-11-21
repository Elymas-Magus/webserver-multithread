#include "stream.h"


Stream *
initStream()
{
    Stream * stream = (Stream *) malloc(sizeof(Stream));

    stream->file = NULL;
    stream->path = (String) malloc(MAX_STREAM_PATH);
    stream->mode = (String) malloc(MAX_STREAM_MODE);
    stream->open = streamOpen;
    stream->close = streamClose;
    stream->getContent = getContent;
    stream->imageFile = STREAM_ERROR;

    strcpy(stream->mode, "r");    
}

int
streamOpen(Stream * stream)
{
    if (stream->path == NULL) {
        WARNING("Stream path cannot be NULL");
        return STREAM_ERROR;
    }

    stream->file = fopen(stream->path, stream->mode);

    if (stream->file == NULL) {
        WARNING("File couldn't be open!");
        return STREAM_ERROR;
    }

    return STREAM_SUCCESS;
}

int
streamClose(Stream * stream)
{
    if (stream->file == NULL) {
        WARNING("Not opened file!");
        return STREAM_ERROR;
    }

    if (fclose(stream->file) != 0) {
        WARNING("File couldn't be closed!");
        return STREAM_ERROR;
    }

    return STREAM_SUCCESS;
}

String
getContent(Stream * stream)
{
    if (stream->file == NULL && stream->path == NULL) {
        WARNING("Empty stream!");
        return NULL;
    }

    int i = 0;
    char character;
    String content = (String) malloc(MAX_STREAM_FILE_LENGTH);

    if (stream->file != NULL) {
        stream->file = fopen(stream->path, "r");
    }

    if (stream->file == NULL) {
        WARNING("File couldn't be opened. Filename %s\n", stream->path);
        return NULL;
    }

    for (i = 0; (character = fgetc(stream->file)) != EOF; i++) {
        content[i] = character;        
    }
    content[i] = 0;

    return content;
}

String
fileGetContent(String filename)
{
    if (filename == NULL) {
        WARNING("Filename is required\n");
        return NULL;
    }

    int i;
    char character;
    String content = (String) malloc(MAX_STREAM_FILE_LENGTH);

    Stream * stream = initStream();

    strcpy(stream->path, filename);
    
    return getContent(stream);
}