/**
 * @file parser.c
 * @author Filip Sapak (xsapak05)
 * @brief IFJ20 - parser
 */

#include "parser.h"

int parse(){

    dataInit();
    prog();
    Token = list.pHead;
    prog();
    htabClear(globaltab);
    htabFree(globaltab);
    return 0;
}

int dataInit(){
    functionParse = 1;
    Token = list.pHead;
    globaltab = htabInit(19501);
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

    GET_NEXT(Token);//
    func();
    if(!functionParse){
        func_n();
        HTabIterator_t tmp = htabFind(globaltab,"main");
        if(!tmp.ptr){
            errorExit(semanticIdentifierError,"main function missing\n");
        }
    }
    return 0;
    
}
int func(){

    //GET_NEXT(Token);
    CHECK_TYPE(KEYWORD_FUNC);
    GET_NEXT(Token);
    CHECK_TYPE(ID);
    HTab_t* localtab = htabInit(19501);
    if(functionParse){ // iba pri prvom prechode kvoli definiciam funkcii
        HTabIterator_t tmp = htabFind(globaltab,Token->value);
        if(tmp.ptr){
            errorExit(semanticIdentifierError,"parser: function redefinition\n");
        }
        HTabIterator_t iter = htabFindOrAdd(globaltab,Token->value);
    }
    GET_NEXT(Token);
    CHECK_TYPE(BRACKET_ROUND_OPEN);
    GET_NEXT(Token);
    params(localtab); 
    GET_NEXT(Token);
    func_types();
                if(functionParse){ ///prvy prechod kvoli definiciam funkcii
                        while(Token->type != KEYWORD_FUNC){
                            if(Token == list.pTail){
                                functionParse--;
                                return 0;
                                break;
                            }
                            GET_NEXT(Token);
                        }
                    func();
                    return 0;
                }  
    body(localtab);
    htabClear(localtab);
    htabFree(localtab);
    return 0;
}

int params(HTab_t* localtab){
    
    if(Token->type == BRACKET_ROUND_CLOSE){
        //iter.ptr->data->params = "";
        return 0;  // no params
    }
    CHECK_TYPE(ID);
    HTabIterator_t tmp = htabFindOrAdd(localtab,Token->value);

    GET_NEXT(Token);
    type(tmp,localtab);
    GET_NEXT(Token);
    params_n(localtab);
}

int type(){

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
int params_n(HTab_t* localtab){

    if(Token->type == BRACKET_ROUND_CLOSE){
        return 0;  // no more params
    }
    CHECK_TYPE(COMMA);
    GET_NEXT(Token);
    CHECK_TYPE(ID);
    HTabIterator_t tmp = htabFind(localtab,Token->value);
    if(tmp.ptr){
        errorExit(semanticIdentifierError,"parser: function parameters redefinition\n");
    }
    tmp = htabFindOrAdd(localtab,Token->value);
    GET_NEXT(Token);
    type();
    GET_NEXT(Token);
    params_n(localtab);
}
int func_types(){

    if(Token->type == BRACKET_CURLY_OPEN){

        //iter.ptr->data->type = TYPE_UNDEFINED; //return type undefined
        return 0;  // no return types && no brackets
    }

    if(Token->type == BRACKET_ROUND_OPEN && Token->pNext->type == BRACKET_ROUND_CLOSE){

       // iter.ptr->data->type = TYPE_UNDEFINED; //return type undefined
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
        GET_NEXT(Token);//
        func();
        func_n();
    }
    return 0;
}

int body(HTab_t* localtab){
   
    CHECK_TYPE(BRACKET_CURLY_OPEN);
    GET_NEXT(Token);
    statement(localtab);
    if(Token->type == BRACKET_CURLY_CLOSE){
        return 0;
    }
    statement_n(localtab);
}

int statement(HTab_t* localtab){

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

            default: errorExit(syntaxError,"in statement"); break;
        }
        return 0;
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
                break;

        default: errorExit(syntaxError,"in statement"); break;
    }

    /* if(returnFlag && Token->type != KEYWORD_RETURN){
        errorExit(syntaxError,"function with return type/s doesnt return anything");
    }*/
    return 0;
   
}

int statement_n(HTab_t* localtab){

    if(Token->type == BRACKET_CURLY_CLOSE){
        return 0;
    }
    
    statement(localtab);
    
    statement_n(localtab);


}

int definition(HTab_t* localtab){

    CHECK_TYPE(ID);
    Token_t *tokenID = Token;
    GET_NEXT(Token);
    CHECK_TYPE(OPERATOR_DEFINE);
    HTabIterator_t tmp = htabFind(localtab,tokenID->value);
    if(tmp.ptr /*&& tmp.ptr->data->defined*/){
        errorExit(semanticIdentifierError,"variable redefinition\n");
    }
    tmp = htabFindOrAdd(localtab,tokenID->value);
    //tmp.ptr->data->defined = 1;

    GET_NEXT(Token);
    expression(localtab);//hodit tam aj iterator kvoli datatype

}

int assignment(HTab_t* localtab){

    id(localtab);
    
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

int _if(HTab_t* localtab){

    CHECK_TYPE(KEYWORD_IF);
    
    GET_NEXT(Token);
    if(Token->type == BRACKET_CURLY_OPEN){
        errorExit(syntaxError,"no expression in if\n");
    }
    expression(localtab);
    HTab_t* nextlocaltab = htabInit(19501);
   // nextlocaltab = localtab;
    
    body(nextlocaltab);
    
    
    GET_NEXT(Token);
    CHECK_TYPE(KEYWORD_ELSE);
    
    GET_NEXT(Token);
    htabClear(nextlocaltab);
    //nextlocaltab = localtab;
    
    body(nextlocaltab);
    htabClear(nextlocaltab);
    htabFree(nextlocaltab);

}

int _for(HTab_t* localtab){

    GET_NEXT(Token);
    TEST_TYPE(ID);
    if(decisionFlag){
        CHECK_TYPE(ID);
        definition(localtab);
    }
    CHECK_TYPE(SEMICOLON); // eps
    GET_NEXT(Token);
    if(Token->type == SEMICOLON){
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

int _call(HTab_t* localtab){

    CHECK_TYPE(ID);
    HTabIterator_t tmp = htabFind(globaltab,Token->value);
    if(!tmp.ptr){
        errorExit(semanticIdentifierError,"calling function that is not defined\n");
    }
    GET_NEXT(Token);
    CHECK_TYPE(BRACKET_ROUND_OPEN);
    
    GET_NEXT(Token);
    if(Token->type == BRACKET_ROUND_CLOSE){
        return 0; // no params in call
    }
    _call_param(localtab);

}

int _call_param(HTab_t* localtab){

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
                    errorExit(semanticIdentifierError,"parser: _ in call param\n");
                }
                if(!tmp.ptr){
                        errorExit(semanticIdentifierError,"variable in function call is not defined\n");
                }
            break;

        default: errorExit(syntaxError,"parser: in call param \n");break;

    }

    GET_NEXT(Token);
    _call_param_n(localtab);

}

int _call_param_n(HTab_t* localtab){

    if(Token->type == BRACKET_ROUND_CLOSE){
        return 0; //no more params
    }
    CHECK_TYPE(COMMA);
    GET_NEXT(Token);
    _call_param(localtab);

}

int _return(HTab_t* localtab){

    
    TEST_TYPE(EOL);
    DECIDE();//eps
    CHECK_TYPE(KEYWORD_RETURN);
    GET_NEXT(Token);
    expressions(localtab);
    
}

int id(HTab_t* localtab){

    CHECK_TYPE(ID);
            HTabIterator_t tmp = htabFind(localtab,Token->value);
            if(!tmp.ptr && strcmp(Token->value,"_")){
                errorExit(semanticIdentifierError,"variable not defined\n");
            }
    GET_NEXT(Token);
    id_n(localtab);

}

int id_n(HTab_t* localtab){

    if(Token->type != COMMA){
        return 0;
    }
    GET_NEXT(Token);
    CHECK_TYPE(ID);
            HTabIterator_t tmp = htabFind(localtab,Token->value);
            if(!tmp.ptr){
                errorExit(semanticIdentifierError,"variable not defined\n");
            }
    GET_NEXT(Token);
    id_n(localtab);

}

int expressions(HTab_t* localtab){

    expression(localtab);
    expression_n(localtab);

}

int expression_n(HTab_t* localtab){

    TEST_TYPE(COMMA);
    if(decisionFlag){
        CHECK_TYPE(COMMA);
       
        GET_NEXT(Token);
        expression(localtab);
        expression_n(localtab);
    }
    //eps
    return 0;
    
}

int expression(HTab_t* localtab){
    if(Token->type == EOL){
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
		if(decisionFlag){errorExit(syntaxError,"= in expression\n");break;}
		TEST_TYPE(OPERATOR_DEFINE);
		if(decisionFlag){errorExit(syntaxError,":= in expression\n");break;}
        if(Token->type == ID && !strcmp("_",Token->value)){errorExit(semanticIdentifierError,"_ in expression\n");break;}
		GET_NEXT(Token);

	}
    return 0;
}
