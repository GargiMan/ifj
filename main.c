/**
 * @file main.c
 * @author Marek Gergel (xgerge01)
 * @brief IFJ20 Compiler
 */

#include <stdlib.h>
#include <stdio.h>
#include "list.h"
#include "scanner.h"
#include "error.h"

//token list
List_t list = {NULL, NULL};

int main() {

    //scanner
    getTokens();

    //parser




    //debug
    ListItem_t* temp = LIST_HEAD;
    while (temp) {
        printf("%s\n",temp->token->value);
        temp = ITEM_NEXT(temp);
    }

    //free resources
    LIST_CLEAR;

    return 0;
}