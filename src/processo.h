#include "io.h"

enum estadoProcesso {
    novo, pronto, execucao, bloqueado, finalizado
};

struct processo {
    int tempoChegada;
    int tempoServico;
    int tempoRestante;
    io *listaIO;
    int quantidadeIO;
    // PCB
    int PID;        // Process  ID
    int PPID;       // Parent Process ID
    int prioridade;
    enum estadoProcesso status;
};