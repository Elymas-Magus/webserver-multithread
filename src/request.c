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

const HttpHeaders httpHeaders[] = {
    {"Content-Type", "text/html; charset=utf-8"},
    {"Content-Length", ""},
    {"Keep-Alive", "timeout, max=999"},
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
        strcpy(request->body, "");
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

    HttpHeaders * header = (HttpHeaders *) malloc(sizeof(HttpHeaders));
    request->headers = arrayInit();
    strcpy(header->key, key);
    strcpy(header->value, value);
    arrayPush(request->headers, (void **) header, sizeof(HttpHeaders));

    return true;
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
sendResponse(HttpRequest * request, int httpResponseIndex, int clientSocket)
{
    HttpHeaders * header;
    String httpLine = (String) malloc(MAX_HTTP_HEADER_LINE);
    HttpResponseCode response = httpResponseCode[httpResponseIndex];

    setResponse(request, response);
    sprintf(httpLine, "%s %s %s\n", request->httpVersion, response.code, response.state);
    printf("%s %s %s\n", request->httpVersion, response.code, response.state);
    write(clientSocket, httpLine, strlen(httpLine));
    for (Node * no = request->headers->first; no; no = no->next) {
        header = (HttpHeaders *) no->content;
        sprintf(httpLine, "%s: %s\n", header->key, header->value);
        printf("%s: %s\n", header->key, header->value);
        write(clientSocket, httpLine, strlen(httpLine));
    }
    sprintf(httpLine, "\n%s\n", request->body);
    printf("\n%s\n", request->body);
    write(clientSocket, httpLine, strlen(httpLine));

    close(clientSocket);
}