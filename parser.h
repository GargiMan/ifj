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
#include "symtable.h"
#define CHECK_TYPE(TOKENTYPE) \
    if (Token->type != TOKENTYPE){ \
        errorExit(syntaxError,"parser: syntaxERR \n"); \
    }   \

#define CHECK_EOF() \
    if (Token == list.pTail) {return 0;}else{ errorExit(syntaxError, "No EOF\n");}

#define TEST_EOF() \
    if (Token == list.pTail) {return 0;}

#define TEST_TYPE(TOKENTYPE) \
    if (Token->type == TOKENTYPE){ \
        decisionFlag = 1; \
    } else \
    { \
        decisionFlag = 0; \
    } \

#define DECIDE() \
    if (decisionFlag){ \
      return 0;} \

#define CHECK_STATE(state) \
    if (state() == syntaxError){ \
        errorExit(syntaxError, "....\n");} \

#define SET_FLAG(flag)(flag = 1) \

#define GET_NEXT(token) if(token->pNext == NULL){errorExit(syntaxError, "Function with no ending\n");}else{(token = TOKEN_NEXT(token));}

    
Token_t *Token;
int decisionFlag;
char* funcName;
int returnFlag;
/*
int parse();//
int prog();//
int exec();//
int func();//
int func_types();//
int types_n();//
int type();//
int params();//
int params_n();//
int func_n();//
int body();//
int statement_n();//
int statement();//
int definition();//
int assignment();//
int id();//
int id_n();//
int expressions();//
int expression_n();//
int _if();//
int _for();
int _call();//
int _return();//
int _call_param();
int _call_param_();
int expression();*/

#endif