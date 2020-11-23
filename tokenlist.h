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
    EOL = 0,                    // EOL
    IDENTIFIER,                 // id
    NUMBER,                     // num
    STRING,                     // str
    KEYWORD_INT = 10,           // int
    KEYWORD_FLOAT64,            // float64
    KEYWORD_STRING,             // string
    KEYWORD_IF,                 // if
    KEYWORD_ELSE,               // else
    KEYWORD_FOR,                // for
    KEYWORD_FUNC,               // func
    KEYWORD_RETURN,             // return
    KEYWORD_PACKAGE,            // package
    OPERATOR_DEFINE = 30,       // :=
    OPERATOR_ASSIGN,            // =
    OPERATOR_PLUS,              // +
    OPERATOR_MINUS,             // -
    OPERATOR_MUL,               // *
    OPERATOR_DIV,               // /
    OPERATOR_AND,               // &&
    OPERATOR_OR,                // ||
    OPERATOR_NOT,               // !
    OPERATOR_NOT_EQUAL          // !=
    OPERATOR_EQUAL,             // ==
    OPERATOR_GREATER,           // >
    OPERATOR_GREATER_OR_EQUAL,  // >=
    OPERATOR_LESS,              // <
    OPERATOR_LESS_OR_EQUAL,     // <=
    BRACKET_ROUND_OPEN = 50,    // (
    BRACKET_ROUND_CLOSE,        // )
    BRACKET_CURLY_OPEN,         // {
    BRACKET_CURLY_CLOSE,        // }
} TokenType_t;

typedef String_t TokenValue_t;

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