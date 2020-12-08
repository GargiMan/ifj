/**
 * @file symtable.h
 * @author Marek Gergel (xgerge01)
 * @brief IFJ20 - Header for symtable operations
 */

#ifndef __SYMTABLE_H__
#define __SYMTABLE_H__

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>         // uint32_t
#include <string.h>         // size_t
#include <stdbool.h>        // bool
#include "string.h"
#include "error.h"

typedef enum HTabDataType {
	TYPE_UNDEFINED,	        // Data type undefined
	TYPE_INT,		        // Integer data type
	TYPE_DOUBLE,	        // Double data type
	TYPE_STRING,	        // String data type
	TYPE_BOOL,		        // Bool data type 
} HTabDataType_t;

typedef struct HTabData {
  HTabDataType_t type;      // data type
  bool def;                 // defined
  char* params;             // parameters
  char* funcTypes;              // global
} HTabData_t;

// Item typedef
typedef char* HTabKey_t;                // typ klice v zazname
typedef struct HTabItem HTabItem_t;     // typedef pre item

// Item
struct HTabItem {
    HTabKey_t key;                      // hash kluc prvku
    HTabData_t* data;                   // hodnota(obsah) prvku
    HTabItem_t* next;                   // ukazatel na dalsi prvok
};                                      // neuplna deklarace struktury

// Table
typedef struct HTab {
    size_t size;                        // aktualny pocet zaznamov [key,data,next] v tabulke
    size_t arr_size;                    // velkost pola ukazatelov 
    HTabItem_t* array[];                // pole ukazatelov (ptr na NULL alebo na 1. zaznam)
} HTab_t;

// Iterator
typedef struct HTabIterator {
    HTabItem_t* ptr;                    // ukazatel na polozku
    const HTab_t* t;                    // ve ktere tabulce
    size_t idx;                         // ve kterem seznamu tabulky
} HTabIterator_t;

extern HTab_t* globaltab;

// hash func
size_t htabHashFun(HTabKey_t str);                                          // hash function

// table create / delete
HTab_t* htabInit(size_t n);                                                 // konstruktor tabulky
void htabClear(HTab_t* t);                                                  // rusi vsechny zaznamy
void htabFree(HTab_t* t);                                                   // destruktor tabulky (rusi zaznamy)

// get table data
size_t htabSize(const HTab_t* t);                                           // pocet zaznamu v tabulce
size_t htabBucketCount(const HTab_t* t);                                    // velikost pole ptr (tabulky)

// find / create or delete item
HTabIterator_t htabFind(HTab_t* t, HTabKey_t key);                          // hledani klice v tabulce
HTabIterator_t htabFindOrAdd(HTab_t* t, HTabKey_t key);                     // hledani klice v tabulce (ak nenajde, tak vytvori)
void htabErase(HTab_t* t, HTabIterator_t it);                               // rusi zadany zaznam

// iterator change ptr
HTabIterator_t htabBegin(const HTab_t* t);                                  // iterator na prvni zaznam
HTabIterator_t htabEnd(const HTab_t* t);                                    // iterator _za_ posledni zaznam
HTabIterator_t htabIteratorNext(HTabIterator_t it);                         // iterator na dalsi zaznam

// test: iterator != NULL / end()
inline bool htabIteratorValid(HTabIterator_t it) { return it.ptr!=NULL; }
// test: iterator1 == iterator2
inline bool htabIteratorEqual(HTabIterator_t it1, HTabIterator_t it2) { return it1.ptr==it2.ptr && it1.t == it2.t; }

// cteni a zapis pres iterator
HTabKey_t htabIteratorGetKey(HTabIterator_t it);                            // vraci klic ze zaznamu iteratora
HTabData_t* htabIteratorGetData(HTabIterator_t it);                        // vraci hodnotu ze zaznamu iteratora
HTabData_t* htabIteratorSetData(HTabIterator_t it, HTabData_t* data);      // zmeni a vraci hodnotu ze zaznamu iteratora

#endif // __SYMTABLE_H__