#include <stdio.h>
#include <stdlib.h>
#include "fila.h"

void inicializa(FilaEnc *f){
    f->frente = NULL;
    f->final = NULL;
}

int estaVazia(const FilaEnc *f){
    return f->frente == NULL;
}

int enqueue(FilaEnc *f, int cliente){
    Nodo *novo = (Nodo*) malloc(sizeof(Nodo));
    if (novo == NULL){
        return 0;
    }

    novo->id = cliente;
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

int dequeue(FilaEnc *f, int *idRemovido){
    Nodo *removido;

    if (estaVazia(f)){
        return 0;
    }

    removido = f->frente;
    *idRemovido = removido->id;
    f->frente = removido->prox;

    if (f->frente == NULL){
        f->final = NULL;
    }

    free(removido);
    return 1;
}

int frente(const FilaEnc *f, int *id){
    if (estaVazia(f)){
        return 0;
    }

    *id = f->frente->id;
    return 1;
}

int tamanho(const FilaEnc *f){
    Nodo *aux = f->frente;
    int contador = 0;

    while (aux != NULL) {
        contador++;
        aux = aux->prox;
    }

    return contador;
}