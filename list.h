/**
 * @file list.h
 * @author Marek Gergel (xgerge01)
 * @brief IFJ20 - Operations over list and list item for literals
 */

#ifndef __LIST_H__
#define __LIST_H__

#include <stdlib.h>

//list item structure

typedef struct LListItem LListItem_t;

struct LListItem {
    char* token;
    LListItem_t* pNext;
};

//list structure

typedef struct LList {
    LListItem_t* pHead;
    LListItem_t* pTail;
} LList_t;


//list variable declared in main.c
extern LList_t list;

//list item operations (create, set token, next, destroy)

#define ITEM_TOKEN(item) (item->token)
#define ITEM_NEXT(item) (item->pNext)

#define ITEM_CREATE(item) do { if ((item = malloc(sizeof(LListItem_t)))) {ITEM_TOKEN(item) = NULL; ITEM_NEXT(item) = NULL;} else {}} while (0)
#define ITEM_SET_TOKEN(item, token) (ITEM_TOKEN(item) = token)
#define ITEM_DESTROY(item) (free(item))

//list operations (add item, clear, is empty)

#define LIST_HEAD list.pHead
#define LIST_TAIL list.pTail
#define LIST_IS_EMPTY (!LIST_HEAD)

#define LIST_ADD_ITEM(item) if (LIST_IS_EMPTY) {LIST_HEAD = item; LIST_TAIL = item;} else {ITEM_NEXT(LIST_TAIL) = item; LIST_TAIL = item;} (void)0
#define LIST_CLEAR while (!LIST_IS_EMPTY) {LIST_TAIL = ITEM_NEXT(LIST_HEAD); ITEM_DESTROY(LIST_HEAD); LIST_HEAD = LIST_TAIL;}

#endif // __LIST_H__