
#include <stdlib.h>
#include "controller.h"

int calculaTurnaround(struct processo p[]){
    struct processo processoDaVez;
    struct processo processosOrdenados[MAX_PROCESSOS];
    int contadorFinalizados = 0, instanteAtual = 0, tempoDesdeUltimoQuantum = 0, processoVoltandoDeIO = 0;
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
    
    // Simulando a cada instante
    while(contadorFinalizados < MAX_PROCESSOS){
        processoVoltandoDeIO = 0;
        contadorFinalizados = 0;
        printf("\n=======\nInstante atual: %d\n", instanteAtual);

        for(int i = 0; i < MAX_PROCESSOS; i++){
            // if(processosOrdenados[i].quantidadeIO > 0){
            for(int j = 0; j < processosOrdenados[i].quantidadeIO; j++){
                io IOAtual = processosOrdenados[i].listaIO[j];
                if(IOAtual.tempoRelativo != 0) continue;
                printf("ENTROU NO IFODASE %d\n", IOAtual.tempoRelativo);
                processoVoltandoDeIO = 1;
                switch (IOAtual.tipo){
                    case disco:
                        instanteAtual += TEMPO_DISCO;
                        // mover processo para baixa prioridade
                        if(queueHigh.queue[queueHigh.front] == processosOrdenados[i].PID){
                            pop(&queueHigh);
                            push(processosOrdenados[i].PID, &queueLow);
                        }
                        break;
                        
                    case fita:
                        instanteAtual += TEMPO_FITA;
                        if(queueLow.queue[queueLow.front] == processosOrdenados[i].PID){
                            pop(&queueLow);
                            push(processosOrdenados[i].PID, &queueHigh);
                        }
                        break;

                    case impressora:
                        instanteAtual += TEMPO_IMPRESSORA;
                        if(queueLow.queue[queueLow.front] == processosOrdenados[i].PID){
                            pop(&queueLow);
                            push(processosOrdenados[i].PID, &queueHigh);
                        }
                        break;               
                    default:
                        break;
                    }
                    processosOrdenados[i].listaIO[j].tempoRelativo = -1;
                // }
            }

            // Coloca o processo na fila se ele ainda não está nela, se ele se encontra no range do instante e ainda tem tempo restante
            if(processosOrdenados[i].tempoChegada <= instanteAtual && processosOrdenados[i].tempoRestante > 0){
                printf("PID %d entrou no if | numElementos: %d | front: %d | rear: %d\n", processosOrdenados[i].PID, queueHigh.numElementos, queueHigh.front, queueHigh.rear);
                int processoNaFila = 0;

                /*
                possiveis processos:
                - processo na alta e não na baixa: ele não vai ser adicionado na fila, e vai ser exexcutado se for o primeiro
                - processo na baixa e não na alta: não vai ser adicionado na fila, e vai ser executado se for o primeiro da fila e estiver sem fila de alta prioridade
                - processo nem na baixa e nem na alta: vai ser adicionado na fila de alta prioridade
                */

                if(queueHigh.numElementos != 0){
                    for(int j = queueHigh.front; j < queueHigh.numElementos + queueHigh.front; j++){
                        if(processosOrdenados[i].PID == queueHigh.queue[j]){
                            printf("%d já está na fila de alta\n", processosOrdenados[i].PID);
                            processoNaFila = 1;
                        }
                    }
                }

                if(queueLow.numElementos != 0) {
                    for(int j = queueLow.front; j < queueLow.numElementos + queueLow.front; j++){
                        if(processosOrdenados[i].PID == queueLow.queue[j]){
                            printf("%d já está na fila de BAIXA\n", processosOrdenados[i].PID);
                            processoNaFila = 1;
                        }
                    }
                }
                
                if(processoNaFila == 0){
                    push(processosOrdenados[i].PID, &queueHigh);
                }
            }
        }

        if(queueHigh.numElementos > 0){
            printf("(alta prioridade) ");
            printQueue(&queueHigh);
        } else if(queueLow.numElementos > 0){
            printf("(BAIXA prioridade) ");
            printQueue(&queueLow);
        }
        
        // Faz lógica de retirar 1 do tempo do processo, retirar ele da fila e terminar quantum
        if(queueHigh.numElementos > 0){
            int PIDAtual = queueHigh.queue[queueHigh.front];

            // Retira 1 do tempo restante se não for o primeiro instante
            if(processosOrdenados[PIDAtual].tempoRestante > 0 && instanteAtual != 0){
                processosOrdenados[PIDAtual].tempoRestante -= 1;

                // Tira 1 tempo relativo de cada IO do processo
                for(int i = 0; i < processosOrdenados[PIDAtual].quantidadeIO; i++){
                    processosOrdenados[PIDAtual].listaIO[i].tempoRelativo -=1;
                }
                printf("processo PID %d tempo -= 1, agora com tempoRestante: %d\n", PIDAtual, processosOrdenados[PIDAtual].tempoRestante);
            }

            // Se já passou um quantum, tira da fila e reinicia o quantum
            if((tempoDesdeUltimoQuantum == QUANTUM && instanteAtual != 0) || processosOrdenados[PIDAtual].tempoRestante <= 0){
                if(processosOrdenados[PIDAtual].tempoRestante <= 0){
                    printf("%d retirado da fila porque ele finalizou\n", PIDAtual);
                } else {
                    printf("%d retirado da fila porque passou um quantum\n", PIDAtual);
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
                
                // Tira 1 tempo relativo de cada IO do processo
                for(int i = 0; i < processosOrdenados[PIDAtual].quantidadeIO; i++){
                    processosOrdenados[PIDAtual].listaIO[i].tempoRelativo -=1;
                }
                printf("BAIXA PRIORIDADE processo PID %d tempo -= 1, agora com tempoRestante: %d\n", PIDAtual, processosOrdenados[PIDAtual].tempoRestante);
            }

            if(tempoDesdeUltimoQuantum == QUANTUM && instanteAtual != 0 || processosOrdenados[PIDAtual].tempoRestante <= 0){
                if(processosOrdenados[PIDAtual].tempoRestante <= 0){
                    printf("%d retirado da fila porque ele finalizou\n", PIDAtual);
                } else {
                    printf("%d retirado da fila porque passou um quantum\n", PIDAtual);
                }
                pop(&queueLow);
                tempoDesdeUltimoQuantum = 0;
            }
        }

        // Verifica se todos os processos finalizaram
        for(int i = 0; i < MAX_PROCESSOS; i++){
            if(processosOrdenados[i].tempoRestante <= 0){
                printf("finalizou\n");
                contadorFinalizados += 1;
            }
        }
        
        instanteAtual += 1;
        tempoDesdeUltimoQuantum += 1;
    }

    for(int i = 0; i < MAX_PROCESSOS; i++){
        printf("Processo chega:%d\n",processosOrdenados[i].tempoChegada);
    }

    return 0;
}
