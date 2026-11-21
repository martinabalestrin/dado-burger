#ifndef PILHA_H
#define PILHA_H

#include <stdio.h>
#include <stdlib.h>

#include "../listaEnc/listaEnc.h"
#include "../ingrediente/ingrediente.h"

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
int removerIngredienteHamburguer(PilhaEnc *p);
int verificarPedido(const PilhaEnc *p, const ListaEnc *receita);
void imprimirPilhaHamburguer(const PilhaEnc *p);

#endif