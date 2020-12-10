/**
 * @file parser.c
 * @author Filip Sapak (xsapak05)
 * @brief IFJ20 - parser
 */

#include "parser.h"

void parse(){

    dataInit();
    prog();
    Token = list.pHead;
    prog();
    htabClear(globaltab);
    htabFree(localtab);
    
    return;
}

void dataInit(){

    functionParse = 1;
    Token = list.pHead;
    globaltab = htabInit(503);
    localtab = htabInit(503);

    htabFindOrAdd(globaltab,"inputs");
    htabFindOrAdd(globaltab,"inputi");
    htabFindOrAdd(globaltab,"inputf");
    htabFindOrAdd(globaltab,"print");
    htabFindOrAdd(globaltab,"int2float");
    htabFindOrAdd(globaltab,"float2int");
    htabFindOrAdd(globaltab,"len");
    htabFindOrAdd(globaltab,"substr");
    htabFindOrAdd(globaltab,"ord");
    htabFindOrAdd(globaltab,"chr");

    return;
}

void prog(){

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

        return; 
    }

    errorExit(syntaxError, "Prolog form has to be : 'package main'\n");

    return;
}

void exec(){

    GET_NEXT(Token);
    func();

    if(!functionParse){
       
        func_n();
        HTabIterator_t tmp = htabFind(globaltab,"main");

        if(!tmp.ptr) errorExit(semanticIdentifierError,"main function missing\n");
    }

    return;
}

void func(){

    CHECK_TYPE(KEYWORD_FUNC);
    GET_NEXT(Token);
    CHECK_TYPE(ID);

    if(functionParse) { // iba pri prvom prechode kvoli definiciam funkcii
        HTabIterator_t tmp = htabFind(globaltab,Token->value);

        if(tmp.ptr) {
            htabFree(localtab);
            errorExit(semanticIdentifierError,"parser: function redefinition\n");
        }

        htabFindOrAdd(globaltab,Token->value);
    }
    
    GET_NEXT(Token);
    CHECK_TYPE(BRACKET_ROUND_OPEN);
    GET_NEXT(Token);
    params(); 
    GET_NEXT(Token);
    func_types();

    if(functionParse){ ///prvy prechod kvoli definiciam funkcii

        while(Token->type != KEYWORD_FUNC){

            if(Token == list.pTail) {
                functionParse--;
                return;
            }
            GET_NEXT(Token);
        }
        func();
        return;
    }  

    body();

    return;
}

void params(){
    
    if(Token->type == BRACKET_ROUND_CLOSE) return;  // no params

    CHECK_TYPE(ID);
    htabFindOrAdd(localtab,Token->value);
    GET_NEXT(Token);
    type();
    GET_NEXT(Token);
    params_n();

    return;
}

void type(){

    switch(Token->type){
        case KEYWORD_INT:
                //iter.ptr->data->type = TYPE_INT;
            break;
        case KEYWORD_FLOAT64:
                //iter.ptr->data->type = TYPE_DOUBLE;
            break;
        case KEYWORD_STRING:
               // iter.ptr->data->type = TYPE_STRING;
            break;
        default:
            errorExit(syntaxError,"in type\n");
    }

    return;
}

void params_n(){

    if(Token->type == BRACKET_ROUND_CLOSE) return ;  // no more params

    CHECK_TYPE(COMMA);
    GET_NEXT(Token);
    CHECK_TYPE(ID);
    HTabIterator_t tmp = htabFind(localtab,Token->value);

    if(tmp.ptr) {
        htabFree(localtab);
        errorExit(semanticIdentifierError, "parser : function parameters redefinition\n");
    }

    tmp = htabFindOrAdd(localtab,Token->value);
    GET_NEXT(Token);
    type();
    GET_NEXT(Token);
    params_n();

    return;
}

void func_types(){

    if(Token->type == BRACKET_CURLY_OPEN) return;  // no return types && no brackets

    if(Token->type == BRACKET_ROUND_OPEN && Token->pNext->type == BRACKET_ROUND_CLOSE) {

        //return type undefined
        GET_NEXT(Token);// no return types && brackets
        return;
    }

    CHECK_TYPE(BRACKET_ROUND_OPEN);
    GET_NEXT(Token);
    type();
    GET_NEXT(Token);
    types_n();
    GET_NEXT(Token);

    return;
}

void types_n(){

    if(Token->type == BRACKET_ROUND_CLOSE) return;  // no return types

    CHECK_TYPE(COMMA);
    GET_NEXT(Token);
    type();
    GET_NEXT(Token);
    types_n();
}

void func_n(){

    TEST_EOF();
    GET_NEXT(Token);
    TEST_EOF();
    TEST_TYPE(EOL);

    if (decisionFlag) {

        CHECK_TYPE(EOL);
        GET_NEXT(Token);
        func();
        func_n();
    }

    return;
}

void body(){
   
    CHECK_TYPE(BRACKET_CURLY_OPEN);
    GET_NEXT(Token);
    statement();

    if(Token->type == BRACKET_CURLY_CLOSE) return;

    statement_n();
  
    return;
}

void statement(){

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

            default: 
                htabFree(localtab);
                errorExit(syntaxError,"in statement"); break;
        }

        return;
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
               
                return;
                break;

        default: 
                htabFree(localtab);
                errorExit(syntaxError,"in statement"); break;
    }

    return;
}

void statement_n(){

    if(Token->type == BRACKET_CURLY_CLOSE) return;
    
    statement();
    statement_n();

    return;
}

void definition(){

    CHECK_TYPE(ID);
    Token_t *tokenID = Token;
    GET_NEXT(Token);
    CHECK_TYPE(OPERATOR_DEFINE);
    HTabIterator_t tmp = htabFind(localtab,tokenID->value);

    if(tmp.ptr){
        htabFree(localtab);
        errorExit(semanticIdentifierError,"variable redefinition\n");
    }

    tmp = htabFindOrAdd(localtab,tokenID->value);
    GET_NEXT(Token);
    expression();

    return;
}

void assignment(){

    ids();
    CHECK_TYPE(OPERATOR_ASSIGN); 
    GET_NEXT(Token);  
    Token_t *savedTokenID = Token;
    TEST_TYPE(ID); 

    if (decisionFlag) {
        
        GET_NEXT(Token);
        TEST_TYPE(BRACKET_ROUND_OPEN);

        if (decisionFlag) { 

            Token = savedTokenID;
            _call();
            GET_NEXT(Token);

        } else { 

            Token = savedTokenID;
            expressions();
        }

    } else {
        Token = savedTokenID;
        expressions();
    }

    return;
}

void _if(){

    CHECK_TYPE(KEYWORD_IF);
    GET_NEXT(Token);

    if(Token->type == BRACKET_CURLY_OPEN){
        htabFree(localtab);
        errorExit(syntaxError,"no expression in if\n");
    }

    expression();
    body();
    GET_NEXT(Token);
    CHECK_TYPE(KEYWORD_ELSE);
    GET_NEXT(Token);
    body();
   
    return;
}

void _for(){

    GET_NEXT(Token);
    TEST_TYPE(ID);

    if(decisionFlag){
        CHECK_TYPE(ID);
        definition();
    }

    CHECK_TYPE(SEMICOLON); // eps
    GET_NEXT(Token);

    if(Token->type == SEMICOLON){
        htabFree(localtab);
        errorExit(syntaxError,"parser : expected expression in for statement\n");
    }

    expression();
    GET_NEXT(Token);

    if(Token->type == BRACKET_CURLY_OPEN) {     //eps
            body();
    } else {
            assignment();
            body();
    }

    return;
}

void _call(){

    CHECK_TYPE(ID);
    HTabIterator_t tmp = htabFind(globaltab,Token->value);
    if(!tmp.ptr){
        htabFree(localtab);
        errorExit(semanticIdentifierError,"parser : calling function that is not defined\n");
    }
    GET_NEXT(Token);
    CHECK_TYPE(BRACKET_ROUND_OPEN);
    
    GET_NEXT(Token);
    if(Token->type == BRACKET_ROUND_CLOSE) return; // no params in call
    _call_param();

    return;
}

void _call_param(){

    HTabIterator_t tmp = htabFind(localtab,Token->value);
    switch(Token->type){

        case DATA_INT:
            break;

        case DATA_STRING:
            break;

        case DATA_FLOAT64:
            break;

        case ID:
            if(!strcmp("_",Token->value)){
                htabFree(localtab);
                errorExit(semanticIdentifierError,"parser : _ in call param\n");
            }
            if(!tmp.ptr){
                htabFree(localtab);
                errorExit(semanticIdentifierError,"parser : variable in function call is not defined\n");
            }
            break;

        default: 
            htabFree(localtab);
            errorExit(syntaxError,"parser : in call param \n");
            break;
    }

    GET_NEXT(Token);
    _call_param_n();

    return;
}

void _call_param_n(){

    if(Token->type == BRACKET_ROUND_CLOSE) return; //no more params
    CHECK_TYPE(COMMA);
    GET_NEXT(Token);
    _call_param();

    return;
}

void _return(){

    TEST_TYPE(EOL);
    DECIDE();//eps
    CHECK_TYPE(KEYWORD_RETURN);
    GET_NEXT(Token);
    expressions();
    
    return;
}

void ids(){

    CHECK_TYPE(ID);
    HTabIterator_t tmp = htabFind(localtab,Token->value);

    if (!tmp.ptr && strcmp(Token->value,"_")){
        htabFree(localtab);
        errorExit(semanticIdentifierError,"parser : variable not defined\n");
    }

    GET_NEXT(Token);
    ids_n();

    return;
}

void ids_n(){

    if (Token->type != COMMA) return;

    GET_NEXT(Token);
    CHECK_TYPE(ID);
    HTabIterator_t tmp = htabFind(localtab,Token->value);

    if (!tmp.ptr && strcmp(Token->value,"_")){
        htabFree(localtab);
        errorExit(semanticIdentifierError,"parser : variable not defined\n");
    }

    GET_NEXT(Token);
    ids_n();

    return;
}

void expressions(){

    expression();
    expression_n();

    return;
}

void expression_n(){

    TEST_TYPE(COMMA);
    if(decisionFlag){
        CHECK_TYPE(COMMA);
       
        GET_NEXT(Token);
        expression();
        expression_n();
    }

    return;
}

void expression(){

    if(Token->type == EOL) {
        htabFree(localtab);
        errorExit(syntaxError,"parser : no expression\n");
    }

    while(Token != list.pTail) {

		TEST_TYPE(COMMA);
		if(decisionFlag) break;

		TEST_TYPE(EOL);
		if(decisionFlag) break;

		TEST_TYPE(SEMICOLON);
		if(decisionFlag) break;

		TEST_TYPE(BRACKET_CURLY_OPEN);
		if(decisionFlag) break;

		TEST_TYPE(OPERATOR_ASSIGN);
		if(decisionFlag) {
            htabFree(localtab);
            errorExit(syntaxError,"parser : = in expression\n");
            break;
        }

		TEST_TYPE(OPERATOR_DEFINE);
		if(decisionFlag) {
            htabFree(localtab);
            errorExit(syntaxError,"parser : := in expression\n");
            break;
        }

        if(Token->type == ID && !strcmp("_",Token->value)) {
            htabFree(localtab);
            errorExit(semanticIdentifierError,"parser : _ in expression\n");
            break;
        }

        TEST_TYPE(ID);

        if(decisionFlag) {
            HTabIterator_t tmp = htabFind(localtab,Token->value);
            if(!tmp.ptr) {
                htabFree(localtab);
                errorExit(semanticIdentifierError,"parser : id in expression is not defined\n");
            }
        }
		GET_NEXT(Token);
	}

    return;
}
