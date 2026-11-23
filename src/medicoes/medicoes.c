#include "medicoes.h"
#include "../tabelaHash/tabelaHash.h"

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

void inserirAvlDurantePartida(Medicoes *m, MetricasAvl *avl) {
    FILE *arquivo = m->fmedicoes;

    fprintf(arquivo, "-- AVL: buscas durante a partida --\n");
    fprintf(arquivo, "%-25s %d\n", "Total de buscas:", avl->buscas);
    fprintf(arquivo, "%-25s %ld\n", "Total de comparacoes:", avl->comparacoes);
    double media = avl->buscas > 0 ? (double)avl->comparacoes / avl->buscas : 0.0;
    fprintf(arquivo, "%-25s %.2f\n", "Media de comparacoes por busca:", media);
    fprintf(arquivo, "%-25s %.6fs\n\n", "Tempo total acumulado:", avl->tempoAcumulado);

    fflush(arquivo);
}

void inserirHashDurantePartida(Medicoes *m, struct TabelaHash *hash) {
    FILE *arquivo = m->fmedicoes;

    fprintf(arquivo, "-- Tabela hash --\n");
    fprintf(arquivo, "%-25s %d\n", "Tamanho (m):", hash->m);
    fprintf(arquivo, "%-25s %d\n", "Entradas ocupadas (n):", hash->n);
    fprintf(arquivo, "%-25s %.2f\n", "Fator de carga (lambda):", (double)hash->n / hash->m);
    fprintf(arquivo, "%-25s %d\n", "Total de hits:", hash->hits);
    fprintf(arquivo, "%-25s %d\n", "Total de misses:", hash->misses);
    int acessos = hash->hits + hash->misses;
    double taxaAcerto = acessos > 0 ? 100.0 * hash->hits / acessos : 0.0;
    fprintf(arquivo, "%-25s %.1f%%\n", "Taxa de acerto:", taxaAcerto);
    fprintf(arquivo, "%-25s %d\n\n", "Total de colisoes:", hash->colisao);

    fflush(arquivo);
}

void inserirOrdenacaoHistorico(Medicoes *m, MetricasOrdenacao *ordenacao) {
    FILE *arquivo = m->fmedicoes;

    fprintf(arquivo, "-- Ordenacao do historico (ao acessar Historico) --\n");
    fprintf(arquivo, "%-25s %d entradas\n", "Tamanho:", ordenacao->tamanho);
    fprintf(arquivo, "%-25s %.6f s\n", "Merge Sort:", ordenacao->tempo);
    fprintf(arquivo, "================================\n");

    fflush(arquivo);
}