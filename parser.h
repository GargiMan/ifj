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
        htabFree(localtab); \
        errorExit(syntaxError,"parser: syntaxERR \n"); \
    }   \

#define CHECK_EOF() \
    if (Token == list.pTail) {return;}else{ htabFree(localtab); errorExit(syntaxError, "No EOF\n");}

#define TEST_EOF() \
    if (Token == list.pTail) {return;}

#define TEST_TYPE(TOKENTYPE) \
    if (Token->type == TOKENTYPE){ \
        decisionFlag = 1; \
    } else \
    { \
        decisionFlag = 0; \
    } \

#define DECIDE() \
    if (decisionFlag){ \
      return;} \

#define CHECK_STATE(state) \
    if (state() == syntaxError){ \
        errorExit(syntaxError, "....\n");} \

#define SET_FLAG(flag)(flag = 1) \

#define GET_NEXT(token) if(token->pNext == NULL){htabFree(localtab); errorExit(syntaxError, "Function with no ending\n");}else{(token = TOKEN_NEXT(token));}

    
Token_t *Token;
int decisionFlag;
int functionParse;
HTab_t* localtab;
void dataInit();
void parse();//
void prog();//
void exec();//
void func();//
void func_types();//
void types_n();//
void type();//
void params();//
void params_n();//
void func_n();//
void body();//
void statement_n();//
void statement();//
void definition();//
void assignment();//
void ids();//
void ids_n();//
void expressions();//
void expression_n();//
void _if();//
void _for();
void _call();//
void _return();//
void _call_param();
void _call_param_n();
void expression();

#endif // PARSER_H