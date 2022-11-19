#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "list_def.h"

#define INDEX_ERROR         -1
#define FOREACH(NO, LIST)   for ((NO) = LIST->first; (NO); (NO) = NO->next)

/**
 * Create pointer for list (ArrayList)
 * @returns ArrayList
 */
ArrayList arrayInit();

/**
 * Inserts an element at the beginning of the array
 * @returns ArrayList
 */
void
arrayUnshift(ArrayList l, void ** o, size_t osize);

/**
 * Inserts an element at the end of the array
 * @returns ArrayList
 */
void arrayPush(ArrayList l, void ** o, size_t osize);

/**
 * Remove first index of ArrayList
 * And returns the content
 * @returns void **
 */
void ** arrayShift(ArrayList l);

/**
 * Remove last element of ArrayList
 * And return the content
 * @returns void **
 */
void ** arrayPop(ArrayList l);

/**
 * Set nth index in array and replace content
 */
void arraySet(ArrayList l, int n, void ** o);

/**
 * Change two index content in ArrayList
 */
void arrayExchange(ArrayList l, int posA, int posB);

/**
 * Get nth element content in ArrayList
 * @returns void **
 */
void ** arrayItem(ArrayList l, int n);

/**
 * Find element in ArrayList
 * Returns true if the element exists in Array
 * Otherwise returns false
 * @returns bool
 */
bool arrayContain(ArrayList l, void ** needle, int (*compar)(const void ** a, const void ** b));

/**
 * Find element in ArrayList
 * @returns void **
 */
Node * arrayFind(ArrayList l, void ** needle, int (* compar)(const void ** a, const void ** b));

/**
 * Sort the ArrayList using QuickSort
 */
void arraySort(ArrayList l, int (*compar)(const void *, const void *));

/**
 * Find element in ArrayList
 * Returns index if the element exists in Array
 * And -1 if it's not
 * @returns int
 */
int arrayIndex(ArrayList l, void ** needle, int (* compar)(const void ** a, const void ** b));

/**
 * Remove the nth element from ArrayList
 * @returns bool
 */
bool arrayRemove(ArrayList l, int n);

/**
 * Get Node of nth element in ArrayList
 * @returns Node *
 */
Node * arrayNodeByIndex(ArrayList l, int n);

/**
 * Get Node of the first element 
 * Which compar return true
 * @returns Node *
 */
Node * arrayNode(ArrayList l, void ** o, int (* compar)(const void ** a, const void ** b));

/**
 * Create new ArrayList with the elements
 * which callback returns
 * @returns ArrayList
 */
ArrayList arrayMap(ArrayList l, size_t length, void ** (* callback)(const void **, const int, const ArrayList l));

/**
 * Create new ArrayList with the elements
 * which callback returns true
 * @returns ArrayList
 */
ArrayList arrayFilter(ArrayList l, size_t length, int (* callback)(const void **, const int, const ArrayList l));

/**
 * Accumulate ArrayList content
 * @returns void **
 */
void ** arrayReduce(ArrayList l, void ** (* callback)(void ** accum, const void **, const int, const ArrayList l), void ** init);

/**
 * Unallocate memory for ArrayList l
 * @param ArrayList l
 */
void arrayFree(ArrayList l);

#endif // LISTA_H_INCLUDED
