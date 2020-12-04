/**
 * @file parser.c
 * @author Filip Sapak (xsapak05)
 * @brief IFJ20 - parser
 */

#include "parser.h"

int parse(){
    Token = list.pHead;
    prog();
    printf("ALL OK!\n");
    return syntaxOK;
}

int prog(){

   CHECK_TYPE(KEYWORD_PACKAGE); 
   TOKEN_MOVE_NEXT(Token);
   CHECK_TYPE(ID);
    if(!strcmp("main",Token->value)){

        TOKEN_MOVE_NEXT(Token);
        CHECK_TYPE(EOL);
        exec();
        return syntaxOK; 
    
    }

    errorExit(syntaxError, "Prolog form has to be : 'package main'\n");
}

int exec(){
   
    func();
    func_n();
    return syntaxOK;
    
}
int func(){

    TOKEN_MOVE_NEXT(Token);
    CHECK_TYPE(KEYWORD_FUNC);
    TOKEN_MOVE_NEXT(Token);
    CHECK_TYPE(ID);
    TOKEN_MOVE_NEXT(Token);
    CHECK_TYPE(BRACKET_ROUND_OPEN);
    TOKEN_MOVE_NEXT(Token);
    params(); 
    TOKEN_MOVE_NEXT(Token);
    func_types(); //ACT {
    //TOKEN_MOVE_NEXT(Token);
    body();
    return syntaxOK;
}
int params(){
    
    if(Token->type == BRACKET_ROUND_CLOSE){
        return syntaxOK;  // no params
    }
    CHECK_TYPE(ID);
    TOKEN_MOVE_NEXT(Token);
    type();
    TOKEN_MOVE_NEXT(Token);
    params_n();
}

int type(){

    switch(Token->type){
        case KEYWORD_INT:
            break;
        case KEYWORD_FLOAT64:
            break;
        case KEYWORD_STRING:
            break;
        default:
            return syntaxError;
    }

}
int params_n(){

    if(Token->type == BRACKET_ROUND_CLOSE){
        return syntaxOK;  // no more params
    }
    CHECK_TYPE(COMMA);
    TOKEN_MOVE_NEXT(Token);
    CHECK_TYPE(ID);
    TOKEN_MOVE_NEXT(Token);
    type();
    TOKEN_MOVE_NEXT(Token);
    params_n();

}
int func_types(){

   if(Token->type == BRACKET_CURLY_OPEN){
        return syntaxOK;  // no return types
    }
   CHECK_TYPE(BRACKET_ROUND_OPEN);
   TOKEN_MOVE_NEXT(Token);
   type();
   TOKEN_MOVE_NEXT(Token);
   types_n();
   TOKEN_MOVE_NEXT(Token);
}
int types_n(){

    if(Token->type == BRACKET_ROUND_CLOSE){
        return syntaxOK;  // no return types
    }
    CHECK_TYPE(COMMA);
    TOKEN_MOVE_NEXT(Token);
    type();
    TOKEN_MOVE_NEXT(Token);
    types_n();
}
int func_n(){
//<func_n> -> $ <func> <func_n> | eps //ak uz nie su dalsie funkcie tak je EOF
    printf("func_n\n");
    TEST_EOF();
    TOKEN_MOVE_NEXT(Token);
    TEST_EOF();
    TEST_TYPE(EOL);
    if (decisionFlag){
        printf("more functions\n");
        CHECK_TYPE(EOL);
        func();
        printf("FUNC OK\n");
        func_n();
        printf("FUNC_N OK\n");
    }
 
    return syntaxOK;
}
int body(){
    printf("bodyyy\n");
    CHECK_TYPE(BRACKET_CURLY_OPEN);
    printf("curly open pohoda\n");
    TOKEN_MOVE_NEXT(Token);
    statement();
    printf("skok zo statement\n");
    if(Token->type == BRACKET_CURLY_CLOSE){
        printf("jop je to ono\n");
        return syntaxOK;
    }
    statement_n();
}
int statement(){

    CHECK_TYPE(EOL);
    printf("EOL\n");
    TOKEN_MOVE_NEXT(Token);
    Token_t *savedToken = Token; 

    if(Token->type == ID){
        TOKEN_MOVE_NEXT(Token);
        switch(Token->type){
            case BRACKET_ROUND_OPEN: 
                Token = savedToken;
                _call();
                TOKEN_MOVE_NEXT(Token);
                break;

            case OPERATOR_DEFINE: 
                Token = savedToken;
                definition();
                break;

            case COMMA:
            case OPERATOR_ASSIGN: 
                Token = savedToken;
                assignment();
                break;

            default: errorExit(syntaxError,"in statement"); break;
        }
        return syntaxOK;
    }

    switch(Token->type){
        case KEYWORD_IF: 
                _if();
                TOKEN_MOVE_NEXT(Token);
                break;

        case KEYWORD_FOR: 
                _for();
                TOKEN_MOVE_NEXT(Token);
                break;
        
        case KEYWORD_RETURN: 
                printf("skok do returnu\n");
                _return();
                printf("vyskok z returnu\n");
                TOKEN_MOVE_NEXT(Token);
                break;

        case BRACKET_CURLY_CLOSE: 
                printf("nasiel som to?\n");
                break;

        default: errorExit(syntaxError,"in statement"); break;
    }
    return syntaxOK;
   
}

int statement_n(){

    if(Token->type == BRACKET_CURLY_CLOSE){
        printf("jop v statement_n je curly close\n");
        return syntaxOK;
    }
    
    CHECK_STATE(statement);
    
    CHECK_STATE(statement_n);


}
int definition(){
//<definition> -> id := <expression>
    printf("in definition\n");
    CHECK_TYPE(ID);
    printf("ID checked\n");
    TOKEN_MOVE_NEXT(Token);
    CHECK_TYPE(OPERATOR_DEFINE);
    printf("operator define checked\n");
    TOKEN_MOVE_NEXT(Token);
    CHECK_STATE(expression);

}
int assignment(){
//<assignment> -> <ids> = <expressions> | <ids> = <call>
    CHECK_STATE(id);
    printf("checking ids\n");
    CHECK_TYPE(OPERATOR_ASSIGN);
    Token_t *savedTokenAssign = Token; // tokentype "="
    TOKEN_MOVE_NEXT(Token);  //expressions alebo call token
    Token_t *savedTokenID = Token;// tokentype ID
    TEST_TYPE(ID); //ak je za id ( ,tak je to call /// je to ID flag = 1, neni to ID flag = 0
    if(decisionFlag){
        printf("bolo tam ID\n");
        TOKEN_MOVE_NEXT(Token);
        TEST_TYPE(BRACKET_ROUND_OPEN);
        if(decisionFlag){ // ak je to "(",tak vieme ze je to call
            printf("bude call\n");
            Token = savedTokenID;
            CHECK_STATE(_call); ///potrebujem pri volani ID v activeToken
            TOKEN_MOVE_NEXT(Token);
        }else{ // ak to nie je "(", je to expression
            printf("bude vyraz 1\n");
            Token = savedTokenID;//or assign
            CHECK_STATE(expressions);
        }

    }else{// ak to nie je id, je to urcite expression
        printf("bude vyraz 2\n");
        Token = savedTokenID;//or assign
        CHECK_STATE(expressions);
    }

}
int _if(){
//<if> -> if <expression> <body> else <body>
    CHECK_TYPE(KEYWORD_IF);
    printf("keyword IF checked\n");
    TOKEN_MOVE_NEXT(Token);
    expression();
    body();
    printf("von z body\n");
    TOKEN_MOVE_NEXT(Token);
    CHECK_TYPE(KEYWORD_ELSE);
    printf("else OK\n");
    TOKEN_MOVE_NEXT(Token);
    body();

}
int _for(){
//<for> -> for  <definition> | eps ; <expression> ; <assignment> | eps  <body>
    TOKEN_MOVE_NEXT(Token);
    TEST_TYPE(ID);
    if(decisionFlag){
        CHECK_TYPE(ID);
        CHECK_STATE(definition);
    }
    CHECK_TYPE(SEMICOLON); // eps
    TOKEN_MOVE_NEXT(Token);
    CHECK_STATE(expression);
    TOKEN_MOVE_NEXT(Token);
    if(Token->type == BRACKET_CURLY_OPEN){//eps
            body();

    }else{
            CHECK_STATE(assignment);
            body();
    }

}
int _call(){

    TOKEN_MOVE_NEXT(Token);
    CHECK_TYPE(BRACKET_ROUND_OPEN);
    printf("pohoda\n");
    TOKEN_MOVE_NEXT(Token);
    if(Token->type == BRACKET_ROUND_CLOSE){
        return syntaxOK; // no params in call
    }
    _call_param();
    //CHECK_TYPE(BRACKET_ROUND_CLOSE);

}
int _call_param(){

    switch(Token->type){

        case DATA_INT:
            break;

        case DATA_STRING:
            break;

        case DATA_FLOAT64:
            break;

        case ID:
            break;

        default: errorExit(syntaxError,"parser: in call param ");break;

    }
    TOKEN_MOVE_NEXT(Token);
    _call_param_n();

}
int _call_param_n(){

    if(Token->type == BRACKET_ROUND_CLOSE){
        return syntaxOK; //no more params
    }
    CHECK_TYPE(COMMA);
    TOKEN_MOVE_NEXT(Token);
    _call_param();

}

int _return(){
//<return> -> return <expressions> | eps
    //TEST_TYPE(BRACKET_CURLY_CLOSE);
    TEST_TYPE(EOL);
    DECIDE();//eps
    CHECK_TYPE(KEYWORD_RETURN);//return 
    printf("return keyword tested\n");
    TOKEN_MOVE_NEXT(Token);
    CHECK_STATE(expressions);
    
}
int id(){

    CHECK_TYPE(ID);
    TOKEN_MOVE_NEXT(Token);
    id_n();

}
int id_n(){
//<ids_n> -> , id <ids_n> | eps

    if(Token->type != COMMA){
        return syntaxOK;
    }
    TOKEN_MOVE_NEXT(Token);
    CHECK_TYPE(ID);
    TOKEN_MOVE_NEXT(Token);
    id_n();

}

int expressions(){
//<expressions> -> <expression>  <expression_n>

    CHECK_STATE(expression); 
    printf("expression checked\n");
    CHECK_STATE(expression_n);

}
int expression_n(){
//<expression_n> -> , <expression> <expression_n> | eps
    TEST_TYPE(COMMA);
    if(decisionFlag){
        CHECK_TYPE(COMMA);
        printf("COMMA OK\n");
        TOKEN_MOVE_NEXT(Token);
        CHECK_STATE(expression);
        CHECK_STATE(expression_n);
    }
    //eps
    printf("no more expressions\n");
    return syntaxOK;
    
}
int expression(){
    printf("checking expression\n");
	printf("active token: %s\n",Token->value);
    while(Token != list.pTail){
		TEST_TYPE(COMMA);
		if(decisionFlag){printf("je to COMMA\n");break;}
		TEST_TYPE(EOL);
		if(decisionFlag){printf("je to EOL\n");break;}
		TEST_TYPE(SEMICOLON);
		if(decisionFlag){printf("je to SEMICOLON\n");break;}
		TEST_TYPE(BRACKET_CURLY_OPEN);
		if(decisionFlag){printf("je to CURLYOPEN\n");break;}
		TEST_TYPE(OPERATOR_ASSIGN);
		if(decisionFlag){printf("= in expression\n");break;}
		TEST_TYPE(OPERATOR_DEFINE);
		if(decisionFlag){printf(":= in expression\n");break;}
		TOKEN_MOVE_NEXT(Token);
		printf("active token: %s\n",Token->value);
	}

    return syntaxOK;
}