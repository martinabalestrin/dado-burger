#ifndef FILA_H
#define FILA_H
#include <stdio.h>
#include "../cliente/cliente.h"

typedef struct str_Nodo NodoFila;

struct str_Nodo{
    NodoFila *prox;
    Cliente dados;
};

typedef struct{
    NodoFila *frente;
    NodoFila *final;
} FilaEnc;

void inicializa(FilaEnc *f);
int estaVazia(const FilaEnc *f);
int enqueue(FilaEnc *f, Cliente cliente);
int dequeue(FilaEnc *f, Cliente *clienteRemovido);
int frente(const FilaEnc *f, Cliente *cliente);
int tamanho(const FilaEnc *f);

#endif