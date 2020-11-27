/**
 * @file string.h
 * @author Marek Gergel (xgerge01)
 * @brief IFJ20 - Header for operations over string
 */

#ifndef STRING_H
#define STRING_H

#include <string.h>
#include <stdlib.h>

typedef struct String {
    char* str;
    size_t len;
} String_t;

#include "error.h"

String_t* strCreate();

String_t* strAppendChar(String_t* s, char c);

String_t* strAppendChars(String_t* s, char* s1);

void strDestroy(String_t* s);

#endif // STRING_H