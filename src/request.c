#include "request.h"

const char HTTP_VERSIONS[][MAX_HTTP_VERSION_NAME] = {
    "HTTP/0.9",
    "HTTP/1",
    "HTTP/1.1",
    "HTTP/2",
    "HTTP/3",
};

const HttpResponseCode httpResponseCode[] = {
    {0, "200", "OK"},
    {1, "304", "NOT MODIFIED"},
    {2, "400", "BAD REQUEST"},
    {3, "401", "UNAUTHORIZED"},
    {4, "403", "FORBIDDEN"},
    {5, "404", "NOT FOUND"},
    {6, "405", "METHOD NOT ALLOWED"},
    {7, "500", "INTERNAL SERVER ERROR"},
    {8, "501", "NOT IMPLEMENTED"},
    {9, "502", "BAD GATEWAY"},
    {10, "504", "GATEWAY TIMEOUT"},
    {11, "505", "HTTP VERSION NOT SUPPORTED"},
};

bool
extractRequest(HttpRequest * request, String httpMessage, String root, int threadId)
{
    if (httpMessage == NULL || request == NULL || request->headers == NULL) {
        return false;
    }
    
    int i;

    HttpHeaders * header = (HttpHeaders *) malloc(sizeof(HttpHeaders));
    String startLine = (String) malloc(MAX_HTTP_MESSAGE_LINE);
    String headers = (String) malloc(MAX_HTTP_HEADER_SIZE);

    if (header == NULL || startLine == NULL || headers == NULL) {
        return false;
    }

    String body;
    String line;

    for (i = 0; httpMessage[i] != '\n'; i++) {
        startLine[i] = httpMessage[i];
    }
    startLine[i] = 0;

    sscanf(startLine, "%s %s %s", request->method.name, request->path, request->httpVersion);
    parseUri(root, request);

    httpMessage = strlen(startLine) + httpMessage;
    body = strstr(httpMessage, DIVISOR);

    if (body == NULL) {  
        strncpy(headers, httpMessage, strlen(httpMessage) - 1);
        memset(request->body, 0, sizeof(request->body));
    } else {
        strncpy(headers, httpMessage, strlen(httpMessage) - strlen(body) - 1);
        strcpy(request->body, body + 2);
    }

    line = strtok(headers, BREAKLINE);
    sscanf(line, HEADER_LINE_MODEL, header->key, header->value);

    if (! initHeader(request, header)) {
        WARNING("Error inserting header to the HttpListHeaders\n");
    }

    for (line = strtok(NULL, BREAKLINE); line && line[0]; line = strtok(NULL, BREAKLINE)) {
        sscanf(line, HEADER_LINE_MODEL, header->key, header->value);
        if (! insertHeader(request, header)) {
            WARNING("Error inserting header to the HttpListHeaders\n");
        }
    }

    if (! strcmp(request->method.name, HTTP_POST)) {
        strcat(request->query, request->body);
    }

    return true;
}

HttpRequest *
newRequest()
{
    HttpRequest * request = (HttpRequest *) malloc(sizeof(HttpRequest));

    if (request == NULL) {
        return NULL;
    }

    request->headers = newRequestHeaders();
    memset(request->body, 0, sizeof(request->body));
    memset(request->extension, 0, sizeof(request->extension));
    memset(request->path, 0, sizeof(request->path));
    memset(request->mimeType, 0, sizeof(request->mimeType));
    memset(request->query, 0, sizeof(request->query));
    memset(request->httpVersion, 0, sizeof(request->httpVersion));

    return request;
}

HttpListHeaders
newRequestHeaders()
{
    return (HttpListHeaders) arrayInit();
}

bool
isEndOfHttpHeaders(String httpMessage, int index)
{
    return httpMessage[index] == '\n' && httpMessage[index + 1] == '\n';
}

bool
setHeader(HttpRequest * request, String key, String value)
{
    if (request == NULL) {
        return false;
    }

    request->headers = arrayInit();
    return addHeader(request, key, value);
}

bool
addHeader(HttpRequest * request, String key, String value)
{
    if (request == NULL || request->headers == NULL) {
        return false;
    }

    HttpHeaders * header = (HttpHeaders *) mallocOrDie(sizeof(HttpHeaders), "new Header");
    strcpy(header->key, key);
    strcpy(header->value, value);
    arrayPush(request->headers, (void **) header, sizeof(HttpHeaders));

    return true;
}

bool
insertHeader(HttpRequest * request, HttpHeaders * header)
{
    if (request == NULL || request->headers == NULL || header == NULL) {
        return false;
    }

    arrayPush(request->headers, (void **) header, sizeof(HttpHeaders));

    return true;
}

bool
initHeader(HttpRequest * request, HttpHeaders * header)
{
    if (request == NULL || request->headers == NULL || header == NULL) {
        return false;
    }

    request->headers = arrayInit();
    arrayPush(request->headers, (void **) header, sizeof(HttpHeaders));

    return true;
}

void
setResponse(HttpRequest * request, HttpResponseCode httpResponseCode)
{
    request->response = httpResponseCode;
}

void
sendResponse(HttpRequest * request, int responseIndex, SocketFD clientSocket, Stream * stream, int threadId)
{
    Buffer * buffer;

    Node * no;
    HttpHeaders * header;

    String httpLine = (String) malloc(MAX_HTTP_HEADER_LINE);
    HttpResponseCode response = httpResponseCode[responseIndex];

    if (response.index == HTTP_NOT_FOUND) {
        stream->file = open(ERROR_404_PAGE, O_RDONLY);
        if (stream->file == STREAM_ERROR) {
            response = httpResponseCode[HTTP_INTERNAL_SERVER_ERROR];
            LOG_ERROR("File couldn't be opened. Filename %s\n", ERROR_404_PAGE);
        }
    } else if (response.index == HTTP_OK) {
        stream->file = open(stream->path, O_RDONLY);
        if (stream->file == STREAM_ERROR) {
            response = httpResponseCode[HTTP_INTERNAL_SERVER_ERROR];
            LOG_ERROR("File couldn't be opened. Filename %s\n", stream->path);
        }
    }

    if (response.index == HTTP_INTERNAL_SERVER_ERROR) {
        stream->file = open(ERROR_500_PAGE, O_RDONLY);
        if (stream->file == STREAM_ERROR) {
            LOG_ERROR("File couldn't be opened. Filename %s\n", ERROR_500_PAGE);
        }
    }

    sprintf(httpLine, "%s %s %s\r\n", request->httpVersion, response.code, response.state);
    write(clientSocket, httpLine, strlen(httpLine));
    
    int i;
    for (no = request->headers->first, i = 0; no; no = no->next, i++) {
        header = (HttpHeaders *) no->content;
        sprintf(httpLine, "%s: %s\r\n", header->key, header->value);
        if (write(clientSocket, httpLine, strlen(httpLine)) < 0) {
            fprintf(stderr, "[SR:%d] Send Error", threadId);
            break;
        }
    }

    if (write(clientSocket, "\r\n", 2) < 0) {
        fprintf(stderr, "[SR:%d] Send Error\n", threadId);
    }

    if (stream->file != STREAM_ERROR) {
        buffer = (Buffer *) allocate(sizeof(Buffer), "OBuffer");
        buffer->content = (String) allocate(MAX_HTTP_BUFFER, "OBuffer content");
        
        while ((buffer->size = read(stream->file, buffer->content, MAX_HTTP_BUFFER)) > 0) {
            if (write(clientSocket, buffer->content, buffer->size) < 0) {
                fprintf(stderr, "[SR:%d] Send Error", threadId);
                break;
            }
            if (buffer->size < MAX_HTTP_BUFFER) {
                break;
            }
        }
        close(stream->file);
        free(stream);
    }

    close(clientSocket);
}

bool
keepAlive(HttpRequest * request)
{
    Node * no;
    HttpHeaders * header;
    FOREACH (no, request->headers) {
        header = (HttpHeaders *) no->content;
        if (strcmp(header->key, "Connection") == 0) {
            return strcmp(header->value, "keep-alive") == 0;
        }
    }

    return false;
}

void
requestFree(HttpRequest * request)
{
    arrayFree(request->headers);
    free(request);
}

bool
getTypeFromMimeType(String mimeType)
{
    int typeLength;
    String type = (String) malloc(MAX_MIME_TYPE_NAME_LEN);
    String slashPos = strchr(mimeType, '/');

    if (type == NULL) {
        errno = EMFILE;
        return false;
    }

    typeLength = (slashPos != NULL) ?
        ((int)(slashPos - mimeType)) :
        strlen(mimeType);

    memset(type, 0, MAX_MIME_TYPE_NAME_LEN);
    strncpy(type, mimeType, typeLength);

    return type;
}

bool
isTextFile(String mimeType)
{
    int typeLength;
    String slashPos = strchr(mimeType, '/');

    typeLength = (slashPos != NULL) ?
        ((int)(slashPos - mimeType)) :
        strlen(mimeType);

    return strncmp(mimeType, "text", typeLength) == 0;
}

bool
isImageFile(String mimeType)
{
    int typeLength;
    String slashPos = strchr(mimeType, '/');

    typeLength = (slashPos != NULL) ?
        ((int)(slashPos - mimeType)) :
        strlen(mimeType);

    return strncmp(mimeType, "image", typeLength) == 0;
}

bool
isAudioFile(String mimeType)
{
    int typeLength;
    String slashPos = strchr(mimeType, '/');

    typeLength = (slashPos != NULL) ?
        ((int)(slashPos - mimeType)) :
        strlen(mimeType);

    return strncmp(mimeType, "audio", typeLength) == 0;
}

bool
isVideoFile(String mimeType)
{
    int typeLength;
    String slashPos = strchr(mimeType, '/');

    typeLength = (slashPos != NULL) ?
        ((int)(slashPos - mimeType)) :
        strlen(mimeType);

    return strncmp(mimeType, "video", typeLength) == 0;
}

bool
isBinaryFile(String mimeType)
{
    int typeLength;
    String slashPos = strchr(mimeType, '/');

    typeLength = (slashPos != NULL) ?
        ((int)(slashPos - mimeType)) :
        strlen(mimeType);

    return strncmp(mimeType, "application", typeLength) == 0;
}

String
getMimeTypeFormatted(String mimeType)
{
    if (strcmp(mimeType, "text/html") == 0) {
        strcat(mimeType, "; charset=UTF-8");
    }

    return mimeType;
}