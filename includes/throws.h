#ifndef THROWS_H_INCLUDED
#define THROWS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>

#define EMPTY_ARRAY_ERROR               0
#define DIVISION_BY_ZERO_ERROR          1
#define EMPTY_POSITION_ERROR            2
#define TYPE_ERROR                      3
#define RANGE_ERROR                     4
#define REFERENCE_ERROR                 5
#define URI_ERROR                       6
#define INTERNAL_ERROR                  7
#define NOT_DEFINED_ERROR               8

#define MAX_ERROR_MESSAGE_LENGTH        255
#define MAX_ERROR_LINE_LENGTH           255
#define MAX_ERROR_COLUMN_LENGTH         255
#define MAX_ERROR_FILENAME_LENGTH       255
#define MAX_ERROR_FUNCTION_NAME_LENGTH  255

typedef struct throwable {
    char message[MAX_ERROR_MESSAGE_LENGTH];
    char line[MAX_ERROR_LINE_LENGTH];
    char column[MAX_ERROR_COLUMN_LENGTH];
    char filename[MAX_ERROR_FILENAME_LENGTH];
    char functionName[MAX_ERROR_FUNCTION_NAME_LENGTH];
} Throwable;

typedef struct throwableType {
    int code;
    char message[MAX_ERROR_MESSAGE_LENGTH];
} ThrowableType;

/**
 * @param jmp_buf savebuf
 * @param int value
 * @param char line[]
 * @param char functionName[]
 * @param char filename[]
 */
void throws(jmp_buf savebuf, int value, char line[], char column[], char functionName[], char filename[]);

/**
 * @return Throwable *
 */
Throwable * getThrowable();

/**
 * @return ThrowableType
 */
ThrowableType * getThrowableTypes();

/**
 * 
 */
void resetThrow();

#endif // THROWS_H_INCLUDED