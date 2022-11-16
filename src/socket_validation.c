#include "socket_validation.h"

int
check(int exp, const char message[])
{
    if (exp == SOCKET_ERROR) {
        fprintf(stderr, "%s", message);
        perror(message);
        exit(1);
    }

    return exp;
}