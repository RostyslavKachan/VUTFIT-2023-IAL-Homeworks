/*
 * Binárny vyhľadávací strom — iteratívna varianta
 *
 * S využitím dátových typov zo súboru btree.h, zásobníkov zo súborov stack.h a
 * stack.c a pripravených kostier funkcií implementujte binárny vyhľadávací
 * strom bez použitia rekurzie.
 */

#include "../btree.h"
#include "stack.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
   if (tree != NULL)
    *tree = NULL; 
}

/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
  while (tree != NULL){
        if (tree->key == key){     
            *value = tree->value;   
            return true;
        }
        if (tree->key > key) 
            tree = tree->left;
        if (tree->key < key)  
            tree = tree->right;      
    }
    return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
   while (*tree != NULL){
        if ((*tree)->key > key)          
            tree = &(*tree)->left;       
        else if ((*tree)->key < key)
            tree = &(*tree)->right;      
        else if ((*tree)->key == key) { 
            (*tree)->value = value;      
            return;
        }
    }

    if (*tree == NULL){ 
        bst_node_t* createNode = (bst_node_t*)malloc(sizeof(bst_node_t));
        if (createNode) {
            createNode->key = key;
            createNode->value = value;
            createNode->left = NULL;
            createNode->right = NULL;
            *tree = createNode;
        }
    }
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte iteratívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
  if (target->right != NULL) {
        while ((*tree)->right != NULL) 
            tree = &(*tree)->right; 

        target->value = (*tree)->value; 
        target->key = (*tree)->key;
        bst_node_t* deleteNode = *tree;
        *tree = (*tree)->left;
        free(deleteNode); 
    }
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte iteratívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
    while (*tree != NULL){ 
        if ((*tree)->key > key)
            tree = &(*tree)->left;
        else if ((*tree)->key < key)
            tree = &(*tree)->right;
        else if ((*tree)->key == key)      
            break;
    }

    if (*tree != NULL){
        if ((*tree)->left == NULL && (*tree)->right == NULL) { 
            free(*tree);
            *tree = NULL;
        }
        else if ((*tree)->left != NULL && (*tree)->right != NULL) 
            bst_replace_by_rightmost(*tree, &(*tree)->left);
        else if ((*tree)->left != NULL || (*tree)->right != NULL) { 
            bst_node_t* deleteNode = *tree;
            if ((*tree)->left) 
                *tree = (*tree)->left; 
            if ((*tree)->right)
                *tree = (*tree)->right;
            free(deleteNode);
        }
    }
}
/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
    if (*tree != NULL){
        stack_bst_t stack;
        stack_bst_init(&stack);             
        stack_bst_push(&stack, *tree);      

        while (!stack_bst_empty(&stack)){      
        bst_node_t* nodeParent = stack_bst_pop(&stack);    
        if (nodeParent->left != NULL)
            stack_bst_push(&stack, nodeParent->left); 
        if (nodeParent->right != NULL)
            stack_bst_push(&stack, nodeParent->right);
        free(nodeParent);   
        }
        *tree = NULL;
    }
}

/*
 * Pomocná funkcia pre iteratívny preorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu.
 * Nad spracovanými uzlami zavola bst_print_node a uloží ich do zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_preorder(bst_node_t *tree, stack_bst_t *to_visit) {
  while (tree != NULL) {      
        stack_bst_push(to_visit, tree);   
        bst_print_node(tree); 
        tree = tree->left;
    }
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_preorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
  stack_bst_t* stack = malloc(sizeof(stack_bst_t));
    if (stack) {
        stack_bst_init(stack);
        bst_leftmost_preorder(tree, stack);
        while (!stack_bst_empty(stack)) { 
            tree = stack_bst_pop(stack);
            if (tree->right)
                bst_leftmost_preorder(tree->right, stack);
        }
        free(stack);
    }
}

/*
 * Pomocná funkcia pre iteratívny inorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov.
 *
 * Funkciu implementujte iteratívne pomocou zásobníku uzlov a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_inorder(bst_node_t *tree, stack_bst_t *to_visit) {
  while (tree != NULL) {
        stack_bst_push(to_visit, tree);   
        tree = tree->left;
    }
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_inorder a
 * zásobníku uzlov bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
  stack_bst_t* stack = malloc(sizeof(stack_bst_t)); 
    if (stack != NULL) {
        stack_bst_init(stack);  
        bst_leftmost_inorder(tree, stack); 

        while (!stack_bst_empty(stack)) {
            tree = stack_bst_pop(stack); 
            bst_print_node(tree);
            if (tree->right)
                bst_leftmost_inorder(tree->right, stack); 
        }
        free(stack);
    }
  
}

/*
 * Pomocná funkcia pre iteratívny postorder.
 *
 * Prechádza po ľavej vetve k najľavejšiemu uzlu podstromu a ukladá uzly do
 * zásobníku uzlov. Do zásobníku bool hodnôt ukladá informáciu že uzol
 * bol navštívený prvý krát.
 *
 * Funkciu implementujte iteratívne pomocou zásobníkov uzlov a bool hodnôt a bez použitia
 * vlastných pomocných funkcií.
 */
void bst_leftmost_postorder(bst_node_t *tree, stack_bst_t *to_visit,
                            stack_bool_t *first_visit) {
      while (tree != NULL) {
        stack_bst_push(to_visit, tree); 
        stack_bool_push(first_visit, true);
        tree = tree->left;
    }                         
}

/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte iteratívne pomocou funkcie bst_leftmost_postorder a
 * zásobníkov uzlov a bool hodnôt bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
  stack_bst_t* bstNodeStack = malloc(sizeof(stack_bst_t)); 
    if (bstNodeStack != NULL) {
        stack_bool_t* boolStack = malloc(sizeof(stack_bool_t)); 
        if (boolStack != NULL) {
            stack_bst_init(bstNodeStack);   
            stack_bool_init(boolStack);     
            bst_leftmost_postorder(tree, bstNodeStack, boolStack); 

            while (!stack_bst_empty(bstNodeStack)) {
                bst_node_t* nodeBst = stack_bst_pop(bstNodeStack); 
                bool stackFromLeft = stack_bool_pop(boolStack);
                if (stackFromLeft) {
                    stack_bst_push(bstNodeStack, nodeBst);
                    stack_bool_push(boolStack, false);
                    bst_leftmost_postorder(nodeBst->right, bstNodeStack, boolStack);
                }
                else bst_print_node(nodeBst);
            }
            free(bstNodeStack);
            free(boolStack);
        }
    }
}
