/**
 * @file main.c
 * @author Marek Gergel (xgerge01)
 * @brief IFJ20 Main function
 */

//#define DEBUG 1     //uncomment for debug info

#include <stdio.h>
#include "tokenlist.h"
#include "scanner.h"
#include "error.h"
#include "resources.h"

char* rtrType(TokenType_t type);

//global variables
List_t list = {NULL, NULL};
HTab_t* globaltab = NULL;

int main() {

    //scanner
    getTokens();

    //parser




    //debug
    #ifdef DEBUG
    Token_t* temp = LIST_HEAD;
    while (temp) {
        if (temp->type == ID || temp->type == DATA_STRING || temp->type == DATA_INT || temp->type == DATA_FLOAT64) {
            printf("%d\t | %s\n", temp->type, temp->value);
        } else {
            printf("%d\t | %s\n", temp->type, rtrType(temp->type));
        }
        
        TOKEN_MOVE_NEXT(temp);
    }
    #endif

    FREE_RESOURCES;

    return 0;
}


char* rtrType(TokenType_t type) {
    switch (type) {
        case EOL:
            return "EOL";
        case KEYWORD_INT:
            return "int";
        case KEYWORD_FLOAT64:
            return "float64";
        case KEYWORD_STRING:
            return "string";
        case KEYWORD_IF:
            return "if";
        case KEYWORD_ELSE:
            return "else";
        case KEYWORD_FOR:
            return "for";
        case KEYWORD_FUNC:
            return "func";
        case KEYWORD_RETURN:
            return "return";
        case KEYWORD_PACKAGE:
            return "package";
        case OPERATOR_DEFINE:
            return ":=";
        case OPERATOR_ASSIGN:
            return "=";
        case OPERATOR_PLUS:
            return "+";
        case OPERATOR_MINUS:
            return "-";
        case OPERATOR_MUL:
            return "*";
        case OPERATOR_DIV:
            return "/";
        case OPERATOR_AND:
            return "&&";
        case OPERATOR_OR:
            return "||";
        case OPERATOR_NOT:
            return "!";
        case OPERATOR_NOT_EQUAL:
            return "!=";
        case OPERATOR_EQUAL:
            return "==";
        case OPERATOR_GREATER:
            return ">";
        case OPERATOR_GREATER_OR_EQUAL:
            return ">=";
        case OPERATOR_LESS:
            return "<";
        case OPERATOR_LESS_OR_EQUAL:
            return "<=";
        case BRACKET_ROUND_OPEN:
            return "(";
        case BRACKET_ROUND_CLOSE:
            return ")";
        case BRACKET_CURLY_OPEN:
            return "{";
        case BRACKET_CURLY_CLOSE:
            return "}";
        case SEMICOLON:
            return ";";
        case COMMA:
            return ",";
        default:
            return NULL;
    }
}