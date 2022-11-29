#include "socket_validation.h"

extern int errno;

int
validateOrDie(int exp, const char message[])
{
    if (exp == SOCKET_ERROR) {
        LOG_ERROR("%s - (%s)\n", message, strerror(errno));
        exit(1);
    }
    return exp;
}

bool
validate(int exp, const char message[])
{
    bool status = exp != SOCKET_ERROR;
    if (!status) {
        LOG_ERROR("%s - (%s)\n", message, strerror(errno));
    }
    return exp;
}