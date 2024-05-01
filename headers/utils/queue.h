/** Cola generica */
typedef struct queue Queue;


Queue *q_init();
void q_destroy(Queue *q);
void q_put(Queue *q, void *ptr);
void *q_get(Queue *q);
void *q_peek(Queue *q);
int q_empty(Queue *q);
int q_length(Queue *q);
void q_swap(Queue **q1, Queue **q2);




// gracias mateu