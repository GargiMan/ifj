/**
 * @file parser.h
 * @author Filip Sapak (xsapak05)
 * @brief IFJ20 - parser function prototypes
 */


#ifndef PARSER_H
#define PARSER_H

#include "tokenlist.h"
#include "scanner.h"
#include "string.h"
#include "resources.h"
#include "error.h"
//#include "symtable.h"

int parse(List_t *TokenList);
int prog(List_t *TokenList);
int exec(List_t *TokenList);
int func(List_t *TokenList);
int func_types(List_t *TokenList);
int types_n(List_t *TokenList);
int type(List_t *TokenList);
int params(List_t *TokenList);
int params_n(List_t *TokenList);
int func_n(List_t *TokenList);
int body(List_t *TokenList);
int statements(List_t *TokenList);
int statement(List_t *TokenList);
int definition(List_t *TokenList);
int assignment(List_t *TokenList);
int ids(List_t *TokenList);
int ids_n(List_t *TokenList);
int expressions(List_t *TokenList);
int expression_n(List_t *TokenList);
int expression(List_t *TokenList);
int _if(List_t *TokenList);
int _for(List_t *TokenList);
int _call(List_t *TokenList);
int _return(List_t *TokenList);

#endif