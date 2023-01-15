#ifndef SOCKET_VALIDATION_H_INCLUDED
#define SOCKET_VALIDATION_H_INCLUDED

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "type_aliases.h"
#include "logs.h"

#define MAX_ERROR_LENGTH        31
#define SOCKET_ERROR            -1
#define DIE                     die(__LINE__, __FUNCTION__, __FILE__)

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

/**
 * @returns void *
 */
void * validatePointerOrDie(void * pointer, const char message[]);

/**
 * @returns void *
 */
void * mallocOrDie(size_t numBytes, const char message[]);

/**
 * 
 */
void die(int line, const char functionName[], const char filename[]);

/**
 * @returns bool
 */
bool isNotValid(void * pointer);

/**
 * @param void * pointer
 * @returns bool
 */
bool isValid(void * pointer);

/**
 * @param int exp
 * @returns bool
 */
bool validateSock(int exp);

#endif // SOCKET_VALIDATION_H_INCLUDED
