/**
 * @file list.h
 * @author Marek Gergel (xgerge01)
 * @brief IFJ20 - Operations over list and item for literals
 */

#ifndef LIST_H
#define LIST_H

#include <stdlib.h>
#include "token.h"
#include "error.h"

//item structure

typedef struct ListItem ListItem_t;

struct ListItem {
    Token_t* token;
    ListItem_t* pNext;
};

//list structure

typedef struct List {
    ListItem_t* pHead;
    ListItem_t* pTail;
} List_t;

extern List_t list;

//list item operations (create, set token, next, destroy)

#define ITEM_TOKEN(item) (item->token)
#define ITEM_NEXT(item) (item->pNext)

#define ITEM_CREATE(item) do { if ((item = malloc(sizeof(ListItem_t)))) {if (!(ITEM_TOKEN(item) = malloc(sizeof(Token_t)))) errorExit(internalError, "list.h : Token allocation failed"); ITEM_NEXT(item) = NULL;} else {errorExit(internalError, "list.h : Item allocation failed");}} while (0)
#define ITEM_DESTROY(item) (free(ITEM_TOKEN(item)), free(item))

//list operations (add item, clear, is empty)

#define LIST_HEAD (list.pHead)
#define LIST_TAIL (list.pTail)
#define LIST_IS_EMPTY (!LIST_HEAD)

#define LIST_ADD_ITEM(item) if (LIST_IS_EMPTY) {LIST_HEAD = item; LIST_TAIL = item;} else {ITEM_NEXT(LIST_TAIL) = item; LIST_TAIL = item;} (void)0
#define LIST_CLEAR while (!LIST_IS_EMPTY) {LIST_TAIL = ITEM_NEXT(LIST_HEAD); ITEM_DESTROY(LIST_HEAD); LIST_HEAD = LIST_TAIL;}

#endif // LIST_H