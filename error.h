/**
 * @file error.h
 * @author Marek Gergel (xgerge01)
 * @brief IFJ - Error codes and function
 */

#ifndef __ERROR_H__
#define __ERROR_H__

//#include "resources.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum errcodes {
    noError = 0,
    lexicalError = 1,
    syntaxError = 2,
    semanticIdentifierError = 3,
    semanticIdentifierTypeError = 4,
    semanticTypeCompatibilityError = 5,
    semanticFunctionError = 6,
    semanticOtherError = 7,
    semanticZeroDivisionError = 9,
    internalError = 99,
} errcodes_t;

void errorExit(errcodes_t errcode, char* fmt, ...);

#endif // __ERROR_H__