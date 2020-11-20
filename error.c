/**
 * @file error.c
 * @author Marek Gergel (xgerge01)
 * @brief IFJ20 - Exit function
 */

#include "error.h"

void errorExit(errcodes_t errcode, char* errmsg, ...) {
    fflush(stdout);
    fflush(stderr);
    va_list args;
    va_start(args, errmsg);
    fprintf(stderr, "Error: ");
    vfprintf(stderr, errmsg, args);
    va_end(args);
    LIST_CLEAR;
    exit(errcode);
}