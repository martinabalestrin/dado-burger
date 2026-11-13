#include <stdio.h>
#include <stdlib.h>

#include "tabelaHash.h"

int inicializaTabelaHash(TabelaHash *t) {
    t->tabela = calloc(10, sizeof(t->tabela));
    if (t->tabela == NULL) {
        fprintf(stderr, "Erro: memória insuficiente");
        return -1;
    }
    t->colisao = 0;
    t->m = 10;
    t->n = 0;
    return 0;
}

int inserirTabelaHash(TabelaHash *t, Receita dado) {
    int i = dado.cod % t->m;

    NodoHash *novo = malloc(sizeof(NodoHash));
    if (novo == NULL) {
        fprintf(stderr, "Erro: memoria insuficiente para inserir na hash.\n");
        return -1;
    }

    novo->dado = dado;
    novo->prox  = t->tabela[i];

    if (t->tabela[i] != NULL) t->colisao++;
    
    t->tabela[i] = novo;
    t->n++;
    return 0;
}

NodoHash* buscarTabelaHash(TabelaHash *t, int chave) {
    int i = chave % t->m;
    NodoHash *nodo = t->tabela[i];
    while (nodo != NULL) {
        if (nodo->dado.cod == chave) return nodo;
        nodo = nodo->prox;
    }
    return NULL;
}

int removerTabelaHash(TabelaHash *t, int chave) {
    int i = chave % t->m;
    NodoHash *nodo = t->tabela[i];
    NodoHash *ant = NULL;
    while (nodo != NULL) {
        if (nodo->dado.cod == chave) {
            if (ant == NULL)
                t->tabela[i] = nodo->prox;
            else
                ant->prox = nodo->prox;
            free(nodo);
            t->n--;
            return 1;
        }
        ant = nodo;
        nodo = nodo->prox;
    }
    return 0;
}

int buscarPedido(TabelaHash *tabela, NodoAvl *raizAvl, int codPedido, Receita *receita) {
    NodoHash *hash = buscarTabelaHash(tabela, codPedido);
    if (hash != NULL) {
        *receita = hash->dado;
        return 1;
    };
    
    NodoAvl *avl = buscarPedidoAvl(raizAvl, codPedido);
    if (avl == NULL) {
        fprintf(stderr, "Erro: pedido %d nao encontrado.\n", codPedido);
        return 0;
    }
    if (inserirTabelaHash(tabela, avl->dado) != 0) return -1;

    *receita = avl->dado;
    return 1;
}

void destruirTabelaHash(TabelaHash *t) {
    for(int i = 0; i < t->m; i++) {
        NodoHash *nodo = t->tabela[i];
        while (nodo != NULL) {
            NodoHash *prox = nodo->prox;
            free(nodo);
            nodo = prox;
        }
    }

    free(t->tabela);
    *t = (TabelaHash){0};
}