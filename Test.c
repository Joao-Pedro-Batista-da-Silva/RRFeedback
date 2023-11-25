#include <stdio.h>
#include <stdlib.h>
#define MAX_SIZE 100  

int queue[MAX_SIZE];  
int front = -1;  
int rear = -1;    
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
}  

int dequeue() {  
    if (front == -1 || front > rear) {  
        printf("Queue is empty");  
        return -1;  
    }  
    int element = queue[front];  
    front++;  
    return element;  
}  
//Limite de processos 5

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
    int contadorFinalizados = 0, instanteAtual = 0;

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

    //Montando o gráfico RR 
    while(contadorFinalizados < numProcess){
        for(int i = 0; i < 5; i++){
            if(processosOrdenados[i].tempoRestante > 0 && processosOrdenados[i].tempoChegada <= instanteAtual){
                enqueue(processosOrdenados[i].PID);
                if(processosOrdenados[i].tempoRestante > quantum){
                    instanteAtual += quantum;
                    processosOrdenados[i].tempoRestante -= quantum;
                } else {
                    instanteAtual += processosOrdenados[i].tempoRestante;
                    processosOrdenados[i].tempoRestante = 0;
                    contadorFinalizados +=1;
                }
            printf("Instante:%d \tProcesso:%d \tTempo restante:%d\n", instanteAtual, i+1, processosOrdenados[i].tempoRestante);
            }
        }
    }

    for(int i = 0; i < 5; i++){
        printf("Processo chega:%d\n",processosOrdenados[i].tempoChegada);
    }

    return 0;
}

int main(){
    struct processo processos[5];
    struct processo pprov;
    int numProcessos = 5;
    for(int i = 0; i < numProcessos; i++){
        printf("Processo número %d\n", i);
        
        printf("Entre com o tempo de chegada: ");
        scanf("%d", &processos[i].tempoChegada);

        printf("Entre com o tempo de serviço: ");
        scanf("%d", &processos[i].tempoServico);

        printf("Entre com o tipo de IO (0 = Disco, 1 = Fita, 2 = Impressora): ");
        scanf("%d", &processos[i].tipo);

        processos[i].tempoRestante = processos[i].tempoServico;
        processos[i].PID = i;
        processos[i].prioridade = 0;
        processos[i].status = novo;
    }

    /*for(int i = 0; i < 5; i++){
        printf("Processo número %d\n", i);
        printf("%d \t %d \t %d\n\n", processos[i].tempoChegada, processos[i].tempoServico, processos[i].tipo);
    }*/

    calculaTurnaround(processos, 5,5);
    return 0;
}