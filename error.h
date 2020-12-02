/**
 * @file error.h
 * @author Marek Gergel (xgerge01)
 * @brief IFJ20 - Error codes and exit function
 */

#ifndef ERROR_H
#define ERROR_H

#include <stdarg.h>
#include <stdio.h>
#include "resources.h"

typedef enum ErrorCodes {
    lexicalError = 1,
    syntaxError = 2,
    semanticIdentifierError = 3,
    semanticIdentifierTypeError = 4,
    semanticTypeCompatibilityError = 5,
    semanticFunctionError = 6,
    semanticOtherError = 7,
    semanticZeroDivisionError = 9,
    internalError = 99,
} ErrorCodes_t;

void errorExit(ErrorCodes_t errcode, char* msg, ...);

void warningPrint(char* msg, ...);

#endif // ERROR_H