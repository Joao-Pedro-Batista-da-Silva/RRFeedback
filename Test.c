#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 100

//Limite de processos 5

int queue[MAX_SIZE];
int front = -1;
int rear = -1;
int numElementos = 0;
void enqueue(int element) {
    if (rear == MAX_SIZE - 1) {
        printf("Queue is full");
        return;
    }
    if (front == -1) {
        front = 0;
    }
    rear++;
    queue[rear] = element;
    numElementos++;
    printf("entrou %d na fila, numElementos %d\n", element, numElementos);
}

int dequeue() {
    if (front == -1 || front > rear) {
        printf("Queue is empty");
        return -1;
    }
    int element = queue[front];
    front++;
    numElementos--;
    printf("saiu %d da fila, numElementos %d\n", element, numElementos);
    return element;
}

void printaFila(){
    printf("Elementos na fila: ");
    for(int i = front; i < numElementos+front; i++){
        printf("%d ", queue[i]);
    }
    printf("| numElementos: %d\n", numElementos);
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

struct fila {
    int primeiro;
    int ultimo;
};

int calculaTurnaround(struct processo p[], int numProcess,int quantum){
    struct processo processoDaVez;
    struct processo processosOrdenados[5];
    struct fila infoFila;
    int contadorFinalizados = 0, instanteAtual = 0, tempoDesdeUltimoQuantum = 0;

    //Ordenando processos por tempo de chegada
    for(int i = 0; i < numProcess; i++){
        processosOrdenados[i] = p[i];
    }
    for(int i = 0; i < numProcess - 1; i++){
        for(int j = 0; j<numProcess - 1; j++){
            if(processosOrdenados[j].tempoChegada > processosOrdenados[j+1].tempoChegada){
                processoDaVez = processosOrdenados[j];
                processosOrdenados[j] = processosOrdenados[j+1];
                processosOrdenados[j+1] = processoDaVez;
            }
        }
    }
    
    while(contadorFinalizados < numProcess){
        contadorFinalizados = 0;
        // printaFila();
        printf("\n=======\nInstante atual: %d\n", instanteAtual);
        for(int i = 0; i < numProcess; i++){
            if(processosOrdenados[i].tempoChegada <= instanteAtual && processosOrdenados[i].tempoRestante > 0){
                printf("PID %d entrou no if | numElementos: %d | front: %d | rear: %d\n", processosOrdenados[i].PID, numElementos, front, rear);
                int processoNaFila = 0;

                if(numElementos != 0){
                    for(int j = front; j < numElementos+front; j++){
                        if(processosOrdenados[i].PID == queue[j]){
                            printf("%d já está na fila\n", processosOrdenados[i].PID);
                            processoNaFila = 1;
                        }
                    }
                }
                
                if(processoNaFila == 0){
                    enqueue(processosOrdenados[i].PID);
                }
            }
        }

        printaFila();

        if(numElementos > 0){
            int PIDAtual = queue[front];
            if(processosOrdenados[PIDAtual].tempoRestante > 0 && instanteAtual != 0){
                processosOrdenados[PIDAtual].tempoRestante -= 1;
                printf("processo PID %d tempo -= 1, agora com tempoRestante: %d\n", PIDAtual, processosOrdenados[PIDAtual].tempoRestante);
            }
            // se já passou um quantum, tirar da fila
            if(tempoDesdeUltimoQuantum == 5 && instanteAtual != 0){ // instante 19+5
                printf("%d retirado da fila porque passou um quantum\n", PIDAtual);
                dequeue();
                tempoDesdeUltimoQuantum = 0;
            } else if(processosOrdenados[PIDAtual].tempoRestante <= 0){ // instante 19
                printf("%d retirado da fila porque ele finalizou\n", PIDAtual);
                dequeue();
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
    
    /*
    while(contadorFinalizados < numProcess){
        for(int i = 0; i < 5; i++){
            if(processosOrdenados[i].tempoRestante > 0 && processosOrdenados[i].tempoChegada <= instanteAtual){
                if(processosOrdenados[i].tempoRestante > quantum){
                    //acaba Quantum
                    instanteAtual += quantum;
                    processosOrdenados[i].tempoRestante -= quantum;
                } else {
                    //acaba Quantum
                    instanteAtual += processosOrdenados[i].tempoRestante;
                    processosOrdenados[i].tempoRestante = 0;
                    contadorFinalizados +=1;
                }
            printf("Instante:%d \tProcesso:%d \tTempo restante:%d\n", instanteAtual, i+1, processosOrdenados[i].tempoRestante);
            }
        }
    }
    */

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

    //for(int i = 0; i < 5; i++){
    //    printf("Processo número %d\n", i);
    //    printf("%d \t %d \t %d\n\n", processos[i].tempoChegada, processos[i].tempoServico, processos[i].tipo);
    //}
    
    calculaTurnaround(processos, 5,5);
    return 0;
}