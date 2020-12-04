/**
 * @file string.c
 * @author Marek Gergel (xgerge01)
 * @brief IFJ20 - Operations over string
 */

#include "string.h"

void strClear(String_t *s) {

    s->str = malloc(sizeof(char) * STR_ALLOC_INC);
    s->str[0] = '\0';
    s->len = 0;

    return;
}

int strIsFull(String_t* s) {

    return ((s->len % STR_ALLOC_INC) == 0);
}

void strIncreaseSize(String_t* s) { 

    size_t alloc_size = sizeof(char) * ((s->len / STR_ALLOC_INC) + STR_ALLOC_INC + 1);      //+1 for \0

    //printf("%ld/%ld\n", s->len, alloc_size);

    s->str = realloc(s->str, alloc_size);
    if (!(s->str)) errorExit(internalError, "string : String allocation failed");
    if (s->len == 0) s->str[0] = '\0';

    return;
}

void strAppendChar(String_t* s, char c) {

    if (strIsFull(s)) strIncreaseSize(s);

    s->len++;
    s->str[s->len-1] = c;
    s->str[s->len] = '\0';

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