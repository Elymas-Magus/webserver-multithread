#include "uri.h"

MimeType mimeTypes[] = {
    {".html", "text/html"},
    {".xml", "text/xml"},
    {".xhtml", "application/xhtml+xml"},
    {".txt", "text/plain"},
    {".rtf", "application/rtf"},
    {".pdf", "application/pdf"},
    {".word", "application/msword"},
    {".png", "image/png"},
    {".gif", "image/gif"},
    {".jpg", "image/jpeg"},
    {".jpeg", "image/jpeg"},
    {".au", "audio/basic"},
    {".mpeg", "video/mpeg"},
    {".mpg", "video/mpeg"},
    {".avi", "video/x-msvideo"},
    {".gz", "application/x-gzip"},
    {".tar", "application/x-tar"},
    {".css", "text/css"},
    {NULL, "text/plain"}
};

void
parseUri(String root, HttpRequest * request)
{
    int filenameLength;
    char filename[strlen(request->path)];
    String queryPos, lastSlash, lastDot;
    String path = (String) mallocOrDie(
        MAX_SERVER_PATH_LENGTH, "path (parseUri)"
    );
    
    strcpy(path, request->path); 
    queryPos = strchr(path, INTERROGATION);
    filenameLength = (queryPos != NULL)
        ? ((int)(queryPos - path))
        : strlen(path);

    memset(request->filename, 0, sizeof(request->filename));
    strncpy(request->filename, request->path, filenameLength); 
    strcpy(request->path, root);    
    strcpy(filename, request->filename);
    strcat(request->path, filename);

    lastSlash = strrchr(request->path, SLASH_CHAR);    
    lastDot = strrchr(lastSlash, DOT);
    
    memset(request->filename, 0, sizeof(request->filename));
    strcpy(request->filename, filename);

    if ((lastDot == NULL) && (request->path[strlen(request->path) - 1] != SLASH_CHAR)){
        strcat(request->path, SLASH);
    }
    if (queryPos != NULL) {
        strcpy(request->query, (queryPos + 1));
    } else {
        strcpy(request->query, "");
    }

    if (request->path[strlen(request->path) - 1] == SLASH_CHAR) {
        strcat(request->path, DEFAULT_FILENAME);
        strcat(request->filename, DEFAULT_FILENAME);
    }

    printf("Extension: %s\n", getFileExtension(request->filename));
    strcpy(request->mimeType, getFileExtension(request->filename));
    strcpy(request->extension, strrchr(request->filename, DOT));
    return; 
}

const String
getFileType(const String type)
{
    validatePointerOrDie(type, "Type pointer cannot be null\n");

    for(int i = 0; mimeTypes[i].type; i++) {
        if (strcmp(type, mimeTypes[i].type) == 0) {
            return mimeTypes[i].value;
        }
    }
    return DEFAULT_FILETYPE;
}

const String
getFileExtension(const String filename)
{
    validatePointerOrDie(filename, "Filename pointer cannot be null\n");

    const String type = strrchr(filename, DOT);
    return getFileType(type);
}