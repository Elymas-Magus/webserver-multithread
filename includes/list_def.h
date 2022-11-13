#ifndef LISTDEF_H_INCLUDED
#define LISTDEF_H_INCLUDED

typedef struct node
{
    void ** content;   // pointer to the item (info)
    struct node * prev; // pointer to the next Node
    struct node * next; // pointer to the next Node
} Node;

typedef struct list
{
    Node * first; 	// pointer to the first list item
    Node * last; 	// pointer to the last list item
    unsigned int length;   	// number of items in the list
} List;

typedef struct list * ArrayList;

#endif // LISTDEF_H_INCLUDED
