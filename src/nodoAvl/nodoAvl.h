#include <stdio.h>

#include "../receita.h"

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
NodoAvl* avlRotDir(NodoAvl *y);
NodoAvl* avlRotEsq(NodoAvl *x);
NodoAvl* avlReequilibrar(NodoAvl *raiz);
NodoAvl* avlBuscar(NodoAvl *raiz, int cod);
NodoAvl* avlInserir(NodoAvl *raiz, Receita valor);
NodoAvl* avlMinimo(NodoAvl *raiz);
NodoAvl* avlRemover(NodoAvl *raiz, int cod);
NodoAvl* avlDestruir(NodoAvl *raiz);

NodoAvl* carregarPedidos(NodoAvl *raiz, char nomeArquivo[]);
Receita buscarPedido(NodoAvl *raiz, int codigo);