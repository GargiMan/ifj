/**
 * @file parser.c
 * @author Filip Sapak (xsapak05)
 * @brief IFJ20 - parser
 */

#include "parser.h"
Token_t *activeToken;
int decisionFlag;
#define NEXT_TOKEN(activeToken)(activeToken = activeToken->pNext)
#define syntaxOK  0
#define CHECK_OK(result) if (result != syntaxOK){return result;}
#define CHECK_TYPE(TOKENTYPE) \
    if (activeToken->type != TOKENTYPE){ \
        return syntaxError; \
    }
#define CHECK_EOF() \
    if (activeToken == list.pTail) {return syntaxOK;}else{return syntaxError;} ///// treba tokentype EOF

#define TEST_TYPE(TOKENTYPE) \
    if (activeToken->type == TOKENTYPE){ \
        decisionFlag = 1; \
    } else \
    { \
        decisionFlag = 0; \
    } \

#define DECIDE() \
    if (decisionFlag){return syntaxOK;} \

#define CHECK_STATE(state) \
    if (state() == syntaxError){ \
        return syntaxError; \
    }else{ return syntaxOK;} \

    



int parse(){

    activeToken = list.pHead;
    CHECK_STATE(prog);
    return syntaxOK;
}

//<prog> -> package id $ <exec>
int prog(){

   CHECK_TYPE(KEYWORD_PACKAGE); 
   NEXT_TOKEN(activeToken);
   CHECK_TYPE(IDENTIFIER);
    if(!strcmp("main",activeToken->value)){ // hodil by sa tokentype keyword main
        NEXT_TOKEN(activeToken);
        CHECK_TYPE(EOL); // $
        CHECK_STATE(exec);
        CHECK_EOF();
        return syntaxOK; //syntax ok
    
    }
    return syntaxError;
}


int exec(){
//<exec> -> <func> <func_n>
   
    NEXT_TOKEN(activeToken);
    CHECK_STATE(func);

    NEXT_TOKEN(activeToken);
    CHECK_STATE(func_n);

    
}
int func(){
//<func> -> func id (<params>)<func_types>{ $ <body>}
    
    CHECK_TYPE(KEYWORD_FUNC);
    NEXT_TOKEN(activeToken);
    CHECK_TYPE(IDENTIFIER);
    //TODO ulozit ID do symtable
    NEXT_TOKEN(activeToken);
    CHECK_TYPE(BRACKET_ROUND_OPEN);
    NEXT_TOKEN(activeToken); // id alebo eps
    CHECK_STATE(params);
    
    //NEXT_TOKEN();
    CHECK_TYPE(BRACKET_ROUND_CLOSE);
    NEXT_TOKEN(activeToken);
    CHECK_STATE(func_types);
    
    NEXT_TOKEN(activeToken);
    CHECK_TYPE(BRACKET_CURLY_OPEN);
    NEXT_TOKEN(activeToken);
    CHECK_TYPE(EOL);
    CHECK_STATE(body);

}
int params(){
    //<params> -> id <type> <p  arams_n> | eps
    
    TEST_TYPE(BRACKET_ROUND_CLOSE);
    DECIDE();
    CHECK_TYPE(IDENTIFIER);
    //TODO ulozit ID do symtable
    NEXT_TOKEN(activeToken);
    CHECK_STATE(type);
    NEXT_TOKEN(activeToken);
    TEST_TYPE(BRACKET_ROUND_CLOSE);
    DECIDE();
    CHECK_STATE(params_n);

}

int type(){
//<type> -> int | float64 | string
   
    TEST_TYPE(KEYWORD_INT);
    DECIDE();
    TEST_TYPE(KEYWORD_FLOAT64);
    DECIDE();
    TEST_TYPE(KEYWORD_STRING);
    DECIDE();
    return syntaxError;

}
int params_n(){
    //<params_n> -> , id <type> <params_n> | eps
    TEST_TYPE(BRACKET_ROUND_CLOSE);
    DECIDE();
    CHECK_TYPE(COMMA);
    NEXT_TOKEN(activeToken);
    CHECK_TYPE(IDENTIFIER);
    //ID do symtable
    NEXT_TOKEN(activeToken);
    CHECK_STATE(type);
    NEXT_TOKEN(activeToken);
    CHECK_STATE(params_n);

}
int func_types(){
   // <func_types> -> (int <types_n>) | (float64 <types_n>) | (string <types_n>) | eps
   TEST_TYPE(BRACKET_CURLY_OPEN);
   DECIDE();
   CHECK_TYPE(BRACKET_ROUND_OPEN);
   NEXT_TOKEN(activeToken);
   CHECK_STATE(type);
   CHECK_STATE(types_n);
   CHECK_TYPE(BRACKET_ROUND_CLOSE);


}
int types_n(){
    //<types_n> -> , <type> <types_n> | eps
    TEST_TYPE(BRACKET_ROUND_CLOSE);
    DECIDE();
    CHECK_TYPE(COMMA);
    NEXT_TOKEN(activeToken);
    CHECK_STATE(type);
    CHECK_STATE(types_n);

}
int func_n(){
//<func_n> -> $ <func> <func_n> | eps
    return 0;
}
int body(){
//act token EOL
//<body> -> <statement> <statements>
    CHECK_STATE(statement);
    NEXT_TOKEN(activeToken);
    CHECK_STATE(statements);
}
int statement(){
//<statement> -> <definition> | <assignment> | <if> | <for> | <call> | <return>
    NEXT_TOKEN(activeToken);
    //TODO switch podla typu tokenu

    return 0;
}

int statements(){
//<statements> -> <statement> | eps
    return 0;

}
int definition(){
//<definition> -> id := <expression>
    CHECK_TYPE(IDENTIFIER);
    NEXT_TOKEN(activeToken);
    CHECK_TYPE(OPERATOR_DEFINE);
    NEXT_TOKEN(activeToken);
    CHECK_STATE(expression);

}
int assignment(){
//<assignment> -> <ids> = <expressions> | <ids> = <call>
    CHECK_STATE(ids);
    NEXT_TOKEN(activeToken);
    CHECK_TYPE(OPERATOR_ASSIGN);
    //expressions alebo call

    return 0;
}
int _if(){
//<if> -> if (<expression>) { $ <statements> } else { $ <statements>}
    return 0;
}
int _for(){
//<for> -> for (<definition> | <assignment> | eps ; <expression> ; <assignment> ) { $ <body> }
    return 0;
}
int _call(){
//<call> -> id(<params>)
    return 0;

}
int _return(){
//<return> -> return <expressions>
    return 0;
}
int ids(){
//<ids> -> id <ids_n>
    
    NEXT_TOKEN(activeToken);
    CHECK_TYPE(IDENTIFIER);
    CHECK_STATE(ids_n);

}
int ids_n(){
//<ids_n> -> , id <ids_n> | eps
    NEXT_TOKEN(activeToken);
    TEST_TYPE(OPERATOR_ASSIGN);
    DECIDE();
    CHECK_TYPE(COMMA);
    NEXT_TOKEN(activeToken);
    CHECK_TYPE(IDENTIFIER);
    CHECK_STATE(ids_n);

}

int expressions(){
//<expressions> -> <expression> <expression_n>
    return 0;

}
int expression_n(){
//<expression_n> -> , <expression> <expression_n> | eps
    return 0;
}
//TODO parsovanie expressions
int expression(){
    return 0;
}