#include "validation.h"

extern int errno;

void
die(int line, const char functionName[], const char filename[])
{
    LOG("LINE: %d; FUNCTION: %s; FILENAME: %s\n", line, functionName, filename);
    exit(1);
}

int
validateOrDie(int exp, const char message[])
{
    if (exp == SOCKET_ERROR) {
        LOG_ERROR("%s - (%s)\n", message, strerror(errno));
        DIE;
    }
    return exp;
}

bool
validate(int exp, const char message[])
{
    bool status = exp != SOCKET_ERROR;
    if (!status) {
        LOG_ERROR("%s - (%s)\n", message, strerror(errno));
        return false;
    }
    return true;
}

bool
validateSock(int exp)
{
    return exp > 0;
}

void *
validatePointer(void * pointer, const char message[])
{
    if (isNotValid(pointer)) {
        LOG_ERROR("%s - (%s)\n", message, strerror(errno));
    }
    return pointer;
}

void *
validatePointerOrDie(void * pointer, const char message[])
{
    if (isNotValid(pointer)) {
        LOG_ERROR("%s - (%s)\n", message, strerror(errno));
        DIE;
    }
    return pointer;
}

void *
mallocOrDie(size_t numBytes, const char message[])
{
    char error[MAX_ERROR_LENGTH + strlen(message)];

    strcpy(error, "Failed to allocate memory for ");
    strcat(error, message);

    return validatePointerOrDie(malloc(numBytes), error);
}

void *
allocate(size_t numBytes, const char message[])
{
    char error[MAX_ERROR_LENGTH + strlen(message)];

    strcpy(error, "Failed to allocate memory for ");
    strcat(error, message);

    return validatePointer(malloc(numBytes), error);
}

bool
isNotValid(void * pointer)
{
    return pointer == NULL;
}

bool
isValid(void * pointer)
{
    return !isNotValid(pointer);
}