#ifndef NODOAVL_H
#define NODOAVL_H

#include "../receita.h"
#include "../medicoes/medicoes.h"
#include "../timer/timer.h"

typedef struct str_NodoAvl NodoAvl;

struct str_NodoAvl {
    Receita dado;
    NodoAvl *esq;
    NodoAvl *dir;
    int altura;
};

int avlAltura(const NodoAvl *no);
int avlFb(const NodoAvl *no);
void avlAtualizarAltura(NodoAvl *no);
NodoAvl* avlRotDir(NodoAvl *y, long *rotacoesInsercao);
NodoAvl* avlRotEsq(NodoAvl *x, long *rotacoesInsercao);
NodoAvl* avlReequilibrar(NodoAvl *raiz, long *rotacoesInsercao);
NodoAvl* avlBuscar(NodoAvl *raiz, int cod, MetricasAvl *m);
NodoAvl* avlInserir(NodoAvl *raiz, Receita valor, long *nosInseridos, long *rotacoesInsercao);
NodoAvl* avlMinimo(NodoAvl *raiz);
NodoAvl* avlRemover(NodoAvl *raiz, int cod);
NodoAvl* avlDestruir(NodoAvl *raiz);
void avlDebugImprimir(const NodoAvl *raiz);

NodoAvl* carregarPedidos(NodoAvl *raiz, char nomeArquivo[], long *nosInseridos, long *rotacoesInsercao);
NodoAvl* buscarPedidoAvl(NodoAvl *raiz, int codigo, MetricasAvl *m);

#endif