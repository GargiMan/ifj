/**
 * @file scanner.c
 * @author Marek Gergel (xgerge01)
 * @brief IFJ20 Compiler
 */

#include "scanner.h"

//makro create signle/double char token

void getTokens() {

    int c = 0, c2 = 0;
    String_t* string = NULL;
    Token_t* token = NULL;

    while (!IS_EOF(c)) {

        if (c2) {
            c = c2;
            c2 = 0;
        } else {
            c = getchar();
        }

        switch (c) {

            case '\t': case '\0': case ' ':     //not term
                break;

            case '\n': case '\r':               //eol
                TOKEN_CREATE(token);
                TOKEN_SET_VALUE(token, strAppendChar(NULL, c));
                TOKEN_SET_TYPE(token, EOL);
                LIST_ADD_TOKEN(token);
                break;

            case ':':                           //colon ->
                switch (c2 = getchar()) {
                    case '=':                           //declare
                        TOKEN_CREATE(token);
                        string = strAppendChar(NULL, c);
                        string = strAppendChar(string, c2);
                        TOKEN_SET_VALUE(token, string);
                        TOKEN_SET_TYPE(token, OPERATOR_DEFINE);
                        LIST_ADD_TOKEN(token);
                        c2 = 0;
                        break;
                    default:                            //error
                        strDestroy(string);
                        errorExit(lexicalError, "scanner : Operator '%c' does not exist\n", c);
                        break;
                }
                break;

            case '=':                           //equals ->
                switch (c2 = getchar()) {
                    case '=':                           //equals to
                        TOKEN_CREATE(token);
                        string = strAppendChar(NULL, c);
                        string = strAppendChar(string, c2);
                        TOKEN_SET_VALUE(token, string);
                        TOKEN_SET_TYPE(token, OPERATOR_EQUAL);
                        LIST_ADD_TOKEN(token);
                        c2 = 0;
                        break;
                    default:                            //assign
                        TOKEN_CREATE(token);
                        TOKEN_SET_VALUE(token, strAppendChar(NULL, c));
                        TOKEN_SET_TYPE(token, OPERATOR_ASSIGN);
                        LIST_ADD_TOKEN(token);
                        break;
                }
                break;

            case '+':                           //plus
                TOKEN_CREATE(token);
                TOKEN_SET_VALUE(token, strAppendChar(NULL, c));
                TOKEN_SET_TYPE(token, OPERATOR_PLUS);
                LIST_ADD_TOKEN(token);
                break;

            case '-':                           //minus
                TOKEN_CREATE(token);
                TOKEN_SET_VALUE(token, strAppendChar(NULL, c));
                TOKEN_SET_TYPE(token, OPERATOR_MINUS);
                LIST_ADD_TOKEN(token);
                break;

            case '*':                           //mul
                TOKEN_CREATE(token);
                TOKEN_SET_VALUE(token, strAppendChar(NULL, c));
                TOKEN_SET_TYPE(token, OPERATOR_MUL);
                LIST_ADD_TOKEN(token);
                break;

            case '/':                           //slash ->
                switch (c2 = getchar()) {
                    case '/':                           //single line comment
                        while (!(IS_EOL(c2) || IS_EOF(c2))) {
                            c2 = getchar();
                        }
                        c2 = 0;
                        break;
                    case '*':                           //multi line comment
                        while (!(((c2 = getchar()) == '*') && (getchar() == '/'))) {
                            if (IS_EOF(c2)) {
                                strDestroy(string);
                                errorExit(lexicalError, "Multi-line comment has not ending, reached EOF\n");
                            }
                        }
                        c2 = 0;
                        break;
                    default:                            //div
                        TOKEN_CREATE(token);
                        TOKEN_SET_VALUE(token, strAppendChar(NULL, c));
                        TOKEN_SET_TYPE(token, OPERATOR_DIV);
                        LIST_ADD_TOKEN(token);
                        break;
                }
                break;

            case '&':                           //or ->
                switch (c2 = getchar()) {
                    case '&':                           //and
                        TOKEN_CREATE(token);
                        string = strAppendChar(NULL, c);
                        string = strAppendChar(string, c2);
                        TOKEN_SET_VALUE(token, string);
                        TOKEN_SET_TYPE(token, OPERATOR_AND);
                        LIST_ADD_TOKEN(token);
                        c2 = 0;
                        break;
                    default:                            //error
                        strDestroy(string);
                        errorExit(lexicalError, "scanner : Operator '%c' does not exist\n", c);
                        break;
                }
                break;

            case '|':                           //or ->
                switch (c2 = getchar()) {
                    case '|':                           //or
                        TOKEN_CREATE(token);
                        string = strAppendChar(NULL, c);
                        string = strAppendChar(string, c2);
                        TOKEN_SET_VALUE(token, string);
                        TOKEN_SET_TYPE(token, OPERATOR_OR);
                        LIST_ADD_TOKEN(token);
                        c2 = 0;
                        break;
                    default:                            //error
                        strDestroy(string);
                        errorExit(lexicalError, "scanner : Operator '%c' does not exist\n", c);
                        break;
                }
                break;

            case '!':                           //not ->
                switch (c2 = getchar()) {
                    case '=':                           //not equal
                        TOKEN_CREATE(token);
                        string = strAppendChar(NULL, c);
                        string = strAppendChar(string, c2);
                        TOKEN_SET_VALUE(token, string);
                        TOKEN_SET_TYPE(token, OPERATOR_NOT_EQUAL);
                        LIST_ADD_TOKEN(token);
                        c2 = 0;
                        break;
                    default:                            //not
                        TOKEN_CREATE(token);
                        TOKEN_SET_VALUE(token, strAppendChar(NULL, c));
                        TOKEN_SET_TYPE(token, OPERATOR_NOT);
                        LIST_ADD_TOKEN(token);
                        break;
                }
                break;

            case '>':                           //greater ->
                switch (c2 = getchar()) {
                    case '=':                           //greater or equal to
                        TOKEN_CREATE(token);
                        string = strAppendChar(NULL, c);
                        string = strAppendChar(string, c2);
                        TOKEN_SET_VALUE(token, string);
                        TOKEN_SET_TYPE(token, OPERATOR_GREATER_OR_EQUAL);
                        LIST_ADD_TOKEN(token);
                        c2 = 0;
                        break;
                    default:                            //greater
                        TOKEN_CREATE(token);
                        TOKEN_SET_VALUE(token, strAppendChar(NULL, c));
                        TOKEN_SET_TYPE(token, OPERATOR_GREATER);
                        LIST_ADD_TOKEN(token);
                        break;
                }
                break;

            case '<':                           //less ->
                switch (c2 = getchar()) {
                    case '=':                           //less or equal to
                        TOKEN_CREATE(token);
                        string = strAppendChar(NULL, c);
                        string = strAppendChar(string, c2);
                        TOKEN_SET_VALUE(token, string);
                        TOKEN_SET_TYPE(token, OPERATOR_LESS_OR_EQUAL);
                        LIST_ADD_TOKEN(token);
                        c2 = 0;
                        break;
                    default:                            //less
                        TOKEN_CREATE(token);
                        TOKEN_SET_VALUE(token, strAppendChar(NULL, c));
                        TOKEN_SET_TYPE(token, OPERATOR_LESS);
                        LIST_ADD_TOKEN(token);
                        break;
                }
                break;

            case '(':                           //round open
                TOKEN_CREATE(token);
                TOKEN_SET_VALUE(token, strAppendChar(NULL, c));
                TOKEN_SET_TYPE(token, BRACKET_ROUND_OPEN);
                LIST_ADD_TOKEN(token);
                break;

            case ')':                           //round close
                TOKEN_CREATE(token);
                TOKEN_SET_VALUE(token, strAppendChar(NULL, c));
                TOKEN_SET_TYPE(token, BRACKET_ROUND_CLOSE);
                LIST_ADD_TOKEN(token);
                break;

            case '{':                           //curly open
                TOKEN_CREATE(token);
                TOKEN_SET_VALUE(token, strAppendChar(NULL, c));
                TOKEN_SET_TYPE(token, BRACKET_CURLY_OPEN);
                LIST_ADD_TOKEN(token);
                break;

            case '}':                           //curly close
                TOKEN_CREATE(token);
                TOKEN_SET_VALUE(token, strAppendChar(NULL, c));
                TOKEN_SET_TYPE(token, BRACKET_CURLY_CLOSE);
                LIST_ADD_TOKEN(token);
                break;

            case ';':                           //semicolon
                TOKEN_CREATE(token);
                TOKEN_SET_VALUE(token, strAppendChar(NULL, c));
                TOKEN_SET_TYPE(token, SEMICOLON);
                LIST_ADD_TOKEN(token);
                break;

            case ',':                           //comma
                TOKEN_CREATE(token);
                TOKEN_SET_VALUE(token, strAppendChar(NULL, c));
                TOKEN_SET_TYPE(token, COMMA);
                LIST_ADD_TOKEN(token);
                break;

            case '"':                           //double quote
                TOKEN_CREATE(token);
                string = NULL;

                while (!IS_DOUBLE_QUOTE(c2 = getchar())) {

                    if (IS_EOF(c2)) {
                        strDestroy(string);
                        errorExit(lexicalError, "String has no ending, reached EOF\n");
                    }

                    if (c2 == '\\') {               //escape sequence
                        string = strAppendChar(string, c2);
                        switch (c2 = getchar()){
                            case '"': case 'n': case 't': case '\\':
                                string = strAppendChar(string, c2);
                                break;
                            case 'x':
                                string = strAppendChar(string, c2);           //add x
                                c = getchar();                      //read first hex
                                c2 = getchar();                     //read second hex
                                //check hex number
                                if (!(IS_NUMBER_HEX(c) && IS_NUMBER_HEX(c2))) {
                                    strDestroy(string);
                                    errorExit(lexicalError, "Escape sequence '\\x%c%c' has incorrect hexadecimal value\n", c, c2);
                                }
                                string = strAppendChar(string, c);            //write first hex;
                                string = strAppendChar(string, c2);           //write second hex;
                                break;
                            default:
                                errorExit(lexicalError, "Escape sequence '\\%c' does not exist\n", c2);
                                break;
                        }
                    } else {
                        string = strAppendChar(string, c2);
                    }
                }

                c2 = 0;
                TOKEN_SET_VALUE(token, string);
                TOKEN_SET_TYPE(token, DATA_STRING);
                LIST_ADD_TOKEN(token);
                break;

                                                //numbers
            case '.': case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':
    
                if (IS_DOT(c)) {
                    strDestroy(string);
                    errorExit(lexicalError, "Dot can not be at beginning of float number\n");
                }

                TOKEN_CREATE(token);
                string = strAppendChar(NULL, c);
                int state = 0; //0 int , 1 float64 , 2 exponent , 3 float64 and exponent

                while ((c2 = getchar())) {
                    if (IS_EOL(c2)) break;
                    switch (c2) {
                        case '.':
                            if (state & 1) {
                                strDestroy(string);
                                errorExit(lexicalError, "More than one dot in number\n");
                            } else {
                                state |= 1;
                                string = strAppendChar(string, c2);
                            }
                            continue;

                        case 'e': case 'E':
                            if (state & 2) {
                                strDestroy(string);
                                errorExit(lexicalError, "More than one exponent in number\n");
                            } else {
                                state |= 2;
                                string = strAppendChar(string, c2);
                            }
                            continue;

                        default:
                            if (IS_NOT_TERM(c2)) break;
                            if (IS_EXPONENT(string->str[string->len - 1]) && IS_ZERO_CHAR(c2)) {
                                strDestroy(string);
                                errorExit(lexicalError, "Redundant zeroes at beginning of exponent\n");
                            }
                            if (IS_ZERO_CHAR(c) && IS_ZERO_CHAR(c2)) {
                                strDestroy(string);
                                errorExit(lexicalError, "Redundant zeroes at beginning of number\n");
                            }
                            if (!IS_NUMBER(c2)) break;
                            string = strAppendChar(string, c2);
                            continue;
                    }
                    break;
                }

                if (IS_NUMBER(string->str[string->len - 2]) && IS_ZERO_CHAR(c2)) {
                    strDestroy(string);
                    errorExit(lexicalError, "Redundant zeroes after number\n");
                }
                if (!IS_EOL(c2)) c2 = 0;

                TOKEN_SET_VALUE(token, string);
                TOKEN_SET_TYPE(token, (state & 1 ? DATA_FLOAT64 : DATA_INT));
                LIST_ADD_TOKEN(token);
                break;

            default:                            //keywords/identifier

                /*
                if (IS_APLHA(c) || IS_UNDERSCORE(c)) {          //identifier
                TOKEN_CREATE(token);
                string = strAppendChar(NULL, c);
                    while (IS_UNDERSCORE(c2 = getchar()) || IS_APLHA(c2) || IS_NUMBER(c2)) {
                        string = strAppendChar(string, c2);
                    }
                    TOKEN_SET_VALUE(token, string);
                    TOKEN_SET_TYPE(token, IDENTIFIER);
                    LIST_ADD_TOKEN(token);
                }
                */

                break;
        }
    } 

    return;
}