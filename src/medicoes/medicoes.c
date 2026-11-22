#include "medicoes.h"

int inicializarMedicoes(Medicoes *m) {
    m->fmedicoes = fopen("medicoes.log", "w");
    if (m->fmedicoes == NULL) return 0;

    fprintf(m->fmedicoes, "=== RELATORIO DE DESEMPENHO ===\n\n");

    return 1;
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

void inserirAvlCarregamento(Medicoes *m, long nos, int altura, long rotacoes, double tempo) {
    FILE *arquivo = m->fmedicoes;

    fprintf(arquivo, "-- AVL: carregamento --\n");
    fprintf(arquivo, "%-25s %ld\n", "Nos inseridos:", nos);
    fprintf(arquivo, "%-25s %d\n", "Altura resultante:", altura);
    fprintf(arquivo, "%-25s %ld\n", "Rotacoes (insercao):", rotacoes);
    fprintf(arquivo, "%-25s %.6fs\n\n", "Tempo de carregamento:", tempo);
    
    fflush(arquivo);
}

void inserirHistoricoCarregamento(Medicoes *m, int entradas) {
    FILE *arquivo = m->fmedicoes;

    fprintf(arquivo, "-- Historico: carregamento --\n");
    fprintf(arquivo, "%-25s %d\n\n", "Entradas carregadas:", entradas);
    
    fflush(arquivo);
}