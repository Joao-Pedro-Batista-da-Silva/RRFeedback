#include <stdio.h>
#include <stdlib.h>

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
    enum tipoIO tipo;
    // PCB
    int PID;        // Process  ID
    int PPID;       // Parent Process ID
    int prioridade;
    enum estadoProcesso status;
};

int main(){
    struct processo processos[5];
    int numProcessos = 5;
    for(int i = 0; i < numProcessos; i++){
        printf("Processo número %d\n", i);
        
        printf("Entre com o tempo de chegada: ");
        scanf("%d", &processos[i].tempoChegada);

        printf("Entre com o tempo de serviço: ");
        scanf("%d", &processos[i].tempoServico);

        processos[i].PID = i;

        printf("Entre com o tipo de IO (0 = Disco, 1 = Fita, 2 = Impressora): ");
        scanf("%d", processos[i].tipo);

        processos[i].prioridade = 0;
        processos[i].status = novo;
    }

    for(int i = 0; i < 5; i++){
        printf("Processo número %d\n", i);
        printf("%d \t %d \t %d\n\n", processos[i].tempoChegada, processos[i].tempoServico, processos[i].tipo);
    }

    
    // for(int i = 0; i<=4;i++){
    //     printf("Tempo de Chegada: %d\t, Tempo de Servico: %d\n",processos[i].tempoChegada, processos[i].tempoServico);
    // }

    return 0;
}