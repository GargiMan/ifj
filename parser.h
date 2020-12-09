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
    if (Token == list.pTail) {return;}else{ errorExit(syntaxError, "No EOF\n");}

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

#define GET_NEXT(token) if(token->pNext == NULL){errorExit(syntaxError, "Function with no ending\n");}else{(token = TOKEN_NEXT(token));}

    
Token_t *Token;
int decisionFlag;
int functionParse;

void dataInit();
void parse();//
void prog();//
void exec();//
void func();//
void func_types();//
void types_n();//
void type();//
void params(HTab_t* localtab);//
void params_n(HTab_t* localtab);//
void func_n();//
void body(HTab_t* localtab);//
void statement_n(HTab_t* localtab);//
void statement(HTab_t* localtab);//
void definition(HTab_t* localtab);//
void assignment(HTab_t* localtab);//
void id(HTab_t* localtab);//
void id_n(HTab_t* localtab);//
void expressions(HTab_t* localtab);//
void expression_n(HTab_t* localtab);//
void _if(HTab_t* localtab);//
void _for(HTab_t* localtab);
void _call(HTab_t* localtab);//
void _return(HTab_t* localtab);//
void _call_param(HTab_t* localtab);
void _call_param_n(HTab_t* localtab);
void expression(HTab_t* localtab);

#endif // PARSER_H