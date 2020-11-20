/**
 * @file scanner.c
 * @author Marek Gergel (xgerge01)
 * @brief IFJ20 Compiler
 */

#include "scanner.h"

void readFile() {

    int c = 0, c2 = 0;
    String_t s = NULL;
    LListItem_t* temp = NULL;

    while (!IS_EOF(c)) {

        if (c2) {
            c = c2;
            c2 = 0;
        } else {
            c = getchar();
        }

        //slash -> comments or div
        if (IS_SLASH(c)) {

            c2 = getchar();

            //signle line
            if (IS_SLASH(c2)) {
                while (!IS_EOL(c2)) c2 = getchar();
                c2 = 0;
                continue;
            }

            //multi line
            if (IS_ASTERISK(c2)) {
                while (!IS_EOF(c2 = getchar())) { 
                    if (IS_ASTERISK(c2) && IS_SLASH(c2 = getchar())) break;
                }
                c2 = 0;
                continue;
            }
        }

        if (!temp && !IS_NOT_TERM(c)) { //need to add slash or destroy item
            ITEM_CREATE(temp);
            if (!temp) errorExit(internalError, "scanner.h : Item allocation failed");
            s = NULL;
        } 

        //not terminals
        if (IS_NOT_TERM(c)) {
            if (!temp) continue;
            temp->token = s;
            LIST_ADD_ITEM(temp);
            temp = NULL;
            continue;
        }

        //add char
        s = appendChar(s, c);
    } 

    return;
}