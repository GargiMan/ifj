/**
 * @file scanner.c
 * @author Marek Gergel (xgerge01)
 * @brief IFJ20 Compiler
 */

#include "scanner.h"

void getTokens() {

    int c = 0, c2 = 0;
    String_t s = NULL;
    Token_t* token = NULL;

    while (!IS_EOF(c)) {

        if (c2) {
            c = c2;
            c2 = 0;
        } else {
            c = getchar();
        }

        switch (c) {

            case '\n': case '\r':               //eol
                TOKEN_CREATE(token);
                TOKEN_SET_VALUE(token, appendChar(NULL, c));
                LIST_ADD_TOKEN(token);
                break;

            case EOF: case '\t': case ' ':      //not term , skip
                break;

            case '/':                           //slash ->
                switch (c2 = getchar()) {
                    case '/':                           //single line comment
                        while (!IS_EOL(c2)) {
                            c2 = getchar();
                        }
                        c2 = 0;
                        break;
                    case '*':                           //multi line comment
                        while (!IS_EOF(c2)) if (((getchar() == '*') && (getchar() == '/'))) break;
                        c2 = 0;
                        break;
                    default:                            //div
                        TOKEN_CREATE(token);
                        TOKEN_SET_VALUE(token, appendChar(NULL, c));
                        LIST_ADD_TOKEN(token);
                        break;
                }
                break;

            case '*':                           //mul
                TOKEN_CREATE(token);
                TOKEN_SET_VALUE(token, appendChar(NULL, c));
                LIST_ADD_TOKEN(token);
                break;


            case ':':                           //colon ->
                switch (c2 = getchar()) {
                    case '=':                           //declare
                        TOKEN_CREATE(token);
                        s = appendChar(NULL, c);
                        s = appendChar(s, c2);
                        TOKEN_SET_VALUE(token, s);
                        LIST_ADD_TOKEN(token);
                        c2 = 0;
                        break;
                    default:                            //error
                        errorExit(lexicalError, "scanner : Operator ':' is incorrect");
                        break;
                }
                break;

            case '=':                           //equals ->
                switch (c2 = getchar()) {
                    case '=':                           //equals to
                        TOKEN_CREATE(token);
                        s = appendChar(NULL, c);
                        s = appendChar(s, c2);
                        TOKEN_SET_VALUE(token, s);
                        LIST_ADD_TOKEN(token);
                        c2 = 0;
                        break;
                    default:                            //assign
                        TOKEN_CREATE(token);
                        TOKEN_SET_VALUE(token, appendChar(NULL, c));
                        LIST_ADD_TOKEN(token);
                        break;
                }
                break;

            case '>':                           //greater
                switch (c2 = getchar()) {
                    case '=':                           //greater or equal to
                        TOKEN_CREATE(token);
                        s = appendChar(NULL, c);
                        s = appendChar(s, c2);
                        TOKEN_SET_VALUE(token, s);
                        LIST_ADD_TOKEN(token);
                        c2 = 0;
                        break;
                    default:                            //greater than
                        TOKEN_CREATE(token);
                        TOKEN_SET_VALUE(token, appendChar(NULL, c));
                        LIST_ADD_TOKEN(token);
                        break;
                }
                break;

            case '<':                           //less
                switch (c2 = getchar()) {
                    case '=':                           //less or equal to
                        TOKEN_CREATE(token);
                        s = appendChar(NULL, c);
                        s = appendChar(s, c2);
                        TOKEN_SET_VALUE(token, s);
                        LIST_ADD_TOKEN(token);
                        c2 = 0;
                        break;
                    default:                            //less than
                        TOKEN_CREATE(token);
                        TOKEN_SET_VALUE(token, appendChar(NULL, c));
                        LIST_ADD_TOKEN(token);
                        break;
                }
                break;

            default:
                break;
        }
    } 

    return;
}