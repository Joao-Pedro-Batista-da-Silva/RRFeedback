#include <stdio.h>
#include <stdlib.h>
#include "constants.h"
#include "processo.h"
#include "queue.c"
#include "controller.c"

int main(){
    struct processo processos[MAX_PROCESSOS];
    int qntdIO;

    for(int i = 0; i < MAX_PROCESSOS; i++){
        printf("Processo número %d\n", i);
        
        printf("Entre com o tempo de chegada: ");
        scanf("%d", &processos[i].tempoChegada);
        puts("");

        printf("Entre com o tempo de serviço: ");
        scanf("%d", &processos[i].tempoServico);
        puts("");

        printf("Entre com quantos IO terá esse processo: ");
        scanf("%d", &qntdIO);
        processos[i].quantidadeIO = qntdIO;
        processos[i].listaIO = (io*) malloc(sizeof(io) * qntdIO);
        puts("");

        for(int j = 0; j < qntdIO; j++){
            printf("IO número %d\n", j);

            printf("Entre com o tipo do IO: ");
            printf("0 = Disco \t 1 = Fita \t 2 = Impressora\n");
            scanf("%d", &processos[i].listaIO[j].tipo);

            printf("Entre com o tempo relativo ao começo do processo: ");
            scanf("%d", &processos[i].listaIO[j].tempoRelativo);
            
            puts("\n");
        }

        processos[i].tempoRestante = processos[i].tempoServico;
        processos[i].PID = i;
        processos[i].prioridade = 0;
        processos[i].status = novo;
    }
    
    simula(processos);
    return 0;
}