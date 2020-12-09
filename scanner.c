/**
 * @file scanner.c
 * @author Marek Gergel (xgerge01)
 * @brief IFJ20 - Read tokens from stdin to list
 */

#include "scanner.h"

void getTokens() {

    int c = 0;
    Token_t* token = NULL;
    String_t string = {NULL, 0};

    while (!IS_EOF(c = getchar())) {

        if (IS_NOT_TOKEN(c)) {                          //not token
            continue;

        } else if (IS_EOL(c)) {                         //eol
            if (token != NULL && TOKEN_TYPE(token) == EOL) continue;
            LIST_ADD_TOKEN_WT(list, token, EOL);

        } else if (IS_COLON(c)) {                       //define
            if (!IS_EQUALS(getchar())) errorExit(lexicalError, "scanner : Operator ':' does not exist\n");
            LIST_ADD_TOKEN_WT(list, token, OPERATOR_DEFINE);

        } else if (IS_EQUALS(c)) {                      //assign / equals to
            if (IS_EQUALS(c = getchar())) {
                LIST_ADD_TOKEN_WT(list, token, OPERATOR_EQUAL);
            } else {
                LIST_ADD_TOKEN_WT(list, token, OPERATOR_ASSIGN);
                ungetc(c, stdin);
            }

        } else if (IS_PLUS(c)) {                        //plus
            LIST_ADD_TOKEN_WT(list, token, OPERATOR_PLUS);

        } else if (IS_MINUS(c)) {                       //minus
            LIST_ADD_TOKEN_WT(list, token, OPERATOR_MINUS);

        } else if (IS_ASTERISK(c)) {                    //mul
            LIST_ADD_TOKEN_WT(list, token, OPERATOR_MUL);

        } else if (IS_SLASH(c)) {                       //div / coment
            if (IS_SLASH(c = getchar())) {                          //single line comment
                while (!(IS_EOL(c) || IS_EOF(c))) c = getchar();
            } else if (IS_ASTERISK(c)) {                            //multi-line comment
                while (!(((c = getchar()) == '*') && (getchar() == '/'))) {
                    if (IS_EOF(c)) errorExit(lexicalError, "scanner : Multi-line comment has not ending, reached EOF\n");
                }
            } else {                                                //div
                LIST_ADD_TOKEN_WT(list, token, OPERATOR_DIV);
                ungetc(c, stdin);
            }

        } else if (IS_AMPERSAND(c)) {                   //and
            if (!IS_AMPERSAND(getchar())) errorExit(lexicalError, "scanner : Operator '&' does not exist\n");
            LIST_ADD_TOKEN_WT(list, token, OPERATOR_AND);

        } else if (IS_VERTICAL_BAR(c)) {                //or
            if (!IS_VERTICAL_BAR(getchar())) errorExit(lexicalError, "scanner : Operator '|' does not exist\n");
            LIST_ADD_TOKEN_WT(list, token, OPERATOR_OR);

        } else if (IS_EXCLAMATION(c)) {                 //not / not equal
            if (IS_EQUALS(c = getchar())) {
                LIST_ADD_TOKEN_WT(list, token, OPERATOR_NOT_EQUAL);
            } else {
                LIST_ADD_TOKEN_WT(list, token, OPERATOR_NOT);
                ungetc(c, stdin);
            }

        } else if (IS_GREATER(c)) {                     //greater / greater or equal
            if (IS_EQUALS(c = getchar())) {
                LIST_ADD_TOKEN_WT(list, token, OPERATOR_GREATER_OR_EQUAL);
            } else {
                LIST_ADD_TOKEN_WT(list, token, OPERATOR_GREATER);
                ungetc(c, stdin);
            }

        } else if (IS_LESS(c)) {                        //less / less or equal
            if (IS_EQUALS(c = getchar())) {
                LIST_ADD_TOKEN_WT(list, token, OPERATOR_LESS_OR_EQUAL);
            } else {
                LIST_ADD_TOKEN_WT(list, token, OPERATOR_LESS);
                ungetc(c, stdin);
            }

        } else if (IS_ROUND_OPEN(c)) {                  //round open
            LIST_ADD_TOKEN_WT(list, token, BRACKET_ROUND_OPEN);

        } else if (IS_ROUND_CLOSE(c)) {                 //round close
            LIST_ADD_TOKEN_WT(list, token, BRACKET_ROUND_CLOSE);

        } else if (IS_CURLY_OPEN(c)) {                  //curly open
            LIST_ADD_TOKEN_WT(list, token, BRACKET_CURLY_OPEN);

        } else if (IS_CURLY_CLOSE(c)) {                 //curly close
            LIST_ADD_TOKEN_WT(list, token, BRACKET_CURLY_CLOSE);

        } else if (IS_SEMICOLON(c)) {                   //semicolon
            LIST_ADD_TOKEN_WT(list, token, SEMICOLON);

        } else if (IS_COMMA(c)) {                       //comma
            LIST_ADD_TOKEN_WT(list, token, COMMA);

        } else if (IS_DOUBLE_QUOTE(c)) {                //string
            TOKEN_CREATE(token);
            strClear(&string);

            while (!IS_DOUBLE_QUOTE(c = getchar())) {

                if (IS_EOF(c)) {
                    strDestroy(&string);
                    errorExit(lexicalError, "scanner : String has no ending, reached EOF\n");
                }

                if (IS_EOL(c)) {
                    strDestroy(&string);
                    errorExit(lexicalError, "scanner : String has no ending, reached EOL\n");
                }

                strAppendChar(&string, c);

                if (c == '\\') {                                   //escape sequence
                    switch (c = getchar()) {

                        case '"': case 'n': case 't': case '\\':
                            strAppendChar(&string, c);
                            break;

                        case 'x':
                            strAppendChar(&string, c);              //write x
                            strAppendChar(&string, getchar());      //write first hex;
                            strAppendChar(&string, getchar());      //write second hex;
                                                                    //check hex value
                            if (!(IS_NUMBER_HEX(string.str[string.len - 1]) && IS_NUMBER_HEX(string.str[string.len - 2]))) {
                                strDestroy(&string);
                                errorExit(lexicalError, "scanner : Escape sequence '\\x%c%c' has incorrect hexadecimal value\n", string.str[string.len - 1], string.str[string.len - 2]);
                            }
                            break;

                        default:
                            strDestroy(&string);
                            errorExit(lexicalError, "scanner : Escape sequence '\\%c' does not exist\n", c);
                            break;
                    }
                }
            }

            TOKEN_SET_VALUE(token, string.str);
            TOKEN_SET_TYPE(token, DATA_STRING);
            LIST_ADD_TOKEN(list, token);
        
        } else if (IS_NUMBER(c)) {                      //int or float

            TOKEN_CREATE(token);
            strClear(&string);
            strAppendChar(&string, c);
            int state = 0;          //0 int , 1 float64 , 2 int exponent , 3 float64 exponent
            int expsignset = 0;     //0 unset , 1 set

            while ((c = getchar())) {

                if (IS_DOT(c)) {
                    if (state & 1) {
                        strDestroy(&string);
                        errorExit(lexicalError, "scanner : More than one dot in number\n");
                    } else {

                        if (state == 2) {
                            strDestroy(&string);
                            errorExit(lexicalError, "scanner : Exponent value must be at the end of float value\n");
                        }

                        state |= 1;
                        strAppendChar(&string, c);
                    }

                } else if (IS_EXPONENT(c)) {
                    if (state & 2) {
                        strDestroy(&string);
                        errorExit(lexicalError, "scanner : More than one exponent in number\n");
                    } else {

                        if (IS_DOT(string.str[string.len - 1])) {
                            strDestroy(&string);
                            errorExit(lexicalError, "scanner : Float value must have number after floating point\n");
                        }

                        if (state == 1 && IS_ZERO_CHAR(string.str[string.len - 1])) {
                            strDestroy(&string);
                            errorExit(lexicalError, "scanner : Redundant zeroes at end of float number before exponent value\n");
                        }

                        state |= 2;
                        strAppendChar(&string, c);
                    }

                } else if (IS_PLUS(c) || IS_MINUS(c)) {
                    if (!(expsignset) && (state & 2) && IS_EXPONENT(string.str[string.len - 1])) {
                        strAppendChar(&string, c);
                        expsignset = 1;
                    } else if (expsignset || !IS_EXPONENT(string.str[string.len - 1])) {
                        ungetc(c, stdin);
                        break;
                    } else {
                        strDestroy(&string);
                        errorExit(lexicalError, "scanner : Plus or minus character inside number can be only before exponent\n");
                    }

                } else if (IS_NUMBER(c)) {
                    strAppendChar(&string, c);
                    if (state == 0 && string.len > 1 && IS_ZERO_CHAR(string.str[0])) {  //zero at beginning
                        strDestroy(&string);
                        errorExit(lexicalError, "scanner : Redundant zeroes at beginning of number value\n");
                    }
                    
                } else {
                    ungetc(c, stdin);
                    break;
                }
            }

            if (state == 1 && IS_DOT(string.str[string.len - 1])) {
                strDestroy(&string);
                errorExit(lexicalError, "scanner : Float number is missing float value\n");
            }

            if (state == 1 && IS_ZERO_CHAR(string.str[string.len - 1]) && !IS_DOT(string.str[string.len - 2])) {
                strDestroy(&string);
                errorExit(lexicalError, "scanner : Redundant zeroes at end of float number value\n");
            }

            if ((state & 2) && (IS_EXPONENT(string.str[string.len - 1]) || IS_PLUS(string.str[string.len - 1]) || IS_MINUS(string.str[string.len - 1]))) {
                strDestroy(&string);
                errorExit(lexicalError, "scanner : Number with exponent is missing exponent value\n");
            }

            TOKEN_SET_VALUE(token, string.str);
            TOKEN_SET_TYPE(token, (state & 1 ? DATA_FLOAT64 : DATA_INT));
            LIST_ADD_TOKEN(list, token);

        } else if (IS_APLHA(c) || IS_UNDERSCORE(c)) {   //id or keyword
            
            TOKEN_CREATE(token);
            strClear(&string);
            strAppendChar(&string, c);
            TOKEN_SET_TYPE(token, ID);

            while (IS_UNDERSCORE(c = getchar()) || IS_APLHA(c) || IS_NUMBER(c)) {

                strAppendChar(&string, c);

                //check for keyword
                switch (string.len) {
                    case 2:
                        if (KW_IF(string.str)) TOKEN_SET_TYPE(token, KEYWORD_IF);
                        break;
                    case 3:
                        if (KW_INT(string.str)) TOKEN_SET_TYPE(token, KEYWORD_INT);
                        else if (KW_FOR(string.str)) TOKEN_SET_TYPE(token, KEYWORD_FOR);
                        break;
                    case 4:
                        if (KW_ELSE(string.str)) TOKEN_SET_TYPE(token, KEYWORD_ELSE);
                        else if (KW_FUNC(string.str)) TOKEN_SET_TYPE(token, KEYWORD_FUNC);
                        break;
                    case 6:
                        if (KW_RETURN(string.str)) TOKEN_SET_TYPE(token, KEYWORD_RETURN);
                        else if (KW_STRING(string.str)) TOKEN_SET_TYPE(token, KEYWORD_STRING);
                        break;
                    case 7:
                        if (KW_FLOAT64(string.str)) TOKEN_SET_TYPE(token, KEYWORD_FLOAT64);
                        else if (KW_PACKAGE(string.str)) TOKEN_SET_TYPE(token, KEYWORD_PACKAGE);
                        break;
                }
            }

            ungetc(c, stdin);

            if (TOKEN_TYPE(token) == ID) {
                TOKEN_SET_VALUE(token, string.str);
            } else {
                strDestroy(&string);
            }

            LIST_ADD_TOKEN(list, token);

        } else {                                        //eof or error
            if (IS_EOF(c)) return;
            errorExit(lexicalError, "scanner : Can not create valid token with %c character at beginning\n", c);
        }
    } 

    return;
}