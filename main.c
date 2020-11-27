/**
 * @file main.c
 * @author Marek Gergel (xgerge01)
 * @brief IFJ20 Compiler
 */

#include <stdio.h>
#include "tokenlist.h"
#include "scanner.h"
#include "error.h"
#include "resources.h"

//token list
List_t list = {NULL, NULL};

int main() {

    //scanner
    getTokens();

    //parser


    //debug
    Token_t* temp = LIST_HEAD;
    while (temp) {
        printf("%s\n",(temp->type == EOL ? "EOL" : temp->value->str));
        TOKEN_MOVE_NEXT(temp);
    }

    FREE_RESOURCES;

    return 0;
}