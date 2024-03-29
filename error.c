/**
 * @file error.c
 * @author Marek Gergel (xgerge01)
 * @brief IFJ20 - Exit function
 */

#include "error.h"

void errorExit(ErrorCodes_t errcode, char* msg, ...) {
    fflush(stdout);
    fflush(stderr);
    va_list args;
    va_start(args, msg);
    fprintf(stderr, "Error: ");
    vfprintf(stderr, msg, args);
    va_end(args);
    FREE_RESOURCES;
    exit(errcode);
}

void warningPrint(char* msg, ...) {
    fflush(stdout);
    fflush(stderr);
    va_list args;
    va_start(args, msg);
    fprintf(stderr, "Warning: ");
    vfprintf(stderr, msg, args);
    va_end(args);
}