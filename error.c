/**
 * @file error.c
 * @author Marek Gergel (xgerge01)
 * @brief IFJ - Error exit function
 */

#include "error.h"

void errorExit(int errcode, char* errmsg, ...) {
    fflush(stdout);
    fflush(stderr);
    va_list args;
    va_start(args, errmsg);
    fprintf(stderr, "Error: ");     //error prefix
    vfprintf(stderr, errmsg, args);    //error message
    va_end(args);
    //freeResources();
    exit(errcode);
}