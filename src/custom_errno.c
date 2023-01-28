#include "custom_errno.h"

int c_errno = 0;

void
setErrno(int value)
{
    c_errno = value;
}

bool
hasErro()
{
    return c_errno != 0;
}

int
getErrno()
{
    return c_errno;
}