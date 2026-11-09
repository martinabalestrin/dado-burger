#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "listaEnc.h"

void inicializarListaEnc(ListaEnc *lista) {
    lista->ini = NULL;
}

int tamanhoListaEnc(const ListaEnc *lista) {
    NodoListaEnc *aux = lista->ini;
    int contador = 0;

    while (aux != NULL) {
        contador++;
        aux = aux->prox;
    }

    return contador;
}

void imprimirListaEnc(const ListaEnc *lista) {
    NodoListaEnc *aux;
    aux = lista->ini;

    while (aux != NULL) {
        printf("Ingrediente: %c\n", aux->ingrediente);
        aux = aux->prox;
    }
}

int buscarListaEnc(const ListaEnc *lista, char ingrediente) {
    NodoListaEnc *aux;
    aux = lista->ini;

    while (aux != NULL) {
        if (aux->ingrediente == ingrediente) return 1;
        aux = aux->prox;
    }

    return 0;
}

int inserirInicioListaEnc(ListaEnc *lista, char ingrediente) {
    NodoListaEnc *novo = (NodoListaEnc*) malloc(sizeof(NodoListaEnc));
    if (novo == NULL) return 0;

    novo->ingrediente = ingrediente;
    novo->prox = lista->ini;
    lista->ini = novo;

    return 1;
}

int inserirFimListaEnc(ListaEnc *lista, char ingrediente) {
    NodoListaEnc *novo = (NodoListaEnc*) malloc(sizeof(NodoListaEnc));;
    if (novo == NULL) return 0;
    novo->ingrediente = ingrediente;
    novo->prox = NULL;

    if (lista->ini == NULL) lista->ini = novo;
    else {
        NodoListaEnc *aux = lista->ini;
        while (aux->prox != NULL) aux = aux->prox;
        aux->prox = novo;
    }

    return 1;
}

int removerIngrediente(ListaEnc *lista, char ingrediente) {
    NodoListaEnc *ant = NULL;
    NodoListaEnc *aux = lista->ini;

    while (aux != NULL && aux->ingrediente != ingrediente) {
        ant = aux;
        aux = aux->prox;
    }

    if (aux == NULL) return 0;
    if (ant == NULL) lista->ini = aux->prox;
    else ant->prox = aux->prox;

    free(aux);
    return 1;
}

void destruirListaEnc(ListaEnc *lista) {
    NodoListaEnc *ant;
    NodoListaEnc *aux;
    aux = lista->ini;

    while (aux != NULL) {
        ant = aux;
        aux = aux->prox;
        free(ant);
    }

    lista->ini = NULL;
}

ListaEnc* criarPedido(Receita pedido) {
    int tam = (int)strlen(pedido.ordem) - 1; 

    ListaEnc *lista = (ListaEnc*) malloc(sizeof(*lista));
    if (lista == NULL) return NULL;
    inicializarListaEnc(lista);
    
    for (int i = tam; i >= 0; i--) {
        inserirInicioListaEnc(lista, pedido.ordem[i]);
    }

    return lista;
}