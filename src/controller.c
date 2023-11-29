#include <stdlib.h>
#include "controller.h"

int simula(struct processo p[]){
    struct processo processoDaVez;
    struct processo processosOrdenados[MAX_PROCESSOS];
    int contadorFinalizados = 0, instanteAtual = 0, tempoDesdeUltimoQuantum = 0, processoVoltandoDeIO = 0;
    char charProcesso[MAX_PROCESSOS];
    queue_type queueHigh, queueLow, queueIO;
    startQueue(&queueHigh);
    startQueue(&queueLow);
    startQueue(&queueIO);

    // Inicializando o processosOrdenados
    for(int i = 0; i < MAX_PROCESSOS; i++){
        processosOrdenados[i] = p[i];
    }

    // Ordenando processos por tempo de chegada
    for(int i = 0; i < MAX_PROCESSOS - 1; i++){
        for(int j = 0; j < MAX_PROCESSOS - 1; j++){
            if(processosOrdenados[j].tempoChegada > processosOrdenados[j+1].tempoChegada){
                processoDaVez = processosOrdenados[j];
                processosOrdenados[j] = processosOrdenados[j+1];
                processosOrdenados[j+1] = processoDaVez;
            }
        }
    }

    printf("Instante \t | Processo\n");
    printf("-----------------+");
    for(int i = 0; i < MAX_PROCESSOS; i++)
        printf("--------");
    printf("\n");

    // Simulando a cada instante
    while(contadorFinalizados < MAX_PROCESSOS){
        processoVoltandoDeIO = 0;
        contadorFinalizados = 0;

        for(int i = 0; i < MAX_PROCESSOS; i++)
            charProcesso[i] = ' ';

        printf("%d\t\t |\t", instanteAtual);
        if(instanteAtual == 0){
            for(int i = 0; i < MAX_PROCESSOS; i++){
                printf("%d\t", i+1);
            }
        }

        for(int i = 0; i < MAX_PROCESSOS; i++){
            for(int j = 0; j < processosOrdenados[i].quantidadeIO; j++){
                io IOAtual = processosOrdenados[i].listaIO[j];
                if(IOAtual.tempoRelativo != 0) continue;
                processoVoltandoDeIO = 1;
                int tempoDeIO, tempoAntes = instanteAtual;
                switch (IOAtual.tipo){
                    case disco:
                        tempoDeIO = TEMPO_DISCO;
                        if(queueHigh.queue[queueHigh.front] == processosOrdenados[i].PID){
                            pop(&queueHigh);
                            push(processosOrdenados[i].PID, &queueLow);
                        }
                        tempoDesdeUltimoQuantum=0;
                        break;
                        
                    case fita:
                        tempoDeIO = TEMPO_FITA;
                        if(queueLow.queue[queueLow.front] == processosOrdenados[i].PID){
                            pop(&queueLow);
                            push(processosOrdenados[i].PID, &queueHigh);
                        }
                        tempoDesdeUltimoQuantum=0;
                        break;

                    case impressora:
                        tempoDeIO = TEMPO_IMPRESSORA;
                        if(queueLow.queue[queueLow.front] == processosOrdenados[i].PID){
                            pop(&queueLow);
                            push(processosOrdenados[i].PID, &queueHigh);
                        }
                        tempoDesdeUltimoQuantum=0;
                        break;               
                    default:
                        break;
                    }

                processosOrdenados[i].listaIO[j].tempoRelativo = -1;
            }

            // Coloca o processo na fila se ele ainda não está nela, se ele se encontra no range do instante e ainda tem tempo restante
            if(processosOrdenados[i].tempoChegada <= instanteAtual && processosOrdenados[i].tempoRestante > 0){
                int processoNaFila = 0;

                if(queueHigh.numElementos != 0){
                    for(int j = queueHigh.front; j < queueHigh.numElementos + queueHigh.front; j++){
                        if(processosOrdenados[i].PID == queueHigh.queue[j]){
                            processoNaFila = 1;
                        }
                    }
                }

                if(queueLow.numElementos != 0) {
                    for(int j = queueLow.front; j < queueLow.numElementos + queueLow.front; j++){
                        if(processosOrdenados[i].PID == queueLow.queue[j]){
                            processoNaFila = 1;
                        }
                    }
                }
                
                if(processoNaFila == 0){
                    push(processosOrdenados[i].PID, &queueHigh);
                }
            }
        }
        
        // Faz lógica de retirar 1 do tempo do processo, retirar ele da fila e terminar quantum
        if(queueHigh.numElementos > 0){
            int PIDAtual = queueHigh.queue[queueHigh.front];

            // Retira 1 do tempo restante se não for o primeiro instante
            if(processosOrdenados[PIDAtual].tempoRestante > 0 && instanteAtual != 0){
                processosOrdenados[PIDAtual].tempoRestante -= 1;
                charProcesso[PIDAtual] = CHAR_FILA_ALTA;

                // Tira 1 tempo relativo de cada IO do processo
                for(int i = 0; i < processosOrdenados[PIDAtual].quantidadeIO; i++){
                    processosOrdenados[PIDAtual].listaIO[i].tempoRelativo -=1;
                }
            }

            // Se já passou um quantum, tira da fila e reinicia o quantum
            if((tempoDesdeUltimoQuantum == QUANTUM && instanteAtual != 0) || processosOrdenados[PIDAtual].tempoRestante <= 0){
                if(processosOrdenados[PIDAtual].tempoRestante > 0){
                    if(processoVoltandoDeIO == 0){
                        push(processosOrdenados[PIDAtual].PID, &queueLow);
                    }
                }
                pop(&queueHigh);
                tempoDesdeUltimoQuantum = 0;
            }
        } else if(queueLow.numElementos > 0){
            int PIDAtual = queueLow.queue[queueLow.front];

            if(processosOrdenados[PIDAtual].tempoRestante > 0 && instanteAtual != 0){
                processosOrdenados[PIDAtual].tempoRestante -= 1;
                charProcesso[PIDAtual] = CHAR_FILA_BAIXA;
                
                // Tira 1 tempo relativo de cada IO do processo
                for(int i = 0; i < processosOrdenados[PIDAtual].quantidadeIO; i++){
                    processosOrdenados[PIDAtual].listaIO[i].tempoRelativo -=1;
                }
            }

            if(tempoDesdeUltimoQuantum == QUANTUM && instanteAtual != 0 || processosOrdenados[PIDAtual].tempoRestante <= 0){
                pop(&queueLow);
                tempoDesdeUltimoQuantum = 0;
            }
        }

        // Verifica se todos os processos finalizaram
        for(int i = 0; i < MAX_PROCESSOS; i++){
            if(processosOrdenados[i].tempoRestante <= 0){
                contadorFinalizados += 1;
            }
        }

        for(int i = 0; i < MAX_PROCESSOS; i++)
            printf("%c\t", charProcesso[i]);
        
        printf("\n");
        instanteAtual += 1;
        if(instanteAtual == 1) continue;
        tempoDesdeUltimoQuantum += 1;
    }

    return 0;
}
