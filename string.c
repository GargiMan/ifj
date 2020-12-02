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

int strIsFull(String_t* s) {

    return ((s->len % STR_ALLOC_INC) == 0);
}

void strIncreaseSize(String_t* s) {

    s->str = realloc(s->str, sizeof(char) * ((s->len / STR_ALLOC_INC) + STR_ALLOC_INC));
    if (!(s->str)) errorExit(internalError, "string : String allocation failed");

    return;
}

void strAppendChar(String_t* s, char c) {

    //printf("%ld/%ld\n", s->len, sizeof(char) * ((s->len / STR_ALLOC_INC) + STR_ALLOC_INC));

    if (strIsFull(s)) strIncreaseSize(s);

    s->str[s->len] = c;
    s->len++;

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