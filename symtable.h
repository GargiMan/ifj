
/**
 * @file symtable.h
 * @author 
 * @brief IFJ20 Compiler
 */

#ifndef __SYMTABLE_H__
#define __SYMTABLE_H__

#include <stdlib.h>
#include "string.h"    // size_t
#include <stdbool.h>    // bool
#define MAX_HTSIZE 101
#include "error.h"
#include "parser.h"

typedef enum
{
	TYPE_UNDEFINED,	/// Data type undefined
	TYPE_INT,		/// Integer data type
	TYPE_DOUBLE,	/// Double data type
	TYPE_STRING,	/// String data type
	TYPE_BOOL,		/// Bool data type 

} Data_type;

typedef struct
{
  Data_type type;  
  bool defined;
  String_t *params;
  char* identifier;
  bool global;

} tData;

/* typ klíče (například identifikace zboží) */
//typedef char* tKey;


/*Datová položka TRP s explicitně řetězenými synonymy*/
 typedef struct tHTItem{
	const char* key;				/* klíč  */
	tData *data;				/* obsah */
	struct tHTItem* ptrnext;	/* ukazatel na další synonymum */
} tHTItem;

/* TRP s explicitně zřetězenými synonymy. */
typedef tHTItem* tHTable[MAX_HTSIZE];

/* Pro účely testování je vhodné mít možnost volby velikosti pole,
   kterým je vyhledávací tabulka implementována. Fyzicky je deklarováno
   pole o rozměru MAX_HTSIZE, ale při implementaci vašich procedur uvažujte
   velikost HTSIZE.  Ve skriptu se před voláním řešených procedur musí
   objevit příkaz HTSIZE N, kde N je velikost požadovaného prostoru.

   POZOR! Pro správnou funkci TRP musí být hodnota této proměnné prvočíslem.
*/
extern int HTSIZE;

/* Hlavičky řešených procedur a funkcí. */

int hashCode (const char* key );

void htInit ( tHTable* ptrht );

tHTItem* htSearch ( tHTable* ptrht, const char* key );

void htInsert ( tHTable* ptrht,const char* key, tData *data ,int Data_type);

tData* htRead ( tHTable* ptrht,const char* key );

void htDelete ( tHTable* ptrht,const char* key );

void htClearAll ( tHTable* ptrht );
#endif // __SYMTABLE_H__