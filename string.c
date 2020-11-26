/**
 * @file string.c
 * @author Marek Gergel (xgerge01)
 * @brief IFJ20 Compiler
 */

#include "string.h"

String_t* strCreate() {

    String_t* tmp = malloc(sizeof(String_t));
    if (!tmp) {
        errorExit(internalError, "string : String allocation failed");
        return NULL;
    }



    return tmp;
}

String_t* strAppendChar(String_t* s, char c) {

    size_t len = 0;
    if (s) len = strlen(s);

    String_t* tmp = realloc(s, sizeof(size_t) + (len + 1) * sizeof(char));
    if (!tmp) {
        free(s);
        errorExit(internalError, "string : String allocation failed");
        return NULL;
    }

    tmp[len] = c;

    return tmp;
}

String_t* strAppendString(String_t* s, char* s1) {

    size_t len = 0, len1 = 0;
    if (s) len = strlen(s);
    if (s1) len1 = strlen(s1);

    String_t* tmp = realloc(s, sizeof(size_t) + (len + len1) * sizeof(char));
    if (!tmp) {
        free(s);
        errorExit(internalError, "string : String allocation failed");
        return NULL;
    }

    for (int i = 0; i < len1; i++) tmp[len+i] = s1[i];

    return tmp;
}