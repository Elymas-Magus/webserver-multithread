#ifndef REQUEST_H_INCLUDED
#define REQUEST_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
// #include "util.h"
// #include "list.h"

#define AGAIN EAGAIN

#define HTTP_PARSE_INVALID_METHOD        10
#define HTTP_PARSE_INVALID_REQUEST       11
#define HTTP_PARSE_INVALID_HEADER        12

#define HTTP_UNKNOWN                     0x0001
#define HTTP_GET                         0x0002
#define HTTP_HEAD                        0x0004
#define HTTP_POST                        0x0008

#define HTTP_OK                          200
#define HTTP_NOT_MODIFIED                304
#define HTTP_NOT_FOUND                   404
#define MAX_BUF 8124

typedef struct request {
    char * root;
    int fd;
    int epoll_fd;
    char buff[MAX_BUF];
    size_t pos;
    size_t last;
    int state;
    int method;
    int http_major;
    int http_minor;

    void * request_start;
    void * method_end;
    void * uri_start;
    void * uri_end;
    void * path_start;
    void * path_end;
    void * query_start;
    void * query_end;
    void * request_end;

    struct list_head list;
    void * cur_header_key_start;
    void * cur_header_key_end;
    void * cur_header_value_start;
    void * cur_header_value_end;
    void * timer;
} request_t;

#endif // REQUEST_H_INCLUDED
