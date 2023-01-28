#ifndef CUSTOM_ERRNO_H
#define CUSTOM_ERRNO_H

#include <stdbool.h>

extern int c_errno = 0;

void setErrno(int value);

bool hasErro();

int getErrno();

#endif