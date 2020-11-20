/**
 * @file scanner.c
 * @author Marek Gergel (xgerge01)
 * @brief IFJ20 Compiler
 */

#include "scanner.h"

void readFile(FILE* fd) {
    int c = 0;
    String_t s = NULL;

    if (!fd) errorExit(internalError, "scanner.h : File descriptor is not valid");

    LListItem_t* temp = NULL;

    while (!IS_EOF(c)) {

        c = getc(fd);

        if (!temp && !IS_NOT_TERM(c)) {
            ITEM_CREATE(temp);
            if (!temp) errorExit(internalError, "scanner.h : Item allocation failed");
            s = NULL;
        } 

        if (IS_NOT_TERM(c)) {
            if (!temp) continue;
            temp->token = s;
            LIST_ADD_ITEM(temp);
            temp = NULL;
            continue;
        }

        s = appendChar(s, c);
    } 

    return;
}