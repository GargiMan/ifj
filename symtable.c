/**
 * @file symtable.c
 * @author Marek Gergel (xgerge01)
 * @brief IFJ20 - Operations over symtable
 */

#include "symtable.h"

size_t htabHashFun(HTabKey_t str) {

    //hash variables
    uint32_t h = 0;     // must be 32bit int
    const unsigned char* p;

    //hash index calculation
    for (p = (const unsigned char*)str; *p != '\0'; p++) h = 65599 * h + *p;

    return h;
}

HTab_t* htabInit(size_t n) {

    //table allocation = table struct + item struct pointer * array size
    HTab_t* tab = malloc(sizeof(HTab_t) + sizeof(HTabItem_t*) * n); 
    if (tab == NULL) errorExit(internalError, "symtable : Table allocation failed\n");

    //set table array size and number of items
    tab->size = 0;
    tab->arr_size = n;

    //set all item pointers to NULL
    for (size_t i = 0; i < n; i++) tab->array[i] = NULL;

    return tab;
}

void htabClear(HTab_t* t) {

    //check table pointer
    if (t == NULL) return;

    //move to next index
    for (size_t i = 0; i < t->arr_size; i++) {

        //erase item and move to next one
        while (t->array[i] != NULL) {
            HTabItem_t* item_next = t->array[i]->next;
            free(t->array[i]->key);
            if (t->array[i]->data) free(t->array[i]->data->params);
            if (t->array[i]->data) free(t->array[i]->data->functypes);
            free(t->array[i]->data);
            free(t->array[i]);
            t->array[i] = item_next;
        }
    }

    //set number of items in table
    t->size = 0;

    return;
}

void htabFree(HTab_t* t) {
    
    //erase all items
    htabClear(t);

    //erase table
    free(t);
    
    return;
}

size_t htabSize(const HTab_t* t) {

    //check table pointer
    if (t == NULL) return 0;

    //return number of items in table
    return t->size;
}

size_t htabBucketCount(const HTab_t* t) {

    //check table pointer
    if (t == NULL) return 0;

    //return size of table array
    return t->arr_size;
}

HTabIterator_t htabFind(HTab_t* t, HTabKey_t key) {

    //check table pointer
    if (t == NULL || key == NULL) return htabEnd(t);

    //index calculation for key
    size_t index = htabHashFun(key) % t->arr_size;

    //new iterator for return
    HTabIterator_t it_found = {t->array[index], t, index};

    //search for key
    while(htabIteratorValid(it_found) && it_found.idx == index) {
        if (strcmp(htabIteratorGetKey(it_found), key) == 0) return it_found;
        it_found = htabIteratorNext(it_found);
    }

    return it_found;
}

HTabIterator_t htabFindOrAdd(HTab_t* t, HTabKey_t key) {

    //check table pointer
    if (t == NULL || key == NULL) return htabEnd(t);

    //index calculation for key
    size_t index = htabHashFun(key) % t->arr_size;

    //new iterator for return
    HTabIterator_t it_found = {t->array[index], t, index};

    //search for key
    while(htabIteratorValid(it_found) && it_found.idx == index) {
        if (strcmp(htabIteratorGetKey(it_found), key) == 0) return it_found;
        it_found = htabIteratorNext(it_found);
    }

    //item allocation and check pointer
    it_found.ptr = malloc(sizeof(HTabItem_t));
    if (it_found.ptr == NULL) errorExit(internalError, "symtable : New item allocation failed");

    //key allocation and check pointer
    it_found.ptr->key = malloc(strlen(key) + 1);
    if (it_found.ptr->key == NULL) {
        free(it_found.ptr);
        errorExit(internalError, "symtable : New item key allocation failed");
    }

    //data allocation and check pointer
    it_found.ptr->data = malloc(sizeof(HTabData_t));
    if (it_found.ptr->data == NULL) {
        free(it_found.ptr->key);
        free(it_found.ptr);
        errorExit(internalError, "symtable : New item data allocation failed");
    }

    //set key and value for item
    strcpy(it_found.ptr->key,key);
    htabIteratorSetData(it_found, 0);
    
    //add new item in table
    it_found.ptr->next = t->array[index];
    t->array[index] = it_found.ptr;
    t->size++;

    return it_found;
}

void htabErase(HTab_t* t, HTabIterator_t it) {

    //check table and iterator pointer
    if (t == NULL || !htabIteratorValid(it)) return;

    //first item at index will be erased
    if (t->array[it.idx] == it.ptr) {

        //change pointer at index
        t->array[it.idx] = it.ptr->next;

    } else { 
        
        //find previous iterator
        HTabIterator_t it_prev = {t->array[it.idx], t, it.idx};
        while (it_prev.ptr->next != it.ptr) it_prev.ptr = it_prev.ptr->next;
        it_prev.ptr->next = it.ptr->next;
    }

    //erase item
    free(it.ptr->key);
    if (it.ptr->data) free(it.ptr->data->params);
    if (it.ptr->data) free(it.ptr->data->functypes);
    free(it.ptr->data);
    free(it.ptr);
    t->size--;

    return;
}

HTabIterator_t htabBegin(const HTab_t* t) {

    //new iterator for return
    HTabIterator_t it_begin = {NULL, t, 0};

    //check table pointer
    if (t == NULL) return it_begin;

    //search for first index with item
    while (it_begin.idx < t->arr_size && t->array[it_begin.idx] == NULL) it_begin.idx++;

    //empty table
    if (it_begin.idx == t->arr_size) it_begin.idx = 0;

    //set pointer with index
    it_begin.ptr = t->array[it_begin.idx];

    return it_begin;
}

HTabIterator_t htabEnd(const HTab_t* t) {

    //new iterator for return
    HTabIterator_t it_end = {NULL, t, 0};

    //check table pointer
    if (t == NULL) return it_end;

    //set index after table pointer check
    it_end.idx = t->arr_size - 1;

    //search for last index with item
    while (it_end.idx > 0 && t->array[it_end.idx] == NULL) it_end.idx--;

    //not last index is end or first is end
    if ((it_end.idx > 0 && it_end.idx < t->arr_size - 1) || (it_end.idx == 0 && t->array[it_end.idx] != NULL)) it_end.idx++;

    return it_end;
}

HTabIterator_t htabIteratorNext(HTabIterator_t it) {

    //check iterator pointer
    if (!htabIteratorValid(it)) return it;

    //new iterator for return
    HTabIterator_t it_next = {NULL, it.t, it.idx};

    //not last item at index
    if (it.ptr->next != NULL) {
        
        //move to next item within same index
        it_next.ptr = it.ptr->next;

    //last at index
    } else {

        //move next index until next is not null
        while (it_next.ptr == NULL) {
            if (it_next.t->arr_size == ++it_next.idx) return htabEnd(it_next.t);
            it_next.ptr = it_next.t->array[it_next.idx];
        }
    }

    return it_next;
}

// test: iterator != end()
extern bool htabIteratorValid(HTabIterator_t it);

// test: iterator1 == iterator2
extern bool htabIteratorEqual(HTabIterator_t it1, HTabIterator_t it2);

HTabKey_t htabIteratorGetKey(HTabIterator_t it) {

    //check iterator pointer
    if (!htabIteratorValid(it)) return NULL;

    //return key from iterator
    return it.ptr->key;
}

HTabData_t* htabIteratorGetData(HTabIterator_t it) {

    //check iterator pointer
    if (!htabIteratorValid(it)) return NULL;

    //return value from iterator
    return it.ptr->data;
}

HTabData_t* htabIteratorSetData(HTabIterator_t it, HTabData_t* data) {

    //check iterator pointer
    if (!htabIteratorValid(it)) return NULL;

    //set iterator (item) value and return it
    return it.ptr->data = data;
}