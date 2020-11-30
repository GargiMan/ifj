/**
 * @file string.c
 * @author Marek Gergel (xgerge01)
 * @brief IFJ20 Compiler
 */

#include "string.h"

void strClear(String_t *s) {

    s->str = NULL;
    s->len = 0;

    return;
}

void strIncreaseSize(String_t* s) {

    s->str = realloc(s->str, (s->len % STR_ALLOC_INC ? s->len / STR_ALLOC_INC : (s->len / STR_ALLOC_INC) + 1));
    if (!(s->str)) errorExit(internalError, "string : String allocation failed");

    return;
}

void strAppendChar(String_t* s, char c) {

    if (!(s->len % STR_ALLOC_INC)) strIncreaseSize(s);

    s->str[s->len] = c;
    s->len += 1;

    return;
}

void strAppendChars(String_t* s, char* cs) {

    size_t len = strlen(cs);

    for (size_t i = 0; i < len; i++) strAppendChar(s, cs[i]);

    return;
}

void strDestroy(String_t* s) {

    free(s->str);

    return;
}