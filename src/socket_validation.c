#include "socket_validation.h"

extern int errno;

int
check(int exp, const char message[])
{
    if (exp == SOCKET_ERROR) {
        LOG_ERROR("%s - (%s)\n", message, strerror(errno));
        exit(1);
    }

    return exp;
}