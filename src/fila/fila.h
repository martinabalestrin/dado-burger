#include <stdio.h>

typedef struct str_Nodo Nodo;

struct str_Nodo{
    Nodo *prox;
    int id;
};

typedef struct{
    Nodo *frente;
    Nodo *final;
} FilaEnc;

void inicializa(FilaEnc *f);
int estaVazia(const FilaEnc *f);
int enqueue(FilaEnc *f, int cliente);
int dequeue(FilaEnc *f, int *idRemovido);
int frente(const FilaEnc *f, int *id);
int tamanho(const FilaEnc *f);