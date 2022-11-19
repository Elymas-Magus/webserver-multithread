#include "list.h"


void quickSort(ArrayList l, int inicio, int fim, int (*compar)(const void *, const void *));
int quickSortParticionador(ArrayList l, int inicio, int fim, int (*compar)(const void *, const void *));

ArrayList
arrayInit()
{
    ArrayList l = (ArrayList) malloc(sizeof(List));

    l->first = l->last = NULL;
    l->length = 0;

    return l;
}

void
arrayPush(ArrayList l, void ** o, size_t osize)
{
    Node * no = (Node *) malloc (sizeof(Node));
    no->content = (void **) malloc(osize);

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
    Node * no = (Node *) malloc (sizeof(Node));
    no->content = (void **) malloc(osize);

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
arrayShift(ArrayList l)
{
    void ** content = (void **) malloc(sizeof(void *));
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
arrayPop(ArrayList l)
{
    void ** content = (void **) malloc(sizeof(void *));
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
arraySet(ArrayList l, int n, void ** o)
{
    int i;
    Node * no;

    if (l->length <= n || n < 0) {
        return;
    }

    no = l->first;
    for (i = 0; i < n; i++) {
        no = no->next;
    }

    if (no != NULL) {
        no->content = o;
    }
}


void **
arrayItem(ArrayList l, int n)
{
    int i;
    Node * no;

    if (l->length <= n || n < 0) {
        return NULL;
    }

    if (l->length / 2 > n) {
        no = l->first;
        for (i = 0; i < n; i++) {
            no = no->next;
        }
    } else {
        no = l->last;
        for (i = l->length; i > n; i--) {
            no = no->prev;
        }
    }

    return no == NULL ?
        NULL : no->content;
}


Node *
arrayFind(ArrayList l, void ** needle, int (*compar)(const void ** a, const void ** b))
{
    Node * no;

    if (needle == NULL || compar == NULL) {
        return NULL;
    }

    for (no = l->first; no; no = no->next) {
        if (compar((const void **) no->content, (const void **) needle) == 0) {
            return no;
        }
    }

    return NULL;
}


bool
arrayContain(ArrayList l, void ** needle, int (*compar)(const void ** a, const void ** b))
{
    Node * no;

    if (needle == NULL || compar == NULL) {
        return false;
    }

    for (no = l->first; no; no = no->next) {
        if (compar((const void **) no->content, (const void **) needle) == 0) {
            return true;
        }
    }

    return false;
}


int
arrayIndex(ArrayList l, void ** needle, int (*compar)(const void ** a, const void ** b))
{
    int i = 0;
    Node * no;

    if (needle == NULL || compar == NULL) {
        return INDEX_ERROR;
    }

    for (no = l->first; no; no = no->next) {
        if (compar((const void **) no->content, (const void **) needle) == 0) {
            return i;
        }
        i++;
    }

    return INDEX_ERROR;
}


Node *
arrayNode(ArrayList l, void ** o, int (*compar)(const void ** a, const void ** b))
{
    Node * no;

    if (o == NULL || compar == NULL) {
        return NULL;
    }

    for (no = l->first; no; no = no->next) {
        if (compar((const void **) no->content, (const void **) o) == 0) {
            return no;
        }
    }

    return NULL;
}


Node *
arrayNodeByIndex(ArrayList l, int n)
{
    int i = 0;
    Node * no;

    if (n < 0 || n >= l->length) {
        return NULL;
    }

    for (no = l->first; no; no = no->next) {
        if (i == n) {
            return no;
        }
        i++;
    }

    return NULL;
}

void
arrayExchange(ArrayList l, int posA, int posB)
{
    void ** auxiliar;
    Node * noA = arrayNodeByIndex(l, posA);
    Node * noB = arrayNodeByIndex(l, posB);

    if (noA != NULL && noB != NULL) {
        auxiliar = noA->content;
        noA->content = noB->content;
        noB->content = auxiliar;
    }

}


bool
arrayRemove(ArrayList l, int n)
{
    int i;
    Node * no;

    if (l->length <= n || n < 0) {
        return false;
    }

    no = l->first;
    for (i = 0; i < n; i++) {
        no = no->next;
    }

    no->next->prev = no->prev;
    no->prev->next = no->next;

    free(no);

    return true;
}

void
arraySort(ArrayList l, int (*compar)(const void *, const void *))
{
    quickSort(l, 0, l->length - 1, compar);
}

void
quickSort(ArrayList l, int inicio, int fim, int (*compar)(const void *, const void *))
{
    int pivo;

    if (fim <= l->length && fim > inicio) {
        pivo = quickSortParticionador(l, inicio, fim, compar);
        quickSort(l, inicio, pivo - 1, compar);
        quickSort(l, pivo + 1, fim, compar);
    }
}

int
quickSortParticionador(ArrayList l, int inicio, int fim, int (*compar)(const void *, const void *))
{
    int esq, dir;
    void ** pivo, ** aux;

    esq = inicio;
    dir = fim;

    pivo = arrayItem(l, inicio);
    while (esq < dir) {
        while((aux = arrayItem(l, esq)) != NULL && compar((const void **) aux, (const void **) pivo) <= 0) {
            esq++;
        }
        while((aux = arrayItem(l, dir)) != NULL && compar((const void **) aux, (const void **) pivo) > 0) {
            dir--;
        }
        if (esq < dir) {
            arrayExchange(l, esq, dir);
        }
    }
    arraySet(l, inicio, arrayItem(l, dir));
    arraySet(l, dir, pivo);

    return dir;
}

ArrayList
arrayMap(ArrayList l, size_t length, void ** (* callback)(const void **, const int, const ArrayList l))
{
    int i = 0;
    Node * no;
    ArrayList newList = arrayInit();

    for (no = l->first; no; no = no->next) {
        arrayPush(newList, callback((const void **) no->content, i++, l), length);
    }

    return newList;
}

ArrayList
arrayFilter(ArrayList l, size_t length, int (* callback)(const void **, const int, const ArrayList l))
{
    int i = 0;
    Node * no;
    ArrayList newList = arrayInit();

    for (no = l->first; no; no = no->next) {
        if (callback((const void **) no->content, i++, l)) {
            arrayPush(newList, no->content, length);
        }
    }

    return newList;
}

void **
arrayReduce(ArrayList l, void ** (* callback)(void ** accum, const void **, const int, const ArrayList l), void ** init)
{
    int i = 0;
    Node * no;
    void ** accum = (void **) malloc(sizeof(void *));

    *accum = *init; 
    for (no = l->first; no; no = no->next) {
        accum = callback(accum, (const void **) no->content, i++, l);
    }

    return accum;
}

void
arrayFree(ArrayList l)
{
    Node * nodeList = l->first;
    Node * tmp;
    while (nodeList != NULL) {
        tmp = nodeList;
        nodeList = nodeList->next;
        free(tmp);
    }
    free(l);
}