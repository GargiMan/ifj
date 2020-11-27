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

    tmp->str[0] = '\0';
    tmp->len = 0;

    return tmp;
}

String_t* strAppendChar(String_t* s, char c) {

    String_t* tmp = s;
    size_t len = 0;

    if (tmp) {
        len = tmp->len;
    } else {
        tmp = strCreate();
    }

    char* str = realloc(tmp->str, (len + 1) * sizeof(char));
    if (!str) {
        strDestroy(s);
        errorExit(internalError, "string : String allocation failed");
        return NULL;
    }

    str[len] = c;

    tmp->str = str;
    tmp->len = len + 1;

    return tmp;
}

String_t* strAppendChars(String_t* s, char* s1) {

    String_t* tmp = s;
    size_t len = 0, len1 = 0;

    if (tmp) {
        len = tmp->len;
    } else {
        tmp = strCreate();
    }

    if (!s1) return tmp;
    len1 = strlen(s1);

    char* str = realloc(tmp->str, (len + len1) * sizeof(char));
    if (!str) {
        strDestroy(s);
        errorExit(internalError, "string : String allocation failed");
        return NULL;
    }

    for (size_t i = 0; i < len1; i++) str[len+i] = s1[i];

    tmp->str = str;
    tmp->len = len + len1;

    return tmp;
}

void strDestroy(String_t* s) {

    if (s) free(s->str);
    free(s);

    return;
}