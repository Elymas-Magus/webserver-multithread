#include "request.h"

const char HTTP_VERSIONS[][MAX_HTTP_VERSION_NAME] = {
    "HTTP/0.9",
    "HTTP/1",
    "HTTP/1.1",
    "HTTP/2",
    "HTTP/3",
};

const HttpResponseCode httpResponseCode[] = {
    {"200", "OK"},
    {"304", "NOT MODIFIED"},
    {"400", "BAD REQUEST"},
    {"401", "UNAUTHORIZED"},
    {"403", "FORBIDDEN"},
    {"404", "NOT FOUND"},
    {"405", "METHOD NOT ALLOWED"},
    {"500", "INTERNAL SERVER ERROR"},
    {"501", "NOT IMPLEMENTED"},
    {"502", "BAD GATEWAY"},
    {"504", "GATEWAY TIMEOUT"},
    {"505", "HTTP VERSION NOT SUPPORTED"},
};

bool
extractRequest(HttpRequest * request, String httpMessage, String root)
{
    if (httpMessage == NULL || request == NULL) {
        return false;
    }
    
    int i;
    HttpHeaders * header = (HttpHeaders *) malloc(sizeof(HttpHeaders));
    String startLine = (String) malloc(MAX_HTTP_MESSAGE_LINE);
    String headers = (String) malloc(MAX_HTTP_HEADER_SIZE);
    String body;
    String line;

    request->headers = newRequestHeaders();
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

    if (initHeader(request, header) == false) {
        WARNING("Error inserting header to the HttpListHeaders\n");
    }

    for (line = strtok(NULL, BREAKLINE); line && line[0]; line = strtok(NULL, BREAKLINE)) {
        sscanf(line, HEADER_LINE_MODEL, header->key, header->value);
        if (insertHeader(request, header) == false) {
            WARNING("Error inserting header to the HttpListHeaders\n");
        }
    }

    return true;
}

HttpRequest *
newRequest()
{
    HttpRequest * request = (HttpRequest *) malloc(sizeof(HttpRequest));

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

    HttpHeaders * header = (HttpHeaders *) malloc(sizeof(HttpHeaders));
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
sendResponse(HttpRequest * request, int httpResponseIndex, int clientSocket, Stream * stream)
{
    if (stream->file != NULL) {
        sendTextResponse(request, httpResponseIndex, clientSocket, stream->file);
    }
    if (stream->imageFile != STREAM_ERROR) {
        sendImageResponse(request, httpResponseIndex, clientSocket, stream->imageFile);
    }
}

void
sendImageResponse(HttpRequest * request, int httpResponseIndex, int clientSocket, IMAGE file)
{
    String buffer;
    HttpResponseCode response = httpResponseCode[httpResponseIndex];

    setResponse(request, response);
    if (httpResponseIndex == HTTP_OK) {        
        sendfile(clientSocket, file, NULL, MAX_HTTP_BUFFER);
    } else {
        buffer = (String) malloc(MIN_HTTP_BUFFER);
        
        printf("Sending 0 bytesRead\n");

        memset(buffer, 0, MIN_HTTP_BUFFER);
        sendHttpResponse(request, clientSocket, buffer, 0);
    }
    
    printf("Close\n");
    close(clientSocket);
}

void
sendTextResponse(HttpRequest * request, int httpResponseIndex, int clientSocket, FILE * file)
{
    size_t bytesRead;
    String contentLength;
    String buffer;
    
    HttpResponseCode response = httpResponseCode[httpResponseIndex];

    setResponse(request, response);
    if (httpResponseIndex == HTTP_OK) {
        buffer = (String) malloc(MAX_HTTP_BUFFER);
        contentLength = (String) malloc(MAX_CONTENT_TYPE_LEN);
        
        if ((bytesRead = fread(buffer, 1, MAX_HTTP_BUFFER, file)) > 0) {
            printf("Sending %zu bytesRead\n", bytesRead);
            sprintf(contentLength, "%zu", bytesRead);
            addHeader(request, "Content-Length", contentLength);
            sendHttpResponse(request, clientSocket, buffer, bytesRead);
        }
        fclose(file);
    } else {
        buffer = (String) malloc(MIN_HTTP_BUFFER);
        
        printf("Sending 0 bytesRead\n");

        memset(buffer, 0, MIN_HTTP_BUFFER);
        sendHttpResponse(request, clientSocket, buffer, 0);
    }
    
    printf("Close\n");
    close(clientSocket);
}

void
sendHttpResponse(HttpRequest * request, int clientSocket, String buffer, size_t bytesRead)
{
    Node * no;
    HttpHeaders * header;

    String httpLine = (String) malloc(MAX_HTTP_HEADER_LINE);
    HttpResponseCode response = request->response;

    sprintf(httpLine, "%s %s %s\r\n", request->httpVersion, response.code, response.state);
    printf("%s %s %s\n", request->httpVersion, response.code, response.state);
    write(clientSocket, httpLine, strlen(httpLine));
    
    FOREACH (no, request->headers) {
        header = (HttpHeaders *) no->content;
        sprintf(httpLine, "%s: %s\r\n", header->key, header->value);
        printf("%s: %s\n", header->key, header->value);
        write(clientSocket, httpLine, strlen(httpLine));
    }

    write(clientSocket, "\n", 1);
    write(clientSocket, buffer, bytesRead);
}

bool
printHeaders(HttpRequest * request)
{
    Node * no;
    HttpHeaders * header;
    FOREACH (no, request->headers) {
        header = (HttpHeaders *) no->content;
        printf("Header ->( %s )\n", header->value);
    }

    return false;
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