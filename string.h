/**
 * @file string.h
 * @author Marek Gergel (xgerge01)
 * @brief IFJ20 - Header for operations over string
 */

#ifndef STRING_H
#define STRING_H

typedef char* String_t;

#include <string.h>
#include <stdlib.h>
#include "error.h"

String_t appendChar(String_t s, char c);

#endif // STRING_H