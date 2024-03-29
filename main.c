/**
 * @file main.c
 * @author Marek Gergel (xgerge01)
 * @brief IFJ20 Main function
 */

#include <stdio.h>
#include "tokenlist.h"
#include "scanner.h"
#include "error.h"
#include "resources.h"
#include "parser.h"

//global variables
List_t list = {NULL, NULL};
HTab_t* globaltab = NULL;

int main() {

    //scanner
    getTokens();

    //parser
    parse();

    FREE_RESOURCES;

    return 0;
}