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
    //prog();
    return 0;
}

int prog(){

   if(Token->type == EOL){
       GET_NEXT(Token); // ak je EOL pred package
   }
   CHECK_TYPE(KEYWORD_PACKAGE); 
   GET_NEXT(Token);
   CHECK_TYPE(ID);
    if(!strcmp("main",Token->value)){
        GET_NEXT(Token);
        CHECK_TYPE(EOL);
        exec();
        return 0; 
    
    }

    errorExit(syntaxError, "Prolog form has to be : 'package main'\n");
}

int exec(){
   
    func();
    func_n();
    return 0;
    
}
int func(){

    GET_NEXT(Token);
    CHECK_TYPE(KEYWORD_FUNC);
    GET_NEXT(Token);
    CHECK_TYPE(ID);

    GET_NEXT(Token);
    CHECK_TYPE(BRACKET_ROUND_OPEN);
    GET_NEXT(Token);
    params(); 
    GET_NEXT(Token);
    func_types();
    body();
    return 0;
}

int params(){
    
    if(Token->type == BRACKET_ROUND_CLOSE){
        return 0;  // no params
    }
    CHECK_TYPE(ID);
    GET_NEXT(Token);
    type();
    GET_NEXT(Token);
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
            errorExit(syntaxError,"in type\n");
    }

}
int params_n(){

    if(Token->type == BRACKET_ROUND_CLOSE){
        return 0;  // no more params
    }
    CHECK_TYPE(COMMA);
    GET_NEXT(Token);
    CHECK_TYPE(ID);
    GET_NEXT(Token);
    type();
    GET_NEXT(Token);
    params_n();
}
int func_types(){

    if(Token->type == BRACKET_CURLY_OPEN){
        return 0;  // no return types && no brackets
    }

    if(Token->type == BRACKET_ROUND_OPEN && Token->pNext->type == BRACKET_ROUND_CLOSE){
        GET_NEXT(Token);// no return types && brackets
        return 0;
    }
   CHECK_TYPE(BRACKET_ROUND_OPEN);
   GET_NEXT(Token);
   //SET_FLAG(returnFlag);
   type();
   GET_NEXT(Token);
   types_n();
   GET_NEXT(Token);

}

int types_n(){

    if(Token->type == BRACKET_ROUND_CLOSE){
        return 0;  // no return types
    }
    CHECK_TYPE(COMMA);
    GET_NEXT(Token);
    type();
    GET_NEXT(Token);
    types_n();
}

int func_n(){
    TEST_EOF();
    GET_NEXT(Token);
    TEST_EOF();
    TEST_TYPE(EOL);
    if (decisionFlag){
        CHECK_TYPE(EOL);
        func();
        func_n();
    }
    return 0;
}

int body(){
   
    CHECK_TYPE(BRACKET_CURLY_OPEN);
    GET_NEXT(Token);
    statement();
    if(Token->type == BRACKET_CURLY_CLOSE){
        return 0;
    }
    statement_n();
}

int statement(){

    CHECK_TYPE(EOL);
    GET_NEXT(Token);
    Token_t *savedToken = Token; 

    if(Token->type == ID){
        GET_NEXT(Token);
        switch(Token->type){
            case BRACKET_ROUND_OPEN: 
                Token = savedToken;
                _call();
                GET_NEXT(Token);
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
        return 0;
    }

    switch(Token->type){
        case KEYWORD_IF: 
                _if();
                GET_NEXT(Token);
                break;

        case KEYWORD_FOR: 
                _for();
                GET_NEXT(Token);
                break;
        
        case KEYWORD_RETURN: 
                _return();
                GET_NEXT(Token);
                break;

        case BRACKET_CURLY_CLOSE: 
                break;

        default: errorExit(syntaxError,"in statement"); break;
    }

   /* if(returnFlag && Token->type != KEYWORD_RETURN){
        errorExit(syntaxError,"function with return type/s doesnt return anything");
    }*/
    return 0;
   
}

int statement_n(){

    if(Token->type == BRACKET_CURLY_CLOSE){
        return 0;
    }
    
    CHECK_STATE(statement);
    
    CHECK_STATE(statement_n);


}

int definition(){

    CHECK_TYPE(ID);
    
    GET_NEXT(Token);
    CHECK_TYPE(OPERATOR_DEFINE);

    GET_NEXT(Token);
    CHECK_STATE(expression);

}

int assignment(){

    CHECK_STATE(id);
    
    CHECK_TYPE(OPERATOR_ASSIGN);
    Token_t *savedTokenAssign = Token; 
    GET_NEXT(Token);  
    Token_t *savedTokenID = Token;
    TEST_TYPE(ID); 
    if(decisionFlag){
        
        GET_NEXT(Token);
        TEST_TYPE(BRACKET_ROUND_OPEN);
        if(decisionFlag){ 
            
            Token = savedTokenID;
            CHECK_STATE(_call); 
            GET_NEXT(Token);
        }else{ 
            
            Token = savedTokenID;
            CHECK_STATE(expressions);
        }

    }else{
        
        Token = savedTokenID;
        CHECK_STATE(expressions);
    }

}

int _if(){

    CHECK_TYPE(KEYWORD_IF);
    
    GET_NEXT(Token);
    if(Token->type == BRACKET_CURLY_OPEN){
        errorExit(syntaxError,"no expression in if\n");
    }
    expression();
    body();
    
    GET_NEXT(Token);
    CHECK_TYPE(KEYWORD_ELSE);
    
    GET_NEXT(Token);
    body();

}

int _for(){

    GET_NEXT(Token);
    TEST_TYPE(ID);
    if(decisionFlag){
        CHECK_TYPE(ID);
        CHECK_STATE(definition);
    }
    CHECK_TYPE(SEMICOLON); // eps
    GET_NEXT(Token);
    if(Token->type == SEMICOLON){
        errorExit(syntaxError,"expected expression in for statement\n");
    }
    CHECK_STATE(expression);
    GET_NEXT(Token);
    if(Token->type == BRACKET_CURLY_OPEN){//eps
            body();
    }
    else{
            CHECK_STATE(assignment);
            body();
    }

}

int _call(){

    GET_NEXT(Token);
    CHECK_TYPE(BRACKET_ROUND_OPEN);
    
    GET_NEXT(Token);
    if(Token->type == BRACKET_ROUND_CLOSE){
        return 0; // no params in call
    }
    _call_param();

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
                if(!strcmp("_",Token->value)){
                    errorExit(semanticFunctionError,"parser: _ in call param\n");
                }
            break;

        default: errorExit(syntaxError,"parser: in call param \n");break;

    }

    GET_NEXT(Token);
    _call_param_n();

}

int _call_param_n(){

    if(Token->type == BRACKET_ROUND_CLOSE){
        return 0; //no more params
    }
    CHECK_TYPE(COMMA);
    GET_NEXT(Token);
    _call_param();

}

int _return(){

    
    TEST_TYPE(EOL);
    DECIDE();//eps
    CHECK_TYPE(KEYWORD_RETURN);
    GET_NEXT(Token);
    CHECK_STATE(expressions);
    
}

int id(){

    CHECK_TYPE(ID);
    GET_NEXT(Token);
    id_n();

}

int id_n(){

    if(Token->type != COMMA){
        return 0;
    }
    GET_NEXT(Token);
    CHECK_TYPE(ID);
    GET_NEXT(Token);
    id_n();

}

int expressions(){

    CHECK_STATE(expression); 
    CHECK_STATE(expression_n);

}

int expression_n(){

    TEST_TYPE(COMMA);
    if(decisionFlag){
        CHECK_TYPE(COMMA);
       
        GET_NEXT(Token);
        CHECK_STATE(expression);
        CHECK_STATE(expression_n);
    }
    //eps
    return 0;
    
}

int expression(){
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
		GET_NEXT(Token);
		printf("active token: %s\n",Token->value);
	}
    return 0;
}
