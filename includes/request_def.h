#ifndef REQUEST_DEF_H_INCLUDED
#define REQUEST_DEF_H_INCLUDED

#include "list.h"
#include "type_aliases.h"

#define MAX_HTTP_MESSAGE_LINE            255
#define MAX_ROOT_PATH_LEN                100
#define MAX_METHOD_NAME_LEN              10
#define MAX_HTTP_VERSION_NAME            10

#define MAX_HEADER_KEY_LEN               255
#define MAX_HEADER_VALUE_LEN             255

#define MAX_HTTP_HEADER_SIZE             40960
#define MAX_HTTP_BODY_SIZE               81920
#define MAX_HTTP_MESSAGE_LENGTH          122880

#define HTTP_UNKNOWN                     0x0001
#define HTTP_GET                         0x0002
#define HTTP_HEAD                        0x0004
#define HTTP_POST                        0x0008

#define HTTP_NOT_MODIFIED                1
#define HTTP_OK                          0
#define HTTP_BAD_REQUEST                 2
#define HTTP_UNAUTHORIZED                3
#define HTTP_FORBIDDEN                   4
#define HTTP_NOT_FOUND                   5
#define HTTP_METHOD_NOT_ALLOWED          6
#define HTTP_INTERNAL SERVER ERROR       7
#define HTTP_NOT_IMPLEMENTED             8
#define HTTP_BAD_GATEWAY                 9
#define HTTP_GATEWAY_TIMEOUT             10
#define HTTP_HTTP_VERSION_NOT_SUPPORTED  11

#define HTTP_VERSION_0s9                 0
#define HTTP_VERSION_1                   0
#define HTTP_VERSION_1s1                 0
#define HTTP_VERSION_2                   0
#define HTTP_VERSION_3                   0

#define BREAKLINE                        "\n"
#define DIVISOR                          "\n\n"
#define HEADER_LINE_MODEL                "%[^:]: %[^\n] "

typedef ArrayList HttpListHeaders;

typedef struct httpMethod {
    int index;
    char name[MAX_METHOD_NAME_LEN];
} HttpMethod;

typedef struct httpHeader {
    char key[MAX_HEADER_KEY_LEN];
    char value[MAX_HEADER_VALUE_LEN];
} HttpHeaders;

typedef struct httpResponseCode {
    char code[MAX_HEADER_KEY_LEN];
    char state[MAX_HEADER_VALUE_LEN];
} HttpResponseCode;

typedef struct httpRequest {
    char path[MAX_ROOT_PATH_LEN];
    char httpVersion[MAX_HTTP_VERSION_NAME];

    char body[MAX_HTTP_BODY_SIZE];
    void * timer;

    HttpMethod method;
    HttpResponseCode response;
    HttpListHeaders headers;
} HttpRequest;

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
    {"Access-Control-Allow-Origin", "*"},
    {"Content-Type", "text/html; charset=utf-8"},
    {"Content-Length", ""},
    {"Keep-Alive", "timeout, max=999"},
    {"Last-Modified", ""},
    {"Server", ""},
    {"Set-Cookie", "csrftoken="},
    {"Date", ""},
    {"X-Cache-Info", "caching"},
};

#endif // REQUEST_DEF_H_INCLUDED
