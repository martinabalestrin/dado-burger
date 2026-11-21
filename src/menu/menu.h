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
#include "../ordenacao/ordenacaoHistorico.h"

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
void abrirHistorico(ListaContDin *historico);

#endif