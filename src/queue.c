#include "queue.h"

node_t * head = NULL;
node_t * tail = NULL;

void
enqueue(Client * client)
{
    node_t * newNode = malloc(sizeof(node_t));

    newNode->client = client;
    newNode->next = NULL;

    if (tail == NULL) {
        head = newNode;
    } else {
        tail->next = newNode;
    }

    tail = newNode;
}

Client *
dequeue()
{
    if (head == NULL) {
        return NULL;
    }

    Client * result = head->client;
    node_t * temp = head;
    head = head->next;

    if (head == NULL) {
        tail = NULL;
    }

    free(temp);

    return result;
}

bool
queueIsEmpty()
{
    return head == NULL;
}