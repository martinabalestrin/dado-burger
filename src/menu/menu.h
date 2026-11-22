#ifndef MENU_H
#define MENU_H

#include <stdio.h>

#include "../receita.h"
#include "../medicoes/medicoes.h"
#include "../nodoAvl/nodoAvl.h"
#include "../listaContDin/listaContDin.h"
#include "../listaEnc/listaEnc.h"
#include "../cliente/cliente.h"
#include "../fila/fila.h"
#include "../tabelaHash/tabelaHash.h"
#include "../partida/partida.h"
#include "../pilha/pilha.h"
#include "../heap/heap.h"
#include "../ingrediente/ingrediente.h"

typedef enum {
    DIFICULDADE_FACIL = 1,
    DIFICULDADE_MEDIO = 2,
    DIFICULDADE_DIFICIL = 3
} Dificuldade;

typedef struct {
    Medicoes *medicoes;
    NodoAvl *pedidos;
    ListaContDin *historico;
    TabelaHash *pedidosCache;
    char *arquivoHistorico;
    long qntPedidos;
} ContextoJogo;

void executarMenu(ContextoJogo *contextoJogo);
void limparTela();
void novoJogo(ContextoJogo *contextoJogo);
void comoJogar();
int atenderClientes(ContextoJogo *contextoJogo, Fila *filaClientes, int *pontuacao, Dificuldade dificuldade);
void visualizarEstoque(const MinHeap *carne, const MinHeap *ovo, const MinHeap *bacon, const int estoquePreparados[4]);
char selecionarIngrediente(int apenasPreparaveis);

#endif