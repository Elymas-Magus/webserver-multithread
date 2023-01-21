#ifndef REQUEST_H_INCLUDED
#define REQUEST_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/sendfile.h>

#include "uri.h"
#include "request_def.h"
#include "type_aliases.h"
#include "stream.h"
#include "logs.h"
#include "validation.h"

#define ERROR_404_PAGE "./pages/not_found.html"
#define ERROR_500_PAGE "./pages/internal_error.html"

extern const char HTTP_VERSIONS[][MAX_HTTP_VERSION_NAME];
extern const HttpResponseCode httpResponseCode[];

extern MimeType mimeTypes[];

/**
 * Converts a String into a HttpRequest object
 * Mount the object with the Http message from client
 * @param HttpRequest * request
 * @returns String filename
 */
bool extractRequest(HttpRequest * request, String httpMessage, String root, int threadId);

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
void sendResponse(HttpRequest * request, int httpResponseIndex, int clientSocket, Stream * stream, int threadId);

/**
 * Create new request and set default attributes
 * @returns HttpRequest *
 */
HttpRequest * newRequest();

/**
 * Print all request headers
 * @param HttpRequest *
 */
bool printHeaders(HttpRequest * request);

/**
 * Check if request needs to keep alive
 * @param HttpRequest *
 */
bool keepAlive(HttpRequest * request);

/**
 * Destroys request and it's attributes
 * @param HttpRequest * request
 */
void requestFree(HttpRequest * request);

/**
 * Get the type from file's mime type
 * @param String mimeType
 * @returns bool
*/
bool getTypeFromMimeType(String mimeType);

/**
 * Checks if the mime type corresponds to a text file
 * @param String mimeType
 * @returns bool
*/
bool isTextFile(String mimeType);

/**
 * Checks if the mime type corresponds to a image file
 * @param String mimeType
 * @returns bool
*/
bool isImageFile(String mimeType);

/**
 * Checks if the mime type corresponds to a audio file
 * @param String mimeType
 * @returns bool
*/
bool isAudioFile(String mimeType);

/**
 * Checks if the mime type corresponds to a video file
 * @param String mimeType
 * @returns bool
*/
bool isVideoFile(String mimeType);

/**
 * Checks if the mime type corresponds to a binary file
 * @param String mimeType
 * @returns bool
*/
bool isBinaryFile(String mimeType);

/**
 * Returns formatted mime type
 * If is html file, adds charset
 * @param String mimeType
 * @returns String
*/
String getMimeTypeFormatted(String mimeType);

#endif // REQUEST_H_INCLUDED
