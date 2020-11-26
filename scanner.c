/**
 * @file scanner.c
 * @author Marek Gergel (xgerge01)
 * @brief IFJ20 Compiler
 */

#include "scanner.h"

//makro create signle/double char token

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
                        s = strAppendChar(NULL, c);
                        s = strAppendChar(s, c2);
                        TOKEN_SET_VALUE(token, s);
                        TOKEN_SET_TYPE(token, OPERATOR_DEFINE);
                        LIST_ADD_TOKEN(token);
                        c2 = 0;
                        break;
                    default:                            //error
                        errorExit(lexicalError, "scanner : Operator '%c' does not exist\n", c);
                        break;
                }
                break;

            case '=':                           //equals ->
                switch (c2 = getchar()) {
                    case '=':                           //equals to
                        TOKEN_CREATE(token);
                        s = strAppendChar(NULL, c);
                        s = strAppendChar(s, c2);
                        TOKEN_SET_VALUE(token, s);
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
                            if (IS_EOF(c2)) errorExit(lexicalError, "Multi-line comment has not ending, reached EOF\n");
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
                        s = strAppendChar(NULL, c);
                        s = strAppendChar(s, c2);
                        TOKEN_SET_VALUE(token, s);
                        TOKEN_SET_TYPE(token, OPERATOR_AND);
                        LIST_ADD_TOKEN(token);
                        c2 = 0;
                        break;
                    default:                            //error
                        errorExit(lexicalError, "scanner : Operator '%c' does not exist\n", c);
                        break;
                }
                break;

            case '|':                           //or ->
                switch (c2 = getchar()) {
                    case '|':                           //or
                        TOKEN_CREATE(token);
                        s = strAppendChar(NULL, c);
                        s = strAppendChar(s, c2);
                        TOKEN_SET_VALUE(token, s);
                        TOKEN_SET_TYPE(token, OPERATOR_OR);
                        LIST_ADD_TOKEN(token);
                        c2 = 0;
                        break;
                    default:                            //error
                        errorExit(lexicalError, "scanner : Operator '%c' does not exist\n", c);
                        break;
                }
                break;

            case '!':                           //not ->
                switch (c2 = getchar()) {
                    case '=':                           //not equal
                        TOKEN_CREATE(token);
                        s = strAppendChar(NULL, c);
                        s = strAppendChar(s, c2);
                        TOKEN_SET_VALUE(token, s);
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
                        s = strAppendChar(NULL, c);
                        s = strAppendChar(s, c2);
                        TOKEN_SET_VALUE(token, s);
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
                        s = strAppendChar(NULL, c);
                        s = strAppendChar(s, c2);
                        TOKEN_SET_VALUE(token, s);
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
                s = NULL;

                while (!IS_DOUBLE_QUOTE(c2 = getchar())) {

                    if (IS_EOF(c2)) errorExit(lexicalError, "String '%s' has no ending, reached EOF\n", s);
            
                    if (c2 == '\\') {               //escape sequence
                        s = strAppendChar(s, c2);
                        switch (c2 = getchar()){
                            case '"': case 'n': case 't': case '\\':
                                s = strAppendChar(s, c2);
                                break;
                            case 'x':
                                s = strAppendChar(s, c2);
                                c = getchar();
                                c2 = getchar();
                                if (!(IS_NUMBER_HEX(c) && IS_NUMBER_HEX(c2))) errorExit(lexicalError, "Escape sequence '\\x%c%c' has incorrect hexadecimal value\n", c, c2);
                                s = strAppendChar(s, c);
                                s = strAppendChar(s, c2);
                                break;
                            default:
                                errorExit(lexicalError, "Escape sequence '\\%c' does not exist\n", c2);
                                break;
                        }
                    } else {
                        s = strAppendChar(s, c2);
                    }
                }

                c2 = 0;
                TOKEN_SET_VALUE(token, s);
                TOKEN_SET_TYPE(token, DATA_STRING);
                LIST_ADD_TOKEN(token);
                break;

            default:                            //keywords, data, identifier
                
                /*
                if (IS_APLHA(c) || IS_UNDERSCORE(c)) {          //identifier
                TOKEN_CREATE(token);
                s = strAppendChar(NULL, c);
                    while (IS_UNDERSCORE(c2 = getchar()) || IS_APLHA(c2) || IS_NUMBER(c2)) {
                        s = strAppendChar(s, c2);
                    }
                    TOKEN_SET_VALUE(token, s);
                    TOKEN_SET_TYPE(token, IDENTIFIER);
                    LIST_ADD_TOKEN(token);
                }
                */

                break;
        }
    } 

    return;
}