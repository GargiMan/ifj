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
}

void exec(){

    GET_NEXT(Token);//
    func();
    if(!functionParse){
       
        func_n();
       
        HTabIterator_t tmp = htabFind(globaltab,"main");
        if(!tmp.ptr){
            errorExit(semanticIdentifierError,"main function missing\n");
        }
    }
    return;
    
}
void func(){

    //GET_NEXT(Token);
    CHECK_TYPE(KEYWORD_FUNC);
    GET_NEXT(Token);
    CHECK_TYPE(ID);
    localtab = htabInit(503);
    if(functionParse){ // iba pri prvom prechode kvoli definiciam funkcii
        HTabIterator_t tmp = htabFind(globaltab,Token->value);
        if(tmp.ptr){
            htabFree(localtab);
            errorExit(semanticIdentifierError,"parser: function redefinition\n");
        }
        //HTabIterator_t iter = 
        htabFindOrAdd(globaltab,Token->value);
    }
    GET_NEXT(Token);
    CHECK_TYPE(BRACKET_ROUND_OPEN);
    GET_NEXT(Token);
    params(localtab); 
    GET_NEXT(Token);
    func_types();
                if(functionParse){ ///prvy prechod kvoli definiciam funkcii
                    htabFree(localtab);
                        while(Token->type != KEYWORD_FUNC){
                            if(Token == list.pTail){
                                functionParse--;
                                return;
                                break;
                            }
                            GET_NEXT(Token);
                        }
                    func();
                    return;
                }  
    body(localtab);
    return;
}

void params(HTab_t* localtab){
    
    if(Token->type == BRACKET_ROUND_CLOSE){
        //iter.ptr->data->params = "";
        return ;  // no params
    }
    CHECK_TYPE(ID);
    HTabIterator_t tmp = htabFindOrAdd(localtab,Token->value);

    GET_NEXT(Token);
    type(tmp,localtab);
    GET_NEXT(Token);
    params_n(localtab);
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

}
void params_n(HTab_t* localtab){

    if(Token->type == BRACKET_ROUND_CLOSE){
        return ;  // no more params
    }
    CHECK_TYPE(COMMA);
    GET_NEXT(Token);
    CHECK_TYPE(ID);
    HTabIterator_t tmp = htabFind(localtab,Token->value);
    if(tmp.ptr){
        htabFree(localtab);
        errorExit(semanticIdentifierError,"parser: function parameters redefinition\n");
    }
    tmp = htabFindOrAdd(localtab,Token->value);
    GET_NEXT(Token);
    type();
    GET_NEXT(Token);
    params_n(localtab);
}
void func_types(){

    if(Token->type == BRACKET_CURLY_OPEN){

        //iter.ptr->data->type = TYPE_UNDEFINED; //return type undefined
        return;  // no return types && no brackets
    }

    if(Token->type == BRACKET_ROUND_OPEN && Token->pNext->type == BRACKET_ROUND_CLOSE){

       // iter.ptr->data->type = TYPE_UNDEFINED; //return type undefined
        GET_NEXT(Token);// no return types && brackets
        return;
    }
   CHECK_TYPE(BRACKET_ROUND_OPEN);
   GET_NEXT(Token);
   //SET_FLAG(returnFlag);
   type();
   GET_NEXT(Token);
   types_n();
   GET_NEXT(Token);

}

void types_n(){

    if(Token->type == BRACKET_ROUND_CLOSE){
        return;  // no return types
    }
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
    if (decisionFlag){
        CHECK_TYPE(EOL);
        GET_NEXT(Token);//
        func();
      
        func_n();
    }
    return;
}

void body(HTab_t* localtab){
   
    CHECK_TYPE(BRACKET_CURLY_OPEN);
    GET_NEXT(Token);
    statement(localtab);
    if(Token->type == BRACKET_CURLY_CLOSE){
        return;
    }
    statement_n(localtab);
  
    return;
}

void statement(HTab_t* localtab){

    CHECK_TYPE(EOL);
    GET_NEXT(Token);
    Token_t *savedToken = Token; 

    if(Token->type == ID){
        GET_NEXT(Token);
        switch(Token->type){
            case BRACKET_ROUND_OPEN: 
                Token = savedToken;
                _call(localtab);
                GET_NEXT(Token);
                break;

            case OPERATOR_DEFINE: 
                Token = savedToken;
                definition(localtab);
                break;

            case COMMA:
            case OPERATOR_ASSIGN: 
                Token = savedToken;
                assignment(localtab);
                break;

            default: 
                htabFree(localtab);
                errorExit(syntaxError,"in statement"); break;
        }
        return;
    }

    switch(Token->type){
        case KEYWORD_IF: 
                _if(localtab);
                GET_NEXT(Token);
                break;

        case KEYWORD_FOR: 
                _for(localtab);
                GET_NEXT(Token);
                break;
        
        case KEYWORD_RETURN: 
                _return(localtab);
                GET_NEXT(Token);
                break;

        case BRACKET_CURLY_CLOSE: 
               
                return;
                break;

        default: 
                htabFree(localtab);
                errorExit(syntaxError,"in statement"); break;
    }

    /* if(returnFlag && Token->type != KEYWORD_RETURN){
        errorExit(syntaxError,"function with return type/s doesnt return anything");
    }*/
    return;
   
}

void statement_n(HTab_t* localtab){

   
    if(Token->type == BRACKET_CURLY_CLOSE){
        return;
    }
    
    statement(localtab);
    
    statement_n(localtab);


}

void definition(HTab_t* localtab){

   
    CHECK_TYPE(ID);
    Token_t *tokenID = Token;
    GET_NEXT(Token);
    CHECK_TYPE(OPERATOR_DEFINE);
    HTabIterator_t tmp = htabFind(localtab,tokenID->value);
    if(tmp.ptr /*&& tmp.ptr->data->defined*/){
        htabFree(localtab);
        errorExit(semanticIdentifierError,"variable redefinition\n");
    }
    tmp = htabFindOrAdd(localtab,tokenID->value);
    //tmp.ptr->data->defined = 1;

    GET_NEXT(Token);
    expression(localtab);

}

void assignment(HTab_t* localtab){

    id(localtab);
    
    CHECK_TYPE(OPERATOR_ASSIGN); 
    GET_NEXT(Token);  
    Token_t *savedTokenID = Token;
    TEST_TYPE(ID); 
    if(decisionFlag){
        
        GET_NEXT(Token);
        TEST_TYPE(BRACKET_ROUND_OPEN);
        if(decisionFlag){ 
            
            Token = savedTokenID;
            _call(localtab);
            GET_NEXT(Token);
        }else{ 
            
            Token = savedTokenID;
            expressions(localtab);
        }

    }else{
        
        Token = savedTokenID;
        expressions(localtab);
    }

}

void _if(HTab_t* localtab){

    CHECK_TYPE(KEYWORD_IF);
    
    GET_NEXT(Token);
    if(Token->type == BRACKET_CURLY_OPEN){
        htabFree(localtab);
        errorExit(syntaxError,"no expression in if\n");
    }
    expression(localtab);
   // HTab_t* nextlocaltab = htabInit(503);
   // nextlocaltab = localtab;
    
    body(localtab);
    
    
    GET_NEXT(Token);
    CHECK_TYPE(KEYWORD_ELSE);
  
    GET_NEXT(Token);
   // htabClear(nextlocaltab);
    //nextlocaltab = localtab;
    
    body(localtab);
   
    return;
    //htabFree(nextlocaltab);

}

void _for(HTab_t* localtab){

    GET_NEXT(Token);
    TEST_TYPE(ID);
    if(decisionFlag){
        CHECK_TYPE(ID);
        definition(localtab);
    }
    CHECK_TYPE(SEMICOLON); // eps
    GET_NEXT(Token);
    if(Token->type == SEMICOLON){
        htabFree(localtab);
        errorExit(syntaxError,"expected expression in for statement\n");
    }
    expression(localtab);
    GET_NEXT(Token);
    if(Token->type == BRACKET_CURLY_OPEN){//eps
            body(localtab);
    }
    else{
            assignment(localtab);
            body(localtab);
    }

}

void _call(HTab_t* localtab){


    CHECK_TYPE(ID);
    HTabIterator_t tmp = htabFind(globaltab,Token->value);
    if(!tmp.ptr){
        htabFree(localtab);
        errorExit(semanticIdentifierError,"calling function that is not defined\n");
    }
    GET_NEXT(Token);
    CHECK_TYPE(BRACKET_ROUND_OPEN);
    
    GET_NEXT(Token);
    if(Token->type == BRACKET_ROUND_CLOSE){
        return; // no params in call
    }
    _call_param(localtab);
    return;
}

void _call_param(HTab_t* localtab){

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
                    errorExit(semanticIdentifierError,"parser: _ in call param\n");
                }
                if(!tmp.ptr){
                    htabFree(localtab);
                    errorExit(semanticIdentifierError,"variable in function call is not defined\n");
                }
            break;

        default: 
        htabFree(localtab);
        errorExit(syntaxError,"parser: in call param \n");break;

    }

    GET_NEXT(Token);
    _call_param_n(localtab);

}

void _call_param_n(HTab_t* localtab){

    if(Token->type == BRACKET_ROUND_CLOSE){
        return; //no more params
    }
    CHECK_TYPE(COMMA);
    GET_NEXT(Token);
    _call_param(localtab);

}

void _return(HTab_t* localtab){

    
    TEST_TYPE(EOL);
    DECIDE();//eps
    CHECK_TYPE(KEYWORD_RETURN);
    GET_NEXT(Token);
    expressions(localtab);
    
}

void id(HTab_t* localtab){

    CHECK_TYPE(ID);
            HTabIterator_t tmp = htabFind(localtab,Token->value);
            if(!tmp.ptr && strcmp(Token->value,"_")){
                htabFree(localtab);
                errorExit(semanticIdentifierError,"variable not defined\n");
            }
    GET_NEXT(Token);
    id_n(localtab);

}

void id_n(HTab_t* localtab){

    if(Token->type != COMMA){
        return;
    }
    GET_NEXT(Token);
    CHECK_TYPE(ID);
            HTabIterator_t tmp = htabFind(localtab,Token->value);
            if(!tmp.ptr && strcmp(Token->value,"_")){
                htabFree(localtab);
                errorExit(semanticIdentifierError,"variable not defined\n");
            }
    GET_NEXT(Token);
    id_n(localtab);

}

void expressions(HTab_t* localtab){

    expression(localtab);
    expression_n(localtab);

}

void expression_n(HTab_t* localtab){

    TEST_TYPE(COMMA);
    if(decisionFlag){
        CHECK_TYPE(COMMA);
       
        GET_NEXT(Token);
        expression(localtab);
        expression_n(localtab);
    }
    //eps
    return;
    
}

void expression(HTab_t* localtab){
    if(Token->type == EOL){
        htabFree(localtab);
        errorExit(syntaxError,"no expression\n");
    }
    while(Token != list.pTail){
		TEST_TYPE(COMMA);
		if(decisionFlag){break;}
		TEST_TYPE(EOL);
		if(decisionFlag){break;}
		TEST_TYPE(SEMICOLON);
		if(decisionFlag){break;}
		TEST_TYPE(BRACKET_CURLY_OPEN);
		if(decisionFlag){break;}
		TEST_TYPE(OPERATOR_ASSIGN);
		if(decisionFlag){
            htabFree(localtab);
            errorExit(syntaxError,"= in expression\n");break;}
		TEST_TYPE(OPERATOR_DEFINE);
		if(decisionFlag){
            htabFree(localtab);
            errorExit(syntaxError,":= in expression\n");break;}
        if(Token->type == ID && !strcmp("_",Token->value)){
            htabFree(localtab);
            errorExit(semanticIdentifierError,"_ in expression\n");break;}
        TEST_TYPE(ID);
        if(decisionFlag){
            HTabIterator_t tmp = htabFind(localtab,Token->value);
            if(!tmp.ptr){
                htabFree(localtab);
                errorExit(semanticIdentifierError,"id in expression is not defined\n");
            }
        }
		GET_NEXT(Token);

	}
    return;
}
