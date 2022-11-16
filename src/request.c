#include "request.h"

String
stringifyRequest(HttpRequest * request)
{
    if (request == NULL) {
        return NULL;
    }

    // HttpHeaders * header;
    String httpMessage = (String) malloc(MAX_HTTP_MESSAGE_LENGTH);
    // HttpResponseCode response = request->response;
    // Node * no;

    // sprintf(httpMessage, "%s %s %s\n", request->httpVersion, response.state, response.code);
    // for (no = request->headers->first; no; no = no->next) {
    //     header = (HttpHeaders *) no->content;
    //     sprintf(httpMessage, "%s%s: %s\n", httpMessage, header->key, header->value);
    // }
    // sprintf(httpMessage, "%s\n%s\n", httpMessage, request->body);

    return httpMessage;
}

HttpRequest *
extractRequest(String httpMessage)
{
    if (httpMessage == NULL) {
        return NULL;
    }
    
    int i;
    HttpHeaders * header = (HttpHeaders *) malloc(sizeof(HttpHeaders));
    HttpRequest * request = (HttpRequest *) malloc(sizeof(HttpRequest));
    String startLine = (String) malloc(MAX_HTTP_MESSAGE_LINE);
    String headers = (String) malloc(MAX_HTTP_HEADER_SIZE);
    String body = (String) malloc(MAX_HTTP_BODY_SIZE);
    String line;

    request->headers = newRequestHeaders();
    for (i = 0; httpMessage[i] || httpMessage[i] != '\n'; i++) {
        startLine[i] = httpMessage[i];
    }
    startLine[i] = 0;
    sscanf(startLine, "%s %s %s", request->method.name, request->path, request->httpVersion);

    body = strstr(httpMessage, DIVISOR);
    strncpy(headers, httpMessage, strlen(httpMessage) - strlen(body));
    strcpy(request->body, body + 2);

    line = strtok(headers, BREAKLINE);

    sscanf(line, HEADER_LINE_MODEL, header->key, header->value);
    initHeader(request, header);

    for (line = strtok(NULL, BREAKLINE); line; line = strtok(NULL, BREAKLINE)) {
        header = (HttpHeaders *) malloc(sizeof(HttpHeaders));

        sscanf(line, HEADER_LINE_MODEL, header->key, header->value);
        insertHeader(request, header);        
    }

    return request;
}

HttpListHeaders
newRequestHeaders()
{
    return (HttpListHeaders) malloc(sizeof(List));
}

bool
isEndOfHttpHeaders(String httpMessage, int index)
{
    return httpMessage[index] == '\n' && httpMessage[index + 1] == '\n';
}

bool
addHeader(HttpRequest * request, String key, String value)
{
    if (request == NULL || request->headers == NULL) {
        return false;
    }

    HttpHeaders * header = (HttpHeaders *) malloc(sizeof(HttpHeaders));
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

bool
setHeader(HttpRequest * request, String key, String value)
{
    if (request == NULL || request->headers == NULL) {
        return false;
    }

    HttpHeaders * header = (HttpHeaders *) malloc(sizeof(HttpHeaders));
    request->headers = arrayInit();
    arrayPush(request->headers, (void **) header, sizeof(HttpHeaders));

    return true;
}

void
sendResponse(HttpRequest * request, int httpResponseIndex, int clientSocket)
{
    String httpMessage = stringifyRequest(request);

    request->response = httpResponseCode[httpResponseIndex];
    write(clientSocket, httpMessage, strlen(httpMessage));

    close(clientSocket);
}