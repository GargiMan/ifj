/**
 * @file string.c
 * @author Marek Gergel (xgerge01)
 * @brief IFJ20 Compiler
 */

#include "string.h"

String_t appendChar(String_t s, char c) {

    size_t len = 0;
    if (s) len = strlen(s);

    String_t tmp = realloc(s, (len + 1) * sizeof(char));
    if (!tmp) {
        free(s);
        errorExit(internalError, "string : Char allocation failed");
        return NULL;
    }

    tmp[len] = c;

    return tmp;
}

