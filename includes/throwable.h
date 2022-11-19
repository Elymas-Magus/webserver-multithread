#ifndef THROWABLE_H_INCLUDED
#define THROWABLE_H_INCLUDED

#include <setjmp.h>

#include "throws.h"

extern jmp_buf savebuf;
extern int exceptionVal;

#define TRY if ((exceptionVal = setjmp(savebuf)) == 0)
#define CATCH(NUM) else if (exceptionVal == NUM)
#define CATCHALL else
#define FINALLY if (1)
#define THROW(NUM) throws(savebuf, NUM, __LINE__, __FUNCTION__, __FILE__)

#endif // THROWABLE_H_INCLUDED