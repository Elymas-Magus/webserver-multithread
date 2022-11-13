#ifndef SOCKET_VALIDATION_H_INCLUDED
#define SOCKET_VALIDATION_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>

#define SOCKET_ERROR -1

/**
 * Receive an integer from <sys/socket.h> functions
 * And check if is valid or is a SOCKET_ERROR
 * If is SOCKET_ERROR die
 * @param int exp
 * @param const char[] message
 * @return void
 */
int check(int exp, const char message[]);


#endif // SOCKET_VALIDATION_H_INCLUDED
