#ifndef REQUEST_H_INCLUDED
#define REQUEST_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#include "uri.h"
#include "request_def.h"
#include "type_aliases.h"

extern const char HTTP_VERSIONS[][MAX_HTTP_VERSION_NAME];
extern const HttpResponseCode httpResponseCode[];
extern const HttpHeaders httpHeaders[];

/**
 * Converts a String into a HttpRequest object
 * Mount the object with the Http message from client
 * @param HttpRequest * request
 * @returns String filename
 */
bool extractRequest(HttpRequest * request, String httpMessage, String root);

/**
 * Create new HttpHeader list
 * @param HttpRequest * request
 * @returns String filename
 */
HttpListHeaders newRequestHeaders();

/**
 * Converts a String into a HttpRequest object 
 * @param HttpRequest * request
 * @returns bool status
 */
bool setHeader(HttpRequest * request, String key, String value);

/**
 * Converts a String into a HttpRequest object 
 * @param HttpRequest * request
 * @returns bool status
 */
bool addHeader(HttpRequest * request, String key, String value);

/**
 * Converts a String into a HttpRequest object 
 * @param HttpRequest * request
 * @returns bool status
 */
bool isEndOfHttpHeaders(String httpMessage, int index);

/**
 * Add headers to the request
 * @param HttpRequest * request
 * @returns bool status
 */
bool insertHeader(HttpRequest * request, HttpHeaders * header);

/**
 * Init request headers and
 * Add headers to the request 
 * @param HttpRequest * request
 * @returns bool status
 */
bool initHeader(HttpRequest * request, HttpHeaders * header);

/**
 * Get request data and send the response to the client
 * @param HttpRequest * request
 * @param int httpResponseIndex
 * @param int clientSocket
 */
void sendResponse(HttpRequest * request, int httpResponseIndex, int clientSocket);

/**
 * Destroys request and it's attributes
 * @param HttpRequest * request
 */
void requestFree(HttpRequest * request);

#endif // REQUEST_H_INCLUDED
