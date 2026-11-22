#ifndef MEDICOES_H
#define MEDICOES_H

#include <stdio.h>

#include "../nodoAvl/nodoAvl.h"

typedef struct {
    FILE *fmedicoes;
} Medicoes;

int inicializarMedicoes(Medicoes *m);
int inserirValor(Medicoes *m, const char *nome, double valor);
int fecharMedicoes(Medicoes *m);
void inserirAvlCarregamento(Medicoes *m, long nos, int altura, long rotacoes, double tempo);
void inserirHistoricoCarregamento(Medicoes *m, int entradas);

#endif