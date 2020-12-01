/**
 * @file scanner.c
 * @author Marek Gergel (xgerge01)
 * @brief IFJ20 Compiler
 */

#include "scanner.h"

void getTokens() {

    int c = 0, c2 = 0;
    Token_t* token = NULL;
    String_t string = {NULL, 0};

    while (!IS_EOF(c)) {

        if (c2) {
            c = c2;
            c2 = 0;
        } else {
            c = getchar();
        }

        if (IS_NOT_TERM(c)) {                           //not term
            continue;

        } else if (IS_EOL(c)) {                         //eol
            LIST_ADD_TOKEN_WT(token, EOL);

        } else if (IS_COLON(c)) {                       //define
            if (!IS_EQUALS(getchar())) errorExit(lexicalError, "scanner : Operator ':' does not exist\n");
            LIST_ADD_TOKEN_WT(token, OPERATOR_DEFINE);

        } else if (IS_EQUALS(c)) {                      //assign / equals to
            if (IS_EQUALS(c2 = getchar())) {
                LIST_ADD_TOKEN_WT(token, OPERATOR_EQUAL);
                c2 = 0;
            } else {
                LIST_ADD_TOKEN_WT(token, OPERATOR_ASSIGN);
            }

        } else if (IS_PLUS(c)) {                        //plus
            LIST_ADD_TOKEN_WT(token, OPERATOR_PLUS);

        } else if (IS_MINUS(c)) {                       //minus
            LIST_ADD_TOKEN_WT(token, OPERATOR_MINUS);

        } else if (IS_ASTERISK(c)) {                    //mul
            LIST_ADD_TOKEN_WT(token, OPERATOR_MUL);

        } else if (IS_SLASH(c)) {                       //div / coment
            if (IS_SLASH(c2 = getchar())) {
                while (!(IS_EOL(c2) || IS_EOF(c2))) {
                    c2 = getchar();
                }
                c2 = 0;
            } else if (IS_ASTERISK(c2)) {
                while (!(((c2 = getchar()) == '*') && (getchar() == '/'))) {
                    if (IS_EOF(c2)) errorExit(lexicalError, "Multi-line comment has not ending, reached EOF\n");
                }
                c2 = 0;
            } else {
                LIST_ADD_TOKEN_WT(token, OPERATOR_DIV);
            }

        } else if (IS_AMPERSAND(c)) {                   //and
            if (!IS_AMPERSAND(getchar())) errorExit(lexicalError, "scanner : Operator '&' does not exist\n");
            LIST_ADD_TOKEN_WT(token, OPERATOR_AND);

        } else if (IS_VERTICAL_BAR(c)) {                //or
            if (!IS_VERTICAL_BAR(getchar())) errorExit(lexicalError, "scanner : Operator '|' does not exist\n");
            LIST_ADD_TOKEN_WT(token, OPERATOR_OR);

        } else if (IS_EXCLAMATION(c)) {                 //not / not equal
            if (IS_EQUALS(c2 = getchar())) {
                LIST_ADD_TOKEN_WT(token, OPERATOR_NOT_EQUAL);
                c2 = 0;
            } else {
                LIST_ADD_TOKEN_WT(token, OPERATOR_NOT);
            }

        } else if (IS_GREATER(c)) {                     //greater / greater or equal
            if (IS_EQUALS(c2 = getchar())) {
                LIST_ADD_TOKEN_WT(token, OPERATOR_GREATER_OR_EQUAL);
                c2 = 0;
            } else {
                LIST_ADD_TOKEN_WT(token, OPERATOR_GREATER);
            }

        } else if (IS_LESS(c)) {                        //less / less or equal
            if (IS_EQUALS(c2 = getchar())) {
                LIST_ADD_TOKEN_WT(token, OPERATOR_LESS_OR_EQUAL);
                c2 = 0;
            } else {
                LIST_ADD_TOKEN_WT(token, OPERATOR_LESS);
            }

        } else if (IS_ROUND_OPEN(c)) {                  //round open
            LIST_ADD_TOKEN_WT(token, BRACKET_ROUND_OPEN);

        } else if (IS_ROUND_CLOSE(c)) {                 //round close
            LIST_ADD_TOKEN_WT(token, BRACKET_ROUND_CLOSE);

        } else if (IS_CURLY_OPEN(c)) {                  //curly open
            LIST_ADD_TOKEN_WT(token, BRACKET_CURLY_OPEN);

        } else if (IS_CURLY_CLOSE(c)) {                 //curly close
            LIST_ADD_TOKEN_WT(token, BRACKET_CURLY_CLOSE);

        } else if (IS_SEMICOLON(c)) {                   //semicolon
            LIST_ADD_TOKEN_WT(token, SEMICOLON);

        } else if (IS_COMMA(c)) {                       //comma
            LIST_ADD_TOKEN_WT(token, COMMA);

        } else if (IS_DOUBLE_QUOTE(c)) {                //string
            TOKEN_CREATE(token);
            strClear(&string);

            while (!IS_DOUBLE_QUOTE(c2 = getchar())) {

                if (IS_EOF(c2)) {
                    strDestroy(&string);
                    errorExit(lexicalError, "String has no ending, reached EOF\n");
                }

                if (c2 == '\\') {                                   //escape sequence
                    strAppendChar(&string, c2);
                    switch (c2 = getchar()){
                        case '"': case 'n': case 't': case '\\':
                            strAppendChar(&string, c2);
                            break;
                        case 'x':
                            strAppendChar(&string, c2);             //add x
                            c = getchar();                          //read first hex
                            c2 = getchar();                         //read second hex
                                                                    //check hex value
                            if (!(IS_NUMBER_HEX(c) && IS_NUMBER_HEX(c2))) {
                                strDestroy(&string);
                                errorExit(lexicalError, "Escape sequence '\\x%c%c' has incorrect hexadecimal value\n", c, c2);
                            }
                            strAppendChar(&string, c);              //write first hex;
                            strAppendChar(&string, c2);             //write second hex;
                            break;
                        default:
                            strDestroy(&string);
                            errorExit(lexicalError, "Escape sequence '\\%c' does not exist\n", c2);
                            break;
                    }
                } else {
                    strAppendChar(&string, c2);
                }
            }

            c2 = 0;
            TOKEN_SET_VALUE(token, string.str);
            TOKEN_SET_TYPE(token, DATA_STRING);
            LIST_ADD_TOKEN(token);
        
        } else if (IS_NUMBER(c)) {                      //int or float

            TOKEN_CREATE(token);
            strClear(&string);
            strAppendChar(&string, c);
            int state = 0; //0 int , 1 float64 , 2 int exponent , 3 float64 exponent

            while ((c2 = getchar())) {
                if (IS_EOL(c2)) break;
                switch (c2) {
                    case '.':
                        if (state & 1) {
                            strDestroy(&string);
                            errorExit(lexicalError, "More than one dot in number\n");
                        } else {
                            state |= 1;
                            strAppendChar(&string, c2);
                        }
                        continue;

                    case 'e': case 'E':
                        if (state & 2) {
                            strDestroy(&string);
                            errorExit(lexicalError, "More than one exponent in number\n");
                        } else {
                            state |= 2;
                            strAppendChar(&string, c2);
                        }
                        continue;

                    default:
                        if (IS_NOT_TERM(c2)) break;
                        if (IS_EXPONENT(string.str[string.len - 1]) && IS_ZERO_CHAR(c2)) {
                            strDestroy(&string);
                            errorExit(lexicalError, "Redundant zeroes at beginning of exponent\n");
                        }
                        if (IS_ZERO_CHAR(c) && IS_ZERO_CHAR(c2)) {
                            strDestroy(&string);
                            errorExit(lexicalError, "Redundant zeroes at beginning of number\n");
                        }
                        if (!IS_NUMBER(c2)) break;
                        strAppendChar(&string, c2);
                        continue;
                    }
                    break;
                }

            if (IS_NUMBER(string.str[string.len - 2]) && IS_ZERO_CHAR(c2)) {
                strDestroy(&string);
                errorExit(lexicalError, "Redundant zeroes after number\n");
            }
            if (!IS_EOL(c2)) c2 = 0;

            TOKEN_SET_VALUE(token, string.str);
            TOKEN_SET_TYPE(token, (state & 1 ? DATA_FLOAT64 : DATA_INT));
            LIST_ADD_TOKEN(token);

        } else if (IS_APLHA(c) || IS_UNDERSCORE(c)) {   //id
            /*
            TOKEN_CREATE(token);
            strClear(&string);
            strAppendChar(&string, c);
                while (IS_UNDERSCORE(c2 = getchar()) || IS_APLHA(c2) || IS_NUMBER(c2)) {
                    strAppendChar(&string, c2);
                }
                TOKEN_SET_VALUE(token, string.str);
                TOKEN_SET_TYPE(token, IDENTIFIER);
                LIST_ADD_TOKEN(token);
            }
            */

        } else {
            errorExit(lexicalError, "Can not create valid token with '%c' at beginning\n", c);
        }
    } 

    return;
}