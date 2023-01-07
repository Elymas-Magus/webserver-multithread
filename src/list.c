#include "list.h"


void quickSort(ArrayList l, int inicio, int fim, int (*compar)(const void *, const void *));
int quickSortParticionador(ArrayList l, int inicio, int fim, int (*compar)(const void *, const void *));

ArrayList
arrayInit()
{
    ArrayList l = (ArrayList) mallocOrDie(sizeof(List), "ArrayList");

    l->first = l->last = NULL;
    l->length = 0;

    return l;
}

void
arrayPush(ArrayList l, void ** o, size_t osize)
{
    Node * no = (Node *) mallocOrDie(sizeof(Node), "ArrayList push node");
    no->content = (void **) mallocOrDie(osize, "ArrayList push content");

    memcpy(no->content, o, osize);
    no->prev = NULL;
    no->next = NULL;

    if (l->first == NULL) {
        l->first = l->last = no;
    } else {
        no->prev = l->last;
        l->last->next = no;
        l->last = no;
    }

    l->length++;
}

void
arrayUnshift(ArrayList l, void ** o, size_t osize)
{
    Node * no = (Node *) mallocOrDie(sizeof(Node), "ArrayList unshift node");
    no->content = (void **) mallocOrDie(osize, "ArrayList unshift content");

    memcpy(no->content, o, osize);
    no->prev = NULL;
    no->next = NULL;

    if (l->first == NULL) {
        l->first = l->last = no;
    } else {
        no->next = l->first;
        l->first->prev = no;
        l->first = no;
    }

    l->length++;
}


void **
arrayShift(ArrayList l, size_t osize)
{
    void ** content = (void **) mallocOrDie(osize, "ArrayList shift content");
    Node * first = l->first;

    *content = *first->content;
    if (l->first != NULL) {
        l->first = l->first->next;

        if (l->first != NULL){
            l->first->prev = NULL;

            if (l->first->next == NULL) {
                l->last = l->first;
            }
        } else {
            l->last = l->first;
        }

        l->length--;
    }
    free(first);

    return content;
}


void **
arrayPop(ArrayList l, size_t osize)
{
    void ** content = (void **) mallocOrDie(osize, "ArrayList pop content");
    Node * last = l->last;

    if (l->last == NULL || l->length == 0) {
        return NULL;
    }

    *content = *last->content;
    if (l->last->prev == NULL) {
        l->first = l->last = NULL;
    } else {
        l->last = l->last->prev;
        l->last->next = NULL;
    }

    l->length--;

    free(last);

    return content;
}

void
arrayFree(ArrayList l)
{
    Node * tmp;
    Node * nodeList = l->first;
    while (nodeList != NULL) {
        tmp = nodeList;
        nodeList = nodeList->next;
        free(tmp);
    }
    free(l);
}