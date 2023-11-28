enum tipoIO {
    disco, fita, impressora
};

typedef struct io_t {
    int tempoRelativo;
    enum tipoIO tipo;
} io;