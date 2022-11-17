#include "throws.h"

Throwable * throwable;
ThrowableType throwableTypes[] = {
    {EMPTY_ARRAY_ERROR, "ERROR: The array is empty"},
    {DIVISION_BY_ZERO_ERROR, "ERROR: Division by zero"},
    {EMPTY_POSITION_ERROR, "ERROR: The position is empty"},
    {TYPE_ERROR, "ERROR: Type error"},
    {RANGE_ERROR, "ERROR: Range Error"},
    {REFERENCE_ERROR, "ERROR: Reference error"},
    {URI_ERROR, "ERROR: URI error"},
    {INTERNAL_ERROR, "ERROR: Internal error"},
    {NOT_DEFINED_ERROR, "ERROR: Not defined"},
};

void
throws(jmp_buf savebuf, int value, char line[], char column[], char functionName[], char filename[])
{
    throwable = (Throwable *) malloc(sizeof(Throwable));

    strcpy(throwable->message, throwableTypes[value].message); 
    strcpy(throwable->line, line); 
    strcpy(throwable->column, column); 
    strcpy(throwable->functionName, functionName); 
    strcpy(throwable->filename, filename); 

    longjmp(savebuf, value);
}

Throwable *
getThrowable()
{
    return throwable;
}

ThrowableType *
getThrowableTypes()
{
    return throwableTypes;
}

void
resetThrow()
{
    throwable = NULL;
}