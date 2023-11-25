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
    int tempoRestante;
    enum tipoIO tipo;
    // PCB
    int PID;        // Process  ID
    int PPID;       // Parent Process ID
    int prioridade;
    enum estadoProcesso status;
};

int calculaTurnaround(struct processo p[], int numProcess,int quantum){
    // é, pode ser
    //if(processo.tempoChegada<quantum)
    struct processo processoDaVez;
    struct processo processosOrdenados[5];
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
    // dá pra gnt fazer antes do if ele printar o instante, e depois printar o instante somado, sim sim, perai já volto rapidinho
    //q1(0-5s):  1, 2, 3
    //tr         7, 10, 9

    //1 volta pro final no instante 5, p4 chega no instante 6, e p5 chega no instante 8
    //q2(5-10s): 2, 3, 1, 4, 5
    //tr        10, 9, 2, 9, 4
    
    //q3(10-15s): 3, 1, 4, 5, 2
    //tr          9, 2, 9, 4, 5
    
    //q4(15-17s): 1, 4, 5, 2, 3
    //tr          2, 9, 4, 5, 4
    
    //q5(17-22s): 4, 5, 2, 3
    //tr          9, 4, 5, 4
    
    //q6(22-26s): 5, 2, 3, 4
    //tr          4, 5, 4, 4
    
    //q7(26-31s): 2, 3, 4
    //tr          5, 4, 4
    
    //q8(31-35s): 3, 4
    //tr          4, 4
    
    //q9(35-39s): 4
    //tr          4

    while(contadorFinalizados < numProcess){
        for(int i = 0; i < 5; i++){
            if(processosOrdenados[i].tempoRestante > 0 && processosOrdenados[i].tempoChegada <= instanteAtual){
                
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

    for(int i = 0; i < 5; i++){
        printf("Processo número %d\n", i);
        printf("%d \t %d \t %d\n\n", processos[i].tempoChegada, processos[i].tempoServico, processos[i].tipo);
    }

    // for(int i = 0; i<=4;i++){
    //     printf("Tempo de Chegada: %d\t, Tempo de Servico: %d\n",processos[i].tempoChegada, processos[i].tempoServico);
    // }

    for(int i = 0; i < numProcessos; i++){
        
    }
    calculaTurnaround(processos, 5,5);
    return 0;
}