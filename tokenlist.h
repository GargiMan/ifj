/**
 * @file tokenlist.h
 * @author Marek Gergel (xgerge01)
 * @brief IFJ20 - Operations over list with tokens
 */

#ifndef TOKENLIST_H
#define TOKENLIST_H

#include <stdlib.h>
#include "error.h"

//token structure

typedef enum TokenType {
    EOL = 0,                    // 0 end of line
    ID,                         // 1 id
    DATA_INT,                   // 2 int data
    DATA_FLOAT64,               // 3 float data
    DATA_STRING,                // 4 string data
    KEYWORD_INT = 10,           // 10 int
    KEYWORD_FLOAT64,            // 11 float64
    KEYWORD_STRING,             // 12 string
    KEYWORD_IF,                 // 13 if
    KEYWORD_ELSE,               // 14 else
    KEYWORD_FOR,                // 15 for
    KEYWORD_FUNC,               // 16 func
    KEYWORD_RETURN,             // 17 return
    KEYWORD_PACKAGE,            // 18 package
    OPERATOR_DEFINE = 20,       // 20 :=
    OPERATOR_ASSIGN,            // 21 =
    OPERATOR_PLUS,              // 22 +
    OPERATOR_MINUS,             // 23 -
    OPERATOR_MUL,               // 24 *
    OPERATOR_DIV,               // 25 /
    OPERATOR_AND,               // 26 &&
    OPERATOR_OR,                // 27 ||
    OPERATOR_NOT,               // 28 !
    OPERATOR_NOT_EQUAL,         // 29 !=
    OPERATOR_EQUAL,             // 30 ==
    OPERATOR_GREATER,           // 31 >
    OPERATOR_GREATER_OR_EQUAL,  // 32 >=
    OPERATOR_LESS,              // 33 <
    OPERATOR_LESS_OR_EQUAL,     // 34 <=
    BRACKET_ROUND_OPEN = 40,    // 40 (
    BRACKET_ROUND_CLOSE,        // 41 )
    BRACKET_CURLY_OPEN,         // 42 {
    BRACKET_CURLY_CLOSE,        // 43 }
    SEMICOLON,                  // 44 ;
    COMMA,                      // 45,
} TokenType_t;

typedef char* TokenValue_t;
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

#define TOKEN_CREATE(token) do { if ((token = malloc(sizeof(Token_t)))) {TOKEN_SET_VALUE(token, NULL); TOKEN_SET_NEXT(token, NULL);} else {errorExit(internalError, "tokenlist : Token allocation failed");}} while (0)
#define TOKEN_CREATE_WT(token, type) do { if ((token = malloc(sizeof(Token_t)))) {TOKEN_SET_TYPE(token, type); TOKEN_SET_VALUE(token, NULL); TOKEN_SET_NEXT(token, NULL);} else {errorExit(internalError, "tokenlist : Token allocation failed");}} while (0)
#define TOKEN_MOVE_NEXT(token) (token = TOKEN_NEXT(token))
#define TOKEN_IS_TYPE(token, type) (TOKEN_TYPE(token) == type)
#define TOKEN_DESTROY(token) do { free(token->value); free(token); } while (0)

//list operations

#define LIST_HEAD(list) (list.pHead)
#define LIST_TAIL(list) (list.pTail)

#define LIST_IS_EMPTY(list) (!LIST_HEAD(list))

#define LIST_ADD_TOKEN(list, token) do { if (LIST_IS_EMPTY(list)) {LIST_HEAD(list) = token; LIST_TAIL(list) = token;} else {TOKEN_SET_NEXT(LIST_TAIL(list), token); LIST_TAIL(list) = token;}} while (0)
#define LIST_ADD_TOKEN_WT(list, token, type) do { TOKEN_CREATE_WT(token, type); if (LIST_IS_EMPTY(list)) {LIST_HEAD(list) = token; LIST_TAIL(list) = token;} else {TOKEN_SET_NEXT(LIST_TAIL(list), token); LIST_TAIL(list) = token;}} while (0)
#define LIST_CLEAR(list) while (!LIST_IS_EMPTY(list)) {LIST_TAIL(list) = TOKEN_NEXT(LIST_HEAD(list)); TOKEN_DESTROY(LIST_HEAD(list)); LIST_HEAD(list) = LIST_TAIL(list);}

#endif // TOKENLIST_H