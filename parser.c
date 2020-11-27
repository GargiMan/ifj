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
#define CHECK_TYPE(TOKENTYPE) \
    if (activeToken->type != TOKENTYPE){ \
        errorExit(syntaxError,"parser: .... "); \
    }   \

#define CHECK_EOF() \
    if (activeToken == list.pTail) {return syntaxOK;}else{ errorExit(syntaxError, "No EOF");} ///// treba tokentype EOF

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
        errorExit(syntaxError, "....");} \

    



int parse(){

    tHTable* globalSymTable;
    htInit(globalSymTable);
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
    errorExit(syntaxError, "in prog");
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
    CHECK_TYPE(BRACKET_CURLY_CLOSE);

}
int params(){
    //<params> -> id <type> <params_n> | eps
    
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
    errorExit(syntaxError, "in type");

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
//<func_n> -> $ <func> <func_n> | eps //ak uz nie su dalsie funkcie tak je EOF
    TEST_TYPE(EOL);
    if (decisionFlag){
        CHECK_TYPE(EOL);
        NEXT_TOKEN(activeToken);
        CHECK_STATE(func);
        CHECK_STATE(func_n);
    }
    NEXT_TOKEN(activeToken);
    CHECK_EOF()
}
int body(){
//act token EOL
//<body> -> <statement> <statements>
    CHECK_STATE(statement); //ak je body prazdne tak activeToken by mal byt } //ak vyjdeme z call tak activeToken je )
    CHECK_STATE(statements);
}
int statement(){
//<statement> -> <definition> | <assignment> | <if> | <for> | <call> | <return>

    NEXT_TOKEN(activeToken); // prvy token v body okrem EOLu
    //TODO switch podla typu tokenu
    Token_t *savedToken = activeToken; // ulozenie prveho tokenu

    // <call> | <definition> | <assignment>
    TEST_TYPE(IDENTIFIER);
    if(decisionFlag){
        CHECK_TYPE(IDENTIFIER);
        NEXT_TOKEN(activeToken);
        switch(activeToken->type){

            activeToken = savedToken;

            case BRACKET_ROUND_OPEN: CHECK_STATE(_call);

            case OPERATOR_DEFINE: CHECK_STATE(definition);
        
            case OPERATOR_ASSIGN: CHECK_STATE(assignment);

            default: return syntaxError;
        }
    }

    //<if> | <for> | <return>
    TEST_TYPE(KEYWORD_IF);
    if(decisionFlag){
        CHECK_STATE(_if);
    }
    TEST_TYPE(KEYWORD_FOR);
    if(decisionFlag){
        CHECK_STATE(_for);
    }
    CHECK_STATE(_return);
}

int statements(){
//<statements> -> <statement> <statements> | eps
    TEST_TYPE(BRACKET_CURLY_CLOSE);
    DECIDE();// eps
    
    CHECK_STATE(statement);
    NEXT_TOKEN(activeToken); 
    CHECK_STATE(statements);


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
    Token_t *savedTokenAssign = activeToken; // tokentype "="
    NEXT_TOKEN(activeToken);  //expressions alebo call token
    Token_t *savedTokenID = activeToken;// tokentype ID
    TEST_TYPE(IDENTIFIER); //ak je za id ( ,tak je to call /// je to ID flag = 1, neni to ID flag = 0
    if(decisionFlag){
        NEXT_TOKEN(activeToken);
        TEST_TYPE(BRACKET_ROUND_OPEN);
        if(decisionFlag){ // ak je to "(",tak vieme ze je to call
            activeToken = savedTokenID;
            CHECK_STATE(_call); ///potrebujem pri volani ID v activeToken
        }else{ // ak to nie je "(", je to expression
            activeToken = savedTokenAssign;
            CHECK_STATE(expressions);
        }

    }else{// ak to nie je id, je to urcite expression

        activeToken = savedTokenAssign;
        CHECK_STATE(expressions);
    }

}
int _if(){
//<if> -> if <expression> { $ <body> } else { $ <body>}
    CHECK_TYPE(KEYWORD_IF);
    NEXT_TOKEN(activeToken);
    CHECK_STATE(expression);
    NEXT_TOKEN(activeToken);
    CHECK_TYPE(BRACKET_CURLY_OPEN);
    NEXT_TOKEN(activeToken);
    CHECK_TYPE(EOL);
    CHECK_STATE(body);
    CHECK_TYPE(BRACKET_CURLY_CLOSE);
    NEXT_TOKEN(activeToken);
    CHECK_TYPE(KEYWORD_ELSE);
    NEXT_TOKEN(activeToken);
    CHECK_TYPE(BRACKET_CURLY_OPEN);
    NEXT_TOKEN(activeToken);
    CHECK_TYPE(EOL);
    CHECK_STATE(body);
    CHECK_TYPE(BRACKET_CURLY_CLOSE);

}
int _for(){
//<for> -> for  <definition> | <assignment> | eps ; <expression> ; <assignment>  { $ <body> }
    CHECK_TYPE(KEYWORD_FOR);
    NEXT_TOKEN(activeToken);
    Token_t *savedToken = activeToken; // uloz ID / bodkociarku
    TEST_TYPE(IDENTIFIER);
    if(decisionFlag){
        CHECK_TYPE(IDENTIFIER);
        NEXT_TOKEN(activeToken);
        TEST_TYPE(OPERATOR_DEFINE);
        if(decisionFlag){
            CHECK_TYPE(OPERATOR_DEFINE);
            activeToken = savedToken;
            CHECK_STATE(definition); // definition chce prve ID

        }else{
            activeToken = savedToken;                   // ID,ID,ID = //// ID:=
            CHECK_STATE(assignment);

        }
    }
    CHECK_TYPE(SEMICOLON); // eps
    NEXT_TOKEN(activeToken);
    CHECK_STATE(expression);
    //predat expression ; ako $
    NEXT_TOKEN(activeToken);
    CHECK_TYPE(SEMICOLON);
    NEXT_TOKEN(activeToken);
    CHECK_STATE(assignment);
    NEXT_TOKEN(activeToken);
    CHECK_TYPE(BRACKET_CURLY_OPEN);
    NEXT_TOKEN(activeToken);
    CHECK_TYPE(EOL);
    CHECK_STATE(body);
    CHECK_TYPE(BRACKET_CURLY_CLOSE);

}
int _call(){
//<call> -> id(<params>)
    //NEXT_TOKEN(activeToken);
    CHECK_TYPE(IDENTIFIER);
    NEXT_TOKEN(activeToken);
    CHECK_TYPE(BRACKET_ROUND_OPEN);
    CHECK_STATE(params);
    CHECK_TYPE(BRACKET_ROUND_CLOSE);

}
int _return(){
//<return> -> return <expressions> | eps
    TEST_TYPE(BRACKET_CURLY_CLOSE);
    DECIDE();//eps
    CHECK_TYPE(KEYWORD_RETURN);//return 
    NEXT_TOKEN(activeToken);
    CHECK_STATE(expressions);
    
}
int ids(){
//<ids> -> id <ids_n>
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

    CHECK_STATE(expression);
    CHECK_STATE(expression_n);

}
int expression_n(){
//<expression_n> -> , <expression> <expression_n> | eps
    TEST_TYPE(COMMA);
    if(decisionFlag){
        CHECK_TYPE(COMMA);
        NEXT_TOKEN(activeToken);
        CHECK_STATE(expression);
        CHECK_STATE(expression_n);
    }
    return syntaxOK;
    
}
//TODO parsovanie expressions
int expression(){
    return syntaxOK;
}