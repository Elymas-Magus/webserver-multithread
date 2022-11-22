#ifndef REQUEST_DEF_H_INCLUDED
#define REQUEST_DEF_H_INCLUDED

#include "type_aliases.h"
#include "list.h"

#define MAX_HTTP_MESSAGE_LINE            255
#define MAX_ROOT_PATH_LEN                100
#define MAX_EXTENSION_NAME_LEN           10
#define MAX_METHOD_NAME_LEN              10
#define MAX_HTTP_VERSION_NAME            10
#define MAX_MIME_TYPE_NAME_LEN           20
#define MAX_CONTENT_TYPE_LEN             50

#define MAX_HEADER_KEY_LEN               255
#define MAX_HEADER_VALUE_LEN             255

#define MAX_HTTP_HEADER_LINE             40960
#define MAX_HTTP_HEADER_SIZE             40960
#define MAX_HTTP_BODY_SIZE               40960
#define MAX_HTTP_MESSAGE_LENGTH          81920
#define MAX_HTTP_BUFFER                  4096

#define MIN_CONTENT_TYPE_LEN             2
#define MIN_HTTP_BUFFER                  2

#define HTTP_HEADER_CONTENT_TYPE         0

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
#define HTTP_INTERNAL_SERVER_ERROR       7
#define HTTP_NOT_IMPLEMENTED             8
#define HTTP_BAD_GATEWAY                 9
#define HTTP_GATEWAY_TIMEOUT             10
#define HTTP_HTTP_VERSION_NOT_SUPPORTED  11

#define HTTP_VERSION_0s9                 0
#define HTTP_VERSION_1                   1
#define HTTP_VERSION_1s1                 2
#define HTTP_VERSION_2                   3
#define HTTP_VERSION_3                   4

#define BREAKLINE                        "\n"
#define DIVISOR                          "\r\n\r\n"
#define HEADER_LINE_MODEL                "%[^:]: %[^\r\n]%*c"

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
    int index;
    char code[MAX_HEADER_KEY_LEN];
    char state[MAX_HEADER_VALUE_LEN];
} HttpResponseCode;

typedef struct httpRequest {
    char path[MAX_ROOT_PATH_LEN];
    char filename[MAX_ROOT_PATH_LEN];
    char extension[MAX_EXTENSION_NAME_LEN];
    char mimeType[MAX_MIME_TYPE_NAME_LEN];
    char query[MAX_ROOT_PATH_LEN];
    char httpVersion[MAX_HTTP_VERSION_NAME];

    char body[MAX_HTTP_MESSAGE_LENGTH];

    HttpMethod method;
    HttpResponseCode response;
    HttpListHeaders headers;
} HttpRequest;

#endif // REQUEST_DEF_H_INCLUDED