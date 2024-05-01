#include <stdlib.h>
#include "../../headers/utils/queue.h"


typedef struct node {
    void *val;
    struct node *next;
} QueueNode;

struct queue {
    QueueNode *first, **plast;
    int len;
};

Queue *q_init() {
    Queue *q = malloc(sizeof(Queue));
    q->first = NULL;
    q->plast = &q->first;
    q->len = 0;
    return q;
}

void q_put(Queue *q, void *ptr) {
    QueueNode *node = malloc(sizeof(QueueNode));
    node->val = ptr;
    node->next = NULL;
    *q->plast = node;
    q->plast = &node->next;
    q->len++;
}

void *q_get(Queue *q) {
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

void *q_peek(Queue *q) {
    if (q->first == NULL) {
        return NULL;
    } else {
        return q->first->val;
    }
}

int q_empty(Queue *q) {
    return q->first == NULL;
}

int q_length(Queue *q) {
    return q->len;
}

void q_destroy(Queue *q) {
    QueueNode *node = q->first;
    while (node != NULL) {
        QueueNode *next = node->next;
        free(node);
        node = next;
    }
    free(q);
}

void q_swap(Queue **q1, Queue **q2){
    Queue *temp = *q1;
    *q1 = *q2;
    *q2 = temp;
}