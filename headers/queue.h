/** Cola generica */
typedef struct queue Queue;


Queue *makeQueue();
void destroyQueue(Queue *q);
void put(Queue *q, void *ptr);
void *get(Queue *q);
void *peek(Queue *q);
int emptyQueue(Queue *q);
int queueLength(Queue *q);




// gracias mateu