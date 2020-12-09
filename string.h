/**
 * @file string.h
 * @author Marek Gergel (xgerge01)
 * @brief IFJ20 - Header for operations over string
 */

#ifndef STRING_H
#define STRING_H

#include <string.h>
#include <stdlib.h>
#include "error.h"

#define STR_ALLOC_INC 1

typedef struct String {
    char* str;
    size_t len;
} String_t;

void strClear(String_t *s);

int strIsFull(String_t* s);

void strIncreaseSize(String_t* s);

void strAppendChar(String_t* s, char c);

void strAppendChars(String_t* s, char* cs);

void strDestroy(String_t* s);

char* charPtrAppendChars(char* cptr, char* cs);

#endif // STRING_H