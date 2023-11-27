#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 100

//Limite de processos 5

typedef struct q {
    int queue[MAX_SIZE];
    int front;
    int rear;
    int numElementos;
} queue_type;

void startQueue(queue_type *queueTemp){
    queueTemp->front = -1;
    queueTemp->rear = -1;
    queueTemp->numElementos = 0;
}

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

void printaFila(queue_type *queueTemp){
    printf("Elementos na fila: ");
    for(int i = queueTemp->front; i < queueTemp->numElementos + queueTemp->front; i++){
        printf("%d ", queueTemp->queue[i]);
    }
    printf("| numElementos: %d\n", queueTemp->numElementos);
    puts("");
}

enum tipoIO{
    disco, fita, impressora
};

enum estadoProcesso{
    novo, pronto, execucao, bloqueado, finalizado
};

struct processo{
    int tempoChegada;
    int tempoServico;
    int tempoRestante;
    enum tipoIO tipo;
    // PCB
    int PID;        // Process  ID
    int PPID;       // Parent Process ID
    int prioridade;
    enum estadoProcesso status;
};

int calculaTurnaround(struct processo p[], int numProcess,int quantum){
    struct processo processoDaVez;
    struct processo processosOrdenados[5];
    //struct fila infoFila;
    int contadorFinalizados = 0, instanteAtual = 0, tempoDesdeUltimoQuantum = 0;
    queue_type queueHigh, queueLow;
    startQueue(&queueHigh);
    startQueue(&queueLow);

    // Inicializando o processosOrdenados
    for(int i = 0; i < numProcess; i++){
        processosOrdenados[i] = p[i];
    }

    // Ordenando processos por tempo de chegada
    for(int i = 0; i < numProcess - 1; i++){
        for(int j = 0; j<numProcess - 1; j++){
            if(processosOrdenados[j].tempoChegada > processosOrdenados[j+1].tempoChegada){
                processoDaVez = processosOrdenados[j];
                processosOrdenados[j] = processosOrdenados[j+1];
                processosOrdenados[j+1] = processoDaVez;
            }
        }
    }
    
    // Simulando a cada instante
    while(contadorFinalizados < numProcess){
        contadorFinalizados = 0;
        printf("\n=======\nInstante atual: %d\n", instanteAtual);

        // Coloca o processo na fila se ele ainda não está nela, se ele se encontra no range do instante e ainda tem tempo restante
        for(int i = 0; i < numProcess; i++){
            if(processosOrdenados[i].tempoChegada <= instanteAtual && processosOrdenados[i].tempoRestante > 0){
                printf("PID %d entrou no if | numElementos: %d | front: %d | rear: %d\n", processosOrdenados[i].PID, queueHigh.numElementos, queueHigh.front, queueHigh.rear);
                int processoNaFila = 0;

                if(queueHigh.numElementos != 0){
                    for(int j = queueHigh.front; j < queueHigh.numElementos + queueHigh.front; j++){
                        if(processosOrdenados[i].PID == queueHigh.queue[j]){
                            printf("%d já está na fila\n", processosOrdenados[i].PID);
                            processoNaFila = 1;
                        }
                    }
                }
                
                if(processoNaFila == 0){
                    push(processosOrdenados[i].PID, &queueHigh);
                }
            }
        }

        printaFila(&queueHigh);
        
        if(queueHigh.numElementos > 0){
            int PIDAtual = queueHigh.queue[queueHigh.front];
            if(processosOrdenados[PIDAtual].tempoRestante > 0 && instanteAtual != 0){
                processosOrdenados[PIDAtual].tempoRestante -= 1;
                printf("processo PID %d tempo -= 1, agora com tempoRestante: %d\n", PIDAtual, processosOrdenados[PIDAtual].tempoRestante);
            }
            // se já passou um quantum, tirar da fila
            if(tempoDesdeUltimoQuantum == 5 && instanteAtual != 0){ // instante 19+5
                printf("%d retirado da fila porque passou um quantum\n", PIDAtual);
                pop(&queueHigh);
                tempoDesdeUltimoQuantum = 0;
            } else if(processosOrdenados[PIDAtual].tempoRestante <= 0){ // instante 19
                printf("%d retirado da fila porque ele finalizou\n", PIDAtual);
                pop(&queueHigh);
                tempoDesdeUltimoQuantum = 0;
            }
        }

        for(int i = 0; i < numProcess; i++){
            if(processosOrdenados[i].tempoRestante <= 0){
                printf("finalizou\n");
                contadorFinalizados += 1;
            }
        }
        
        instanteAtual += 1;
        tempoDesdeUltimoQuantum += 1;
    }

    for(int i = 0; i < 5; i++){
        printf("Processo chega:%d\n",processosOrdenados[i].tempoChegada);
    }

    return 0;
}

int main(){
    struct processo processos[5];
    int numProcessos = 5;
    for(int i = 0; i < numProcessos; i++){
        //printf("Processo número %d\n", i);
        
        //printf("Entre com o tempo de chegada: ");
        scanf("%d", &processos[i].tempoChegada);

        //printf("Entre com o tempo de serviço: ");
        scanf("%d", &processos[i].tempoServico);

        //printf("Entre com o tipo de IO (0 = Disco, 1 = Fita, 2 = Impressora): ");
        scanf("%d", &processos[i].tipo);

        processos[i].tempoRestante = processos[i].tempoServico;
        processos[i].PID = i;
        processos[i].prioridade = 0;
        processos[i].status = novo;
    }
    
    calculaTurnaround(processos, 5,5);
    return 0;
}