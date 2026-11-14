#include <stdio.h>
#include <stdlib.h>
#include "../src/ordenacao/ordenacaoHistorico.h"

void exibirRanking(const Partida *vetor, int n, const char *titulo, double tempo) {
    printf("\n=== %s (Tempo: %.6f s) ===\n", titulo, tempo);
    for (int i = 0; i < n; i++) {
        printf("%2d. %-20s | %d pts\n", i + 1, vetor[i].nome, vetor[i].pontuacao);
    }
}

int main() {
    ListaContDin historico;
    inicializarLCD(&historico);
    carregarHistorico(&historico, "tests/dados/historico_teste.txt");

    // TESTE: ORDEM ALFABÉTICA
    // Resultado esperado: Ana, Bruno, Carlos, Diana
    Partida *copiaAlfabetica = criarCopiaPartidas(&historico);
    if (copiaAlfabetica != NULL) {
        double tempoAlfa = ordenarPartidas(copiaAlfabetica, historico.tamanho, ORDEM_ALFABETICA);
        exibirRanking(copiaAlfabetica, historico.tamanho, "HISTORICO (A-Z)", tempoAlfa);
        free(copiaAlfabetica); // libera a cópia depois de exibir
    }

    // TESTE: PONTUAÇÃO DECRESCENTE
    // Resultado esperado: Ana (90), Bruno (70), Diana (70), Carlos (50)
    Partida *copiaPontuacao = criarCopiaPartidas(&historico);
    if (copiaPontuacao != NULL) {
        double tempoPontos = ordenarPartidas(copiaPontuacao, historico.tamanho, ORDEM_PONTUACAO_DECRESCENTE);
        exibirRanking(copiaPontuacao, historico.tamanho, "RANKING (PONTUACAO)", tempoPontos);
        free(copiaPontuacao); // libera a cópia depois de exibir
    }

    // confirma que a lista original e o arquivo permaneceram intactos
    depurarHistorico(&historico);
    destruirLCD(&historico);

    return 0;
}