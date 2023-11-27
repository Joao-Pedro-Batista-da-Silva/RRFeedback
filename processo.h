enum tipoIO {
    disco, fita, impressora
};

enum estadoProcesso {
    novo, pronto, execucao, bloqueado, finalizado
};

struct processo {
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