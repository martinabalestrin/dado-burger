#ifndef MEDICOES_H
#define MEDICOES_H

#include <stdio.h>

struct TabelaHash;

typedef struct {
    FILE *fmedicoes;
} Medicoes;

typedef struct {
    int buscas;
    long comparacoes;
    double tempoAcumulado;
} MetricasAvl;

typedef struct {
    int tamanho;
    double tempo;
} MetricasOrdenacao;

int inicializarMedicoes(Medicoes *m);
int inserirValor(Medicoes *m, const char *nome, double valor);
int fecharMedicoes(Medicoes *m);
void inserirAvlCarregamento(Medicoes *m, long nos, int altura, long rotacoes, double tempo);
void inserirHistoricoCarregamento(Medicoes *m, int entradas);
void inserirAvlDurantePartida(Medicoes *m, MetricasAvl *avl);
void inserirHashDurantePartida(Medicoes *m, struct TabelaHash *hash);
void inserirOrdenacaoHistorico(Medicoes *m, MetricasOrdenacao *ordenacao);

#endif