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
    {FILE_INSERTION_ERROR, "ERROR: The file insertion failed!"},
    {OPENING_FILE_ERROR, "ERROR: Error opening file!"},
    {FILE_READING_ERROR, "ERROR: Error reading file!"},
    {FILE_REALPATH_ERROR, "ERROR: Error to get file's absolute path"}
};

void
throws(jmp_buf savebuf, int value, int line, const char functionName[], const char filename[])
{
    throwable = (Throwable *) malloc(sizeof(Throwable));

    strcpy(throwable->message, throwableTypes[value].message); 
    strcpy(throwable->functionName, functionName); 
    strcpy(throwable->filename, filename); 
    throwable->line = line;

    LOG(
        "MESSAGE: %s; LINE: %d; FUNCTION: %s; FILENAME: %s\n",
        throwable->message, throwable->line, throwable->functionName, throwable->filename
    );

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

String
getCurrentThrowableMessage()
{
    return throwable->message;
}

String
getThrowableMessage(int index)
{
    return throwableTypes[index].message;
}

void
resetThrow()
{
    throwable = NULL;
}