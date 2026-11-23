#ifndef TABELAHASH_H
#define TABELAHASH_H

#include "../receita.h"
#include "../nodoAvl/nodoAvl.h"
#include "../medicoes/medicoes.h"

typedef struct NodoHash {
    Receita dado;
    struct NodoHash *prox;
} NodoHash;

typedef struct TabelaHash {
    NodoHash **tabela;
    int m;
    int n;
    int colisao;
    int hits;
    int misses;
} TabelaHash;

int inicializaTabelaHash(TabelaHash *t);
int inserirTabelaHash(TabelaHash *t, Receita dado);
NodoHash* buscarTabelaHash(TabelaHash *t, int chave);
int removerTabelaHash(TabelaHash *t, int chave);
int buscarPedido(TabelaHash *tabela, NodoAvl *raizAvl, int codPedido, Receita *receita, MetricasAvl *metricasAvl);
void destruirTabelaHash(TabelaHash *t);

#endif