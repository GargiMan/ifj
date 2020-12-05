/**
 * @file resources.h
 * @author Marek Gergel (xgerge01)
 * @brief IFJ20 - Free resources
 */

#ifndef RESOURCES_H
#define RESOURCES_H

#include "tokenlist.h"
#include "symtable.h"

#define FREE_RESOURCES do {LIST_CLEAR; htabFree(globaltab);} while (0)

#endif // RESOURCES_H