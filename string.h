/**
 * @file string.h
 * @author Marek Gergel (xgerge01)
 * @brief IFJ20 - Header for operations over string
 */

#ifndef __STRING_H__
#define __STRING_H__

#include <string.h>
#include <stdlib.h>
#include "error.h"

typedef char* String_t;

String_t appendChar(String_t s, char c);

#endif // __STRING_H__