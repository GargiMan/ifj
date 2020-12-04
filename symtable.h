/**
 * @file symtable.h
 * @author Marek Gergel (xgerge01)
 * @brief IFJ20 Header for symtable operations
 */

#ifndef __SYMTABLE_H__
#define __SYMTABLE_H__

#include <stdlib.h>
#include <string.h>     // size_t
#include <stdbool.h>    // bool
#include "error.h"

typedef enum HTabDataType {
	TYPE_UNDEFINED,	/// Data type undefined
	TYPE_INT,		/// Integer data type
	TYPE_DOUBLE,	/// Double data type
	TYPE_STRING,	/// String data type
	TYPE_BOOL,		/// Bool data type 
} HTabDataType_t;

typedef struct HTabData {
  HTabDataType_t type;      // data type
  bool def;                 // defined
  String_t *params;         // parameters
  char* id;                 // identifier
  bool global;              // global
} HTabData_t;

// Item typedef
typedef char* HTabKey_t;               // typ klice v zazname
typedef struct htab_item HTabItem_t;   // typedef pre item

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

// funkce pro práci s tabulkou:

//hash func
size_t htab_hash_fun(HTabKey_t str);                                        // hash function

//table create / delete
HTab_t *htab_init(size_t n);                                                // konstruktor tabulky
void htab_clear(HTab_t * t);                                                // rusi vsechny zaznamy
void htab_free(HTab_t * t);                                                 // destruktor tabulky

//table data
size_t htab_size(const HTab_t * t);                                         // pocet zaznamu v tabulce
size_t htab_bucket_count(const HTab_t * t);                                 // velikost pole ptr (tabulky)

// find / create or delete item
HTabIterator_t htab_find(HTab_t * t, HTabKey_t key);                        // hledani klice v tabulce
HTabIterator_t htab_lookup_add(HTab_t * t, HTabKey_t key);                  // hledani klice v tabulce (ak nenajde, tak vytvori)
void htab_erase(HTab_t * t, HTabIterator_t it);                             // rusi zadany zaznam

// funkce pro praci s iteratorem:

HTabIterator_t htab_begin(const HTab_t * t);                                // iterator na prvni zaznam
HTabIterator_t htab_end(const HTab_t * t);                                  // iterator _za_ posledni zaznam
HTabIterator_t htab_iterator_next(HTabIterator_t it);                       // iterator++

// test: iterator != end()
inline bool htab_iterator_valid(HTabIterator_t it) { return it.ptr!=NULL; }
// test: iterator1 == iterator2
inline bool htab_iterator_equal(HTabIterator_t it1, HTabIterator_t it2) { return it1.ptr==it2.ptr && it1.t == it2.t; }

// cteni a zapis pres iterator
HTabKey_t htab_iterator_get_key(HTabIterator_t it);                         // vraci klic ze zaznamu iteratora
HTabData_t htab_iterator_get_value(HTabIterator_t it);                      // vraci hodnotu ze zaznamu iteratora
HTabData_t htab_iterator_set_value(HTabIterator_t it, HTabData_t data);     // zmeni a vraci hodnotu ze zaznamu iteratora

#endif // __SYMTABLE_H__