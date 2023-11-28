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
        
        scanf("%d", &processos[i].tempoChegada);
        puts("");
        printf("Tempo de chegada: %d\n", processos[i].tempoChegada);

        scanf("%d", &processos[i].tempoServico);
        printf("Tempo de serviço: %d\n",processos[i].tempoServico);
        puts("");

        scanf("%d", &qntdIO);
        printf("Numero de IO terá esse processo: %d\n", qntdIO);
        processos[i].quantidadeIO = qntdIO;
        processos[i].listaIO = (io*) malloc(sizeof(io) * qntdIO);
        puts("");

        for(int j = 0; j < qntdIO; j++){
            printf("IO número %d\n", j+1);

            scanf("%d", &processos[i].listaIO[j].tipo);
            printf("Tipo do IO: %d\n",processos[i].listaIO[j].tipo);
            printf("0 = Disco \t 1 = Fita \t 2 = Impressora\n");

            scanf("%d", &processos[i].listaIO[j].tempoRelativo);
            printf("Entre com o tempo relativo de IO ao começo do processo: %d",processos[i].listaIO[j].tempoRelativo);
            
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