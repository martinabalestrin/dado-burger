#ifndef PILHA_H
#define PILHA_H
#include <stdio.h>
#include <stdlib.h>
#include "../listaEnc/listaEnc.h"

// definições dos pães`
// obs: defini como 'i' o inferior por uma questão de funcionamento das funções (achei melhor assim)
// o pão superior tá definido como no enunciado do trabalho
#define PAO_INFERIOR 'i' // pão inferior: base já no prato, fora da receita
#define PAO_SUPERIOR 'z' // pão superior: encerra a montagem e bloqueia novas adições

typedef struct str_NodoPilha {
    char ingrediente;
    struct str_NodoPilha *prox;
} NodoPilha;

typedef struct {
    NodoPilha *topo;
} PilhaEnc;

// funções básicas do TAD Pilha
void inicializaPilha(PilhaEnc *p);
int estaVaziaPilha(const PilhaEnc *p);
int pushPilha(PilhaEnc *p, char ingrediente);
int popPilha(PilhaEnc *p, char *ingredienteRemovido);
int topoPilha(const PilhaEnc *p, char *ingrediente);
int tamanhoPilha(const PilhaEnc *p);
void destruirPilha(PilhaEnc *p);

// funções da montagem do hambúrguer
int iniciarMontagemHamburguer(PilhaEnc *p);
int adicionarIngredienteHamburguer(PilhaEnc *p, char ingrediente);
int removerIngredienteHamburguer(PilhaEnc *p, char *ingredienteRemovido);
int verificarPedido(const PilhaEnc *p, const ListaEnc *receita);
void imprimirPilhaHamburguer(const PilhaEnc *p);

#endif