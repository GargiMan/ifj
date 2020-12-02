#include <stdio.h>
#include <malloc.h>
#include "string.h"
#include "symtable.h"

int HTSIZE = MAX_HTSIZE;
int solved;

/*          -------
** Rozptylovací funkce - jejím úkolem je zpracovat zadaný klíč a přidělit
** mu index v rozmezí 0..HTSize-1.  V ideálním případě by mělo dojít
** k rovnoměrnému rozptýlení těchto klíčů po celé tabulce.  V rámci
** pokusů se můžete zamyslet nad kvalitou této funkce.  (Funkce nebyla
** volena s ohledem na maximální kvalitu výsledku). }
*/

int hashCode (const char* key ) {
	int retval = 1;
	int keylen = strlen(key);
	for ( int i=0; i<keylen; i++ )
		retval += key[i];
	return ( retval % HTSIZE );
}

/*
** Inicializace tabulky s explicitně zřetězenými synonymy.  Tato procedura
** se volá pouze před prvním použitím tabulky.
*/

void htInit ( tHTable* ptrht ) {

 	if(!ptrht){

		return;
	}
	memset(*ptrht,NULL,sizeof(struct tHTItem*)*HTSIZE);
}

/* TRP s explicitně zřetězenými synonymy.
** Vyhledání prvku v TRP ptrht podle zadaného klíče key.  Pokud je
** daný prvek nalezen, vrací se ukazatel na daný prvek. Pokud prvek nalezen není,
** vrací se hodnota NULL.
**
*/

tHTItem* htSearch ( tHTable* ptrht,const char* key ) {

    if(!ptrht || !(*ptrht)){
	  	return NULL;
	}
	tHTItem* tmp = (*ptrht)[hashCode(key)];
	while(tmp){
		if(!strcmp(tmp->key, key)){
			return tmp;
		}
		tmp = tmp->ptrnext;
	}
	return NULL;
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vkládá do tabulky ptrht položku s klíčem key a s daty
** data.  Protože jde o vyhledávací tabulku, nemůže být prvek se stejným
** klíčem uložen v tabulce více než jedenkrát.  Pokud se vkládá prvek,
** jehož klíč se již v tabulce nachází, aktualizujte jeho datovou část.
**
** Využijte dříve vytvořenou funkci htSearch.  Při vkládání nového
** prvku do seznamu synonym použijte co nejefektivnější způsob,
** tedy proveďte.vložení prvku na začátek seznamu.
**/

void htInsert ( tHTable* ptrht,const char* key, tData *data , int Data_type ) {

 	if(!ptrht){
	  	return;
 	}
	tHTItem* tmp = htSearch(ptrht,key);
	if(tmp){
		//ak uz existuje v danej tabulke,tak chyba
	  	//return 1;
	}
	else{

	  	tmp = (*ptrht)[hashCode(key)];
	  	tHTItem* newItem = malloc(sizeof(struct tHTItem));
		if (!newItem){
			//return;
		}

		newItem->key = key;
		newItem->data->global = data->global;
		newItem->data->identifier = data->identifier;
		newItem->data->type = data->type;
		newItem->data->params = data->params;
		newItem->data->defined = data->defined;
		(*ptrht)[hashCode(key)] = newItem;
		newItem->ptrnext = tmp;
		//return 0;
	}
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato funkce zjišťuje hodnotu datové části položky zadané klíčem.
** Pokud je položka nalezena, vrací funkce ukazatel na položku
** Pokud položka nalezena nebyla, vrací se funkční hodnota NULL
**
** Využijte dříve vytvořenou funkci HTSearch.
*/

tData* htRead ( tHTable* ptrht,const char* key ) {

	if(!ptrht){
	  	return NULL;
	}
	tHTItem* tmp = htSearch(ptrht,key);

	if(tmp){
	  	return &(tmp->data);
	}
	else{
		return NULL;
	}
}

/*
** TRP s explicitně zřetězenými synonymy.
** Tato procedura vyjme položku s klíčem key z tabulky
** ptrht.  Uvolněnou položku korektně zrušte.  Pokud položka s uvedeným
** klíčem neexistuje, dělejte, jako kdyby se nic nestalo (tj. nedělejte
** nic).
**
** V tomto případě NEVYUŽÍVEJTE dříve vytvořenou funkci HTSearch.
*/

void htDelete ( tHTable* ptrht,const char* key ) {

 	if(!ptrht || !(*ptrht)){
	  	return;
	}
	
	tHTItem* tmp = (*ptrht)[hashCode(key)];
	tHTItem* tmp_prev = NULL;
	tHTItem* tmp_found = NULL;
	if(!tmp){
	  	return;
	}

	while (tmp){
		if (!strcmp(tmp->key, key)){
			tmp_found = tmp;
			break;
		}
		tmp_prev = tmp;
		tmp = tmp->ptrnext;
	}
	if(!tmp_found){
		return;
	}
	if(!tmp_prev){
		tmp = tmp_found->ptrnext;
		free(tmp_found);
		(*ptrht)[hashCode(key)] = tmp;
		return;
	}

	tmp = tmp_found->ptrnext;
	free(tmp_found);
	tmp_prev->ptrnext = tmp;
	
}

/* TRP s explicitně zřetězenými synonymy.
** Tato procedura zruší všechny položky tabulky, korektně uvolní prostor,
** který tyto položky zabíraly, a uvede tabulku do počátečního stavu.
*/

void htClearAll ( tHTable* ptrht ) {

	for (int i = 0; i < HTSIZE; ++i)
	{
		tHTItem* tmp = (*ptrht)[i];
		while (tmp)
		{
			tHTItem* next = tmp->ptrnext;
			free(tmp);
			tmp = next;
		}

		(*ptrht)[i] = NULL;
	}
}
