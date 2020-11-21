/**
 * @file tokenlist.h
 * @author Marek Gergel (xgerge01)
 * @brief IFJ20 - Operations over list with tokens
 */

#ifndef TOKENLIST_H
#define TOKENLIST_H

#include <stdlib.h>
#include "string.h"
#include "error.h"

//token structure

typedef enum TokenType {
    IDENTIFIER = 0,
    OPERATOR = 1,
    KEYWORD = 2,
} TokenType_t;

typedef TokenValue_t String_t;

typedef struct Token Token_t;

struct Token {
    TokenType_t type;
    TokenValue_t value;
    Token_t* pNext;
};

//list structure

typedef struct List {
    Token_t* pHead;
    Token_t* pTail;
} List_t;

extern List_t list;

//token operations

#define TOKEN_TYPE(token) (token->type)
#define TOKEN_VALUE(token) (token->value)
#define TOKEN_NEXT(token) (token->pNext)

#define TOKEN_SET_TYPE(token, type) (TOKEN_TYPE(token) = type)
#define TOKEN_SET_VALUE(token, value) (TOKEN_VALUE(token) = value)
#define TOKEN_SET_NEXT(token, next) (TOKEN_NEXT(token) = next)

#define TOKEN_CREATE(token) do { if ((token = malloc(sizeof(Token_t)))) {TOKEN_SET_NEXT(token, NULL);} else {errorExit(internalError, "list.h : Token allocation failed");}} while (0)
#define TOKEN_MOVE_NEXT(token) (token = TOKEN_NEXT(token))
#define TOKEN_DESTROY(token) free(token)

//list operations

#define LIST_HEAD (list.pHead)
#define LIST_TAIL (list.pTail)

#define LIST_IS_EMPTY (!LIST_HEAD)

#define LIST_ADD_TOKEN(token) do { if (LIST_IS_EMPTY) {LIST_HEAD = token; LIST_TAIL = token;} else {TOKEN_SET_NEXT(LIST_TAIL, token); LIST_TAIL = token;}} while (0)
#define LIST_CLEAR while (!LIST_IS_EMPTY) {LIST_TAIL = TOKEN_NEXT(LIST_HEAD); TOKEN_DESTROY(LIST_HEAD); LIST_HEAD = LIST_TAIL;}

#endif // TOKENLIST_H