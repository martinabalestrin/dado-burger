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
#include "../ordenacao/ordenacaoHistorico.h"

typedef enum {
    DIFICULDADE_FACIL = 1,
    DIFICULDADE_MEDIO = 2,
    DIFICULDADE_DIFICIL = 3
} Dificuldade;

// Representa um ingrediente perecível atualmente em cozimento
typedef struct {
    char simbolo;
    int turnoPronto; // Turno em que finalizará o cozimento e irá para o estoque (MinHeap)
} ItemCozinhando;

// Lista dinâmica para gerenciar múltiplos ingredientes cozinhando simultaneamente
typedef struct {
    ItemCozinhando *itens;
    int quantidade;
    int capacidade;
} FilaCozimento;

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
void visualizarEstoque(const MinHeap *carne, const MinHeap *ovo, const MinHeap *bacon, const int estoquePreparados[4], const FilaCozimento *cozimento, int turnoAtual);
char selecionarIngrediente(int apenasPreparaveis);
void abrirHistorico(ListaContDin *historico);

#endif