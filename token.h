/**
 * @file token.h
 * @author Marek Gergel (xgerge01)
 * @brief IFJ20 - Operations with tokens and token structure
 */

#ifndef TOKEN_H
#define TOKEN_H

#include "string.h"

typedef enum TokenType {
    IDENTIFIER = 0,
    OPERATOR = 1,
    KEYWORD = 2,
} TokenType_t;

typedef struct Token {
    TokenType_t type;
    String_t value;
} Token_t;

#define TOKEN_TYPE(token) (token->type)
#define TOKEN_VALUE(token) (token->value)

#define TOKEN_SET_TYPE(token, type) (TOKEN_TYPE(token) = type)
#define TOKEN_SET_VALUE(token, type) (TOKEN_VALUE(token) = value)

#endif // TOKEN_H