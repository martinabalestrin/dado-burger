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
} Fila;

void inicializaFila(Fila *f);
int estaVaziaFila(const Fila *f);
int enqueueFila(Fila *f, Cliente cliente);
int dequeueFila(Fila *f, Cliente *clienteRemovido);
int frenteFila(const Fila *f, Cliente *cliente);
int tamanhoFila(const Fila *f);

#endif