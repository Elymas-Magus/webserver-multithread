#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "list_def.h"
#include "validation.h"

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
void ** arrayShift(ArrayList l, size_t osize);

/**
 * Remove last element of ArrayList
 * And return the content
 * @returns void **
 */
void ** arrayPop(ArrayList l, size_t osize);

/**
 * Unallocate memory for ArrayList l
 * @param ArrayList l
 */
void arrayFree(ArrayList l);

#endif // LISTA_H_INCLUDED
