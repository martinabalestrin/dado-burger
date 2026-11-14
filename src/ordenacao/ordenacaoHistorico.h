#ifndef ORDENACAO_HISTORICO_H
#define ORDENACAO_HISTORICO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../listaContDin/listaContDin.h"
#include "../partida.h"
#include "../timer/timer.h"

// definição dos 2 modos de ordenação: ordem alfabética e pontuação decrescente
typedef enum {
    ORDEM_ALFABETICA,
    ORDEM_PONTUACAO_DECRESCENTE
} CriterioOrdenacao;

Partida *criarCopiaPartidas(const ListaContDin *lista);     // cria cópia independente do vetor de partidas da lista original
double ordenarPartidas(Partida *copia, int n, CriterioOrdenacao criterio);      // função principal que executa o merge sort e cronometra apenas a ordenação

#endif