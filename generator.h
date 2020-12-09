/**
 * @file generator.h
 * @author Patrik Jacola (xjacol00)
 * @brief Generator of output code interface
 */
 
#include <stdio.h>
#include "tokenlist.c"

/**
* Generates header of result file.
*
*
*/

void generate_file_header();

/**
* Generates built in function CHR.
*
*
*/

void generate_CHR();

/**
* Generates built in function LEN.
*
*
*/

void generate_LEN();

/**
* Generates start of main function.
*
*
*/

void generate_main_start();

/**
* Generates end of main function.
*
*
*/

void generate_main_end();

/**
* Generates print function.
*
*
*/

void generate_print();

/**
* Generates basic operations.
*
*@param insl.
*/

void generate(Instruction_t ins1);