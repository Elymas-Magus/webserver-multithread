#ifndef SOCKET_VALIDATION_H_INCLUDED
#define SOCKET_VALIDATION_H_INCLUDED

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "type_aliases.h"
#include "logs.h"

#define SOCKET_ERROR            -1

/**
 * Receive an integer from <sys/socket.h> functions
 * And check if is valid or is a SOCKET_ERROR
 * If it is SOCKET_ERROR registers and dies
 * @param int exp
 * @param const char[] message
 * @returns void
 */
int validateOrDie(int exp, const char message[]);

/**
 * Receive an integer from <sys/socket.h> functions
 * And check if is valid or is a SOCKET_ERROR
 * If is SOCKET_ERROR registers
 * @param int exp
 * @param const char[] message
 * @returns void
 */
bool validate(int exp, const char message[]);


#endif // SOCKET_VALIDATION_H_INCLUDED
