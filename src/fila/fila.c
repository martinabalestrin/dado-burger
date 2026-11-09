#include <stdio.h>
#include <stdlib.h>
#include "fila.h"
#include "../cliente/cliente.h"

void inicializa(FilaEnc *f){
    f->frente = NULL;
    f->final = NULL;
}

int estaVazia(const FilaEnc *f){
    return f->frente == NULL;
}

int enqueue(FilaEnc *f, Cliente cliente){
    NodoFila *novo = (NodoFila*) malloc(sizeof(NodoFila));
    if (novo == NULL){
        return 0;
    }

    novo->dados = cliente;
    novo->prox = NULL;

    if (f->final != NULL){
        f->final->prox = novo;
    }
    else{
        f->frente = novo;
    }

    f->final = novo;
    return 1;
}

int dequeue(FilaEnc *f, Cliente *clienteRemovido){
    NodoFila *removido;

    if (estaVazia(f)){
        return 0;
    }

    removido = f->frente;
    *clienteRemovido = removido->dados;
    f->frente = removido->prox;

    if (f->frente == NULL){
        f->final = NULL;
    }

    free(removido);
    return 1;
}

int frente(const FilaEnc *f, Cliente *cliente){
    if (estaVazia(f)){
        return 0;
    }

    *cliente = f->frente->dados;
    return 1;
}

int tamanho(const FilaEnc *f){
    NodoFila *aux = f->frente;
    int contador = 0;

    while (aux != NULL) {
        contador++;
        aux = aux->prox;
    }

    return contador;
}