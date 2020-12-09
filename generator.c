/**
 * @file generator.c
 * @author Patrik Jacola (xjacol00)
 * @brief Generator of output code
 */

#include <stdio.h>
#include <stdlib.h>
#include "generator.h"

typedef enum Instruction {
	MOVE = 0;			//function, frame
	CREATEFRAME;
	PUSHFRAME;
	POPFRAME;
	DEFVAR;
	CALL;
	RETURN;
	PUSHS = 10;			//stack
	POPS;
	CLEARS;
	ADD = 20;			//arithmetic
	SUB;
	MUL;
	DIV;
	IDIV;
	ADDS;
	SUBS;
	MULS;
	DIVS;
	IDIVS;
	LT;					//compare
	GT;
	EQ;
	LTS;
	GTS;
	EQS;
	AND;
	OR;
	NOT;
	ANDS;
	ORS;
	NOTS;
	INT2FLOAT;			//conversion
	FLOAT2INT;
	INT2CHAR;
	STRI2INT;
	INT2FLOATS;
	FLOAT2INTS;
	INT2CHARS;
	STRI2INTS;
	READ = 50;			//input/output
	WRITE;
	CONCAT = 60;		//string
	STRLEN;
	GETCHAR;
	SETCHAR;
	TYPE = 70;			//types
	LABEL = 80;			//label, jump, exit
	JUMP;
	JUMPIFEQ;
	JUMPIFNEQ;
	JUMPIFEQS;
	JUMPIFNEQS;
	EXIT;
	BREAK;				//debug
	DPRINT;
} Instruction_t

#define CHR
	"\n # Built-in function Chr"											\
	"\n LABEL $chr"															\
	"\n PUSHFRAME"															\
	"\n DEFVAR LF@%ret"														\
	"\n MOVE LF@%ret string@"												\
	"\n DEFVAR LF@max"														\
	"\n LT LF@max LF@%0 int@0"												\
	"\n JUMPIFEQ $chr$return LF@max bool@true"								\
	"\n GT LF@max LF@%0 int@255"											\
	"\n JUMPIFEQ $chr$return LF@max bool@true"								\
	"\n INT2CHAR LF@%ret LF@%0"												\
	"\n LABEL $chr$return"													\
	"\n POPFRAME"															\
	"\n RETURN"

#define LEN
	"\n # Built-in function Len"											\
	"\n LABEL $len"															\
	"\n PUSHFRAME"															\
	"\n DEFVAR LF@%ret"														\
	"\n STRLEN LF@%ret LF@%0"												\
	"\n POPFRAME"															\
	"\n RETURN"


#define ORD



#define SUBSTR



#define PRINT



	
void generate_file_header() {

	printf("# Start of program\n");
	printf(".IFJcode20\n");
	printf("DEFVAR GF@%input_prompt\n");
	printf("MOVE GF@%input_prompt string@?\\032\n");
	printf("DEFVAR GF@%tmp_op1\n");
	printf("DEFVAR GF@%tmp_op2\n");
	printf("DEFVAR GF@%tmp_op3\n");
	printf("DEFVAR GF@%exp_result\n");
	printf("JUMP $$main\n");

	return;
}
void generate_CHR(){
	printf(CHR);

void generate_LEN(){
	printf(LEN);
}

void generate(Instruction_t ins1) {

		switch(token->type){
			case ADD:
				printf("PUSHFRAME\n CREATEFRAME\n");
				printf("DEFVAR TF@%suc\n");
				printf("ADD");
				printf(" ");
				printf("TF@%suc\n");
				printf(" ");
				printf("");
				printf(" ");
				printf("");
				printf("\n");
				break;
		
			case SUB:
				printf("PUSHFRAME\n CREATEFRAME\n");
				printf("DEFVAR TF@%min\n");
				printf("SUB");
				printf(" ");
				printf("TF@%min\n");
				printf(" ");
				printf("");
				printf(" ");
				printf("");
				printf("\n");
				break;
		
			case MUL:
				printf("PUSHFRAME\n CREATEFRAME\n");
				printf("DEFVAR TF@%nas\n");
				printf("MUL");
				printf(" ");
				printf("TF@%nas\n");
				printf(" ");
				printf("");
				printf(" ");
				printf("");
				printf("\n");
				break;

			case DIV:
				printf("PUSHFRAME\n CREATEFRAME\n");
				printf("DEFVAR TF@%del\n");
				printf("DIV");
				printf(" ");
				printf("TF@%del\n");
				printf(" ");
				printf("");
				printf(" ");
				printf("");
				printf("\n");
				break;
			
			case AND:
				printf("PUSHFRAME\n CREATEFRAME\n");
				printf("DEFVAR TF@%a\n");
				printf("AND");
				printf(" ");
				printf("TF@%a\n");
				printf(" ");
				printf("");
				printf(" ");
				printf("");
				printf("\n");
				break;
		
			case OR:
				printf("PUSHFRAME\n CREATEFRAME\n");
				printf("DEFVAR TF@%or\n");
				printf("OR");
				printf(" ");
				printf("TF@%or\n");
				printf(" ");
				printf("");
				printf(" ");
				printf("");
				printf("\n");
				break;
			
			case NOT:
				printf("PUSHFRAME\n CREATEFRAME\n");
				printf("DEFVAR TF@%nie\n");
				printf("NOT");
				printf(" ");
				printf("TF@%nie\n");
				printf(" ");
				printf("");
				printf(" ");
				printf("");
				printf("\n");
				break;
				
			case LT:
				printf("PUSHFRAME\n CREATEFRAME\n");
				printf("DEFVAR TF@%men\n");
				printf("LT");
				printf(" ");
				printf("TF@%men\n");
				printf(" ");
				printf("");
				printf(" ");
				printf("");
				printf("\n");
				break;
			
			case GT:
				printf("PUSHFRAME\n CREATEFRAME\n");
				printf("DEFVAR TF@%via\n");
				printf("GT");
				printf(" ");
				printf("TF@%via\n");
				printf(" ");
				printf("");
				printf(" ");
				printf("");
				printf("\n");
				break;
		
			case EQ:
				printf("PUSHFRAME\n CREATEFRAME\n");
				printf("DEFVAR TF@%rov\n");
				printf("EQ");
				printf(" ");
				printf("TF@%rov\n");
				printf(" ");
				printf("");
				printf(" ");
				printf("");
				printf("\n");
				break;
		
		
		
		
		
		
		
		
		
		
		
		
		}
	}

	return;
}