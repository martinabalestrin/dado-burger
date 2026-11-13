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

void inicializaFila(FilaEnc *f);
int estaVaziaFila(const FilaEnc *f);
int enqueueFila(FilaEnc *f, Cliente cliente);
int dequeueFila(FilaEnc *f, Cliente *clienteRemovido);
int frenteFila(const FilaEnc *f, Cliente *cliente);
int tamanhoFila(const FilaEnc *f);

#endif