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
#include "request_def.h"

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
