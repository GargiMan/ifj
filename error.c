/**
 * @file error.c
 * @author Marek Gergel (xgerge01)
 * @brief IFJ20 Compiler
 */

#include "error.h"

void errorExit(errcodes_t errcode, char* errmsg, ...) {
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