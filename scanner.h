/**
 * @file scanner.h
 * @author Marek Gergel (xgerge01)
 * @brief IFJ20 - Header for operations with tokens
 */

#ifndef SCANNER_H
#define SCANNER_H

#include <stdio.h>
#include "error.h"
#include "string.h"
#include "tokenlist.h"

//assign inside NOT allowed
#define IS_EOL(c) (c == '\n' || c == '\r')
#define IS_NOT_TOKEN(c) (c == ' ' || c == '\t' || c == '\0')
#define IS_NUMBER(c) (c > 47 && c < 58) // 0-9
#define IS_NUMBER_HEX(c) ((c > 47 && c < 58) || (c > 64 && c < 71) || (c > 96 && c < 103))
#define IS_APLHA(c) ((c > 64 && c < 91) || (c > 96 && c < 123)) // a-z || A-Z
#define IS_EXPONENT(c) (c == 'e' || c == 'E')

//assign inside allowed
#define IS_EOF(c) ((c) == EOF)
#define IS_UNDERSCORE(c) ((c) == '_')
#define IS_DOUBLE_QUOTE(c) ((c) == '"')
#define IS_DOT(c) ((c) == '.')
#define IS_ZERO_CHAR(c) ((c) == '0')
#define IS_COLON(c) ((c) == ':')
#define IS_SEMICOLON(c) ((c) == ';')
#define IS_COMMA(c) ((c) == ',')
#define IS_MINUS(c) ((c) == '-')
#define IS_PLUS(c) ((c) == '+')
#define IS_SLASH(c) ((c) == '/')
#define IS_ASTERISK(c) ((c) == '*')
#define IS_AMPERSAND(c) ((c) == '&')
#define IS_VERTICAL_BAR(c) ((c) == '|')
#define IS_EXCLAMATION(c) ((c) == '!')
#define IS_EQUALS(c) ((c) == '=')
#define IS_LESS(c) ((c) == '<')
#define IS_GREATER(c) ((c) == '>')
#define IS_ROUND_OPEN(c) ((c) == '(')
#define IS_ROUND_CLOSE(c) ((c) == ')')
#define IS_CURLY_OPEN(c) ((c) == '{')
#define IS_CURLY_CLOSE(c) ((c) == '}')

//keywords
#define KW_INT(s) (!strcmp("int", s))
#define KW_FLOAT64(s) (!strcmp("float64", s))
#define KW_STRING(s) (!strcmp("string", s))
#define KW_IF(s) (!strcmp("if", s))
#define KW_ELSE(s) (!strcmp("else", s))
#define KW_FOR(s) (!strcmp("for", s))
#define KW_FUNC(s) (!strcmp("func", s))
#define KW_RETURN(s) (!strcmp("return", s))
#define KW_PACKAGE(s) (!strcmp("package", s))

void getTokens();

#endif // SCANNER_H