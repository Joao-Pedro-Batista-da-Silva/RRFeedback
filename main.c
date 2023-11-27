#include <stdio.h>
#include <stdlib.h>
#include "queue.c"
#include "processo.h"
#include "controller.c"

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