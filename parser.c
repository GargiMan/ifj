/**
 * @file parser.c
 * @author Filip Sapak (xsapak05)
 * @brief IFJ20 - parser
 */

#include "parser.h"
Token_t *activeToken;
int epsFlag;
#define NEXT_TOKEN(activeToken)(activeToken = activeToken->pNext)
#define SYNTAX_OK  0
#define CHECK_OK(result) if (result != SYNTAX_OK){return result;}
#define CHECK_TYPE(TOKENTYPE) \
    if (activeToken->type != TOKENTYPE){ \
        return syntaxError; \
    }
#define CHECK_EOF() \
    if (activeToken->type != EndOfFile) {return syntaxError;} ///// treba tokentype EOF

#define CHECK_EPS(TOKENTYPE) \
    if (activeToken->type == TOKENTYPE){ \
        epsFlag = 1; \
    } else \
    { \
        epsFlag = 0; \
    } \
    



int parse(){
    activeToken = list.pHead;
    int result = prog();
    return result;
}

//<prog> -> package id $ <exec>
int prog(){

   int result;
   CHECK_TYPE(KEYWORD_PACKAGE); 
   NEXT_TOKEN(activeToken);
   CHECK_TYPE(IDENTIFIER);
    if(!strcmp("main",activeToken->value)){ // hodil by sa tokentype keyword main
        NEXT_TOKEN(activeToken);
        CHECK_TYPE(EOL); // $
        result = exec(); //all ok -> next step
        CHECK_OK(result); //kontrola vysledku
        CHECK_EOF();
        return SYNTAX_OK; //syntax ok
    
    }
    return syntaxError;
}


int exec(){
//<exec> -> <func> <func_n>
    int result;
    NEXT_TOKEN(activeToken);
    result = func();

    NEXT_TOKEN(activeToken);
    result = func_n();
    
    return result;
}
int func(){
//<func> -> func id (<params>)<func_types>{ $ <body>}
    int result;
    CHECK_TYPE(KEYWORD_FUNC);
    NEXT_TOKEN(activeToken);
    CHECK_TYPE(IDENTIFIER);
    //TODO ulozit ID do symtable
    NEXT_TOKEN(activeToken);
    CHECK_TYPE(BRACKET_ROUND_OPEN);
    NEXT_TOKEN(activeToken); // id alebo eps
    result = params();
    CHECK_OK(result);
    //NEXT_TOKEN();
    CHECK_TYPE(BRACKET_ROUND_CLOSE);
    NEXT_TOKEN(activeToken);
    result = func_types();
    CHECK_OK(result);
    NEXT_TOKEN(activeToken);
    CHECK_TYPE(BRACKET_CURLY_OPEN);
    NEXT_TOKEN(activeToken);
    CHECK_TYPE(EOL);
    result = body();
    CHECK_OK(result);

    return result;
}
int params(){
    //<params> -> id <type> <p  arams_n> | eps
    int result;
    CHECK_EPS(BRACKET_ROUND_CLOSE);
    if(epsFlag){
        return SYNTAX_OK;
    }
    CHECK_TYPE(IDENTIFIER);
    //TODO ulozit ID do symtable
    NEXT_TOKEN(activeToken);
    result = type();
    CHECK_OK(result);
    NEXT_TOKEN(activeToken);
    CHECK_EPS(BRACKET_ROUND_CLOSE);
    if(epsFlag){
        return SYNTAX_OK;
    }
    result = params_n();

    return result;

}

int type(){
//<type> -> int | float64 | string
if (activeToken->type == KEYWORD_INT) {
		// <TYPE> -> int
		return SYNTAX_OK;
	} else if (activeToken->type == KEYWORD_FLOAT64) {
		// <TYPE> -> float64
		return SYNTAX_OK;
	} if (activeToken->type == KEYWORD_STRING) {
		// <TYPE> -> string
		return SYNTAX_OK;
	}

	return syntaxError;
}
int params_n(){
    //<params_n> -> , id <type> <params_n> | eps
    CHECK_EPS(BRACKET_ROUND_CLOSE);
    if(epsFlag){
        return SYNTAX_OK;
    }
    CHECK_TYPE(COMMA);
    NEXT_TOKEN(activeToken);
    CHECK_TYPE(IDENTIFIER);
    //ID do symtable
    NEXT_TOKEN(activeToken);
    int result = type();
    CHECK_OK(result);
    NEXT_TOKEN(activeToken);
    result = params_n();
    return result; ///neni treba asi

}
int func_types(){
   // <func_types> -> (int <types_n>) | (float64 <types_n>) | (string <types_n>) | eps
   CHECK_EPS(BRACKET_CURLY_OPEN);
   if(epsFlag){return SYNTAX_OK;}
   CHECK_TYPE(BRACKET_ROUND_OPEN);
   NEXT_TOKEN(activeToken);
   int result = type();
   CHECK_OK(result);
   result = types_n();
   CHECK_OK(result);
   CHECK_TYPE(BRACKET_ROUND_CLOSE)
   return result;


}
int types_n(){
    //<types_n> -> , <type> <types_n> | eps
    CHECK_EPS(BRACKET_ROUND_CLOSE);
    if(epsFlag){return SYNTAX_OK;}
    CHECK_TYPE(COMMA);
    NEXT_TOKEN(activeToken);
    int result = type();
    CHECK_OK(result);
    result = types_n();
    return result; // asi netreba

}
int func_n(){
//<func_n> -> $ <func> <func_n> | eps
    return 0;
}
int body(){
//act token EOL
//<body> -> <statement> <statements>
    int result = statement();
    CHECK_OK(result);
    NEXT_TOKEN(activeToken);
    result = statements();
    CHECK_OK(result);
    return result;
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
    int result;
    CHECK_TYPE(IDENTIFIER);
    NEXT_TOKEN(activeToken);
    CHECK_TYPE(OPERATOR_DEFINE);
    NEXT_TOKEN(activeToken);
    result = expression();
    CHECK_OK(result);
    return result;

}
int assignment(){
//<assignment> -> <ids> = <expressions> | <ids> = <call>
    int result = ids();
    CHECK_OK(result);
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
    int result;
    NEXT_TOKEN(activeToken);
    CHECK_TYPE(IDENTIFIER);
    result = ids_n();
    CHECK_OK(result);
    return result;

}
int ids_n(){
//<ids_n> -> , id <ids_n> | eps
    int result;
    NEXT_TOKEN(activeToken);
    CHECK_EPS(OPERATOR_ASSIGN);
    if(epsFlag){return SYNTAX_OK;}
    CHECK_TYPE(COMMA);
    NEXT_TOKEN(activeToken);
    CHECK_TYPE(IDENTIFIER);
    result = ids_n();
    return result;//asi netreba

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