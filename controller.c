
#include <stdlib.h>

int calculaTurnaround(struct processo p[]){
    struct processo processoDaVez;
    struct processo processosOrdenados[5];
    int contadorFinalizados = 0, instanteAtual = 0, tempoDesdeUltimoQuantum = 0;
    queue_type queueHigh, queueLow;
    startQueue(&queueHigh);
    startQueue(&queueLow);

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
        contadorFinalizados = 0;
        printf("\n=======\nInstante atual: %d\n", instanteAtual);

        // Coloca o processo na fila se ele ainda não está nela, se ele se encontra no range do instante e ainda tem tempo restante
        for(int i = 0; i < MAX_PROCESSOS; i++){
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

        printQueue(&queueHigh);
        
        if(queueHigh.numElementos > 0){
            int PIDAtual = queueHigh.queue[queueHigh.front];

            // Retira 1 do tempo restante se não for o primeiro instante
            if(processosOrdenados[PIDAtual].tempoRestante > 0 && instanteAtual != 0){
                processosOrdenados[PIDAtual].tempoRestante -= 1;
                printf("processo PID %d tempo -= 1, agora com tempoRestante: %d\n", PIDAtual, processosOrdenados[PIDAtual].tempoRestante);
            }

            // Se já passou um quantum, tira da fila e reinicia o quantum
            // TODO: colocar no mesmo if, com um OR (||)
            if(tempoDesdeUltimoQuantum == QUANTUM && instanteAtual != 0){
                printf("%d retirado da fila porque passou um quantum\n", PIDAtual);
                pop(&queueHigh);
                tempoDesdeUltimoQuantum = 0;
            } else if(processosOrdenados[PIDAtual].tempoRestante <= 0){
                printf("%d retirado da fila porque ele finalizou\n", PIDAtual);
                pop(&queueHigh);
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

    for(int i = 0; i < 5; i++){
        printf("Processo chega:%d\n",processosOrdenados[i].tempoChegada);
    }

    return 0;
}
