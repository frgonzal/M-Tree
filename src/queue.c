#include <stdlib.h>
#include "../headers/queue.h"


typedef struct node {
    void *val;
    struct node *next;
} QueueNode;

struct queue {
    QueueNode *first, **plast;
    int len;
};

Queue *makeQueue() {
    Queue *q = malloc(sizeof(Queue));
    q->first = NULL;
    q->plast = &q->first;
    q->len = 0;
    return q;
}

void put(Queue *q, void *ptr) {
    QueueNode *node = malloc(sizeof(QueueNode));
    node->val = ptr;
    node->next = NULL;
    *q->plast = node;
    q->plast = &node->next;
    q->len++;
}

void *get(Queue *q) {
    if (q->first == NULL) {
        return NULL;
    }
    void *val = q->first->val;
    QueueNode *next = q->first->next;
    free(q->first);
    q->first = next;
    if (next == NULL) {
        q->plast = &q->first;
    }
    q->len--;
    return val;
}

void *peek(Queue *q) {
    if (q->first == NULL) {
        return NULL;
    } else {
        return q->first->val;
    }
}

int emptyQueue(Queue *q) {
    return q->first == NULL;
}

int queueLength(Queue *q) {
    return q->len;
}

void destroyQueue(Queue *q) {
    QueueNode *node = q->first;
    while (node != NULL) {
        QueueNode *next = node->next;
        free(node);
        node = next;
    }
    free(q);
}