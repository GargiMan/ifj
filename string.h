/**
 * @file string.h
 * @author Marek Gergel (xgerge01)
 * @brief IFJ20 - Header for operations over string
 */

#ifndef STRING_H
#define STRING_H

typedef struct String {
    char* str;
    size_t len;
} String_t;

#include <string.h>
#include <stdlib.h>
#include "error.h"

String_t* strCreate();

String_t* strAppendChar(String_t s, char c);

String_t* strAppendString(String_t s, String_t s1);

#endif // STRING_H