#include "medicoes.h"

int inicializarMedicoes(Medicoes *m) {
    m->fmedicoes = fopen("medicoes.log", "w");
    if (m->fmedicoes == NULL) return 1;
    return 0;
}

int inserirValor(Medicoes *m, const char *nome, double valor) {
    if (m->fmedicoes == NULL) return 1;
    fprintf(m->fmedicoes, "%s: %f\n", nome, valor);
    return 0;
}

int fecharMedicoes(Medicoes *m) {
    if (m->fmedicoes == NULL) return 1;
    fclose(m->fmedicoes);
    m->fmedicoes = NULL;
    return 0;
}