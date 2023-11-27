#include "constants.h"

typedef struct q {
    int queue[MAX_SIZE];
    int front;
    int rear;
    int numElementos;
} queue_type;

void push(int element, queue_type *queueTemp);
int pop(queue_type *queueTemp);
void printQueue(queue_type *queueTemp);
void startQueue(queue_type *queueTemp);