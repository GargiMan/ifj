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
int statements();//
int statement();//
int definition();//
int assignment();//
int ids();//
int ids_n();//
int expressions();//
int expression_n();//
int expression();
int _if();//
int _for();
int _call();//
int _return();//

#endif