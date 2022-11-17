#ifndef THROWABLE_H_INCLUDED
#define THROWABLE_H_INCLUDED

#include <setjmp.h>
#include "throws.h"

jmp_buf savebuf;
int exceptionVal;

#define TRY (resetThrow(), if ((exceptionVal = setjmp(savebuf)) == 0))
#define CATCH(NUM) else if (exceptionVal == NUM)
#define CATCHALL else
#define THROW(NUM) throws(savebuf, NUM, __LINE__, __COLUMN__, __FUNCTION__, __FILE__)
#define WARNING(...) fprintf(stderr, __VA_ARGS__)
#define PRINT_EXPRESSION(x) (printf("%s = %d\n", #x, (x)))

#endif // THROWABLE_H_INCLUDED