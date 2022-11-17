#ifndef REQUEST_H_INCLUDED
#define REQUEST_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

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

/**
 * Converts a HttpRequest object into a String
 * Mount the Http message for send to the client 
 * @param HttpRequest * request
 * @return String filename
 */
String stringifyRequest(HttpRequest * request);

/**
 * Converts a String into a HttpRequest object
 * Mount the object with the Http message from client
 * @param HttpRequest * request
 * @return String filename
 */
HttpRequest * extractRequest(String httpMessage);

/**
 * Create new HttpHeader list
 * @param HttpRequest * request
 * @return String filename
 */
HttpListHeaders newRequestHeaders();

/**
 * Converts a String into a HttpRequest object 
 * @param HttpRequest * request
 * @return bool status
 */
bool setHeader(HttpRequest * request, String key, String value);

/**
 * Converts a String into a HttpRequest object 
 * @param HttpRequest * request
 * @return bool status
 */
bool addHeader(HttpRequest * request, String key, String value);

/**
 * Converts a String into a HttpRequest object 
 * @param HttpRequest * request
 * @return bool status
 */
bool isEndOfHttpHeaders(String httpMessage, int index);

/**
 * Add headers to the request
 * @param HttpRequest * request
 * @return bool status
 */
bool insertHeader(HttpRequest * request, HttpHeaders * header);

/**
 * Init request headers and
 * Add headers to the request 
 * @param HttpRequest * request
 * @return bool status
 */
bool initHeader(HttpRequest * request, HttpHeaders * header);

void sendResponse(HttpRequest * request, int httpResponseIndex, int clientSocket);

#endif // REQUEST_H_INCLUDED
