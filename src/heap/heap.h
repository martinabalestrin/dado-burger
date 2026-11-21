#ifndef HEAP_H
#define HEAP_H
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int *dados;      // vetor que armazena os turno de vencimento
    int tamanho;     // quantidade atual de itens no heap
    int capacidade;  // capacidade alocada
    int validade;    // turnos de duração do ingrediente depois do preparo
} MinHeap;

int inicializarHeap(MinHeap *h, int validade);
void destruirHeap(MinHeap *h);
int estaVazioHeap(const MinHeap *h);
int inserirHeap(MinHeap *h, int turnoAtual);           // preparar ingrediente
int proximoVencimentoHeap(const MinHeap *h, int *venc); // consultar topo
int removerHeap(MinHeap *h, int *vencRemovido);         // consumir topo
int descartarVencidosHeap(MinHeap *h, int turnoAtual);  // limpar vencidos no início do turno
void imprimirHeap(const MinHeap *h, const char *nome);  // pra depurar a estrutura

#endif