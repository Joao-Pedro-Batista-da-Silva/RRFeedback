#include "queue.h"

void push(int element, queue_type *queueTemp) {
    if (queueTemp->rear == MAX_SIZE - 1) {
        printf("Queue is full");
        return;
    }
    if (queueTemp->front == -1) {
        queueTemp->front = 0;
    }
    queueTemp->rear++;
    queueTemp->queue[queueTemp->rear] = element;
    queueTemp->numElementos++;
    printf("entrou %d na fila, numElementos %d\n", element, queueTemp->numElementos);
}

int pop(queue_type *queueTemp)  {
    if (queueTemp->front == -1 || queueTemp->front > queueTemp->rear) {
        printf("Queue is empty");
        return -1;
    }
    int element = queueTemp->queue[queueTemp->front];
    queueTemp->front++;
    queueTemp->numElementos--;
    printf("saiu %d da fila, numElementos %d\n", element, queueTemp->numElementos);
    return element;
}

void printQueue(queue_type *queueTemp){
    printf("Elementos na fila: ");
    for(int i = queueTemp->front; i < queueTemp->numElementos + queueTemp->front; i++){
        printf("%d ", queueTemp->queue[i]);
    }
    printf("| numElementos: %d\n", queueTemp->numElementos);
    puts("");
}

void startQueue(queue_type *queueTemp){
    queueTemp->front = -1;
    queueTemp->rear = -1;
    queueTemp->numElementos = 0;
}