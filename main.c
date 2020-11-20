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
LList_t list = {NULL, NULL};

int main(int argc, char* argv[]) {

    //input select
    FILE* fd = stdin;
    if (argc == 2) fd = fopen(argv[1],"r");
    if (!fd) {
        errorExit(internalError, "File descriptor is incorrect\n");
        return 1;
    }

    //read
    readFile(fd);

    //print
    LListItem_t* temp = LIST_HEAD;
    while (temp) {
        printf("%s\n",temp->token);
        temp = ITEM_NEXT(temp);
    }

    //free resources
    LIST_CLEAR;
    fclose(fd);


    return 0;
}