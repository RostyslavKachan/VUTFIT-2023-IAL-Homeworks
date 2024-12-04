/*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) {
  if (table != NULL) 
        for (int i = 0; i < HT_SIZE; ++i)
            (*table)[i] = NULL;   
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
 if (table != NULL) {
        int i = get_hash(key);
        ht_item_t* itemTable = (*table)[i];

        while (itemTable != NULL) {
            if (itemTable->key == key)             
                return itemTable;
            itemTable = itemTable->next;            
        }
    }

    return NULL;
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
  if (table != NULL) {
        ht_item_t* itemTable = ht_search(table, key);
        if (itemTable != NULL)
            itemTable->value = value;
        else {
            ht_item_t* newItem = malloc(sizeof(ht_item_t));
            if (newItem != NULL) {
                newItem->key = key;
                newItem->value = value;
                newItem->next = NULL;

                int i = get_hash(key);
                if ((*table)[i] != NULL){
                    ht_item_t* replaceItem = (*table)[i];
                    (*table)[i] = newItem;
                    newItem->next = replaceItem;
                }
                else 
                    (*table)[i] = newItem;            
            }
        }
    }
}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
  if (table != NULL) {
        ht_item_t* itemTable = ht_search(table, key);
        if (itemTable != NULL)
            return &itemTable->value;
    }
    return NULL;
}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
  if (table != NULL) {
        int i = get_hash(key);
        ht_item_t* itemTable = (*table)[i];
        ht_item_t* prevItemTable = NULL;
        if (itemTable != NULL) {
            while (itemTable != NULL) {
                if (itemTable->key == key){ 
                    if (prevItemTable == NULL) 
                        (*table)[i] = itemTable->next;
                    else
                        prevItemTable->next = itemTable->next;
                    free(itemTable);
                    break;
                }
                else {
                    prevItemTable = itemTable;
                    itemTable = prevItemTable->next;
                }
            }
        }
    }
}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table) {
  if (table != NULL) {
        for (int i = 0; i < HT_SIZE; ++i){
            ht_item_t* itemTable;
            while ((*table)[i]) {
                itemTable = (*table)[i];
                (*table)[i] = (*table)[i]->next;
                free(itemTable);
            }
            (*table)[i] = NULL;
        }
    }
}
