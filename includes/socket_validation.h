#ifndef SOCKET_VALIDATION_H_INCLUDED
#define SOCKET_VALIDATION_H_INCLUDED

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "type_aliases.h"

#define SOCKET_ERROR            -1

/**
 * Receive an integer from <sys/socket.h> functions
 * And check if is valid or is a SOCKET_ERROR
 * If is SOCKET_ERROR die
 * @param int exp
 * @param const char[] message
 * @returns void
 */
int check(int exp, const char message[]);


#endif // SOCKET_VALIDATION_H_INCLUDED
