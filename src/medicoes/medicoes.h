#include <stdio.h>

typedef struct {
    FILE *fmedicoes;
} Medicoes;

int inicializarMedicoes(Medicoes *m);
int inserirValor(Medicoes *m, const char *nome, double valor);
int fecharMedicoes(Medicoes *m);